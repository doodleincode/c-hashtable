#include <stdio.h>
#include <stdlib.h>

#include "../include/hashtable.h"

int main()
{

	/*Hashtable *hashtable = ht_create(  );

	ht_set( hashtable, "key1", "inky");
	ht_set( hashtable, "key2", "pinky");
	ht_set( hashtable, "key3", "blinky");
	ht_set( hashtable, "key4", "floyd");
	ht_set( hashtable, "min_num", "50");
	ht_set( hashtable, "max_num", "250");
	ht_set( hashtable, "allowed_chars", "abcde");
	ht_set( hashtable, "min_chars", "1" );
	ht_set( hashtable, "max_chars", "35" );

    printf( "key3 = %s\n", ht_get( hashtable, "key3" ) );
	printf( "key1 = %s\n", ht_get( hashtable, "key1" ) );
	printf( "key2 = %s\n", ht_get( hashtable, "key2" ) );	
	printf( "key4 = %s\n", ht_get( hashtable, "key4" ) );
	printf( "min_chars = %d\n", atoi(ht_get( hashtable, "mind_chars" )) );
	printf( "max_chars = %s\n", ht_get( hashtable, "max_chars" ) );
	printf( "min_num = %s\n", ht_get( hashtable, "min_num" ) );
	printf( "max_num = %s\n", ht_get( hashtable, "max_num" ) );
	printf( "allowed_chars = %s\n", ht_get( hashtable, "allowed_chars" ) );
	
	ht_set( hashtable, "allowed_chars", "fghijkl" );
	printf( "allowed_chars (updated) = %s\n", ht_get( hashtable, "allowed_chars" ) );

    ht_free(hashtable);*/
    
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
