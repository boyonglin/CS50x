#include <cs50.h>
#include <stdio.h>
#include <string.h>

int ctoi(int digit, char credit[]);
int sum(int len, char credit[]);

int main(void)
{
    string creditString;
    creditString = get_string("Number: ");
    int len = strlen(creditString);

    char credit[len];
    strcpy(credit, creditString);

    int temp = sum(len, credit);

    if ((len == 13 || len == 16) && (credit[0] == '4'))
    {
        if (temp % 10 == 0)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if ((len == 15) && (credit[0] == '3') && (credit[1] == '4' || credit[1] == '7'))
    {
        if (temp % 10 == 0)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if ((len == 16) && (credit[0] == '5') && (credit[1] == '1' || credit[1] == '2' || credit[1] == '3' || credit[1] == '4'
             || credit[1] == '5'))
    {
        if (temp % 10 == 0)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

int ctoi(int digit, char credit[])
{
    int creditNum = (int)(credit[digit - 1]) - 48;
    return creditNum;
}

int sum(int len, char credit[])
{
    int temp = 0;
    for (int n = len - 1; n > 0; n -= 2)
    {
        int creditNum = ctoi(n, credit);
        temp = temp + (creditNum * 2) % 10 + (creditNum * 2) / 10;
        printf("%i ", temp);
    }
    for (int n = len; n > 0; n -= 2)
    {
        int creditNum = ctoi(n, credit);
        temp = temp + creditNum;
    }
    return temp;
}