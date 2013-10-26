/*
 * Założenia ogólne:
 * -> sieć: mapa [ wierzchołek -> ( { krawędzie wchodzące }, { krawędzie wychodzące } ) ]
 * -> id sieci = klucz w mapie networks
 * -> id jest generowany w ten sposób, że gdy
 *      nie ma w pamięci żadnej sieci, to id = 0, a w przeciwnym
 *      razie nowe id = największe dotychczasowe id + 1
 * 
 * 
 * Konwencje formatowania kodu:
 * -> klamrę otwierającą zawsze piszemy w nowej linii
 * -> między ciałami funkcji dwie linie przerwy
 * -> wcięcia to (dla wygody) roboczo taby, natomiast przed oddaniem
 *      pracy przekonwertujemy je na spacje
 * 
 * 
 * Nomenklatura na potrzeby określania złożoności:
 * -> N - liczba sieci w pamięci
 * -> n - liczba wierzchołków w danej sieci
 * -> m - liczba krawędzi w danej sieci
 * 
 */

#include "network.h"

#include <map>
#include <set>
#include <iostream>
#include <cassert>
using namespace std;

#ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL 0
#endif
const int debug = DEBUG_LEVEL;

/*** DEKLARACJE TYPÓW *************************************************/
typedef string NODE;
//First set holds incoming edges, second set holds outgoing edges
typedef pair<set<NODE>,set<NODE> > NODE_VAL;
typedef map<NODE,NODE_VAL> NET_DATA;
typedef pair<NET_DATA,bool> NET;
typedef map<unsigned long,NET> NET_CON;
/**********************************************************************/


/*** DEKLARACJE ZMIENNYCH GLOBALNYCH **********************************/
NET_CON networks;
/**********************************************************************/


/*** DEKLARACJE FUNKCJI POMOCNICZYCH **********************************/
inline bool exists(const NET_CON& networks, const unsigned long id);
inline bool is_growing(const NET_CON::iterator& net);
inline bool contains_node(const NET_DATA& net_data, const char* label);
inline bool contains_link(const NET_DATA& net_data, const char* slabel, const char* tlabel);
/**********************************************************************/


/*** IMPLEMENTACJE FUNKCJI POMOCNICZYCH *******************************/
inline bool exists(const NET_CON& networks, const unsigned long id)
{
    NET_CON::const_iterator net = networks.find(id);
    return net != networks.end();
}


inline bool is_growing(const NET_CON::iterator& net)
{   
    return net->second.second;
}

inline bool contains_node(const NET_DATA& net_data, const char* label)
{
    NET_DATA::const_iterator node = net_data.find(label);
    return node != net_data.end();
}

inline bool contains_link(const NET_DATA& net_data, const char* slabel, const char* tlabel)
{
    assert(contains_node(net_data, slabel) && contains_node(net_data, tlabel));
    NET_DATA::const_iterator snode = net_data.find(slabel);
    return snode->second.second.count(tlabel);
}
/**********************************************************************/

/*
 * Create new empty network and return its id.
 * 
 * Parameter growing says, wheter the new netrowk
 * shall be growing (growing != 0) or not (growing == 0).
 * 
 * Complexity: O(1)
 * 
 */
unsigned long network_new(int growing)
{
    if (debug) cerr << "network_new(" << growing << "):" << endl;
    
    unsigned long new_id = -1;
    
    if (networks.size() == 0) new_id = 0;
    
    //Biorę ostatni element i do jego klucza dodaję 1
    else new_id = networks.rbegin()->first + 1;
    
    if (debug) cerr << "\tnetworks.size() == " << networks.size() << ", new_id = " << new_id << endl;
    
    networks.insert(make_pair(new_id, make_pair(NET_DATA(), growing)));
    
    return new_id;
}


//Złożoność: O(log N)
void network_delete(unsigned long id)
{
    if (debug) cerr << "network_delete(" << id << "):" << endl;

    NET_CON::iterator net = networks.find(id);      //O(log N)

    if (net == networks.end())
    {
    	if (debug)
    	{
    		cerr << "No network with given id. Aborting.\n";
    	}
    	return;
    }
    
    int n = networks.erase(id);
    
    if (debug) cerr << "\t" << n << " networks have been deleted" << endl;
}


//Złożoność: O(log N)
size_t network_nodes_number(unsigned long id)
{
    if (debug) cerr << "network_nodes_number(" << id << "):" << endl;
    
    //Net - iterator na naszą sieć (typu NETWORK)
    NET_CON::iterator net = networks.find(id);      //O(log N)
    
    //Gdy nie znaleziono sieci o tym kluczu, to ze specyfikacji zwracamy 0
    if (net == networks.end())
    {
        if (debug) cerr << "\tno network with given id found, returning 0" << endl;
        return 0;
    }
        
    if (debug) cerr << "\tgiven network consists out of " << net->second.first.size() << " keys" << endl;
    return net->second.first.size();
}


//Złożoność: O(log N + log m)
void network_add_node(unsigned long id, const char* label)
{
    if (debug) cerr << "network_add_node(" << id << ", " << label << "):" << endl;
    
    //Gdy pusty napis, to nic nie robię
    if (label == NULL)
    {
    	if (debug)
    	{
    		cerr << "No node with given label. Aborting.\n";
    	}
    	return;
    }
    
    NET_CON::iterator net = networks.find(id);      //O(log N)
    
    //Gdy sieć nie istnieje, to nic nie robię
    if (net == networks.end())
    {
        if (debug) cerr << "\tno network with given id found" << endl;
        return;
    }
    
    NET_DATA::iterator node = net->second.first.find(label);    //O(log m)
    
    //Gdy element o etykiecie label istnieje, nic nie robię
    if (node != net->second.first.end())
    {
        if (debug) cerr << "\tnode with given label already exists in given network, returning" << endl;
        return;
    }
    
    //Dodaję element
    net->second.first.insert(make_pair(label, NODE_VAL()));
    if (debug) cerr << "\tnode with given label has been added to given network" << endl;
}


void network_clear(unsigned long id)
{
    if (debug) cerr << "network_clear(" << id << "):" << endl;
    
    //Net - iterator na naszą sieć (typu NETWORK)
    NET_CON::iterator net = networks.find(id);      //O(log N)
    
    //Gdy nie znaleziono sieci o tym kluczu, to nic nie robimy
    if (net == networks.end())
    {
        if (debug) cerr << "\tno network with given id found, returning" << endl;
        return;
    }
    
    //Gdy sieć jest rosnąca, nic nie robimy
    if (is_growing(net))
    {
        if (debug) cerr << "\tgrowing network, can't clear, returning" << endl;
        return;
    }
        
    net->second.first.clear();
    if (debug) cerr << "\tnetwork has been cleared" << endl;
}


size_t network_out_degree(unsigned long id, const char* label)
{
    if (debug) cerr << "network_out_degree(" << id << ", " << label << "):" << endl;
    
    //Net - iterator na naszą sieć (typu NETWORK)
    NET_CON::iterator net = networks.find(id);      //O(log N)
    
    //Gdy nie znaleziono sieci o tym kluczu, zwracamy 0
    if (net == networks.end())
    {
        if (debug) cerr << "\tno network with given id found, returning 0" << endl;
        return 0;
    }
    
    NET_DATA::iterator node = net->second.first.find(label);
    if (node == net->second.first.end())
    {
        if (debug) cerr << "\tno node with this label found, returning 0" << endl;
        return 0;
    }
    
    if (debug) cerr << "\t" << node->second.second.size() << " outgoing edges have been found" << endl;
    
    return node->second.second.size();
}


size_t network_in_degree(unsigned long id, const char* label)
{
    if (debug) cerr << "network_in_degree(" << id << "):" << endl;
        
    NET_CON::iterator net = networks.find(id);    
    //If network does not exist, return 0
    if (net == networks.end())
    {
        if (debug) cerr << "\tno network with given id found, returning 0" << endl;
        return 0;
    }
    
    NET_DATA::iterator node = net->second.first.find(label);
    //If node does not exist, return 0
    if (node == net->second.first.end())
    {
        if (debug) cerr << "\tno node with this label found, returning 0" << endl;
        return 0;
    }
    
    if (debug) cerr << "\t" << node->second.first.size() << " incoming edges have been found" << endl;
    
    return node->second.first.size();
}
    

void network_add_link(unsigned long id, const char* slabel, const char* tlabel)
{
    if (debug) 
    {
        cerr << "network_add_link(" << id << ", " << slabel << ", " << tlabel << "):\n";
    }

    //If either label is null - do nothing
    if (slabel == NULL || tlabel == NULL)
    {
        if (debug)
        {
            cerr << "\tAt least one of the labels is NULL. Aborting\n";
        }
        return; 
    } 

    NET_CON::iterator net = networks.find(id);      //O(log N)

    //If no network with given id exists - do nothing
    if (net == networks.end())
    {
        if (debug)
        {
            cerr << "\tNo network with given id found. Aborting network_add_link.\n";
        }
        return; 
    }
    
    if (!contains_node(net->second.first, slabel))
    {
        network_add_node(id, slabel);
        if (debug) cerr << "\tSource node does not exist, adding." << endl;
    }
    if (!contains_node(net->second.first, tlabel))
    {
        network_add_node(id, tlabel);
        if (debug) cerr << "\tTarget node does not exist, adding." << endl;
    }
    
    if (contains_link(net->second.first, slabel, tlabel))
    {
        if (debug) cerr << "\tGiven link already exists, returning." << endl;
        return;
    }
    
    NET_DATA::iterator snode = net->second.first.find(slabel);
    NET_DATA::iterator tnode = net->second.first.find(tlabel);
    assert(snode != net->second.first.end());
    assert(tnode != net->second.first.end());
    
    snode->second.second.insert(tlabel);
    tnode->second.first.insert(slabel);
    
    if (debug) cerr << "\tLink added." << endl;
}


void network_remove_node(unsigned long id, const char* label)
{
    if (debug) cerr << "network_remove_node(" << id << ", " << label << "):\n";

    if (label == NULL)
    {
        if (debug) cerr << "\tLabel is null. Aborting.\n";
        return;
    }

    NET_CON::iterator net = networks.find(id);        //O(log N)

    //If no network with given id exists - do nothing
    if (net == networks.end())
    {
        if (debug) cerr << "\tNo network with given id found. Aborting.\n";
        return;
    }   

    if (!contains_node(net->second.first, label))
    {
        if (debug) cerr <<"\tNo such node in this network, returning." << endl;
        return;
    }

    NET_DATA::iterator node = net->second.first.find(label);
    set<NODE>::iterator it;

    for (it = node->second.first.begin(); it != node->second.first.end(); it++)
    {
    	net->second.first.find(*it)->second.second.erase(label);
    }
    
    for (it = node->second.second.begin(); it != node->second.second.end(); it++)
    {
    	net->second.first.find(*it)->second.first.erase(label);
    }

    net->second.first.erase(net->second.first.find(label));
}


void network_remove_link(unsigned long id, const char* slabel, const char* tlabel)
{
    if (debug) cerr << "network_remove_link(" << id << ", " << slabel << ", " << tlabel << "):" << endl;
    if (!exists(networks, id))
    {
        if (debug) cerr << "\tNo network with given id found. Returning 0.\n";
        return;
    }
    
    NET_CON::iterator net = networks.find(id);
    
    if (!contains_node(net->second.first, slabel))
    {
        if (debug) cerr << "\tSource node does not exist, returning." << endl;
        return;
    }
    if (!contains_node(net->second.first, tlabel))
    {
        if (debug) cerr << "\tTarget node does not exist, returning." << endl;
        return;
    }
    if (!contains_link(net->second.first, slabel, tlabel))
    {
        if (debug) cerr << "\tNo such link in network, returning." << endl;
        return;
    }
    
    NET_DATA::iterator snode = net->second.first.find(slabel);
    NET_DATA::iterator tnode = net->second.first.find(tlabel);
    assert(snode != net->second.first.end() && tnode != net->second.first.end());
    
    snode->second.second.erase(tlabel);
    tnode->second.first.erase(slabel);
    if (debug) cerr << "\tLink erased." << endl;
}


size_t network_links_number(unsigned long id)
{
    if (debug) cerr << "network_links_number(" << id << "):" << endl;
    if (!exists(networks, id))
    {
        if (debug) cerr << "\tNo network with given id found. Returning 0.\n";
        return 0;
    }
    
    NET_CON::iterator net = networks.find(id);
    
    size_t links_count = 0;
    
    NET_DATA::const_iterator node = net->second.first.begin();
    
    while (node != net->second.first.end())
    {
        links_count += (*node++).second.first.size();
    }
    
    if (debug) cerr << "\tGiven network has " << links_count << " links" << endl;
    
    return links_count;
}
