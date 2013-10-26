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


/*** KOMUNIKATY *******************************************************/
const char CE_NETWORK_NOT_FOUND[] = "Network not found.";
const char CE_NODE_NOT_FOUND[] = "Node not found.";
const char CE_LINK_NOT_FOUND[] = "Link not found.";
const char CE_LABEL_IS_NULL[] = "Label is NULL.";
const char CE_NETWORK_IS_GROWING[] = "Network is growing.";
const char CE_FATAL[] = "Fatal error encountered. Returning neutral value or void.";
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
    
    //Take the last existing element and add 1 to its index.
    else new_id = networks.rbegin()->first + 1;
    
    if (debug) cerr << "\tnetworks.size() == " << networks.size() << ", new_id = " << new_id << endl;
    
    networks.insert(make_pair(new_id, make_pair(NET_DATA(), growing)));
    
    return new_id;
}


/*
 * If network "id" exists, remove it, otherwise
 * do nothing.
 * 
 * Complexity: O(n + m + log N)
 * 
 */
void network_delete(unsigned long id)
{
    if (debug) cerr << "network_delete(" << id << "):" << endl;

    //*** Pre-checks ***************************************************
    if (!exists(networks, id))
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return;
    }    
    NET_CON::iterator net = networks.find(id);
    
    //*** Actual code **************************************************
    int n = networks.erase(id);    
    if (debug) cerr << '\t' << n << " networks have been deleted" << endl;
}


/*
 * If network "id" exists, return its nodes number, otherwise
 * return 0.
 * 
 * Complexity: O(log N)
 * 
 */
size_t network_nodes_number(unsigned long id)
{
    if (debug) cerr << "network_nodes_number(" << id << "):" << endl;
    
    //*** Pre-checks ***************************************************
    if (!exists(networks, id))
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return 0;
    }
    
    NET_CON::iterator net = networks.find(id);
        
    //*** Actual code **************************************************
    if (debug) cerr << "\tGiven network consists out of " << net->second.first.size() << " keys" << endl;
    return net->second.first.size();
}


/*
 * If network "id" exists, return its links number, otherwise
 * return 0.
 * 
 * Complexity: O(n + log N)
 * 
 */
size_t network_links_number(unsigned long id)
{
    if (debug) cerr << "network_links_number(" << id << "):" << endl;
    
    //*** Pre-checks ***************************************************
    if (!exists(networks, id))
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return 0;
    }
    
    //*** Actual code **************************************************
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


/*
 * If network "id" exists and label != NULL, and
 * node "label" not in "id", add node "label" to "id".
 * Otherwise do nothing.
 * 
 * Complexity: O(log N + log n)
 * 
 */
void network_add_node(unsigned long id, const char* label)
{
    if (debug) cerr << "network_add_node(" << id << ", " << label << "):" << endl;    
    
    //*** Pre-checks ***************************************************
    //This check is the fastest, so goes at the very beginning
    if (!label)
    {
    	if (debug) cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;	
    	return;
    }
    
    if (!exists(networks, id))
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return;
    }
    NET_CON::iterator net = networks.find(id);
        
    if (contains_node(net->second.first, label))
    {
        if (debug) cerr << "\tNode with given label already exists in given network." << ' ' << CE_FATAL << endl;
        return;
    }
    
    //*** Actual code **************************************************
    net->second.first.insert(make_pair(label, NODE_VAL()));
    if (debug) cerr << "\tnode with given label has been added to given network" << endl;
}


/*
 * If network "id" exists, and slabel, tlabel != NULL, and
 * link (slabel -> tlabel) not in "id", add such link to "id";
 * otherwise do nothing.
 * If one of the given nodes not in "id", also add it.
 * 
 * Complexity: O(log N + log n + log m)
 * 
 */
void network_add_link(unsigned long id, const char* slabel, const char* tlabel)
{
    if (debug) cerr << "network_add_link(" << id << ", " << slabel << ", " << tlabel << "):" << endl;

    //*** Pre-checks ***************************************************
    if (!(slabel && tlabel))
    {
    	if (debug) cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;	
        return; 
    } 

    if (!exists(networks, id))
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return; 
    }
    NET_CON::iterator net = networks.find(id);
    
    //If some of the nodes are missing, add them
    if (!contains_node(net->second.first, slabel))
    {
        if (debug) cerr << "\tSource node does not exist, adding." << endl;
        network_add_node(id, slabel);
    }
    if (!contains_node(net->second.first, tlabel))
    {
        if (debug) cerr << "\tTarget node does not exist, adding." << endl;
        network_add_node(id, tlabel);
    }
    
    //If the link already exists, do nothing
    //WARNING: This function assumes, that both of the nodes exists, so
    // it HAS to be run after the two previous contains_node functions!
    if (contains_link(net->second.first, slabel, tlabel))
    {
        if (debug) cerr << "\tGiven link already exists." << ' ' << CE_FATAL << endl;
        return;
    }
    
    //*** Actual code **************************************************
    NET_DATA::iterator snode = net->second.first.find(slabel);
    NET_DATA::iterator tnode = net->second.first.find(tlabel);
    assert(snode != net->second.first.end());
    assert(tnode != net->second.first.end());
    
    //Add the actual link
    snode->second.second.insert(tlabel);
    tnode->second.first.insert(slabel);
    
    if (debug) cerr << "\tLink added." << endl;
}


/*
 * If network "id" exists, "label" in "id", and "id" is not growing,
 * remove "label" from "id" (including its in- and outgoing edges).
 * Otherwise do nothing.
 * 
 * Complexity: O(k + log N + log n + log m)
 *      k - number of links starting or ending at the node "label"
 * 
 */
void network_remove_node(unsigned long id, const char* label)
{
    if (debug) cerr << "network_remove_node(" << id << ", " << label << "):" << endl;

    //*** Pre-checks ***************************************************
    if (!label)
    {
    	if (debug) cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;	
        return;
    }
    
    if (!exists(networks, id))
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return;
    }   
    NET_CON::iterator net = networks.find(id);

    //This check goes first, since its faster than the next one
    if (is_growing(net))
    {
        if (debug) cerr << '\t' << CE_NETWORK_IS_GROWING << ' ' << CE_FATAL << endl;
        return;
    }
    
    if (!contains_node(net->second.first, label))
    {
        if (debug) cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;
        return;
    }
    NET_DATA::iterator node = net->second.first.find(label);
    
    //Take care of the links coming into the node
    for (set<NODE>::iterator source_node = node->second.first.begin(); source_node != node->second.first.end(); ++source_node)
    {
        //Get the node our link comes from, and remove the info about it from that node
    	net->second.first.find(*source_node)->second.second.erase(label);
    }
    
    //Take care of the links going out of the node
    for (set<NODE>::iterator target_node = node->second.second.begin(); target_node != node->second.second.end(); ++target_node)
    {
        //Find the node our link goes to, and remove the info about it from that node
    	net->second.first.find(*target_node)->second.first.erase(label);
    }

    //*** Actual code **************************************************
    net->second.first.erase(net->second.first.find(label));
}


/*
 * If a non-growing network "id" exists, and contains a link
 * (slabel, tlabel), remove the link. Otherwise do nothing.
 * 
 * Complexity: O(log N + log n + log m)
 * 
 */
void network_remove_link(unsigned long id, const char* slabel, const char* tlabel)
{
    if (debug) cerr << "network_remove_link(" << id << ", " << slabel << ", " << tlabel << "):" << endl;

    //*** Pre-checks ***************************************************
    if (!(slabel && tlabel))
    {
    	if (debug) cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;	
        return; 
    } 
    
    if (!exists(networks, id))
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return;
    }    
    NET_CON::iterator net = networks.find(id);
    
    if (is_growing(net))
    {
        if (debug) cerr << '\t' << CE_NETWORK_IS_GROWING << ' ' << CE_FATAL << endl;
        return;
    }
    
    if (!contains_node(net->second.first, slabel))
    {
        if (debug) cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;
        return;
    }
    if (!contains_node(net->second.first, tlabel))
    {
        if (debug) cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;
        return;
    }
        
    //WARNING: This function assumes, that both of the nodes exists, so
    //it HAS to be run after the two previous contains_node functions!
    if (!contains_link(net->second.first, slabel, tlabel))
    {
        if (debug) cerr << '\t' << CE_LINK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return;
    }
    
    NET_DATA::iterator snode = net->second.first.find(slabel);
    NET_DATA::iterator tnode = net->second.first.find(tlabel);
    assert(snode != net->second.first.end() && tnode != net->second.first.end());
    
    //*** Actual code **************************************************
    snode->second.second.erase(tlabel);
    tnode->second.first.erase(slabel);
    if (debug) cerr << "\tLink erased." << endl;
}


/*
 * If a non-growing network "id" exists, remove all its nodes and links,
 * otherwise do nothing.
 * 
 * Complexity: O(n + m + log N)
 * 
 */
void network_clear(unsigned long id)
{
    if (debug) cerr << "network_clear(" << id << "):" << endl;
        
    //*** Pre-checks ***************************************************
    if (!exists(networks, id))
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return;
    }    
    NET_CON::iterator net = networks.find(id);      //O(log N)
    
    if (is_growing(net))
    {
        if (debug) cerr << '\t' << CE_NETWORK_IS_GROWING << ' ' << CE_FATAL << endl;
        return;
    }
        
    //*** Actual code **************************************************
    net->second.first.clear();
    if (debug) cerr << "\tnetwork has been cleared" << endl;
}


/*
 * If network "id" exists and contains node "label", return the count
 * of links going out from "label". Otherwise return 0.
 * 
 * Complexity: O(log N + log n)
 * 
 */
size_t network_out_degree(unsigned long id, const char* label)
{
    if (debug) cerr << "network_out_degree(" << id << ", " << label << "):" << endl;

    //*** Pre-checks ***************************************************
    if (!label)
    {
    	if (debug) cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;	
        return 0;
    }
    
    if (!exists(networks, id))
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return 0;
    }
    NET_CON::iterator net = networks.find(id);      //O(log N)
    
    if (!contains_node(net->second.first, label))
    {
        if (debug) cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;
        return 0;
    }    
    NET_DATA::iterator node = net->second.first.find(label);
    
    //*** Actual code **************************************************
    if (debug) cerr << "\t" << node->second.second.size() << " outgoing edges have been found" << endl;
    return node->second.second.size();
}


/*
 * If network "id" exists and contains node "label", return the count
 * of links coming into "label". Otherwise return 0.
 * 
 * Complexity: O(log N + log n)
 * 
 */
size_t network_in_degree(unsigned long id, const char* label)
{
    if (debug) cerr << "network_out_degree(" << id << ", " << label << "):" << endl;

    //*** Pre-checks ***************************************************
    if (!label)
    {
    	if (debug) cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;	
        return 0;
    }
    
    if (!exists(networks, id))
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        return 0;
    }
    NET_CON::iterator net = networks.find(id);      //O(log N)
    
    if (!contains_node(net->second.first, label))
    {
        if (debug) cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;
        return 0;
    }    
    NET_DATA::iterator node = net->second.first.find(label);
        
    //*** Actual code **************************************************
    if (debug) cerr << "\t" << node->second.first.size() << " outgoing edges have been found" << endl;
    return node->second.first.size();
}
    
