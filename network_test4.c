#include <assert.h>
#include <stdio.h>
#include "network.h"
#include "growingnet.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

char buff1[64];
char buff2[64];

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
        sprintf(buff1, "%d", rand()%200);
        sprintf(buff2, "%d", rand()%200);
        network_add_link(rand() % 100, buff1, buff2);
        
        sprintf(buff1, "%d", rand()%200);
        network_add_node(rand() % 100, buff1);
        
        sprintf(buff1, "%d", rand()%200);
        sprintf(buff2, "%d", rand()%200);
        network_remove_link(rand() % 100, buff1, buff2);
        
        sprintf(buff1, "%d", rand()%100);
        network_remove_node(rand() % 100, buff1);
        
        sprintf(buff1, "%d", rand()%200);
        network_out_degree(rand() % 100, buff1);
        network_in_degree(rand() % 100, buff1);
        
        network_links_number(rand() % 100);        
        
        sprintf(buff1, "%d", rand()%200);
        sprintf(buff2, "%d", rand()%200);
        network_nodes_number(rand() % 100);
        
        network_clear(rand() % 100000);
        
        network_delete(rand() % 300000);
    }

    printf("\n*****\nNOTHING CRASHED\n*****\n\n");

    return 0;
}
