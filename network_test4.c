#include <assert.h>
#include <stdio.h>
#include "network.h"
#include "growingnet.h"
#include <time.h>
#include <string>

using namespace std;

int main()
{
    //Zwolnienie blokady maszyny losującej
    srand (time(NULL));

    //Przystępujemy do losowania sieci rosnących
    for (int j = 0; j < 100; j++)
    {
        network_new(rand() % 2);
    }

    //Oraz Twojego szczęśliwego numerka
    for (int j = 0; j < 100000; j++)
    {
        network_add_link(rand() % 100, to_string(rand()%200).c_str(), to_string(rand()%200).c_str());
        network_add_node(rand() % 100, to_string(rand()%200).c_str());
        network_remove_link(rand() % 100, to_string(rand()%200).c_str(), to_string(rand()%200).c_str());
        network_remove_node(rand() % 100, to_string(rand()%200).c_str());
        network_out_degree(rand() % 100, to_string(rand()%200).c_str());
        network_in_degree(rand() % 100, to_string(rand()%200).c_str());
        network_links_number(rand() % 100);
        network_nodes_number(rand() % 100);
        network_clear(rand() % 100000);
        network_delete(rand() % 300000);
    }

    printf("\n*****\nNOTHING CRASHED\n*****\n\n");

    return 0;
}
