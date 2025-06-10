#pragma once
#include "tuilePlacee.h"
#include "enum.h"
#include "pioche.h"
#include "joueur.h"

using namespace std;

class Pioche; //  Pas besoin de tout le fichier joueur.h
class Partie;
class Joueur;


class Action { // Classe abstraite
public:
    virtual int executer() = 0; // Methode virtuelle pure, retour int dans toutes les actions
    virtual void annuler() = 0; // Methode virtuelle pure
    virtual void afficher() const = 0; // Methode virtuelle pure
    virtual ~Action() = default; // Destructeur virtuel
    virtual json toJson() const = 0;
    static Action* fromJson(const json& j, Partie* p);
};

// Selection de tuile dans la pioche
class ActionSelectionTuile : public Action {
private:
    Tuile* tuileSelection;
    int indiceSelection; // Indice de la tuile selectionnee dans la pioche
    Pioche* pioche;
public:
    ActionSelectionTuile(int indice, Pioche* p);
    ~ActionSelectionTuile();
    int executer() override; // Modifie pour retourner un int
    void annuler() override;
    void afficher() const override {
        if (tuileSelection) {
            tuileSelection->afficherTuile();
        }
        else {
            cout << "Aucune tuile selectionnee." << endl;
        }
    }
    Tuile* getTuileSelectionnee() const { return tuileSelection; }
    json toJson() const {
        return {
            {"type", "SelectionTuile"},
            {"indiceTuile", indiceSelection}
        };
    }
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Selection de jeton dans la pioche
class ActionSelectionJeton : public Action {
private:
    Animal jetonSelection;
    int indiceSelection; // Indice du jeton selectionne dans la pioche
    Pioche* pioche;
public:
    ActionSelectionJeton(int indice, Pioche* p);
    ~ActionSelectionJeton();
    int executer() override; // Modifie pour retourner un int
    void annuler() override;
    void afficher() const override {
        cout << "\nAction Selection du jeton :" << endl;
        if (jetonSelection != Animal::Vide) {
            cout << "Jeton selectionne : " << AnimalFormateur{ jetonSelection, Format::Complet } << endl;
        }
        else {
            cout << "Aucun jeton selectionne." << endl;
        }
    }
    json toJson() const override;
    Animal getJetonSelection() const {
        int val = static_cast<int>(jetonSelection);
        std::cerr << "[TRACE] getJetonSelection retourne : " << val << "\n";
        return jetonSelection;
    }
    void setJetonSelection(Animal a) {
        if (static_cast<int>(a) < 0 || static_cast<int>(a) > 5)
            throw std::invalid_argument("JetonSelection invalide : " + std::to_string(static_cast<int>(a)));
        jetonSelection = a;
    }

};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Placer une tuile sur le plateau
class ActionPlacerTuile : public Action {
private:
    Tuile* tuile;
    Position pos;
    Joueur* joueur;
public:
    ActionPlacerTuile(Tuile* t, const Position& p, Joueur* j);
    ~ActionPlacerTuile();
    int executer() override; // Modifie pour retourner un int
    void annuler() override;
    void afficher() const override {
        cout << "\nAction Placement de la tuile " << endl;
        tuile->afficherTuile();
    }
    json toJson() const;
    const Position& getPosition() const { return pos; }
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Placer un jeton sur une tuile qui est deja placee
class ActionPlacerJeton : public Action {
private:
    Position posTuile;
    Animal jeton; // plus simple de travailler par recopie
    Joueur* joueur;
public:
    ActionPlacerJeton(Animal a, TuilePlacee* tuile, Joueur* j) : jeton(a), joueur(j), posTuile(tuile->getPosition()) {
        if (static_cast<int>(jeton) < 0 || static_cast<int>(jeton) > 5)
            throw std::invalid_argument("ActionPlacerJeton: jeton invalide");
    }

    ActionPlacerJeton(Animal a, Position pos, Joueur* j)
        : jeton(a), joueur(j), posTuile(pos) {
    }
    ~ActionPlacerJeton();
    int executer() override; // Modifie pour retourner un int
    void annuler() override;
    void afficher() const override {
        cout << "\nAction Placement du jeton " << endl;
        cout << "Jeton place : " << AnimalFormateur{ jeton, Format::Complet } << endl;
        //cible->afficherTuilePlacee();
    }
    json toJson() const;
    static ActionPlacerJeton* fromJson(const json& j, Partie* partie);
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

class ActionUtiliserJetonNature : public Action { // Classe abstraite
private:
    Action* actionCible;
public:
    ActionUtiliserJetonNature(Action* a);
    virtual ~ActionUtiliserJetonNature();
    virtual int executer() = 0; // Methode virtuelle pure modifiee pour retourner un int
    virtual void annuler() = 0; // Methode virtuelle pure
    virtual void afficher() const = 0;
};

class SelectionTuileJeton : public ActionUtiliserJetonNature {
public:
    SelectionTuileJeton() = default; // Temporaire
    ~SelectionTuileJeton() = default; // Temporaire
    int executer() override;
    void annuler() override;
    void afficher() const override {
        cout << "\nActionUtiliserJeton pour action de selection de tuile et jeton." << endl;
    }
};

class SupressionJetonPioche : public ActionUtiliserJetonNature {
public:
    SupressionJetonPioche() = default; // Temporaire
    ~SupressionJetonPioche() = default; // Temporaire
    int executer() override;
    void annuler() override;
    void afficher() const override {
        cout << "\nActionUtiliserJeton pour action de suppression de jeton pioche." << endl;
    }
};
