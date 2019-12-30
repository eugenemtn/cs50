#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <crypt.h>

// This one uses recursion

const int MAX_PASS_LENGTH = 5;

bool brute_force(string crypted_pass, int length);
bool brute_force_fx(int count, int pos, int n, string all_symbols, string given_hash, string salt, char *guess);
bool check_guess(string guess, string given_hash, string salt);

int main(int argc, string argv[])
{
    if (argc != 2) {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    bool match_found = false;

    if (!brute_force(argv[1], MAX_PASS_LENGTH))
    {
        print("No match was found\n");
        return 0;
    }
    return 0;
}

bool brute_force(string given_hash, int length)
{
    char *all_symbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int n = strlen(all_symbols);
    char salt[3];
    char guess[length + 1];
    strncpy(salt, given_hash, 2);
    salt[2] = '\0';

    for (int i = 1; i <= length; i++)
    {
        brute_force_fx(i, i, n, all_symbols, given_hash, salt, guess);
    }

    return false;
}

bool brute_force_fx(int count, int pos, int n, string all_symbols, string given_hash, string salt, char *guess)
{
    if (pos == 1)
    {
        for (int i = 0; i < n; i++)
        {
            guess[count - pos] = all_symbols[i];
            guess[count] = '\0';
            if (check_guess(guess, given_hash, salt))
            {
                return true;
            }
        }
    } else {
        for (int i = 0; i < n; i++)
        {
            guess[count - pos] = all_symbols[i];
            if (brute_force_fx(count, pos - 1, n, all_symbols, given_hash, salt, guess))
            {
                return true;
            }
        }
    }
    return false;
}

bool check_guess(string guess, string given_hash, string salt)
{
    string guess_hash = crypt(guess, salt);
    if (strcmp(guess_hash, given_hash) == 0)
    {
        printf("Match found: %s\n", guess);
        return true;
    }

    return false;
}
