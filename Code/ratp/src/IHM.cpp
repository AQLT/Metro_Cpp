#include "IHM.h"
#include <iostream>
#include <string>
#include <vector>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>
#include <stdio.h> //printf


#include "Metro.h"
#include "Ligne.h"
#include "Arret.h"
#include <windows.h> //couleurs palette


IHM::IHM()
{
}

IHM::~IHM()
{
    //dtor
}

bool trier_arrets_alphabet(Arret* arret1, Arret* arret2) {
    return (arret1->getNom() < arret2->getNom());
}

void colorer_ecran(HANDLE hConsole, int code_couleur){
    SetConsoleTextAttribute(hConsole, code_couleur);
}


vector<string> IHM::choixItineraire(Metro metro)
{
    colorer_ecran(hConsole, 14); //jaune
    cout << "--------------------------------------------------" << endl;
    cout << "---------------------- MENU ----------------------" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << endl << endl;
    colorer_ecran(hConsole, 15); //blanc

    vector<string> partie_menu;
    partie_menu.push_back("de depart");
    partie_menu.push_back("d'arrivee");
    vector<string> identifiants_depart_arrivee;

    vector<string> id_ligne;
    vector<Ligne*> liste_lignes = metro.getLignes();
    for(int i = 0; i < liste_lignes.size(); ++i){
        id_ligne.push_back(liste_lignes[i]->getNumero());
    }
    std::vector<string>::iterator it = std::unique (id_ligne.begin(), id_ligne.end());                                                          //                ^
    id_ligne.resize(std::distance(id_ligne.begin(),it) );



    for(int m = 0; m < 2; m++)
    {
        vector<Ligne*> lignes_entree;
        cout<<endl<<"Choisissez votre ligne " << partie_menu[m] <<" parmi les lignes suivantes : " << endl;

        colorer_ecran(hConsole, 14); //jaune
        cout << id_ligne[0];
        for(int i = 1; i < (id_ligne.size()-1); ++i){
            cout << ", " << id_ligne[i];
        }
        cout << " et " << id_ligne[id_ligne.size()-1] << endl;
        colorer_ecran(hConsole, 15); //blanc

        string menu_ligne;
        cout<<"Ligne " << partie_menu[m] <<" : ";
        colorer_ecran(hConsole, 14); //jaune
        cin>>menu_ligne;
        colorer_ecran(hConsole, 15); //blanc
        while ( find(id_ligne.begin(), id_ligne.end(), menu_ligne) == id_ligne.end() ){
            cout << "Choix incorrect, veuillez essayer de nouveau :" << endl;
            cin>>menu_ligne;
        }

        cout<<endl;

        // Recuperation des arrêts de la ligne choisie
        for(int i=0; i < liste_lignes.size(); ++i)
        {
            if(liste_lignes[i]->getNumero()==menu_ligne)
            {
                lignes_entree.push_back(liste_lignes[i]);
            }
        }

        vector<string> arrets_entree_libelles;
        vector<Arret*> arrets_entree;
        for(int i=0; i < lignes_entree.size(); ++i)
        {
            for(int j=0; j < lignes_entree[i]->getArrets().size(); ++j)
            {
                if (std::find(arrets_entree_libelles.begin(), arrets_entree_libelles.end(), lignes_entree[i]->getArrets()[j]->getNom()) != arrets_entree_libelles.end())
                {

                }
                else
                {
                    arrets_entree_libelles.push_back(lignes_entree[i]->getArrets()[j]->getNom());
                    arrets_entree.push_back(lignes_entree[i]->getArrets()[j]);
                }
            }
        }
        sort(arrets_entree_libelles.begin(),arrets_entree_libelles.end());
        sort(arrets_entree.begin(),arrets_entree.end(),trier_arrets_alphabet);

        // Choix de l'arrêt
        cout<<endl<<endl<<"Choisissez votre arret " << partie_menu[m] <<" : " << endl << endl;
        for (int i = 0; i < arrets_entree_libelles.size(); ++i)
        {
            colorer_ecran(hConsole, 14); //jaune
            cout<< i << " : ";
            colorer_ecran(hConsole, 15); //blanc
            cout << arrets_entree[i]->getNom()<<endl;
        }
        cout<<endl<<endl<<"Arret " << partie_menu[m]<< " : ";
        int menu_arret_depart;
        colorer_ecran(hConsole, 14); //jaune
        cin>>menu_arret_depart;
        colorer_ecran(hConsole, 15); //blanc
        while(cin.fail()){ //pour eviter bug si on entre une chqine de caracteres
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Choix incorrect, veuillez essayer de nouveau :" << endl;
            cin>>menu_arret_depart;
        }

        while(!(menu_arret_depart >=0 & menu_arret_depart <  arrets_entree_libelles.size())){
            cout << "Choix incorrect, veuillez essayer de nouveau :" << endl;
            cin>>menu_arret_depart;
        }

        colorer_ecran(hConsole, 14); //jaune
        cout<<"("<< arrets_entree[menu_arret_depart]->getNom() <<")";
        colorer_ecran(hConsole, 15); //blanc
        cout<<endl;
        stringstream ss;
        ss << arrets_entree[menu_arret_depart]->getId();
        string str = ss.str();
        identifiants_depart_arrivee.push_back(str);
    }

    cout<< endl << endl;
    return(identifiants_depart_arrivee);
}

bool IHM::choixTypeItineraire(){
        string menu_ligne;
        cout<<"Voulez-vous l'itineraire : " << endl;
       colorer_ecran(hConsole, 14); //jaune
        cout <<" 0 : ";
        colorer_ecran(hConsole, 15); //blanc
        cout <<"Le plus rapide ?" << endl;
        colorer_ecran(hConsole, 14); //jaune
        cout <<" 1 : ";
        colorer_ecran(hConsole, 15); //blanc
        cout <<"Avec le moins de changements ?" << endl;
        vector<string> choix_possibles;
        choix_possibles.push_back("0");
        choix_possibles.push_back("1");
        cin>>menu_ligne;
        while (find(choix_possibles.begin(), choix_possibles.end(), menu_ligne) == choix_possibles.end() ){
            cout << "Choix incorrect, veuillez essayer de nouveau :" << endl;
            cin>>menu_ligne;
        }
        cout << endl  << endl;
        return(menu_ligne == "1");
}

bool IHM::quitter(){
        string menu_ligne;
        cout<< endl << endl << "Voulez-vous : " << endl;
        colorer_ecran(hConsole, 14); //jaune
        cout <<" 0 : ";
        colorer_ecran(hConsole, 15); //blanc
        cout <<"Quitter l'application ?" << endl;
        colorer_ecran(hConsole, 14); //jaune
        cout <<" 1 : ";
        colorer_ecran(hConsole, 15); //blanc
        cout <<"Chercher un nouvel itineraire ?" << endl;
        vector<string> choix_possibles;
        choix_possibles.push_back("0");
        choix_possibles.push_back("1");
        cin>>menu_ligne;
        while (find(choix_possibles.begin(), choix_possibles.end(), menu_ligne) == choix_possibles.end() ){
            cout << "Choix incorrect, veuillez essayer de nouveau :" << endl;
            cin>>menu_ligne;
        }
        cout << endl  << endl;
        return(menu_ligne == "1");
}

void conversion_secondes(int n)
{
     int //n,		/* n : durée donnée en secondes */
	    r,		/* r : le reste du modulo */
	    h,		/* h : le nombre d'heures */
	    m;      /* m : le nombre de minutes */

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

void IHM::AfficherItineraire(Itineraire itineraire)
{
    vector<Node*> it_simplifie = itineraire.it_simplifie;
    /*HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);*/

    vector<string> destination_s;
    vector<int> nb_arrets = itineraire.nombre_arrets; //PROBLEME NB ARRETS
    //int temps_min = CalculerTempsMin(it_simplifie);
    //Node* destination = ;
    int temps_min = it_simplifie[it_simplifie.size()-1]->getDistance(false);

    char buffer[100];
    string string_temp;

    // Affichage
    destination_s = it_simplifie[0]->calculDestination(it_simplifie[1]);

    SetConsoleTextAttribute(hConsole, 14); //jaune
    cout << endl << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "---------------- Votre itineraire ----------------" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << endl << endl;
    SetConsoleTextAttribute(hConsole, 15); //blanc

    cout << "A ";
     SetConsoleTextAttribute(hConsole, 11); //orange
    cout <<  it_simplifie[0]->getNom();
    SetConsoleTextAttribute(hConsole, 15); //blanc
    cout << " prendre la ligne ";
     SetConsoleTextAttribute(hConsole, 24); //vert
    cout << destination_s[0];
     SetConsoleTextAttribute(hConsole, 15); //blanc
    cout << " direction ";
     SetConsoleTextAttribute(hConsole, 13); //violet
    cout << destination_s[1];
     SetConsoleTextAttribute(hConsole, 15); //blanc
    cout << " jusqu a l'arret ";

    for(int i=1; i < (it_simplifie.size()-1); ++i){
        SetConsoleTextAttribute(hConsole, 11); //orange
        cout << it_simplifie[i]->getNom();
        SetConsoleTextAttribute(hConsole, 15); //blanc
        cout << " (" << nb_arrets[i] << " arrets)";
        cout << ", puis prendre la ligne ";
        destination_s = it_simplifie[i]->calculDestination(it_simplifie[i+1]);
        SetConsoleTextAttribute(hConsole, 24); //vert
        cout << destination_s[0];
        SetConsoleTextAttribute(hConsole, 15); //blanc
        cout << " direction ";
          SetConsoleTextAttribute(hConsole, 13); //violet
        cout << destination_s[1];
        SetConsoleTextAttribute(hConsole, 15); //blanc
        cout << " jusqu'a l'arret ";
    }

    cout <<  it_simplifie[it_simplifie.size()-1]->getNom() <<
        " (" << nb_arrets[it_simplifie.size()-1] << " arrets)" <<
        endl << endl;


    cout << endl << "Temps de trajet minimum : ";
    SetConsoleTextAttribute(hConsole, 14); //jaune
    conversion_secondes(temps_min);
    SetConsoleTextAttribute(hConsole, 15); //blanc
    cout << endl;


}