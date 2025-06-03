#pragma once
#include "tuilePlacee.h"
#include "enum.h"
#include "pioche.h"


using namespace std;

class Joueur; //  Pas besoin de tout le fichier joueur.h
class Pioche; // De même

class Action { //classe abstraite
public:
    virtual void executer()=0; //methode virtuelle pure, definition dans les classes derivees
    virtual void annuler()=0; //methode virtuelle pure
    virtual void afficher() const =0;
    virtual ~Action() = default; // destructeur virtuel car la classe est sous-classee

};

// Selection de tuile dans la pioche
class ActionSelectionTuile: public Action {
private:
    Tuile* tuileSelection;
    int indiceSelection; // Indice de la tuile selectionnee dans la pioche
    Pioche* pioche;
public:
    ActionSelectionTuile(int indice, Pioche* p);
    ~ActionSelectionTuile();
    void executer() override;
    void annuler() override;
    void afficher() const override {
        if (tuileSelection) {
            tuileSelection->afficherTuile();
        }
        else {
            cout << "Aucune tuile sélectionnée." << endl;
        }
    }
    Tuile* getTuileSelectionnee() const { return tuileSelection; }
};

// Selection de jeton dans la pioche
class ActionSelectionJeton: public Action {
private:
    Animal jetonSelection;
    int indiceSelection; // Indice du jeton selectionne dans la pioche
    Pioche* pioche;
public:
    ActionSelectionJeton(int indice, Pioche* p);
    ~ActionSelectionJeton();
    void executer() override;
    void annuler()override;
    void afficher() const override {
        cout << "\nAction Selection du jeton :" << endl;
        if (jetonSelection != Animal::Vide) {
            cout << "Jeton selectionne : " << AnimalFormateur{ jetonSelection, Format::Complet } << endl;
        } else {
            cout << "Aucun jeton selectionne." << endl;
        }
    }
};

// Placer une tuile sur le plateau
class ActionPlacerTuile: public Action {
private:
    Tuile* tuile;
    Position pos;
    Joueur* joueur;
public:
    ActionPlacerTuile(Tuile* t, const Position& p, Joueur* j);
    ~ActionPlacerTuile();
    void executer() override;
    void annuler() override;
    void afficher() const override {
        cout << "\nAction Placement de la tuile " << endl;
        tuile->afficherTuile();
    }
};

// Placer un jeton sur une tuile qui est deja placee
class ActionPlacerJeton: public Action {
private:
    TuilePlacee* cible;
    Animal* jeton;
public:
    ActionPlacerJeton(Animal* j, TuilePlacee* c);
    ~ActionPlacerJeton();
    void executer() override;
    void annuler() override;
    void afficher() const override {
        cout << "\nAction Placement du jeton " << endl;
        if (jeton) {
            cout << "Jeton place : " << AnimalFormateur{ *jeton, Format::Complet } << endl;
        } else {
            cout << "Aucun jeton place." << endl;
        }
        cible->afficherTuilePlacee();
    }
};


class ActionUtiliserJetonNature: public Action { //classe abstraite
private:
    Action* actionCible;
public:
    ActionUtiliserJetonNature(Action* a);
    virtual ~ActionUtiliserJetonNature();
    virtual void executer()=0; //methode virtuelle pure
    virtual void annuler()=0; //methode virtuelle pure
    virtual void afficher() const =0;
};

class SelectionTuileJeton : public ActionUtiliserJetonNature {
public:
    SelectionTuileJeton() = default; // Temporaire
    ~SelectionTuileJeton() = default; // Temporaire
    void executer() override;
    void annuler() override;
    void afficher() const override {
        cout << "\nActionUtiliserJeton pour action de selection de tuile et jeton." << endl;
    }
};

class SupressionJetonPioche : public ActionUtiliserJetonNature {
public:
    SupressionJetonPioche() = default; // Temporaire
    ~SupressionJetonPioche() = default; // Temporaire
    void executer() override;
    void annuler() override;
    void afficher() const override {
        cout << "\nActionUtiliserJeton pour action de suppression de jeton pioche." << endl;
    }
};