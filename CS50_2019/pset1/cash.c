#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);

    int cents = round(dollars * 100);
    int coins = 0;

    while (cents > 0)
    {
        if (cents >= 25)
        {
            cents -= 25;
            coins++;
        }
        else if (cents < 25 && cents >= 10)
        {
            cents -= 10;
            coins++;
        }
        else if (cents < 10 && cents >= 5)
        {
            cents -= 5;
            coins++;
        }
        else
        {
            cents -= 1;
            coins++;
        }
    }
    printf("%i\n", coins);
}
