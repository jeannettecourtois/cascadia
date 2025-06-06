#pragma once
#include <string>
#include <vector>
#include "tuilePlacee.h"
#include "controleurGeneral.h"

using namespace std;

class TuileDepart;
class Partie;

class PlateauJoueur {
private:
    vector<TuilePlacee> plateau;
public:
    PlateauJoueur() = default;
    ~PlateauJoueur() = default;
    PlateauJoueur(const PlateauJoueur&) = delete;
    PlateauJoueur& operator=(const PlateauJoueur&) = delete;

    void ajouterTuile(const TuilePlacee&);
    void ajouterTuileDepart(const TuileDepart* set);
    void supprimerTuile(const Position& pos);

    int getNbTuiles() const { return static_cast<int>(plateau.size()); }

    void afficherPlateau() const;

    TuilePlacee* getTuilePlacee(const Position& pos) {
        for (auto& t : plateau) {
            if (t.getPosition() == pos) return &t;
        }
        return nullptr;
    }

    const TuilePlacee* getTuilePlacee(const Position& pos) const {
        for (const auto& t : plateau) {
            if (t.getPosition() == pos) return &t;
        }
        return nullptr;
    }
};


class Joueur {
private:
    int idJoueur;
    int nbJetonNature;
    string nomJoueur;
    PlateauJoueur* plateau;
    Partie* partie;
public:
    Joueur(Partie* p);
    ~Joueur();
    Joueur(const Joueur&) = delete;
    Joueur& operator=(const Joueur&) = delete;

    int calculScore();
    int getNbJetonNature() const { return nbJetonNature; }
    string getNomJoueur() const { return nomJoueur; }
    PlateauJoueur* getPlateau() const { return plateau; }
};
