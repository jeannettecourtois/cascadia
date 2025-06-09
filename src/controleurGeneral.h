#pragma once
#include <string>
#include "partie.h"
#include "joueur.h"
#include "carteMarquageFaune.h"
#include "enum.h"
#include <random>
#include <iostream>

class Partie;

class TuileDepart {
public:
    TuilePlacee* tuiles[3];
    TuileDepart();
    TuileDepart(const TuileDepart& other);
    ~TuileDepart();
    TuilePlacee* getTuile(int i) const { return tuiles[i]; }
    void setTuile(int i, TuilePlacee* t) {;
        if (i >= 0 && i < 3) {
            tuiles[i] = new TuilePlacee(*t);
        } else {
            std::cerr << "Index hors limites pour les tuiles de depart.\n";
        }
    }
};

class JetonFaune {
    Animal ani;
public:
    JetonFaune(Animal a) : ani(a){}
    ~JetonFaune() = default;
    //methodes et accesseurs en lecture 
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
    Tuile* tabTuiles[100]; // 0–84 : normales, + 15 de départ (5 x 3)
    CarteMarquageFaune* tabCartesMarquage[15];

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> distTuiles;
    std::uniform_int_distribution<> distJetons;
    std::uniform_int_distribution<> distCartes;

    ControleurGeneral();
    ControleurGeneral(const ControleurGeneral&) = delete;
    ControleurGeneral& operator=(const ControleurGeneral&) = delete;
    void initialiserTuilesDepart(); // Initialise les tuiles de depart, methode privee
public:
    ~ControleurGeneral();
    static ControleurGeneral& getInstance();

    Tuile* getTuile();
    Tuile* getTuileById(unsigned int id) const {
        if (id >= 100) throw std::out_of_range("Tuile id invalide : " + std::to_string(id));
        return tabTuiles[id];
    }

    JetonFaune* getJetonFaune();
    CarteMarquageFaune* getCarteMarquageParAnimalAleatoire(Animal a);

    TuileDepart* getTuileDepartAleatoire();
    Tuile* getStarterTuileByIndex(int i) {
        if (i < 0 || i >= 5) throw std::out_of_range("Starter tuile invalide");
        return tabTuiles[85 + i];
    }

    Tuile* getStarterTile(unsigned int i) {
        if (i >= 5) throw std::out_of_range("StarterTile index hors limites");
        return tabTuiles[85 + i];
    }
};