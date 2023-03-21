from cs50 import get_string


def main():
    credit = get_string("Number: ")
    length = len(credit)
    temp = sum(length, credit)

    if length in [13, 16] and credit[0] in ["4"]:
        if temp % 10 == 0:
            print("VISA")
        else:
            print("INVALID")

    elif length in [15] and credit[0] in ["3"] and credit[1] in ["4", "7"]:
        if temp % 10 == 0:
            print("AMEX")
        else:
            print("INVALID")

    elif length in [16] and credit[0] in ["5"] and credit[1] in ["1", "2", "3", "4", "5"]:
        if temp % 10 == 0:
            print("MASTERCARD")
        else:
            print("INVALID")

    else:
        print("INVALID")


def sum(length, credit):
    temp = 0
    
    for n in range(length - 1, 0, -2):
        creditNum = int(credit[n - 1])
        creditStr = str(creditNum * 2)
        for l in range(len(creditStr)):
            temp = temp + int(creditStr[l])

    for n in range(length, 0, -2):
        creditNum = int(credit[n - 1])
        temp = temp + creditNum

    return temp


if __name__ == "__main__":
    main()