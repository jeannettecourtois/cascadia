#pragma once
#include <vector>
#include "action.h"

class Action;

using namespace std;

// ControleurTour : gere les actions du tour de jeu
class ControleurTour {
    vector<Action*> listeActions; // Liste des actions effectuees durant le tour du joueur
public:
    ControleurTour();
    ~ControleurTour();
    void executerAction(Action* a);
    void annulerDerniereAction();
    void afficherActions() const;
    vector<Action*>& getListeActions() { return listeActions; }
    void viderActions();

    json toJson() const;
};