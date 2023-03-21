-- Transactions Table
CREATE TABLE "transactions" (
	'id' INTEGER NOT NULL,
    'symbol' TEXT NOT NULL,
	'shares' INTEGER NOT NULL,
	'price' REAL NOT NULL,
	'transacted' DATETIME NOT NULL
);