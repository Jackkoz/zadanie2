#ifndef _GROWING_NET_H
#define _GROWING_NET_H


// We want to disable name mangling in order for the module
// to be usable with programs written in C.
#ifdef __cplusplus
extern "C" {
#endif  // #ifdef __cplusplus


// The constant below is the identifier of Global Growing Network.
// Nodes and links can be added to a growing network, but they cannot be deleted.
extern const unsigned long growingnet;


#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus

#endif  // #ifndef _GROWING_NET_H
