#include "tuilePlacee.h"
#include "tuile.h"
#include "jetonFaune.h"
#include "carteMarquageFaune.h"
#include <iostream>  
#include <random>
#include "enum.h"
#include <vector>
#include "position.h"
#include "action.h"



class PlateauJoueur{
	std::vector<tuilePlacee> tuilesPlacees;
	std::vector<Position> positionsDisponibles;
	size_t maxTuiles = 40; //taille du plateau de jeu
public: 
	PlateauJoueur();
	~PlateauJoueur() = default; 
	void ajouterTuilePlacee(const Tuile& tuile, const Position& pos)
	//void ajouter Jeton Faune 
	void afficherPositionsDisponibles() const;

};