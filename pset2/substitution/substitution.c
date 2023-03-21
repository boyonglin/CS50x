#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool is_valid_key(string s);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (!is_valid_key(argv[1]))
    {
        return 1;
    }

    string plainTxt = get_string("plaintext:  ");
    int plainTxtLen = strlen(plainTxt);
    char cipherTxt[plainTxtLen];

    printf("ciphertext: ");
    for (int i = 0; i < plainTxtLen; i++)
    {
        if (isupper(plainTxt[i]))
        {
            cipherTxt[i] = toupper(argv[1][plainTxt[i] - 'A']);
        }
        else if (islower(plainTxt[i]))
        {
            cipherTxt[i] = tolower(argv[1][plainTxt[i] - 'a']);
        }
        else
        {
            cipherTxt[i] = plainTxt[i];
        }
        printf("%c", cipherTxt[i]);
    }
    printf("\n");
}

bool is_valid_key(string s)
{
    int n = strlen(s);
    int isRepeat = 0;
    if (n == 26)
    {
        for (int i = 0; i < n; i++)
        {
            if (!isalpha(s[i]))
            {
                printf("Key must only contain alphabetic characters.\n");
                return false;
            }
            for (int j = 0; j < n; j++)
            {
                if (i != j)
                {
                    if (tolower(s[i]) == tolower(s[j]))
                    {
                        isRepeat++;
                        if (isRepeat > 0)
                        {
                            printf("Key must not contain repeated characters.\n");
                            return false;
                        }
                    }
                }
            }
        }
    }
    else
    {
        printf("Key must contain 26 characters.\n");
        return false;
    }

    return true;
}