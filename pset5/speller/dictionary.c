// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Improve this hash function
// const unsigned int N = 27; // First letter with A~Z + '
// node *table[N];
// unsigned int hash(const char *word)
// {
//     return toupper(word[0]) - 'A';
// }
// TODO: Choose number of buckets in hash table

#define HASHTABLE_SIZE 65536

// Hash table
node *table[HASHTABLE_SIZE];

// Hashes the word (hash function posted on reddit by delipity)
int hash(char *hashing_word)
{
    unsigned int hash_index = 0;
    for (int i = 0, n = strlen(hashing_word); i < n; i++)
    {
        hash_index = (hash_index << 2) ^ hashing_word[i];
    }
    return hash_index % HASHTABLE_SIZE;
}

int word_count = 0;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        table[i] = NULL;
    }

    // Opens dictionary file
    FILE *dic_file = fopen(dictionary, "r");
    if (dic_file == NULL)
    {
        return false;
    }

    // Read strings from file
    char word[LENGTH + 1];
    while (fscanf(dic_file, "%s", word) != EOF)
    {
        // Create a new node
        node *new_word = malloc(sizeof(node));
        if (new_word == NULL)
        {
            unload();
            return false;
        }
        strcpy(new_word->word, word);

        // Hash word
        int hash_index = hash(new_word->word);

        // Insert node into hash table
        node *head = table[hash_index];
        if (head == NULL)
        {
            new_word->next = NULL;
            table[hash_index] = new_word;
            word_count++;
        }
        else
        {
            new_word->next = table[hash_index];
            table[hash_index] = new_word;
            word_count++;
        }
    }
    fclose(dic_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Case insensitive
    int n = strlen(word);
    char word_lower[LENGTH + 1];
    for (int i = 0; i < n; i++)
    {
        word_lower[i] = tolower(word[i]);
    }
    word_lower[n] = '\0';

    // Traversing linked list
    int hash_index = hash(word_lower);
    node *cursor = table[hash_index];
    while (cursor != NULL)
    {
        if (strcasecmp(word_lower, cursor->word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }

    // The checked word was not found in the dictionary until the end of the cursor, which should be a misspelling
    return false;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Freeing linked list
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
