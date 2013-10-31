#include "growingnet.h"
#include "network.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

int i;
char buf[64] = "";

int main()
{
    fprintf(stderr,"Network Testing Application\n\n\n");
    
    int id1 = network_new(1);
    assert(id1 == 2);
    
    int id2 = network_new(0);
    assert(id2 == 3);
    int id3 = network_new(0);
    assert(id3 == 4);
    
    network_delete(id2);
    
    int id4 = network_new(1);
    assert(id4 == 5);
    
    int id5 = network_new(0);
    assert(id5 == 6);
    
    network_delete(id1);
    network_delete(id3);
    network_delete(id4);
    network_delete(id5);
    
    int id6 = network_new(0);
    assert(id6 == 7);
    
    network_delete(id2);
    network_delete(id6);
    
    int id7 = network_new(1);
    assert(id7 == 8);
    
    network_delete(growingnet);
    network_delete(id7);
    
    int id8 = network_new(0);
    assert(id8 == 9);
    
    int id9 = network_new(1);
    assert(id9 == 10);
    
    network_add_link(growingnet,"a","b");
    
    #ifndef NDEBUG
        printf("\n\nAll asserts passed.\n");
    #endif
    
    return 0;
}
