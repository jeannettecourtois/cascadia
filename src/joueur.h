#pragma once
#include <string>
#include <vector>
#include "tuilePlacee.h"
#include "controleurGeneral.h"
#include "json.hpp"

using json = nlohmann::json;

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
    // renvoie le vector en entier
    vector<TuilePlacee> getPlateau() { return plateau; }

    // Pour sauvegarder
    json toJson() const;
};


class Joueur {
private:
    int idJoueur;
    int nbJetonNature;
    PlateauJoueur* plateau;
    Partie* partie;
public:
    Joueur(Partie* p, int id);
    ~Joueur();
    Joueur(const Joueur&) = delete;
    Joueur& operator=(const Joueur&) = delete;

    int calculScore();
    int getNbJetonNature() const { return nbJetonNature; }
    PlateauJoueur* getPlateau() const { return plateau; }
    int getIdJoueur() const { return idJoueur; }

    // Pour sauvegarder
    json toJson() const;
    static Joueur* fromJson(const json& j, Partie* partie) {
        // on recupere l'id du joueur
        int id = j.at("id");
        Joueur* joueur = new Joueur(partie, id);

        // Plateau
        if (j.contains("plateau")) {
            for (const auto& jtp : j["plateau"]) {
                TuilePlacee tp = TuilePlacee::fromJson(jtp);
                joueur->getPlateau()->ajouterTuile(tp);
            }
        }

        return joueur;
    }
};
