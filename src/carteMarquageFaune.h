#pragma once
#include "enum.h"
#include "controleurGeneral.h"
class PlateauJoueur; // Declaration anticipee

class CarteMarquageFaune {
private:
    Animal animalCarte;
    unsigned int id;
    // m^me chose que tuile
    static unsigned int idCounter;
public:
    // Constructeur par defaut pour le json
    CarteMarquageFaune() {}
    CarteMarquageFaune(const Animal& a);
    ~CarteMarquageFaune();
    Animal GetAnimalCarte() {return animalCarte;}
    int calculPoints(PlateauJoueur&);
    json toJson() const;
    static CarteMarquageFaune fromJson(const json& j) {
        CarteMarquageFaune c;
        c.id = j.at("id");
        c.animalCarte = fromStringAnimal(j.at("animal"));
        return c;
    }
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