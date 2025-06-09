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
    std::vector<Tuile*> getTuiles() { return tuiles; }
};


class SacJeton {
private:
    int nbJetons;
    // vecteur du compte des jetons indexe par Animal
    std::vector<int> compteJetons;
public:
    SacJeton(const std::vector<int>& repartition);
    ~SacJeton();
    Animal Piocher();
    void mettreDansLeSac(Animal a);
    int getNbJetons() const { return nbJetons; }
    std::vector<int> getCompteJetons() { return compteJetons; }

};

class Pioche {
private:
    Tuile* tuiles[4];
    Animal* jetons[4];
    SacTuile* sacDeTuiles;
    SacJeton* sacDeJetons;
    int idTuileChoisis;
public:
    Pioche(int nbJoueurs);
    ~Pioche();
    Pioche(const Pioche&) = delete; //!!! DEBUG
    Pioche& operator=(const Pioche&) = delete; //!!! DEBUG
    void preparerPioche();
    void afficherTuilesDisponibles() const;
    void afficherJetonsDisponibles() const;
    void selectionnerTuile();
    void completerPioche(const Animal* a, const Tuile* t);

    /* Getter */
    Animal* selectionnerJeton() const;
    Tuile* getTuile(int index) const {
        if (index >= 0 && index < 4) {return tuiles[index];}
        return nullptr;}
    Animal* getJeton(int index) const {
        if (index >= 0 && index < 4) {return jetons[index];}
        return nullptr;}
    SacTuile* getSacDeTuiles() const { return sacDeTuiles; }
    SacJeton* getSacDeJetons() const {return sacDeJetons;}

    json toJson() const;
    void fromJson(const json& j);
};