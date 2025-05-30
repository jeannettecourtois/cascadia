#pragma once
#include "tuilePlacee.h"
#include "enum.h"


class Action { //classe abstraite
public:
    virtual void executer()=0; //méthode virtuelle pure, définition dans les classes dérivées
    virtual void annuler()=0; //méthode virtuelle pure
    virtual ~Action() = default; // destructeur virtuel car la classe est sous-classée
};

// Selection de tuile dans la pioche
class ActionSelectionTuile: public Action {
private:
    Tuile* tuileSelection;
public:
    ActionSelectionTuile();
    ~ActionSelectionTuile();
    void executer();
    void annuler();
};

// Selection de jeton dans la pioche
class ActionSelectionJeton: public Action {
private:
    Animal jetonSelection;
public:
    ActionSelectionJeton();
    ~ActionSelectionJeton();
    void executer();
    void annuler();
};

// Placer une tuile sur le plateau
class ActionPlacerTuile: public Action {
private:
    Tuile* tuile;
    Position pos;
public:
    ActionPlacerTuile(Tuile* t, const Position& p);
    ~ActionPlacerTuile();
    void executer();
    void annuler();
};

// Placer un jeton sur une tuile qui est déjà placée
class ActionPlacerJeton: public Action {
private:
    TuilePlacee* cible;
    Animal* jeton;
public:
    ActionPlacerJeton(Animal* j, TuilePlacee* c);
    ~ActionPlacerJeton();
    void executer();
    void annuler();
};


class ActionUtiliserJetonNature: public Action { //classe abstraite
private:
    Action* actionCible;
public:
    ActionUtiliserJetonNature(Action* a);
    virtual ~ActionUtiliserJetonNature();
    virtual void executer()=0; //méthode virtuelle pure
    virtual void annuler()=0; //méthode virtuelle pure
};

class SelectionTuileJeton : public ActionUtiliserJetonNature {
public:
    SelectionTuileJeton() = default; // Temporaire
    ~SelectionTuileJeton() = default; // Temporaire
    void executer();
    void annuler();
};

class SupressionJetonPioche : public ActionUtiliserJetonNature {
public:
    SupressionJetonPioche() = default; // Temporaire
    ~SupressionJetonPioche() = default; // Temporaire
    void executer();
    void annuler();
};