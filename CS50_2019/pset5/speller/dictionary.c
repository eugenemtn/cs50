// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

#define TABLE_SIZE 1024

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int dict_size = 0;

// Number of buckets in hash table
const unsigned int N = TABLE_SIZE;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char check_word[strlen(word) + 1];
    for (int i = 0, n = strlen(word); i <= n; i++)
    {
        check_word[i] = tolower(word[i]);
    }
    int word_hash = hash(check_word);
    node *cursor = table[word_hash];
    while(cursor != NULL)
    {
        if (strcmp(cursor->word, check_word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // djb2 by Dan Bernstein http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c;

    while ((c = *word++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return abs((int) hash % TABLE_SIZE);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    node *n = NULL;
    FILE *dict = fopen(dictionary, "r");

    if (dict == NULL)
    {
        fclose(dict);
        return false;
    }

    char word[LENGTH + 1];
    int w, hash_value;
    dict_size = 0;
    while ((w = fscanf(dict, "%s", word)) != EOF)
    {
        hash_value = hash(word);
        n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(dict);
            return false;
        }
        strcpy(n->word, word);
        n->next = table[hash_value];
        table[hash_value] = n;
        dict_size++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *cursor = NULL;
    node *tmp = NULL;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }
        cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
