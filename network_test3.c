#include <assert.h>
#include <stdio.h>
#include "network.h"
#include "growingnet.h"

int main()
{
    unsigned long n;

    n = network_new(0);
    
    int i = 1;


    //Standardowe wstępne sprawdzenie, zawiera się w poprzednich testach
    network_add_link(n, "a", "b");
    assert(network_nodes_number(n) == 2);
    assert(network_links_number(n) == 1);
    assert(network_out_degree(n, "b") == 0);
    assert(network_out_degree(n, "a") == 1);
    assert(network_in_degree(n, "a") == 0);
    assert(network_in_degree(n, "b") == 1);  
    printf(">>>>>assert %d passed\n\n", i++);

    //Sprawdzenie stopni in/out nieistniejących wierzchołków
    assert(network_out_degree(n, "Natolin") == 0);
    assert(network_out_degree(n, "Młociny") == 0);
    assert(network_out_degree(n, "Kabaty") == 0);
    assert(network_in_degree(n, "Natolin") == 0);
    assert(network_in_degree(n, "Młociny") == 0);
    assert(network_in_degree(n, "Kabaty") == 0);
    printf(">>>>>assert %d passed\n\n", i++);


    //Próba dodania wierzchołków i węzłów do 1000 nieistniejących sieci
    for (int j = 5; j <= 1005; j++)
    {
        network_add_link(j, "a", "b");
        network_add_node(j, "c");
        network_add_link(j, "d", "e");
        network_add_node(j, "d");
        network_add_link(j, "d", "e");
        network_add_node(j, "e");
        network_add_link(j, "Natolin", "Kabaty");
        network_add_node(j, "Centrum");
    }

    //Sprawdzenie, czy przypadkiem nie zepsuło istniejącej sieci
    assert(network_nodes_number(n) == 2);
    assert(network_links_number(n) == 1);
    assert(network_out_degree(n, "b") == 0);
    assert(network_out_degree(n, "a") == 1);
    assert(network_in_degree(n, "a") == 0);
    assert(network_in_degree(n, "b") == 1);  
    printf(">>>>>assert %d passed\n\n", i++);

    //Dodanie 1000 razy istniejącego wierzchołka/krawędzi
     for (int j = 5; j <= 1005; j++)
    {
        network_add_link(j, "a", "b");
        network_add_node(j, "a");
        network_add_node(j, "b");
    }
    assert(network_nodes_number(n) == 2);
    assert(network_links_number(n) == 1);
    assert(network_out_degree(n, "b") == 0);
    assert(network_out_degree(n, "a") == 1);
    assert(network_in_degree(n, "a") == 0);
    assert(network_in_degree(n, "b") == 1);  
    printf(">>>>>assert %d passed\n\n", i++);

    //Test zwrotności sieci
    network_add_link(n, "x", "x");
    network_add_link(n, "y", "y");
    network_add_link(n, "x", "x");
    network_add_link(n, "y", "y");
    network_add_link(n, "x", "x");
    network_add_link(n, "y", "y");
    assert(network_nodes_number(n) == 4);
    assert(network_links_number(n) == 3);
    assert(network_out_degree(n, "b") == 0);
    assert(network_out_degree(n, "a") == 1);
    assert(network_in_degree(n, "a") == 0);
    assert(network_in_degree(n, "b") == 1);
    assert(network_out_degree(n, "x") == 1);
    assert(network_in_degree(n, "x") == 1); 
    assert(network_out_degree(n, "y") == 1);
    assert(network_in_degree(n, "y") == 1); 
    printf(">>>>>assert %d passed\n\n", i++);

    //Dalszy test
    network_add_link(n, "x", "y");
    assert(network_nodes_number(n) == 4);
    assert(network_links_number(n) == 4);
    assert(network_out_degree(n, "b") == 0);
    assert(network_out_degree(n, "a") == 1);
    assert(network_in_degree(n, "a") == 0);
    assert(network_in_degree(n, "b") == 1);
    assert(network_out_degree(n, "x") == 2);
    assert(network_in_degree(n, "x") == 1); 
    assert(network_out_degree(n, "y") == 1);
    assert(network_in_degree(n, "y") == 2); 
    printf(">>>>>assert %d passed\n\n", i++);

    network_add_link(n, "y", "x");
    assert(network_nodes_number(n) == 4);
    assert(network_links_number(n) == 5);
    assert(network_out_degree(n, "b") == 0);
    assert(network_out_degree(n, "a") == 1);
    assert(network_in_degree(n, "a") == 0);
    assert(network_in_degree(n, "b") == 1);
    assert(network_out_degree(n, "x") == 2);
    assert(network_in_degree(n, "x") == 2); 
    assert(network_out_degree(n, "y") == 2);
    assert(network_in_degree(n, "y") == 2); 
    printf(">>>>>assert %d passed\n\n", i++);


    printf("*****\nALL ASSERTIONS PASSED\n*****\n\n");

    return 0;
}
