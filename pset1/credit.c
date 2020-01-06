#include <stdio.h>
#include <cs50.h>

bool validate_card(int count, long card_number);
string resolve_card_type(int first, int second, int count);
int count_digits(long number);
int get_digit_by_position(int digits_amount, int position, long number);

int main(void)
{
    long card_number = get_long("Number: ");
    int digits_amount = count_digits(card_number);
    string card_type = "INVALID";

    if (digits_amount < 13 || digits_amount == 14 || digits_amount > 16)
    {
        printf("%s\n", card_type);
        exit(0);
    }

    if (validate_card(digits_amount, card_number))
    {
        int first_digit = get_digit_by_position(digits_amount, 1, card_number);
        int second_digit = get_digit_by_position(digits_amount, 2, card_number);

        card_type = resolve_card_type(first_digit, second_digit, digits_amount);
        printf("%s\n", card_type);
        exit(0);
    }
    printf("%s\n", card_type);
}

int count_digits(long number)
{
    long degrade = number;
    int counter = 1;

    while (degrade > 10)
    {
        degrade /= 10;

        if (degrade >= 1)
        {
            counter++;
        }
    }

    return counter;
}

bool validate_card(int count, long number)
{
    long degrade = number;
    int double_sum = 0;
    int single_sum = 0;

    for (int i = 1; i < count + 1; i++)
    {
        int current_digit = degrade % 10;
        degrade /= 10;

        if (i % 2 == 0)
        {
            double_sum += current_digit < 5 ? current_digit * 2 : (current_digit * 2) % 10 + 1;
        }
        else
        {
            single_sum += current_digit;
        }
    }

    return (double_sum + single_sum) % 10 == 0;
}

int get_digit_by_position(int digits_amount, int position, long number)
{
    long degrade = number;

    for (int i = 0; i < digits_amount - position; i++)
    {
        degrade /= 10;
    }

    return degrade % 10;
}

string resolve_card_type(int first, int second, int count)
{
    switch (first) {
        case 5:
            if ((second >=1 && second <= 5) && count == 16)
            {
                return "MASTERCARD";
            }
            break;
        case 3:
            if ((second == 4 || second == 7) && count == 15)
            {
                return "AMEX";
            }
            break;
        case 4:
            if (count == 13 || count == 16)
            {
                return "VISA";
            }
    }

    return "INVALID";
}
