#include <assert.h>
#include <stdio.h>
#include "network.h"
#include "growingnet.h"

int main()
{
    unsigned long n, x;

    n = network_new(0);
    x = network_new(1);
    
    int i = 1;

    network_add_link(n, "a", "b");
    assert(network_nodes_number(n) == 2);
    assert(network_links_number(n) == 1);
    assert(network_out_degree(n, "Kabaty") == 0);
    assert(network_out_degree(n, "a") == 1);
    assert(network_in_degree(n, "Kabaty") == 0);
    assert(network_out_degree(n, "Natolin") == 0);
    assert(network_in_degree(n, "b") == 1);  
    printf(">>>>>assert %d passed\n\n", i++);  

    network_remove_node(n, "a");

    network_remove_node(n, "b");
    assert(network_nodes_number(n) == 0);
    assert(network_links_number(n) == 0);
    printf(">>>>>assert %d passed\n\n", i++);

    network_add_node(n, "a");
    assert(network_nodes_number(n) == 1);
    assert(network_links_number(n) == 0);
    printf(">>>>>assert %d passed\n\n", i++);

    network_add_link(n, "b", "a");
    assert(network_nodes_number(n) == 2);
    assert(network_links_number(n) == 1);
    printf(">>>>>assert %d passed\n\n", i++);

    network_clear(n);
    assert(network_nodes_number(n) == 0);
    assert(network_links_number(n) == 0);
    printf(">>>>>assert %d passed\n\n", i++);

    network_remove_node(n, "a");
    network_remove_link(n, "b", "a");
    assert(network_nodes_number(n) == 0);
    assert(network_links_number(n) == 0);
    printf(">>>>>assert %d passed\n\n", i++);

    network_add_link(n, "c", "d");
    network_remove_link(n, "c", "d");
    assert(network_nodes_number(n) == 2);
    assert(network_links_number(n) == 0);
    printf(">>>>>assert %d passed\n\n", i++);

    network_add_link(n, "c", "d");
    network_remove_link(n, "d", "c");
    assert(network_nodes_number(n) == 2);
    assert(network_links_number(n) == 1);
    printf(">>>>>assert %d passed\n\n", i++);

    printf("*****\nALL ASSERTIONS PASSED\n*****\n\n");

    network_add_link(x, "c", "d");
    network_remove_link(x, "c", "d");
    assert(network_nodes_number(x) == 2);
    assert(network_links_number(x) == 1);
    printf(">>>>>assert %d passed\n\n", i++);

    network_add_link(x, "a", "b");
    network_clear(x);
    assert(network_nodes_number(x) == 4);
    assert(network_links_number(x) == 2);
    printf(">>>>>assert %d passed\n\n", i++);

    for (int j = 0; j < 1000; j++)
    {
        network_add_link(x, "a", "b");
        network_clear(x);
        network_remove_link(x, "a", "b");
        assert(network_nodes_number(x) == 4);
        assert(network_links_number(x) == 2);
    }
    printf(">>>>>assert %d passed\n\n", i++);

    for (char c = 'a'; c <= 'd'; c++)
    {
        //printf("%c\n", c); Wypisuje poprawnie alfabet
        char temp[1] = {c};
        network_add_node(x, temp);
    }

    printf("%d\n", network_nodes_number(x));

    assert(network_nodes_number(x) == 26);
    assert(network_links_number(x) == 2);
    

    printf("*****\nALL ASSERTIONS PASSED\n*****\n\n");

    return 0;
}