import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
import datetime

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
# export API_KEY=pk_6191974682da4d84992d2f5dc7cb694f
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    portfolio = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE id = ? GROUP BY symbol", session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    stocks_total = 0

    # ref from jlu90
    for i in range(len(portfolio)):
        if portfolio[i]['SUM(shares)'] == 0:
            del (portfolio[i])

    for item in portfolio:
        symbol_data = lookup(item['symbol'])
        item['name'] = symbol_data['name']
        item['price'] = symbol_data['price']
        item['total'] = symbol_data['price'] * item['SUM(shares)']
        stocks_total += item['total']

    net_total = stocks_total + cash

    return render_template("index.html", portfolio=portfolio, cash=cash, net_total=net_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol")
        symbol_data = lookup(symbol)
        shares = request.form.get("shares")

        if not shares.isdigit():
            return apology("shares have to be positive interger", 400)
        elif isinstance(shares, float):
            shares = float(shares)
            shares = int(shares)
        else:
            shares = int(shares)

        if not symbol:
            return apology("missing symbol", 400)

        elif not symbol_data:
            return apology("invalid symbol", 400)

        if not shares:
            return apology("missing shares", 400)

        elif shares <= 0:
            return apology("shares have to be positive interger", 400)

        price = symbol_data['price']
        amount_due = price * shares
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        if amount_due <= float(cash):
            new_cash = float(cash) - amount_due
            now = datetime.datetime.now()
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])
            db.execute("INSERT INTO transactions (id, symbol, shares, price, transacted) VALUES(?, ?, ?, ?, ?)",
                       session["user_id"], symbol, shares, price, now)
            return redirect("/")
        else:
            return apology("can't afford", 400)

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute("SELECT * FROM transactions WHERE id = ? ORDER BY transacted DESC", session["user_id"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 401)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        symbol = request.form.get("symbol")
        symbol_data = lookup(symbol)

        # Ensure symbol was submitted
        if not symbol:
            return apology("missing symbol", 400)

        # Lookup whether the symbol is included in the API
        elif not symbol_data:
            return apology("invalid symbol", 400)

        # Display the stock quote on quoted.html
        return render_template("quoted.html", name=symbol_data['name'], price=symbol_data['price'], symbol=symbol_data['symbol'])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username was submitted and not already taken
        if not username or len(rows) >= 1:
            return apology("username is not available", 400)

        # Ensure password was submitted
        if not password:
            return apology("missing password", 400)

        # Ensure password confirmation was submitted and was the same as password
        if not confirmation or password != confirmation:
            return apology("password don't match", 400)

        # Hash a password with the given method and salt with a string of the given length
        hash = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

        # Insert database into Query
        row = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    portfolio = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE id = ? GROUP BY symbol", session["user_id"])

    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not shares.isdigit():
            return apology("shares have to be positive interger", 400)
        elif isinstance(shares, float):
            shares = float(shares)
            shares = int(shares)
        else:
            shares = int(shares)

        if not symbol:
            return apology("missing symbol", 400)

        if not lookup(symbol):
            return apology("invalid symbol", 400)

        if not shares:
            return apology("missing shares", 400)

        elif shares <= 0:
            return apology("shares have to be positive interger", 400)

        # ref from jlu90
        avaliable_stock = db.execute(
            "SELECT symbol, SUM(shares) FROM transactions WHERE id = ? AND symbol = ? GROUP BY symbol", session["user_id"], symbol)

        stocks_dict = {}
        for row in avaliable_stock:
            stocks_dict[row['symbol']] = row['SUM(shares)']

        shares_available = stocks_dict[symbol]

        if shares <= int(shares_available):
            cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
            price = lookup(symbol)["price"]
            new_cash = cash + (shares * price)
            now = datetime.datetime.now()
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])
            db.execute("INSERT INTO transactions (id, symbol, shares, price, transacted) VALUES(?, ?, ?, ?, ?)",
                       session["user_id"], symbol, -shares, price, now)
            return redirect("/")

        else:
            return apology("too many shares", 400)

    else:
        return render_template("sell.html", portfolio=portfolio)