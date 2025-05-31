#pragma once
#include <string>
#include "tuilePlacee.h"
#include "controleurGeneral.h"
using namespace std;

class TuileDepart;

class PlateauJoueur {
private:
    int nbTuiles;
    TuilePlacee** tuiles;
public:
    PlateauJoueur();
    ~PlateauJoueur();
    PlateauJoueur(const PlateauJoueur&) = delete;
    PlateauJoueur& operator=(const PlateauJoueur&) = delete;
    void ajouterTuile(const TuilePlacee*);
    void ajouterTuileDepart(const TuileDepart* set);
    int getNbTuiles() const { return nbTuiles; }
    void afficherPlateau() const;
    TuilePlacee* getTuilePlacee(const Position& pos) const {
        for (int i = 0; i < nbTuiles; ++i) {
            if (tuiles[i]->getPosition() == pos) {
                return tuiles[i];
            }
        }
        return nullptr; // Si aucune tuile n'est trouvee a cette position
    }
};


class Joueur {
private:
    int idJoueur;
    int nbJetonNature;
    std::string  nomJoueur;
    PlateauJoueur* plateau;
public:
    Joueur();
    ~Joueur();
    Joueur(const Joueur&) = delete;
    Joueur& operator=(const Joueur&) = delete;
    int calculScore();
    int getNbJetonNature() const { return nbJetonNature; }
    string getNomJoueur() const { return nomJoueur; }
    PlateauJoueur* getPlateau() const { return plateau; }
};