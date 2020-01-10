#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

bool is_input_correct(int argc, string argv[]);

int main(int argc, string argv[])
{
    if (!is_input_correct(argc, argv)) {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    string plain = get_string("plaintext: ");
    string cipher = plain;
    int key = atoi(argv[1]);

    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        if (isupper(plain[i]) || islower(plain[i]))
        {
            int basic_ascii = isupper(plain[i]) ? 65 : 97;
            int c = plain[i] - basic_ascii;
            cipher[i] = (c + key) % 26 + basic_ascii;
        }
    }

    printf("ciphertext: %s\n", cipher);
}

bool is_input_correct(int argc, string argv[])
{
    string key;

    if (argc != 2)
    {
        return false;
    }
    else
    {
        key = argv[1];

        for (int i = 0, n = strlen(key); i < n; i++)
        {
            if (!isdigit(key[i])) {
                return false;
            }
        }
    }
    return true;
}