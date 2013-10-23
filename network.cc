/*
 * Założenia ogólne:
 * -> id sieci = klucz w mapie networks
 * -> id jest generowany w ten sposób, że gdy
 * 		nie ma w pamięci żadnej sieci, to id = 0, a w przeciwnym
 * 		razie nowe id = największe dotychczasowe id + 1
 * 
 * 
 * Konwencje formatowania kodu:
 * -> klamrę otwierającą zawsze piszemy w nowej linii
 * -> między ciałami funkcji dwie linie przerwy
 * -> wcięcia to (dla wygody) roboczo taby, natomiast przed oddaniem
 * 		pracy przekonwertujemy je na spacje
 * 
 */

#include "network.h"

#include <vector>
#include <map>
#include <iostream>
using namespace std;

#ifndef DEBUG_LEVEL
	#define DEBUG_LEVEL 0
#endif
const int debug = DEBUG_LEVEL;

/*** DEKLARACJE TYPÓW *************************************************/
//Multimapa mapuje wierzchołek źródłowy na parę <bool b, string s>, gdzie
//s - wierzchołek docelowy, b - true gdy krawędź w tym kierunku, f - gdy
//krawędź w przeciwnym kierunku
typedef multimap<string,pair<bool,string> > NET_MAP;

//Para <NET_MAP m, bool b>, gdzie m to multimapa jw., a b = true, gdy
//sieć jest rosnąca, false w p.p.
typedef pair<NET_MAP,bool> NETWORK;

typedef map<unsigned long,NETWORK> NETWORK_CONTAINER;
/**********************************************************************/

/*** DEKLARACJE ZMIENNYCH GLOBALNYCH **********************************/
NETWORK_CONTAINER networks;
/**********************************************************************/


unsigned long network_new(int growing)
{
	if (debug) cerr << "network_new(" << growing << "):" << endl;
	
	unsigned long new_id = -1;
	
	if (networks.size() == 0) new_id = 0;
	else new_id = networks.size();
	
	if (debug) cerr << "\tnetworks.size() == " << networks.size() << ", new_id = " << new_id << endl;
	
	networks.insert(make_pair(new_id, make_pair(NET_MAP(), growing)));
	
	return new_id;
}


void network_delete(unsigned long id)
{
	if (debug) cerr << "network_delete(" << id << "):" << endl;
	
	int n = networks.erase(id);
	
	if (debug) cerr << "\t" << n << " elements habe been deleted" << endl;
}


size_t network_nodes_number(unsigned long id)
{
	if (debug) cerr << "network_nodes_number(" << id << "):" << endl;
	
	//Net - iterator na naszą sieć (typu NETWORK)
	NETWORK_CONTAINER::iterator net = networks.find(id);
	
	//Gdy nie znaleziono sieci o tym kluczu, to ze specyfikacji zwracamy 0
	if (net == networks.end())
	{
		if (debug) cerr << "\tno network with given id found, returning 0" << endl;
		return 0;
	}
	
	NET_MAP::size_type unique_keys = 0;
	
	//Sklejam ze sobą pary o tych samych kluczach
	for (NET_MAP::iterator it = net->second.first.begin(); it != net->second.first.end(); it = net->second.first.upper_bound(it->first)) ++unique_keys;
	
	if (debug) cerr << "\tnetwork with given id consists of " << unique_keys << " nodes" << endl;
	
	return unique_keys;
}
