#pragma once
#include <iostream>
#include "tuile.h"
#include "controleurGeneral.h"

class SacTuile {
private:
    int nbTuile;
    Tuile** tuiles;
public:
    SacTuile();
    ~SacTuile();
    Tuile* Piocher();
    void mettreDansLeSac(Tuile&);
};


class SacJeton {
private:
//Est-ce qu'il serait pas mieux de faire une liste base sur l'enumeration animal => on peut rajouter des animaux dans la liste
    int nbOurs;
    int nbCerf;
    int nbSaumon;
    int nbAigle;
    int nbRenard;
public:
    SacJeton();
    ~SacJeton();
    Animal* Piocher();
    void mettreDansLeSac(const Animal& a);
};

class Pioche {
private:
    Tuile* tuiles[4];
    Animal* jetons[4];
    int idTuileChoisis;
    SacTuile* sacDeTuiles;
    SacJeton* sacDeJetons;
public:
    Pioche();
    ~Pioche();
    void preparerPioche();
    void afficherTuilesDisponibles() const;
    void selectionnerTuile();
    Animal selectionnerJeton() const;
    Tuile* getTuile(int i) const { return tuiles[i]; }
    Animal* getJeton(int i) const { return jetons[i]; }
};