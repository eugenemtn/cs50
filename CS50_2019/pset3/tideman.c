// https://cs50.harvard.edu/x/2020/psets/3/tideman/

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
bool cycle_check(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs.
    // Clear preferences array
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        printf("\n");

        record_preferences(ranks);
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[i] = rank;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int count_wins = 0;
    for (int a = 0; a < candidate_count; a++)
    {
        count_wins = 0;
        for (int b = 0; b < candidate_count; b++)
        {
            if (ranks[a] < ranks[b])
            {
                preferences[a][b]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count; j++) {
            if (preferences[i][j] != 0 && preferences[i][j] != preferences[j][i])
            {
                bool straight_is_winner = preferences[i][j] > preferences[j][i];
                pairs[pair_count].winner = straight_is_winner ? i : j;
                pairs[pair_count].loser = straight_is_winner ? j : i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {

        for (int j = 0; j < pair_count; j++)
        {
            pair tmp;
            if (preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {
                tmp = pairs[j + 1];
                pairs[j + 1] = pairs[j];
                pairs[j] = tmp;
            }
        }
    }
    return;
}

// Resolve cycle presence
bool cycle_check(void)
{
    int count_lockers = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        count_lockers = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                count_lockers++;
            }
        }
        if (count_lockers != 1)
        {
            return false;
        }
    }
    return true;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!cycle_check())
        {
            locked[pairs[i].loser][pairs[i].winner] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int count_lockers = 0;
    int min_value = MAX;
    int winner_index = -1;

    for (int i = 0; i < candidate_count; i++)
    {
        count_lockers = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                count_lockers++;
            }
        }
        if (count_lockers < min_value)
        {
            min_value = count_lockers;
            winner_index = i;
        }
    }
    printf("%s\n", candidates[winner_index]);
    return;
}
