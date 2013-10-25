/*
 * Założenia ogólne:
 * -> id sieci = klucz w mapie networks
 * -> id jest generowany w ten sposób, że gdy
 * 		nie ma w pamięci żadnej sieci, to id = 0, a w przeciwnym
 * 		razie nowe id = największe dotychczasowe id + 1
 * -> informację o istnieniu wierzchołka przechowujemy jako
 * 		pętlę od danego wierzchołka do niego samego (wartość bool
 * 		jest w tym przypadku niezdefiniowana)
 * 
 * 
 * Konwencje formatowania kodu:
 * -> klamrę otwierającą zawsze piszemy w nowej linii
 * -> między ciałami funkcji dwie linie przerwy
 * -> wcięcia to (dla wygody) roboczo taby, natomiast przed oddaniem
 * 		pracy przekonwertujemy je na spacje
 * 
 * 
 * Nomenklatura na potrzeby określania złożoności:
 * -> N - liczba sieci w pamięci
 * -> n - liczba wierzchołków w danej sieci
 * -> m - liczba krawędzi w danej sieci
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


//Złożoność: O(1)
unsigned long network_new(int growing)
{
	if (debug) cerr << "network_new(" << growing << "):" << endl;
	
	unsigned long new_id = -1;
	
	if (networks.size() == 0) new_id = 0;
	
	//Biorę ostatni element i do jego klucza dodaję 1
	else new_id = networks.rbegin()->first + 1;
	
	if (debug) cerr << "\tnetworks.size() == " << networks.size() << ", new_id = " << new_id << endl;
	
	networks.insert(make_pair(new_id, make_pair(NET_MAP(), growing)));
	
	return new_id;
}


//Złożoność: O(log N)
void network_delete(unsigned long id)
{
	if (debug) cerr << "network_delete(" << id << "):" << endl;
	
	int n = networks.erase(id);
	
	if (debug) cerr << "\t" << n << " elements habe been deleted" << endl;
}


//Złożoność: O(n + log m + log N)
size_t network_nodes_number(unsigned long id)
{
	if (debug) cerr << "network_nodes_number(" << id << "):" << endl;
	
	//Net - iterator na naszą sieć (typu NETWORK)
	NETWORK_CONTAINER::iterator net = networks.find(id);		//O(log N)
	
	//Gdy nie znaleziono sieci o tym kluczu, to ze specyfikacji zwracamy 0
	if (net == networks.end())
	{
		if (debug) cerr << "\tno network with given id found, returning 0" << endl;
		return 0;
	}
	
	NET_MAP::size_type unique_keys = 0;
	
	//Sklejam ze sobą pary o tych samych kluczach
	//O(n + log m)
	for (NET_MAP::iterator it = net->second.first.begin(); it != net->second.first.end(); it = net->second.first.upper_bound(it->first)) ++unique_keys;
	
	if (debug) cerr << "\tnetwork with given id consists of " << unique_keys << " nodes" << endl;
	
	return unique_keys;
}


//Złożoność: O(log N + log m)
void network_add_node(unsigned long id, const char* label)
{
	if (debug) cerr << "network_add_node(" << id << ", " << label << "):" << endl;
	
	//Gdy pusty napis, to nic nie robię
	if (label == NULL) return;
	
	NETWORK_CONTAINER::iterator net = networks.find(id);		//O(log N)
	
	//Gdy sieć nie istnieje, to nic nie robię
	if (net == networks.end())
	{
		if (debug) cerr << "\tno network with given id found" << endl;
		return;
	}
	
	NET_MAP::iterator element = net->second.first.find(label);	//O(log m)
	
	//Gdy element o etykiecie label istnieje, nic nie robię
	if (element != net->second.first.end())
	{
		if (debug) cerr << "\tnode with given label already exists in given network" << endl;
		return;
	}
	
	//Dodaję element
	net->second.first.insert(make_pair(label, make_pair(false, label)));
	if (debug) cerr << "\tnode with given label has been added to given network" << endl;
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
			cerr << "At least one of the labels is NULL. Aborting\n";
		}
		return;	
	} 

	NETWORK_CONTAINER::iterator net = networks.find(id);		//O(log N)

	//If no network with given id exists - do nothing
	if (net == networks.end())
	{
		if (debug)
		{
			cerr << "No network with given id found. Aborting network_add_link.\n";
		}
		return;
	}

	pair <NET_MAP::iterator, NET_MAP::iterator> searchRange;
	searchRange = net->second.first.equal_range(slabel);  //O(log N)

	bool exists = false; //Checking if the given link already exists
	NET_MAP::iterator it = searchRange.first;

	for (it; searchRange.first != searchRange.second; searchRange.first++)
	{
		if (it->second.second == tlabel)
		{
			cerr << "Link already exists. Aborting.\n";
			exists = true;
		}
	}
	
	if (!exists)
	{
		//Adding the first node if it does not exist
		if (searchRange.first->first != slabel) 
		{
			if (debug)
			{
				cerr << "No node with first label exists, creating:\n";
			}
			network_add_node(id, slabel);
		}

		//Adding the second node if it does not exist
		if (net->second.first.find(label) == net->second.first.end())
		{
			if (debug)
			{
				cerr << "No node with second label exists, creating:\n";
			}
			network_add_node(id, tlabel);
		}
		
		//Adding the link if it does not exist
		if (debug)
		{
			cerr << "Added link from node" << slabel << " to node " << tlabel <<".\n";
		}
		net->insert(make_pair(slabel, make_pair(true, tlabel) ) );
		net->insert(make_pair(tlabel, make_pair(false, slabel) ) );
	}
}
