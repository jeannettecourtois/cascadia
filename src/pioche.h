#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "tuile.h"
#include "controleurGeneral.h"

class SacTuile {
private:
    int nbTuile;
    std::vector<Tuile*> tuiles;
public:
    SacTuile(int nbTuiles);
    ~SacTuile();
    Tuile* Piocher();
    void mettreDansLeSac(Tuile* t);
};


class SacJeton {
private:
    int nbJetons;
    // vecteur du compte des jetons indexé par Animal
    std::vector<int> compteJetons;
public:
    SacJeton(const std::vector<int>& repartition);
    ~SacJeton();
    Animal Piocher();
    void mettreDansLeSac(Animal a);
    int getNbJetons() const { return nbJetons; }
};

class Pioche {
private:
    Tuile* tuiles[4];
    Animal* jetons[4];
    int idTuileChoisis;
    SacTuile* sacDeTuiles;
    SacJeton* sacDeJetons;
public:
    Pioche(int nbJoueurs);
    ~Pioche();
    void preparerPioche();
    void afficherTuilesDisponibles() const;
    void selectionnerTuile();
    void completerPioche();
    Animal* selectionnerJeton() const;
    Tuile* getTuile(int i) const { return tuiles[i]; }
    Animal* getJeton(int i) const { return jetons[i]; }
};