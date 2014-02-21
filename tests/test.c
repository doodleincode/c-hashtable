#include <stdio.h>
#include <stdlib.h>

#include "../include/hashtable.h"

int main()
{    
    Hashtable *ht = init_hashtable();
    ht->add(ht, "key1", "value1");
    ht->add(ht, "key2", "value2");
    ht->add(ht, "key3", "value3");
    
    printf("key1 = %s\n", ht->get(ht, "key1", "not found"));
    printf("key2 = %s\n", ht->get(ht, "key2", "not found"));
    printf("key3 = %s\n", ht->get(ht, "key3", "not found"));
    printf("key4 = %s\n", ht->get(ht, "key4", "not found"));
    
    free_hashtable(ht);
    
    return 0;
}
