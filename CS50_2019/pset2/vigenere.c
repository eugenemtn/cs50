#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

bool is_input_correct(int argc, string argv[]);
int shift(char c);

int main(int argc, string argv[])
{
    if (!is_input_correct(argc, argv)) {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }

    string key = argv[1];
    int key_length = strlen(key);
    int counter = 0;
    string plain = get_string("plaintext: ");
    string cipher = plain;

    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        int plain_basis = isupper(plain[i]) ? 65 : 97;
        int key_basis = isupper(key[counter]) ? 65 : 97;
        int code = plain[i] - plain_basis;
        int key_code = key[counter] - key_basis;

        if (isalpha(plain[i])) {
            cipher[i] = (code + key_code) % 26 + plain_basis;
            counter = (counter == 0 || counter < key_length - 1) ? counter + 1 : 0;
        }
    }

    printf("ciphertext: %s\n", cipher);

    return 0;
}

bool is_input_correct(int argc, string argv[])
{
    if (argc != 2)
    {
        return false;
    }
    else
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (!isalpha(argv[1][i])) {
                return false;
            }
        }
    }
    return true;
}

int shift(char c)
{
    int ascii_basis = isupper(c) ? 65 : 97;
    return (int) c - ascii_basis;
}
