#ifndef _NETWORK_H
#define _NETWORK_H


// We want to disable name mangling in order for the module
// to be usable with programs written in C.
#ifdef __cplusplus
extern "C" {
#endif  // #ifdef __cplusplus


// Following header is needed for datatype `size_t`.
#include <stddef.h>


// Creates a new empty network and returns its id.
// An empty network is a network with empty set of nodes.
// Argument growing indicates whether the network is to be growing (growing != 0)
// or not (growing == 0).
unsigned long network_new(int growing);


// If there exists a network with given id then it is deleted,
// otherwise nothing is done.
void network_delete(unsigned long id);


// If there exists a network with given id then returns the number
// of nodes it contains, otherwise returns 0.
size_t network_nodes_number(unsigned long id);


// If there exists a network with given id then returns the number
// of links it contains, otherwise returns 0.
size_t network_links_number(unsigned long id);


// If there exists a network with given id, label != NULL
// and the network does not contain node named label then
// adds node named label to the network, otherwise nothing is done.
void network_add_node(unsigned long id, const char* label);


// If there exists a network with given id, slabel != NULL and tlabel != NULL,
// and the network does not contain link (slabel, tlabel) then adds link
// (slabel, tlabel) to the network, otherwise nothing is done.
// If the network does not contain either or both of the nodes marking the ends of the link
// then required node(s) is(are) added to the network as well.
void network_add_link(unsigned long id, const char* slabel, const char* tlabel);


// If there exists a network with given id and it contains node named label
// and the network is not growing then the node is deleted from the network
// along with its every incoming and outgoing link, otherwise nothing is done.
void network_remove_node(unsigned long id, const char* label);


// If there exists a network with given id and it contains link (slabel, tlabel)
// and the network is not growing then the link is deleted from the network,
// otherwise nothing is done.
void network_remove_link(unsigned long id, const char* slabel, const char* tlabel);


// If there exists a network with given id and it is not growing
// then each and every of its nodes and links are deleted, otherwise nothing is done.
void network_clear(unsigned long id);


// If there exists a network with given id and it contains node named label
// then it returns the number of the node's outgoing links, otherwise returns 0.
size_t network_out_degree(unsigned long id, const char* label);


// If there exists a network with given id and it contains node named label
// then it returns the number of the node's incoming links, otherwise returns 0.
size_t network_in_degree(unsigned long id, const char* label);


#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus

#endif  // #ifndef _NETWORK_H
