/*
 * General assumptions:
 * -> network: map [ node -> ( { incoming links }, { outgoing links } ) ]
 * -> network id = key in map networks
 * -> id is generated following this scheme:
 *      if there are no networks in memory, then id = 0, else
 *      new id = currently highest id + 1
 * 
 * 
 * Letters used to describe complexity:
 * -> N - the number of networks in memory
 * -> n - the number of nodes in given network
 * -> m - the number of links in given network
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
static const int debug = DEBUG_LEVEL;



/*** TYPES USED FOR FOR NETWORK CONTAINER *****************************/
// The type (label) held in a single node.
typedef string NODE;

// Incoming and outgoing edges of a single node.
// First set holds incoming edges, second set holds outgoing edges.
typedef pair<set<NODE>, set<NODE> > NODE_EDGES;

// Type to hold nodes and its edges.
typedef map<NODE, NODE_EDGES> NODE_MAP;

// Entire network plus flag saying wheter it shall be growing.
typedef pair<NODE_MAP, bool> NET;

// Container two hold multiples networks with their flags.
typedef map<unsigned long, NET> NET_CONTAINER;
/**********************************************************************/



/*** DECLARATIONS OF HELPER FUNCTIONS *********************************/
/*
 * This function returns a reference to global network map.
 * 
 * (Used to prevent the static init order fiasco.)
 * 
 */
NET_CONTAINER& networks();


/*
 * Returns true if and only if given network is growing.
 * 
 */
inline bool is_growing(const NET& net);


/*
 * Returns true if and only if given network contains given node.
 * 
 */
inline bool contains_node(const NODE_MAP& net_data, const char* label);


/*
 * Returns true if and only if given network contains given link.
 * 
 * WARNING: This function assumes, that both of the nodes exists, so
 * it HAS to be run AFTER making sure slabel and tlabel exist
 * 
 */
inline bool contains_link(const NODE_MAP& net_data, const char* slabel, const char* tlabel);
/**********************************************************************/



/*** DEBUG MESSEGES ***************************************************/
static const string CE_NETWORK_NOT_FOUND = "Network not found.";
static const string CE_NODE_NOT_FOUND = "Node not found.";
static const string CE_LINK_NOT_FOUND = "Link not found.";
static const string CE_LABEL_IS_NULL = "Label is NULL.";
static const string CE_NETWORK_IS_GROWING = "Network is growing.";
static const string CE_FATAL = "Fatal error encountered. Returning neutral value or void.";
/**********************************************************************/



/*** IMPLEMENTATIONS OF HELPER FUNCTIONS ******************************/
NET_CONTAINER& networks()
{
    static NET_CONTAINER networks;
    return networks;
}


inline bool is_growing(const NET& net)
{   
    return net.second;
}


inline bool contains_node(const NODE_MAP& net_data, const char* label)
{
    return net_data.count(label);
}


inline bool contains_link(const NODE_MAP& net_data, const char* slabel, const char* tlabel)
{
    // Both nodes should exist.
    assert(contains_node(net_data, slabel) && contains_node(net_data, tlabel));
    
    // Each outgoing link should also be an incoming link at the target node.
    assert(net_data.find(slabel)->second.second.count(tlabel) == net_data.find(tlabel)->second.first.count(slabel));
    
    return net_data.find(slabel)->second.second.count(tlabel);
}
/**********************************************************************/



/*** IMPLEMENTATIONS OF INTERFACE FUNCTIONS ***************************/
/* 
 * Complexity: O(log N)
 * 
 */
unsigned long network_new(int growing)
{
    static ios_base::Init _;
    
    if (debug)
    {
        cerr << "network_new(" << growing << "):" << endl;
    }
    
    unsigned long new_id;
    
    if (networks().size() == 0)
    {
        new_id = 0;
    }
    else
    {
        //Take the last existing element and add 1 to its id.
        new_id = networks().rbegin()->first + 1;
    }
    
    if (debug)
    {
        cerr << "\tnetworks.size() == " << networks().size()
            << ", new_id = " << new_id << endl;
    }
    
    networks().insert(make_pair(new_id, make_pair(NODE_MAP(), growing)));
    
    return new_id;
}


/*
 * Complexity: O(n + m + log N)
 * 
 */
void network_delete(unsigned long id)
{
    if (debug)
    {
        cerr << "network_delete(" << id << "):" << endl;
    }

    NET_CONTAINER::iterator net = networks().find(id);
    if (net == networks().end())
    {
        if (debug)
        {
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return;
    }
    
    // erase(id) returns number of keys removed from map
    size_t n = networks().erase(id);    
    
    if (debug)
    {
        cerr << "\tNumber of deleted networks: " << n << '.' << endl;
    }
}


/*
 * Complexity: O(log N)
 * 
 */
size_t network_nodes_number(unsigned long id)
{
    if (debug)
    {
        cerr << "network_nodes_number(" << id << "):" << endl;
    }
    
    NET_CONTAINER::iterator net = networks().find(id);
    if (net == networks().end())
    {
        if (debug)
        {
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return 0;
    }
        
    if (debug)
    {
        cerr << "\tGiven network contains "
                << net->second.first.size() << " nodes." << endl;
    }
    
    return net->second.first.size();
}


/*
 * Complexity: O(n + log N)
 * 
 */
size_t network_links_number(unsigned long id)
{
    if (debug)
    {
        cerr << "network_links_number(" << id << "):" << endl;
    }
    
    NET_CONTAINER::iterator net = networks().find(id);
    if (net == networks().end())
    {
        if (debug)
        {
            cerr << '\t' << CE_NETWORK_NOT_FOUND
                << ' ' << CE_FATAL << endl;
        }
        
        return 0;
    }
    
    size_t links_count = 0;
    
    NODE_MAP::const_iterator node = net->second.first.begin();
    
    // Sum number of incoming edges of each node.
    while (node != net->second.first.end())
    {
        links_count += node->second.first.size();
        ++node;
    }
    
    if (debug)
    {
        cerr << "\tGiven network contains " << links_count << " links." << endl;
    }
    
    return links_count;
}


/*
 * Complexity: O(log N + log n)
 * 
 */
void network_add_node(unsigned long id, const char* label)
{
    if (debug)
    {
        cerr << "network_add_node(" << id << ", " << label << "):" << endl;    
    }
    
    // This check is the fastest, so goes at the beginning
    if (!label)
    {
        if (debug)
        {
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl; 
        }
        return;
    }
    
    NET_CONTAINER::iterator net = networks().find(id);
    if (net == networks().end())
    {
        if (debug)
        {
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return;
    }
        
    if (contains_node(net->second.first, label))
    {
        if (debug)
        {
            cerr << "\tNode with given label already exists in given network."
                << ' ' << CE_FATAL << endl;
        }
        return;
    }
    
    // Insert the node.
    net->second.first.insert(make_pair(label, NODE_EDGES()));
    
    if (debug)
    {
        cerr << "\tNode with given label has been added to given network." << endl;
    }
}


/*
 * Complexity: O(log N + log n + log m)
 * 
 */
void network_add_link(unsigned long id, const char* slabel, const char* tlabel)
{
    if (debug)
    {
        cerr << "network_add_link(" << id << ", " << slabel << ", " << tlabel << "):" << endl;
    }

    if (!(slabel && tlabel))
    {
        if (debug)
        {
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;
        }
        return; 
    } 

    NET_CONTAINER::iterator net = networks().find(id);
    if (net == networks().end())
    {
        if (debug)
        {
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return; 
    }    
    
    // If some of the nodes are missing, add them.
    if (!contains_node(net->second.first, slabel))
    {
        if (debug)
        {
            cerr << "\tSource node does not exist, adding." << endl;
        }
        network_add_node(id, slabel);
    }
    if (!contains_node(net->second.first, tlabel))
    {
        if (debug)
        {
            cerr << "\tTarget node does not exist, adding." << endl;
        }
        network_add_node(id, tlabel);
    }
    
    // If the link already exists, do nothing.
    if (contains_link(net->second.first, slabel, tlabel))
    {
        if (debug)
        {
            cerr << "\tGiven link already exists." << ' ' << CE_FATAL << endl;
        }
        return;
    }
    
    // Source node
    NODE_MAP::iterator snode = net->second.first.find(slabel);
    assert(snode != net->second.first.end());
    
    // Target node
    NODE_MAP::iterator tnode = net->second.first.find(tlabel);
    assert(tnode != net->second.first.end());
    
    // Add the actual link (inform both source and target node).
    snode->second.second.insert(tlabel);
    tnode->second.first.insert(slabel);
    
    if (debug)
    {
        cerr << "\tLink added." << endl;
    }
}


/*
 * Complexity: O(k + log N + log n + log m)
 *      k - number of links starting or ending at the node "label"
 * 
 */
void network_remove_node(unsigned long id, const char* label)
{
    if (debug)
    {
        cerr << "network_remove_node(" << id << ", " << label << "):" << endl;
    }

    if (!label)
    {
        if (debug)
        {
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;
        }
        return;
    }
    
    NET_CONTAINER::iterator net = networks().find(id);
    if (net == networks().end())
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND
                        << ' ' << CE_FATAL << endl;
        return;
    }   

    //This check goes first, since its faster than the next one.
    if (is_growing(net->second))
    {
        if (debug) cerr << '\t' << CE_NETWORK_IS_GROWING
                        << ' ' << CE_FATAL << endl;
        return;
    }
    
    NODE_MAP::iterator node = net->second.first.find(label);
    if (node == net->second.first.end())
    {
        if (debug) cerr << '\t' << CE_NODE_NOT_FOUND
                        << ' ' << CE_FATAL << endl;
        return;
    }
    
    // Take care of the links coming into the node
    for (
            set<NODE>::iterator source_node = node->second.first.begin();
            source_node != node->second.first.end();
            ++source_node
        )
    {
        // Get the node our link comes from, and remove the info about it from that node
        net->second.first.find(*source_node)->second.second.erase(label);
    }
    
    // Take care of the links going out of the node
    for (
            set<NODE>::iterator target_node = node->second.second.begin();
            target_node != node->second.second.end();
            ++target_node
        )
    {
        // Find the node our link goes to, and remove the info about it from that node
        net->second.first.find(*target_node)->second.first.erase(label);
    }

    net->second.first.erase(net->second.first.find(label));
    if (debug)
    {
        cerr << "\tNode has been removed." << endl;
    }
}


/*
 * Complexity: O(log N + log n + log m)
 * 
 */
void network_remove_link(unsigned long id, const char* slabel, const char* tlabel)
{
    if (debug)
    {
        cerr << "network_remove_link(" << id << ", " << slabel
            << ", " << tlabel << "):" << endl;
    }

    if (!(slabel && tlabel))
    {
        if (debug)
        {
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;
        }
        return; 
    } 
    
    NET_CONTAINER::iterator net = networks().find(id);
    if (net == networks().end())
    {
        if (debug)
        {
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return;
    }
    
    if (is_growing(net->second))
    {
        if (debug)
        {
            cerr << '\t' << CE_NETWORK_IS_GROWING << ' ' << CE_FATAL << endl;
        }
        return;
    }
    
    if (!contains_node(net->second.first, slabel))
    {
        if (debug)
        {
            cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return;
    }
    if (!contains_node(net->second.first, tlabel))
    {
        if (debug)
        {
            cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return;
    }
        
    if (!contains_link(net->second.first, slabel, tlabel))
    {
        if (debug)
        {
            cerr << '\t' << CE_LINK_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return;
    }
    
    NODE_MAP::iterator snode = net->second.first.find(slabel);
    NODE_MAP::iterator tnode = net->second.first.find(tlabel);
    assert(snode != net->second.first.end() && tnode != net->second.first.end());
    
    snode->second.second.erase(tlabel);
    tnode->second.first.erase(slabel);
    
    if (debug)
    {
        cerr << "\tLink erased." << endl;
    }
}


/*
 * Complexity: O(n + m + log N)
 * 
 */
void network_clear(unsigned long id)
{
    if (debug) cerr << "network_clear(" << id << "):" << endl;
        
    NET_CONTAINER::iterator net = networks().find(id);
    if (net == networks().end())
    {
        if (debug)
        {
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return;
    }
    
    if (is_growing(net->second))
    {
        if (debug)
        {
            cerr << '\t' << CE_NETWORK_IS_GROWING << ' ' << CE_FATAL << endl;
        }
        return;
    }
        
    net->second.first.clear();
    if (debug)
    {
        cerr << "\tNetwork has been cleared." << endl;
    }
}


/*
 * Complexity: O(log N + log n)
 * 
 */
size_t network_out_degree(unsigned long id, const char* label)
{
    if (debug)
    {
        cerr << "network_out_degree(" << id << ", " << label << "):" << endl;
    }

    if (!label)
    {
        if (debug) cerr << '\t' << CE_LABEL_IS_NULL
                        << ' ' << CE_FATAL << endl; 
        return 0;
    }
    
    NET_CONTAINER::iterator net = networks().find(id);
    if (net == networks().end())
    {
        if (debug) cerr << '\t' << CE_NETWORK_NOT_FOUND
                        << ' ' << CE_FATAL << endl;
        return 0;
    }
    
    NODE_MAP::iterator node = net->second.first.find(label);
    if (node == net->second.first.end())    
    {
        if (debug) cerr << '\t' << CE_NODE_NOT_FOUND
                        << ' ' << CE_FATAL << endl;
        return 0;
    }    
    
    if (debug)
    {
        cerr << "\t" << node->second.second.size() << " outgoing edges have been found." << endl;
    }
    return node->second.second.size();
}


/*
 * Complexity: O(log N + log n)
 * 
 */
size_t network_in_degree(unsigned long id, const char* label)
{
    if (debug)
    {
        cerr << "network_out_degree(" << id << ", " << label << "):" << endl;
    }

    if (!label)
    {
        if (debug)
        {
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl; 
        }
        return 0;
    }
    
    NET_CONTAINER::iterator net = networks().find(id);
    if (net == networks().end())
    {
        if (debug)
        {
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return 0;
    }
    
    NODE_MAP::iterator node = net->second.first.find(label);
    if (node == net->second.first.end())
    {
        if (debug)
        {
            cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;
        }
        return 0;
    }    
        
    if (debug)
    {
        cerr << "\t" << node->second.first.size()
            << " outgoing edges have been found." << endl;
    }
    return node->second.first.size();
}
    
