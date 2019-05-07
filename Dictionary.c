// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <cs50.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26
#define HASHTABLE_SIZE 65536

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[HASHTABLE_SIZE] = {NULL};

// initialize word count
int wordcount = 0;

//create a hash
//hashtag function from cs50 reddit
int hash_it(const char *word)
{
    unsigned int hash = 0;
    {
        return tolower(word[0]) - 'a';
    }
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }
    return hash % HASHTABLE_SIZE;
};

// initialize head
node *head = NULL;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        strcpy(new_node->word, word);
        int dicwords = hash_it(word);
        if (hashtable[dicwords] == NULL)
        {
            hashtable[dicwords] = new_node;
            new_node->next = NULL;
        }
        else
        {
            new_node->next = hashtable[dicwords];
            hashtable[dicwords] = new_node;
        }
        wordcount++;

    }


    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordcount;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    head = hashtable[hash_it(word)];

    if (head == NULL)
    {
        return false;
    }
    else if (head != NULL)
    {
        node *cursor = head;
        while (cursor != NULL)
        {
            //do something
            if (strcasecmp(cursor->word, word) == 0)
            {
                return true;
            }
            else
            {
                cursor = cursor->next;
            }

        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        head = hashtable[i];
        node *cursor = head;

        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    free(head);
    return true;
}
