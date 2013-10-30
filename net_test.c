//#include "growingnet.h"
#include "network.h"

#include <stdio.h>
#include <assert.h>

int main()
{
    fprintf(stderr,"Network Testing Application\n\n\n");
    
    printf(">>>Creating 3 non-growing networks\n");
    unsigned long id1 = network_new(0), id2 = network_new(0), id3 = network_new(0);
    printf("\n\n");
    
    printf(">>>Createing 2 growing networks\n");
    unsigned long gid1 = network_new(-15), gid2 = network_new(1);
    printf("\n\n");
    
    printf(">>>Number of nodes in %lu\n", id1);    
    assert(network_nodes_number(id1) == 0);
    printf("\n\n");
    
    printf(">>>Number of nodes in %lu\n", -1);
    assert(network_nodes_number(-1) == 0);
    printf("\n\n");
    
    printf(">>>Out degree of a NULL label in %lu\n", id1);
    assert(network_out_degree(id1, NULL) == 0);
    printf("\n\n");
    
    printf(">>>Add node 'test1' to %lu\n", id1);
    network_add_node(id1, "test1");
    printf("\n\n");
    
    printf(">>>Add node 'test2' to %lu\n", id1);
    network_add_node(id1, "test2");
    printf("\n\n");
    
    network_new(0);
    network_new(0);
    network_nodes_number(id1);
    network_clear(id1);
    network_nodes_number(id1);
    network_out_degree(id1, "test1");
    network_add_link(id1, "test1", "test2");
    network_out_degree(id1, "test1");
    
    return 0;
}
