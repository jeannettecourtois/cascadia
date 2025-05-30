#pragma once
#include <string>
#include "partie.h"
#include "joueur.h"
#include "carteMarquageFaune.h"
#include "enum.h"
#include <random>
#include <iostream>

class TuileDepart {
public:
    TuilePlacee* tuiles[3];
    TuileDepart();
    TuileDepart(const TuileDepart& other);
    ~TuileDepart();
    TuilePlacee* getTuile(int i) const { return tuiles[i]; }
};

class JetonFaune {
    Animal ani;
public:
    JetonFaune(Animal a) : ani(a){}
    ~JetonFaune() = default;
    //méthodes et accesseurs en lecture 
    Animal getAnimal() const{return ani;}
};

class jetonNature {
    void afficher() const {}
};

class ControleurGeneral {
private:
    int nbTuilesHabitat;
    int nbJetonFaune;
    int nbJetonsNature;
    int nbCartesMarquageFaune;

    int nbCartesRegles;
    int nbTuilesDepart;
    Tuile** tuiles = nullptr;
    CarteMarquageFaune** cartesRegles = nullptr;
    TuileDepart** tuilesDepart = nullptr;

    JetonFaune* tabJetons[100];
    Tuile* tabTuiles[85];
    CarteMarquageFaune* tabCartesMarquage[15];

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> distTuiles;
    std::uniform_int_distribution<> distJetons;
    std::uniform_int_distribution<> distCartes;

    ControleurGeneral();
    ControleurGeneral(const ControleurGeneral&) = delete;
    ControleurGeneral& operator=(const ControleurGeneral&) = delete;

public:
    ~ControleurGeneral();
    static ControleurGeneral& getInstance();

    Tuile* getTuile();
    JetonFaune* getJetonFaune();
    CarteMarquageFaune* getCarteMarquageFaune();
    CarteMarquageFaune* getCarteRegleAleatoire();
    TuileDepart** getTuilesDepartAleatoires(); // retourne un tableau de 3 pointeurs
};