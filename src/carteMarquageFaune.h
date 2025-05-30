#pragma once
#include "enum.h"
#include "controleurGeneral.h"
class PlateauJoueur; // Déclaration anticipée

class CarteMarquageFaune {
private:
    Animal animalCarte;
public:
    CarteMarquageFaune(const Animal& a);
    ~CarteMarquageFaune();
    Animal GetAnimalCarte() {return animalCarte;}
    int calculPoints(PlateauJoueur&);
};

class CarteMarquageFauneParMasque : public CarteMarquageFaune {
public:
    CarteMarquageFauneParMasque(const Animal& a);
    ~CarteMarquageFauneParMasque();
    int calculPoints(PlateauJoueur&);
};

class CarteMarquageFauneParDiversiteDeType : public CarteMarquageFaune {//Utile pour le renard
public:
    CarteMarquageFauneParDiversiteDeType(const Animal& a);
    ~CarteMarquageFauneParDiversiteDeType();
    int calculPoints(PlateauJoueur&);
};