#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <crypt.h>

// Stupid nested loops

bool brute_force(string crypted_pass);
bool check_guess(string guess, string given_hash, string salt);

int main(int argc, string argv[])
{
    if (argc != 2) {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    bool match_found = false;

    if (!brute_force(argv[1]))
    {
        return 1;
    }
    return 0;
}

bool brute_force(string given_hash)
{
    char *all = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int n = strlen(all);
    char salt[3];
    char guess[6];
    strncpy(salt, given_hash, 2);
    salt[2] = '\0';

    //single letter
    for (int i = 0; i < n; i++)
    {
        guess[0] = all[i];
        guess[1] = '\0';
        if (check_guess(guess, given_hash, salt))
        {
            return true;
        }
    }

    //two letters
    for (int i = 0; i < n; i++)
    {
        guess[0] = all [i];
        for (int j = 0; j < n; j++)
        {
            guess[1] = all[j];
            guess[2] = '\0';
            if (check_guess(guess, given_hash, salt))
            {
                return true;
            }
        }
    }

    //three letters
    for (int i = 0; i < n; i++)
    {
        guess[0] = all [i];
        for (int j = 0; j < n; j++)
        {
            guess[1] = all[j];
            for (int k = 0; k < n; k++)
            {
                guess[2] = all[k];
                guess[3] = '\0';
                if (check_guess(guess, given_hash, salt))
                {
                    return true;
                }
            }
        }
    }

    //four letters
    for (int i = 0; i < n; i++)
    {
        guess[0] = all [i];
        for (int j = 0; j < n; j++)
        {
            guess[1] = all[j];
            for (int k = 0; k < n; k++)
            {
                guess[2] = all[k];
                for (int m = 0; m < n; m++)
                {
                    guess[3] = all[m];
                    guess[4] = '\0';
                    if (check_guess(guess, given_hash, salt))
                    {
                        return true;
                    }
                }
            }
        }
    }

    //five letters
    for (int i = 0; i < n; i++)
    {
        guess[0] = all [i];
        for (int j = 0; j < n; j++)
        {
            guess[1] = all[j];
            for (int k = 0; k < n; k++)
            {
                guess[2] = all[k];
                for (int m = 0; m < n; m++)
                {
                    guess[3] = all[m];
                    for(int s = 0; s < n; s++)
                    {
                        guess[4] = all[s];
                        guess[5] = '\0';
                        if (check_guess(guess, given_hash, salt))
                        {
                            return true;
                        }
                    }
                }
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
        printf("%s\n", guess);
        return true;
    }

    return false;
}
