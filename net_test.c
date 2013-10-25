#include "growingnet.h"
#include "network.h"

#include <stdio.h>

int main()
{
	printf("hello\n");
	
	unsigned long id = network_new(0);
	network_nodes_number(id);
	network_add_node(id, "test1");	
	network_add_node(id, "test2");
	network_new(0);
	network_new(0);
	network_nodes_number(id);
	network_clear(id);
	network_nodes_number(id);
	network_out_degree(id, "test1");
	network_add_link(id, "test1", "test2");
	network_out_degree(id, "test1");
	
	return 0;
}
