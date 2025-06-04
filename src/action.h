#pragma once
#include "tuilePlacee.h"
#include "enum.h"
#include "pioche.h"

using namespace std;

class Joueur; //  Pas besoin de tout le fichier joueur.h
class Pioche; // De même

class Action { // Classe abstraite
public:
    virtual int executer() = 0; // Méthode virtuelle pure, retour int dans toutes les actions
    virtual void annuler() = 0; // Méthode virtuelle pure
    virtual void afficher() const = 0; // Méthode virtuelle pure
    virtual ~Action() = default; // Destructeur virtuel
};

// Sélection de tuile dans la pioche
class ActionSelectionTuile : public Action {
private:
    Tuile* tuileSelection;
    int indiceSelection; // Indice de la tuile sélectionnée dans la pioche
    Pioche* pioche;
public:
    ActionSelectionTuile(int indice, Pioche* p);
    ~ActionSelectionTuile();
    int executer() override; // Modifié pour retourner un int
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Sélection de jeton dans la pioche
class ActionSelectionJeton : public Action {
private:
    Animal jetonSelection;
    int indiceSelection; // Indice du jeton sélectionné dans la pioche
    Pioche* pioche;
public:
    ActionSelectionJeton(int indice, Pioche* p);
    ~ActionSelectionJeton();
    int executer() override; // Modifié pour retourner un int
    void annuler() override;
    void afficher() const override {
        cout << "\nAction Sélection du jeton :" << endl;
        if (jetonSelection != Animal::Vide) {
            cout << "Jeton sélectionné : " << AnimalFormateur{ jetonSelection, Format::Complet } << endl;
        }
        else {
            cout << "Aucun jeton sélectionné." << endl;
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
    int executer() override; // Modifié pour retourner un int
    void annuler() override;
    void afficher() const override {
        cout << "\nAction Placement de la tuile " << endl;
        tuile->afficherTuile();
    }
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Placer un jeton sur une tuile qui est déjà placée
class ActionPlacerJeton : public Action {
private:
    TuilePlacee* cible;
    Animal* jeton;
public:
    ActionPlacerJeton(Animal* j, TuilePlacee* c);
    ~ActionPlacerJeton();
    int executer() override; // Modifié pour retourner un int
    void annuler() override;
    void afficher() const override {
        cout << "\nAction Placement du jeton " << endl;
        if (jeton) {
            cout << "Jeton placé : " << AnimalFormateur{ *jeton, Format::Complet } << endl;
        }
        else {
            cout << "Aucun jeton placé." << endl;
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
    virtual int executer() = 0; // Méthode virtuelle pure modifiée pour retourner un int
    virtual void annuler() = 0; // Méthode virtuelle pure
    virtual void afficher() const = 0;
};

class SelectionTuileJeton : public ActionUtiliserJetonNature {
public:
    SelectionTuileJeton() = default; // Temporaire
    ~SelectionTuileJeton() = default; // Temporaire
    int executer() override;
    void annuler() override;
    void afficher() const override {
        cout << "\nActionUtiliserJeton pour action de sélection de tuile et jeton." << endl;
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
