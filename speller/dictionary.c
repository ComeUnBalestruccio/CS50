// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;


// Number of buckets in hash table
// const unsigned int N = 1;

#define N 968897

// Hash table
node *table[N];

int word_number = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);

    node *cursor;

    cursor = table[index];

    if (table[index] == NULL)
    {
        return false;
    }

    while(cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    free(cursor);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c = 0;

    char *lowerword = malloc(strlen(word) + 1);
    if (lowerword == NULL)
    {
        return 1;
    }

    strcpy(lowerword, word);

    for (int i = 0; i < strlen(lowerword); i++)
    {
        if (isupper(lowerword[i]))
        {
            lowerword[i] = (char)tolower((unsigned char)lowerword[i]);

        }
        hash = ((hash << 5) + hash) + c;
    }

    free(lowerword);
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    char buffer[LENGTH + 1];

    while (fscanf(dict, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, buffer);
        n->next = NULL;

        int index = hash(buffer);

        if (table[index] == NULL)
        {
            table[index] = n;
            word_number++;
        }

        else
        {
            n->next = table[index];
            table[index] = n;
            word_number++;
        }

    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (word_number > 0)
    {
        return word_number;
    }
    else
    {
        unload();
        return 0;
    }

}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;
}
