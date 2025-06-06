#pragma once
#include "tuilePlacee.h"
#include "enum.h"
#include "pioche.h"

using namespace std;

class Joueur; //  Pas besoin de tout le fichier joueur.h
class Pioche; // De meme


class Action { // Classe abstraite
public:
    virtual int executer() = 0; // Methode virtuelle pure, retour int dans toutes les actions
    virtual void annuler() = 0; // Methode virtuelle pure
    virtual void afficher() const = 0; // Methode virtuelle pure
    virtual ~Action() = default; // Destructeur virtuel
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
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Placer un jeton sur une tuile qui est deja placee
class ActionPlacerJeton : public Action {
private:
    TuilePlacee* cible;
    Animal* jeton;
public:
    ActionPlacerJeton(Animal* j, TuilePlacee* c);
    ~ActionPlacerJeton();
    int executer() override; // Modifie pour retourner un int
    void annuler() override;
    void afficher() const override {
        cout << "\nAction Placement du jeton " << endl;
        if (jeton) {
            cout << "Jeton place : " << AnimalFormateur{ *jeton, Format::Complet } << endl;
        }
        else {
            cout << "Aucun jeton place." << endl;
        }
        cible->afficherTuilePlacee();
    }
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
