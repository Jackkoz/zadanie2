/*
 * Authors: Hubert Tarasiuk #335965, Jacek Koziński #334678
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
using namespace std;


#ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL 0
#endif


/*** TYPES USED FOR FOR NETWORK CONTAINER *****************************/
// The type (label) held in a single node.
typedef string NODE;

// Incoming and outgoing links of a single node.
// First set holds incoming links, second set holds outgoing links.
typedef pair<set<NODE>, set<NODE> > NODE_LINKS;

// Type to hold nodes and their links.
typedef map<NODE, NODE_LINKS> NODE_MAP;

// Entire network plus flag saying wheter it shall be growing.
typedef pair<NODE_MAP, bool> NET;

// Container to hold multiples networks with their flags.
typedef map<unsigned long, NET> NET_CONTAINER;
/**********************************************************************/



/*** DEBUG MESSEGES ***************************************************/
static const string CE_NETWORK_NOT_FOUND = "Network not found.";
static const string CE_NODE_NOT_FOUND = "Node not found.";
static const string CE_LINK_NOT_FOUND = "Link not found.";
static const string CE_LABEL_IS_NULL = "Given a NULL label.";
static const string CE_NETWORK_IS_GROWING = "Network is growing.";
static const string CE_FATAL = "Cannot continue. Returning neutral value or void.";
/**********************************************************************/



/*** DECLARATIONS OF HELPER FUNCTIONS *********************************/
// This function returns a reference to global network map.
static NET_CONTAINER& networks();

// This function returns the debug level and ensures
// proper iostream initialization if necessary.
static int debug();

// Returns true if and only if given network is growing.
static inline bool is_growing(const NET& net);
/**********************************************************************/



/*** IMPLEMENTATION OF HELPER FUNCTIONS *******************************/
static NET_CONTAINER& networks()
{
    static NET_CONTAINER networks;
    return networks;
}


static int debug()
{
    if (DEBUG_LEVEL)
    {
        // Ensures that console streams are initialized.
        static ios_base::Init _;
    }
    return DEBUG_LEVEL;
}


static inline bool is_growing(const NET& net)
{   
    return net.second;
}
/**********************************************************************/



/*** IMPLEMENTATION OF INTERFACE FUNCTIONS ****************************/
// Complexity: O(log N)
unsigned long network_new(int growing)
{
    if (debug())
        cerr << "network_new(" << growing << "):" << endl;
    
    static unsigned long new_id = 0;
    
    ++new_id;
    
    networks()[new_id] = make_pair(NODE_MAP(), growing);

    if (debug())
    {
        cerr << "\tCreated a " << (growing ? "growing" : "non-growing")
                << " network with id=" << new_id << '.' << endl;
        cerr << "\tCurrently a total of " << networks().size()
                << " networks in memory." << endl;
    }
    
    
    return new_id;
}


// Complexity: O(n + m + log N)
void network_delete(unsigned long id)
{
    if (debug())
        cerr << "network_delete(" << id << "):" << endl;

    if (!networks().count(id))
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;

        return;
    }
    
    // erase(id) returns number of keys removed from map.
    size_t n_deleted = networks().erase(id);    
    
    if (debug())
    {
        cerr << "\tNumber of deleted networks: " << n_deleted << '.' << endl;
        cerr << "\tNumber of networks left: " << networks().size() << '.' << endl;
    }
}


// Complexity: O(log N)
size_t network_nodes_number(unsigned long id)
{
    if (debug())
        cerr << "network_nodes_number(" << id << "):" << endl;
    
    NET_CONTAINER::iterator net_it = networks().find(id);
    if (net_it == networks().end())
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
            
        return 0;
    }
    
    NODE_MAP& node_map = net_it->second.first;
    
    size_t n_nodes = node_map.size();
        
    if (debug())
    {
        cerr << "\tGiven network contains "
                << n_nodes << " nodes." << endl;
    }
    
    return n_nodes;
}


// Complexity: O(n + log N)
size_t network_links_number(unsigned long id)
{
    if (debug())
        cerr << "network_links_number(" << id << "):" << endl;
    
    NET_CONTAINER::iterator net_it = networks().find(id);
    if (net_it == networks().end())
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;

        return 0;
    }
    
    NODE_MAP& node_map = net_it->second.first;
    
    size_t n_links = 0;
    
    // Sum up number of incoming links for each node.
    NODE_MAP::const_iterator node_it = node_map.begin();    
    while (node_it != node_map.end())
    {
        n_links += node_it->second.first.size();
        ++node_it;
    }
    
    if (debug())
        cerr << "\tGiven network contains " << n_links << " links." << endl;
    
    return n_links;
}


// Complexity: O(log N + log n)
void network_add_node(unsigned long id, const char* label)
{
    if (debug())
    {
        cerr << "network_add_node(" << id << ", "
                << (label ? label : "NULL") << "):" << endl;    
    }
    
    // This check is the fastest, so it goes at the beginning
    if (!label)
    {
        if (debug())
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl; 

        return;
    }
    
    NET_CONTAINER::iterator net_it = networks().find(id);
    if (net_it == networks().end())
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;
        
        return;
    }
    
    NODE_MAP& node_map = net_it->second.first;
        
    if (node_map.count(label))
    {
        if (debug())
        {
            cerr << "\tNode with given label already exists in given network."
                    << ' ' << CE_FATAL << endl;
        }
        
        return;
    }
    
    // Insert the node (with no links for now).
    node_map[label] = NODE_LINKS();
    
    if (debug())
        cerr << "\tNode with given label has been added to given network." << endl;
}


// Complexity: O(log N + log n + log m)
void network_add_link(unsigned long id, const char* slabel, const char* tlabel)
{
    if (debug())
    {
        cerr << "network_add_link(" << id << ", "
                << (slabel ? slabel : "NULL") << ", "
                << (tlabel ? tlabel : "NULL") << "):" << endl;
    }

    if (!(slabel && tlabel))
    {
        if (debug())
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;

        return; 
    } 

    NET_CONTAINER::iterator net_it = networks().find(id);
    if (net_it == networks().end())
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;

        return; 
    }
    
    NODE_MAP& node_map = net_it->second.first;
    
    
    if (debug())
    {
        if (!node_map.count(slabel))
            cerr << "\tSource node will be created." << endl;
        
        if (!node_map.count(tlabel))
            cerr << "\tTarget node will be created." << endl;
    }
    
    // We'll use the second element to find out whether the link existed before.
    pair<set<NODE>::iterator, bool> result;
    
    // Inform the source node about the link.
    result = node_map[slabel].second.insert(tlabel);
    // Inform the target node about the link.
    node_map[tlabel].first.insert(slabel);
    
    if (debug())
    {
        if (result.second)
            cerr << "\tLink added." << endl;
        else
            cerr << "\tGiven link already exists." << endl;
    }
}


// Complexity: O(k + log N + log n + log m)
//      k - number of links starting or ending at the node "label"
void network_remove_node(unsigned long id, const char* label)
{
    if (debug())
    {
        cerr << "network_remove_node(" << id << ", "
                << (label ? label : "NULL") << "):" << endl;
    }

    if (!label)
    {
        if (debug())
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;

        return;
    }
    
    NET_CONTAINER::iterator net_it = networks().find(id);
    if (net_it == networks().end())
    {
        if (debug())
        {
            cerr << '\t' << CE_NETWORK_NOT_FOUND
                    << ' ' << CE_FATAL << endl;
        }
        return;
    }   

    if (is_growing(net_it->second))
    {
        if (debug())
        {
            cerr << '\t' << CE_NETWORK_IS_GROWING
                    << ' ' << CE_FATAL << endl;
        }
        return;
    }
    
    NODE_MAP& node_map = net_it->second.first;
    
    NODE_MAP::iterator node_it = node_map.find(label);
    if (node_it == node_map.end())
    {
        if (debug())
        {
            cerr << '\t' << CE_NODE_NOT_FOUND
                    << ' ' << CE_FATAL << endl;
        }
        return;
    }
    
    NODE_LINKS& node_links = node_it->second;
    
    // Remove incoming links
    set<NODE>::iterator source_node_it = node_links.first.begin();
    while (source_node_it != node_links.first.end())
    {
        network_remove_link(id, source_node_it->c_str(), label);        
        ++source_node_it;
    }
    
    // Remove outgoing links
    set<NODE>::iterator target_node_it = node_links.second.begin();
    while (target_node_it != node_links.second.end())
    {
        network_remove_link(id, label, target_node_it->c_str());
        ++target_node_it;
    }

    // Remove the actual node
    node_map.erase(node_it);
    
    if (debug())
        cerr << "\tNode has been removed." << endl;
}


// Complexity: O(log N + log n + log m)
void network_remove_link(unsigned long id, const char* slabel, const char* tlabel)
{
    if (debug())
    {        
        cerr << "network_remove_link(" << id << ", "
                << (slabel ? slabel : "NULL") << ", "
                << (tlabel ? tlabel : "NULL") << "):" << endl;
    }

    if (!(slabel && tlabel))
    {
        if (debug())
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl;

        return; 
    } 
    
    NET_CONTAINER::iterator net_it = networks().find(id);
    if (net_it == networks().end())
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;

        return;
    }
    
    if (is_growing(net_it->second))
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_IS_GROWING << ' ' << CE_FATAL << endl;

        return;
    }
    
    NODE_MAP& node_map = net_it->second.first;
    
    NODE_MAP::iterator snode = node_map.find(slabel);    
    if (snode == node_map.end())
    {
        if (debug())
            cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;

        return;
    }
    
    NODE_MAP::iterator tnode = node_map.find(tlabel);
    if (tnode == node_map.end())
    {
        if (debug())
            cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;

        return;
    }

    // Check if the link actually exists.
    if (!node_map.find(slabel)->second.second.count(tlabel))
    {
        if (debug())
            cerr << '\t' << CE_LINK_NOT_FOUND << ' ' << CE_FATAL << endl;

        return;
    }

    // Tell the source node about the erasure.
    snode->second.second.erase(tlabel);
    
    // Tell the target node about the erasure.
    tnode->second.first.erase(slabel);
    
    if (debug())
        cerr << "\tLink erased." << endl;
}


// Complexity: O(n + m + log N)
void network_clear(unsigned long id)
{
    if (debug())
        cerr << "network_clear(" << id << "):" << endl;
        
    NET_CONTAINER::iterator net_it = networks().find(id);
    if (net_it == networks().end())
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;

        return;
    }
    
    if (is_growing(net_it->second))
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_IS_GROWING << ' ' << CE_FATAL << endl;

        return;
    }
    
    NODE_MAP& node_map = net_it->second.first;
        
    node_map.clear();
    
    if (debug())
        cerr << "\tNetwork has been cleared." << endl;
}


// Complexity: O(log N + log n)
size_t network_out_degree(unsigned long id, const char* label)
{
    if (debug())
    {
        cerr << "network_out_degree(" << id << ", "
                << (label ? label : "NULL") << "):" << endl;
    }

    if (!label)
    {
        if (debug())
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl; 

        return 0;
    }
    
    NET_CONTAINER::iterator net_it = networks().find(id);
    if (net_it == networks().end())
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;

        return 0;
    }
    
    NODE_MAP& node_map = net_it->second.first;
    
    NODE_MAP::iterator node_it = node_map.find(label);
    if (node_it == node_map.end())    
    {
        if (debug())
            cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;

        return 0;
    }
    
    size_t out_degree = node_it->second.second.size();
    
    if (debug())
        cerr << "\t" << out_degree << " outgoing links have been found." << endl;

    return out_degree;
}


// Complexity: O(log N + log n)
size_t network_in_degree(unsigned long id, const char* label)
{
    if (debug())
    {
        cerr << "network_in_degree(" << id << ", "
                << (label ? label : "NULL") << "):" << endl;
    }

    if (!label)
    {
        if (debug())
            cerr << '\t' << CE_LABEL_IS_NULL << ' ' << CE_FATAL << endl; 

        return 0;
    }
    
    NET_CONTAINER::iterator net_it = networks().find(id);
    if (net_it == networks().end())
    {
        if (debug())
            cerr << '\t' << CE_NETWORK_NOT_FOUND << ' ' << CE_FATAL << endl;

        return 0;
    }
    
    NODE_MAP& node_map = net_it->second.first;
    
    NODE_MAP::iterator node_it = node_map.find(label);
    if (node_it == node_map.end())
    {
        if (debug())
            cerr << '\t' << CE_NODE_NOT_FOUND << ' ' << CE_FATAL << endl;

        return 0;
    }   
    
    size_t in_degree = node_it->second.first.size(); 
        
    if (debug())
        cerr << "\t" << in_degree << " incoming links have been found." << endl;

    return in_degree;
}
    
