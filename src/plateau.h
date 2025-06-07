#include "tuilePlacee.h"
#include "tuile.h"
#include "jetonFaune.h"
#include "carteMarquageFaune.h"
#include <iostream>  
#include <random>
#include "enum.h"
#include <vector>
#include "action.h"

std::vector<Animal> lesAnimaux(animaux);
std::vector<Habitat> lesHabitats(habitats);


//class PlateauJoueur{
//	Tuile** tuiles = nullptr; 
//	JetonFaune** jetonsFaune = nullptr;
//	CarteMarquageFaune** cartesMarquage = nullptr;
//	size_t nbTuiles = 4; 
//	size_t nbJetonFaune = 4; 
//	size_t nbCartesMarquage = 4;
//
//public: 
//	PlateauJoueur();
//	//	tuiles = new Tuile * [nbTuiles]; // Allocation dynamique pour le tableau de tuiles
//	//	jetonsFaune = new JetonFaune * [nbJetonFaune]; // Allocation dynamique pour le tableau de jetons faune
//	//	cartesMarquage = new CarteMarquageFaune * [nbCartesMarquage]; // Allocation dynamique pour le tableau de cartes de marquage
//
//	//	for (unsigned int i = 0; i < nbTuiles; ++i) {
//	//		tuiles[i] = new Tuile(); // Allocation dynamique pour chaque tuile
//	//	}
//	//	for (unsigned int i = 0; i < nbJetonFaune; ++i) {
//	//		int choix= rand() % 5; // Choix aléatoire d'un animal
//	//		jetonsFaune[i] = new JetonFaune(lesAnimaux[choix]); // Exemple d'initialisation, à adapter
//	//	}
//	//	for(unsigned int i = 0; i < nbCartesMarquage; ++i) {
//	//		cartesMarquage[i] = new CarteMarquageFaune(lesAnimaux[rand() % 5]); // Exemple d'initialisation, à adapter
//	//	}
//	//}
//	~PlateauJoueur();
//	PlateauJoueur(const PlateauJoueur& other) = delete; 
//
//	//accesseurs de lecture 
//	size_t getNbTuiles() const { return nbTuiles; }
//	
//};