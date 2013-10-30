//#include "growingnet.h"
#include "network.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

int i;
char buf[64] = "";

int main()
{
    fprintf(stderr,"Network Testing Application\n\n\n");
    
    printf("\n>>>Creating 2 non-growing networks\n");
    unsigned long id1 = network_new(0), id2 = network_new(0);
    
    printf("\n\n>>>Creating 2 growing networks\n");
    unsigned long gid1 = network_new(-15), gid2 = network_new(1);
    
    unsigned long id3 = network_new(0);
    
    printf(">>>Adding 10 nodes to %lu\n", id2);
    for (i=0; i<10; ++i)
    {
        sprintf(buf, "Node #%d", i);
        network_add_node(id2, buf);
    }
    
    printf("\n\n>>>Number of nodes in %lu\n", id2);    
    assert(network_nodes_number(id2) == 10);
    
    network_remove_node(id2, "Node #4");
    network_remove_node(id2, "Node #11");
    network_remove_node(id2, "Node #0");
    network_remove_node(id2, "Node #4");
    assert(network_nodes_number(id2) == 8);
    
    for (i=0; i<10; ++i)
    {
        sprintf(buf, "Node #%d", i);
        assert(network_in_degree(id2, buf) == network_out_degree(id2, buf));
        assert(network_in_degree(id2, buf) == 0);
    }
    
    network_add_link(id2, "Node #4", "Node #9");
    assert(network_nodes_number(id2) == 9);
    
    assert(network_links_number(id2) == 1);
    network_add_link(id2, "Node #9", "Node #14");
    network_add_link(id2, "Node #9", "Node #14");
    assert(network_nodes_number(id2) == 10);
    assert(network_links_number(id2) == 2);
    
    network_add_link(id2, NULL, NULL);
    assert(network_nodes_number(id2) == 10);
    assert(network_links_number(id2) == 2);
    
    network_add_link(id2, "Node #20", NULL);
    assert(network_nodes_number(id2) == 10);
    assert(network_links_number(id2) == 2);
    
    network_add_link(id2, NULL, "Node #21");
    assert(network_nodes_number(id2) == 10);
    assert(network_links_number(id2) == 2);
    
    network_add_link(id2, "Node #9", "Node #14");
    assert(network_links_number(id2) == 2);
    
    network_add_link(id2, "Node #1", "Node #2");
    network_add_link(id2, NULL, "Node #2");
    network_add_link(id2, "Node #1", NULL);
    assert(network_links_number(id2) == 3);
    assert(network_in_degree(id2, "Node #2") == 1);
    assert(network_out_degree(id2, "Node #2") == 0);
    assert(network_in_degree(id2, "Node #1") == 0);
    assert(network_out_degree(id2, "Node #1") == 1);
    network_add_link(id2, NULL, "Node #3");
    network_add_link(id2, "Node #3", NULL);
    assert(network_in_degree(id2, "Node #3") == 0);
    assert(network_out_degree(id2, "Node #3") == 0);
    
    network_add_link(id2, "Node #2", "Node #1");
    assert(network_links_number(id2) == 4);
    
    network_remove_link(id2, "Node #1", "Node #2");
    assert(network_links_number(id2) == 3);
    network_add_link(id2, "Node #1", "Node #5");
    network_add_link(id2, "Node #1", "Node #6");
    assert(network_links_number(id2) == 5);
    assert(network_out_degree(id2, "Node #1") == 2);
    assert(network_in_degree(id2, "Node #1") == 1);
    network_remove_node(id2, "Node #1");
    assert(network_links_number(id2) == 2);
    assert(network_out_degree(id2, "Node #1") == 0);
    assert(network_in_degree(id2, "Node #1") == 0);
    
    network_add_link(id2, "Node #1", "Node #2");
    assert(network_links_number(id2) == 3);    
    
    assert(network_nodes_number(id3) == 0);
    assert(network_links_number(id3) == 0);
    assert(network_in_degree(id3, "Node #14") == 0);
    
    assert(network_links_number(id1) == 0);
    assert(network_nodes_number(id1) == 0);
    assert(network_links_number(gid1) == 0);
    assert(network_nodes_number(gid1) == 0);
    assert(network_links_number(gid2) == 0);
    assert(network_nodes_number(gid2) == 0);
    
    network_add_node(id1, "Node #0");
    assert(network_nodes_number(id1) == 1);
    assert(network_links_number(id1) == 0);
    network_add_link(id1, "Node #22", "Node #0");
    network_add_link(id1, "Node #22", "Node #33");
    assert(network_nodes_number(id1) == 3);
    assert(network_links_number(id1) == 2);
    network_clear(id1);
    assert(network_nodes_number(id1) == 0);
    assert(network_links_number(id1) == 0);
    
    network_add_node(id1, "Node #0");
    network_add_link(id1, "Node #22", "Node #0");
    network_add_link(id1, "Node #22", "Node #33");
    assert(network_nodes_number(id1) == 3);
    assert(network_links_number(id1) == 2);
    
    network_delete(id1);
    assert(network_nodes_number(id1) == 0);
    assert(network_links_number(id1) == 0);
    network_add_node(id1, "Node #0");
    network_add_link(id1, "Node #22", "Node #0");
    network_add_link(id1, "Node #22", "Node #33");
    assert(network_nodes_number(id1) == 0);
    assert(network_links_number(id1) == 0);
    
    
    printf("\n\n>>>Number of nodes in %lu\n", -1);
    assert(network_nodes_number(-1) == 0);
    
    #ifndef NDEBUG
        printf("\n\nAll asserts passed.\n");
    #endif
    
    return 0;
}
