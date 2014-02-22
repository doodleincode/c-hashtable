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
    if ((ht->__table__ = malloc(sizeof(HTEntry *) * USHRT_MAX)) == NULL) {
        return NULL;
    }
    
    for (i = 0; i < USHRT_MAX; i++) {
        ht->__table__[i] = NULL;
    }
    
    // Setup the property/function pointers
    ht->add = _add;
    ht->update = _update;
    ht->get = _get;
    ht->remove = _remove;
    ht->contains_key = _contains_key;
    
    // These are "private" members
    ht->__add_update__ = _add_update;
    ht->__newitem__ = _newitem;
    ht->__hash__ = _hash;
    ht->__size__ = USHRT_MAX;
    
    return ht;
}

void free_hashtable(Hashtable *ht)
{
    int i;
    
    if (ht) {
        for (i = 0; i < ht->__size__; i++) {
            if (ht->__table__[i] != NULL) {
                free(ht->__table__[i]->key);
                free(ht->__table__[i]->value);
                free(ht->__table__[i]);
                ht->__table__[i] = NULL;
            }
        }
        
        free(ht->__table__);
        free(ht);
        ht = NULL;
    }
}

// -----------------------------------------------------------------------------
//  Function pointer implementations. These should not be called directly!
// -----------------------------------------------------------------------------

void _add(Hashtable *this, char *key, char *value)
{
    this->__add_update__(this, key, value);
}

void _update(Hashtable *this, char *key, char *value)
{
    // If the key doesn't exist, show error and return
    if (this->contains_key(this, key) == 0) {
        printf("Attempting to update the key '%s' which does not exist.", key);
        return;
    }
    
    this->__add_update__(this, key, value);
}

const char *_get(Hashtable *this, const char *key, const char *def)
{
    HTEntry *item;
    int hash = this->__hash__(key);
    
    item = this->__table__[hash];
    
    if (item == NULL || item->key == NULL || strcmp(key, item->key) != 0) {
        return def;
    }
    
    return item->value;
}

void _remove(Hashtable *this, const char *key)
{
    // TODO needs to be implemented
}

int _contains_key(Hashtable *this, const char *key)
{
    int hash = this->__hash__(key);
    return ((this->__table__[hash] == NULL) ? 0 : 1);
}

// -----------------------------------------------------------------------------
//  Private methods
// -----------------------------------------------------------------------------

HTEntry *_newitem(char *key, char *value)
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
int _hash(const char *key)
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

void _add_update(Hashtable *this, char *key, char *value)
{
    HTEntry *newitem = NULL;
    HTEntry *curr = NULL;
    HTEntry *prev = NULL;
    int hash = this->__hash__(key);

    curr = this->__table__[hash];

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
    
    if ((newitem = this->__newitem__(key, value)) == NULL) {
        printf("Error creating a new hash entry.");
        return;
    }

    // If at the start of the linked list
    if (curr == this->__table__[hash]) {
        newitem->next = curr;
        this->__table__[hash] = newitem;
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

