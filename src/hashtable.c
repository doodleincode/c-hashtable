/**
 * A very basic hashtable implementation
 *
 * @author  Daniel Hong
 * 
 * This program is licensed under the GNU GENERAL PUBLIC LICENSE Version 2.
 * A LICENSE file should have accompanied this program.
 */
 
// TODO
// - need to check for hash collision when generating new hashes
// - error checking

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "../include/hashtable.h"

Hashtable *init_hashtable()
{
    int i;
    Hashtable *ht = NULL;
    
    // Allocate memory for our hashtable "class"
    if ((ht = malloc(sizeof(Hashtable))) == NULL) {
        return NULL;
    }
    
    // Allocate memory for the table
    // We are just allocating USHRT_MAX number of pointers
    // Memoery for the actual key/value entry will be allocated when an 
    // item is added to the hashtable
    if ((ht->_table = malloc(sizeof(HTEntry *) * USHRT_MAX)) == NULL) {
        return NULL;
    }
    
    for (i = 0; i < USHRT_MAX; i++) {
        ht->_table[i] = NULL;
    }
    
    // Setup the property/function pointers
    ht->add = _add_;
    ht->update = _update_;
    ht->get = _get_;
    ht->remove = _remove_;
    ht->contains_key = _contains_key_;
    
    // These are "private" members
    ht->_add_update = _add_update_;
    ht->_newitem = _newitem_;
    ht->_hash = _hash_;
    ht->_size = USHRT_MAX;
    
    return ht;
}

void free_hashtable(Hashtable *ht)
{
    int i;
    
    if (ht) {
        for (i = 0; i < ht->_size; i++) {
            if (ht->_table[i] != NULL) {
                free(ht->_table[i]->key);
                free(ht->_table[i]->value);
                free(ht->_table[i]);
                ht->_table[i] = NULL;
            }
        }
        
        free(ht->_table);
        free(ht);
        ht = NULL;
    }
}

// -----------------------------------------------------------------------------
//  Function pointer implementations. These should not be called directly!
// -----------------------------------------------------------------------------

static void _add_(Hashtable *this, char *key, char *value)
{
    this->_add_update(this, key, value);
}

static void _update_(Hashtable *this, char *key, char *value)
{
    // If the key doesn't exist, show error and return
    if (this->contains_key(this, key) == 0) {
        printf("Attempting to update the key '%s' which does not exist.", key);
        return;
    }
    
    this->_add_update(this, key, value);
}

static const char *_get_(Hashtable *this, const char *key, const char *def)
{
    HTEntry *item;
    int hash = this->_hash(key);
    
    item = this->_table[hash];
    
    if (item == NULL || item->key == NULL || strcmp(key, item->key) != 0) {
        return def;
    }
    
    return item->value;
}

static void _remove_(Hashtable *this, const char *key)
{
    // TODO needs to be implemented
}

static int _contains_key_(Hashtable *this, const char *key)
{
    int hash = this->_hash(key);
    return ((this->_table[hash] == NULL) ? 0 : 1);
}

// -----------------------------------------------------------------------------
//  Private methods
// -----------------------------------------------------------------------------

static HTEntry *_newitem_(char *key, char *value)
{
    HTEntry *item;
    
    if ((item = malloc(sizeof(HTEntry))) == NULL) {
        return NULL;
    }

    if ((item->key = memcpy(malloc(strlen(key)), key, strlen(key))) == NULL) {
        return NULL;
    }

    if ((item->value = memcpy(malloc(strlen(value)), value, strlen(value))) == NULL) {
        return NULL;
    }

    // Setting the next item pointer to NULL since there isn't one yet
    item->next = NULL;

    return item;
}

/**
 * Create a hash based on the given key
 * Uses the Jenkins hash algorithm
 * http://en.wikipedia.org/wiki/Jenkins_hash_function
 */
static int _hash_(const char *key)
{
    unsigned short hash, i;
    
    for (hash = i = 0; i < strlen(key); ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

static void _add_update_(Hashtable *this, char *key, char *value)
{
    HTEntry *newitem = NULL;
    HTEntry *curr = NULL;
    HTEntry *prev = NULL;
    int hash = this->_hash(key);

    curr = this->_table[hash];

    // Follow the links to find the previous and next items
    // This loop will not execute when adding a new hash
    while(curr != NULL && curr->key != NULL && strcmp(key, curr->key) > 0) {
        prev = curr;
        curr = curr->next;
    }

    // If item already exists, we'll update it
    if (curr != NULL && curr->key != NULL && strcmp(key, curr->key) == 0) {
        free(curr->value);
        curr->value = memcpy(malloc(strlen(value)), value, strlen(value));
        return;
    }
    
    //
    // Otherwise we'll add a new item to the table
    //
    
    if ((newitem = this->_newitem(key, value)) == NULL) {
        printf("Error creating a new hash entry.");
        return;
    }

    // If at the start of the linked list
    if (curr == this->_table[hash]) {
        newitem->next = curr;
        this->_table[hash] = newitem;
    }
    // If at the end of the linked list
    else if (curr == NULL) {
        prev->next = newitem;
    }
    // If in the middle of the list
    else {
        newitem->next = curr;
        prev->next = newitem;
    }
}

