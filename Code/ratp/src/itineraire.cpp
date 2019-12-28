#include "itineraire.h"
#include "Metro.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;
#include <algorithm> //??
#include "Node.h"
#include "Edge.h"
#include <math.h>

Itineraire::itineraire()
{
    //ctor
}


Node* Itineraire::ExtractSmallest(vector<Node*>& nodes, bool min_itineraire)
{
    int size = nodes.size();
    if (size == 0)
        return NULL;
    int smallestPosition = 0;
    Node* smallest = nodes.at(0);
    for (int i=1; i<size; ++i)
    {
        Node* current = nodes.at(i);
        if (current->getDistance(min_itineraire) <
                smallest->getDistance(min_itineraire))
        {
            smallest = current;
            smallestPosition = i;
        }//
    }
    nodes.erase(nodes.begin() + smallestPosition);
    return smallest;
}

bool Itineraire::Contains(vector<Node*>& nodes, Node* node)
{
    const int size = nodes.size();
    for(int i=0; i<size; ++i)
    {
        if (node == nodes.at(i))
        {
            return true;
        }
    }
    return false;
}

vector<Node*>* Itineraire::AdjacentRemainingNodes(Node* node)
{
    vector<Node*>* adjacentNodes = new vector<Node*>();
    const int size = edges.size();
    for(int i=0; i<size; ++i)
    {
        Edge* edge = edges.at(i);
        Node* adjacent = NULL;
        if (edge->node1 == node)
        {
            adjacent = edge->node2;
        }
        //else if (edge->node2 == node) //pas dans les deux sens
        //{
         //   adjacent = edge->node1;
        //}
        if (adjacent && Contains(nodes, adjacent))
        {
            adjacentNodes->push_back(adjacent);
        }
    }
    return adjacentNodes;
}

int Itineraire::Distance(Node* node1, Node* node2, bool min_itineraire)
{
    const int size = edges.size();
    for(int i=0; i<size; ++i)
    {
        Edge* edge = edges.at(i);
        if (edge->Connects(node1, node2))
        {
            return edge->getDistance(min_itineraire);
        }
    }
    return -1; // should never happen
}

void conversion_secondes(int n)
{
     int //n,		/* n : durée donnée en secondes */
	    r,		/* r : le reste du modulo */
	    h,		/* a : le nombre d'heures */
	    m;      /* a : le nombre de minutes */

	//printf("Donnez une durée en secondes : ");
	//scanf("%d", &n);	/* la durée en seconde est entrée */

	if (n>0){
	r=n%3600;		/* on calcul le reste */
	h=(n-r)/3600;
	if (h != 0){
        printf("%dh", h);
	}
    n=r;		/* on ne retient que les secondes restantes */
	r=n%60;		/* on calcul le reste */
	m=(n-r)/60;
	if (h == 0 & m != 0){
		printf("%d min", m);
	}
	if (h != 0 & m != 0){
		printf("%d", m);
	}

	}
	else
	printf("erreur");
	 //temps_min = round(temps_min/60 * 100)/100;
}



void Itineraire::PrintShortestRouteTo(Node* destination)
{
    Node* previous = destination;
    /*cout << "Temps de trajet : "
         << destination->getDistance(false) << endl;
    while (previous)
    {
        cout << previous->getNom() << " (" << previous->getId() << ") ";
        previous = previous->previous;
    }
    cout << endl;*/

    previous = destination;
    Node* next;
    Ligne* ligne;
    vector<Node*> it_simplifie ;
    vector<int> nb_arrets;
    it_simplifie.push_back(previous);
    int nb = 0;
    while (previous)
    {
        next = previous->previous;
        if(next){
            if(!previous->memeArret(next)){
                ++nb;
                if(!previous->memeLigne(next)){
                    nb_arrets.push_back(nb);
                    it_simplifie.push_back(previous);
                    nb = 0;
                }
            }
        }else{
            it_simplifie.push_back(previous);
            nb_arrets.push_back(nb);
        }
        previous = next;
    }
    nb_arrets.push_back(0);//dernier indice non important
    std::reverse(it_simplifie.begin(),it_simplifie.end());
    std::reverse(nb_arrets.begin(),nb_arrets.end());
    int temps_min = destination->getDistance(false);
    //cout <<temps_min;

    cout << endl << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "---------------- Votre itineraire ----------------" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << endl << endl;


    cout << "A " <<  it_simplifie[0]->getNom() << " prendre la ligne ";
    ligne = it_simplifie[0]->getLigne(it_simplifie[1]);
    cout << ligne->getNumero() << " direction "<<
          ligne->getDirection() <<
    " jusqu a l'arret ";

    for(int i=1; i < (it_simplifie.size()-1); ++i){
        cout << it_simplifie[i]->getNom();
        cout << " (" << nb_arrets[i] << " arrets)";
        cout << ", puis prendre la ligne ";
        ligne = it_simplifie[i]->getLigne(it_simplifie[i+1]);
        cout << ligne->getNumero() << " direction "
                    << ligne->getDirection() <<
                     " jusqu'a l'arret ";
    }


    cout <<  it_simplifie[it_simplifie.size()-1]->getNom() <<
        " (" << nb_arrets[it_simplifie.size()-1] << " arrets)" <<
        endl << endl;


    cout << endl  << "Temps de trajet minimum : ";
    conversion_secondes(temps_min);
    cout << endl;

}

// these two not needed
vector<Edge*>* Itineraire::AdjacentEdges(vector<Edge*>& edges, Node* node)
{
    vector<Edge*>* adjacentEdges = new vector<Edge*>();
    const int size = edges.size();
    for(int i=0; i<size; ++i)
    {
        Edge* edge = edges.at(i);
        if (edge->node1 == node)
        {
            cout << "adjacent: " << edge->node2->getId() << endl;
            adjacentEdges->push_back(edge);
        }
        else if (edge->node2 == node)
        {
            cout << "adjacent: " << edge->node1->getId() << endl;
            adjacentEdges->push_back(edge);
        }
    }
    return adjacentEdges;
}
void Itineraire::RemoveEdge(vector<Edge*>& edges, Edge* edge)
{
    vector<Edge*>::iterator it;
    for (it=edges.begin(); it<edges.end(); ++it)
    {
        if (*it == edge)
        {
            edges.erase(it);
            return;
        }
    }
}

int Itineraire::getIndiceFromNode(string identifiant, vector<Node*> les_noeuds)
{
    int id_int;
    stringstream temp_id(identifiant);
    temp_id >> id_int;
    int indice_sortie;
    const int size = les_noeuds.size();
    for(int i=0; i<size; ++i){
 //       cout << "identifiant noeud : " << les_noeuds[i]->id << endl;
 //       cout << "identifiant : " << identifiant << endl;
        if(les_noeuds[i]->getId()== id_int){
            indice_sortie = i;
        }
    }
    return(indice_sortie);
}

vector<string> split(string str, string sep)
{
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    vector<string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL)
    {
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

void Itineraire::Dijkstras(bool min_itineraire)
{
    while (nodes.size() > 0)
    {
        Node* smallest = ExtractSmallest(nodes, min_itineraire);
        vector<Node*>* adjacentNodes =
            AdjacentRemainingNodes(smallest);
        const int size = adjacentNodes->size();
        for (int i=0; i<size; ++i)
        {
            Node* adjacent = adjacentNodes->at(i);
            int distance = Distance(smallest, adjacent, min_itineraire) +
                           smallest->getDistance(min_itineraire);
            if (distance < adjacent->getDistance(min_itineraire))
            {
                adjacent->setDistance(Distance(smallest, adjacent, false) +
                           smallest->getDistance(false),
                                       false); //distance temps
                adjacent->setDistance(Distance(smallest, adjacent, true) +
                           smallest->getDistance(true),
                                       true); // distance changement
                adjacent->previous = smallest;
            }
        }
        delete adjacentNodes;
    }
}
void Itineraire::DijkstrasFinal(string entree, string sortie, bool min_itineraire){ //vector<Node*> nodes, Edge** edges,
    int entree_int = getIndiceFromNode(entree,nodes);
    int sortie_int = getIndiceFromNode(sortie,nodes);

    nodes[entree_int]->setDistance(0, false); // set start node
    nodes[entree_int]->setDistance(0, true);
    Node* node_fin = nodes[sortie_int];
    Node* node_entree = nodes[entree_int];
    Dijkstras(min_itineraire);
    PrintShortestRouteTo(node_fin);
}
void Itineraire::chargerNodes(string chemin, Metro* metro){ //vector<Node*>
    string chaine;
    const char* chemin_char = chemin.c_str();
    ifstream fichier(chemin_char);
    int nligne = 0;
    string ligne;
    getline(fichier,ligne);
    std::vector<std::string> identifiants_ligne;

    //ligne.erase(remove(ligne.begin(), ligne.end(), '\"' ),ligne.end()); //enlever les ""
    //const char* ligne_char = ligne.c_str();
    //identifiants_ligne=split(ligne_char,"\t");

   string lignemetro;
    stringstream ligne_fichier(ligne);
   while(getline(ligne_fichier, lignemetro,'\t') ){
        lignemetro.erase(remove(lignemetro.begin(), lignemetro.end(), '\"' ),lignemetro.end());
        identifiants_ligne.push_back(lignemetro);
   }

        for (int i=0; i<identifiants_ligne.size(); i++)
            {
                Node* noeud = new Node(identifiants_ligne[i], metro);
                nodes.push_back(noeud);
            }
    fichier.close();
    //return(nodes);
}

void Itineraire::chargerEdges(string chemin){
    const char* chemin_char = chemin.c_str();
    std::string chaine;
    std::ifstream fichier(chemin_char); // acquisition
    int nligne = 0;
    do
    {
        string ligne;
        getline(fichier,ligne);
       if (nligne==0)  // Si c'est la première ligne je récupère les identifiants d'arret
       {
        }
        else   // Je recupere les coordonnées
        {
            //edges[nligne-1] = new Edge[nodes.size()]; //test const
            std::vector<string> distances;
            distances=split(ligne,"\t");
            for (int i=1; i<nodes.size()+1; i++) //test const
            {
             stringstream geek(distances[i]);
             int num = 0;
             geek >> num;
             if (num!=(-1))
             {
                Edge* edge = new Edge(nodes[nligne-1],nodes[i-1],num) ;
                edges.push_back(edge);
             }
            }
        }
        if (fichier.eof())
            break;
        nligne++;
    }
    //while ( !fichier.eof() );
    while ( nligne<= 758 ); //BUG

    fichier.close(); // relâchement
}

void Itineraire::chargerDonnees(string wd, Metro* metro){ //vector<Node*>
    chargerNodes(wd + "/Data projet/voisins.txt", metro);
    chargerEdges(wd + "/Data projet/voisins.txt");
}
