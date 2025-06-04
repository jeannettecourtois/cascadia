#include "action.h"

using namespace std;

// Classe Action : implémentation des méthodes de base
int Action::executer() { return 0; }  // On peut définir un retour générique ici
void Action::annuler() {}

// ActionSelectionTuile
ActionSelectionTuile::ActionSelectionTuile(int indice, Pioche* p)
    : indiceSelection(indice), pioche(p), tuileSelection(nullptr) {
}

ActionSelectionTuile::~ActionSelectionTuile() {}

int ActionSelectionTuile::executer() {
    if (indiceSelection < 0 || indiceSelection > 3 || !pioche->getTuile(indiceSelection)) {
        cout << "Indice invalide." << endl;
        return -1;  // Retourne un code d'erreur
    }
    tuileSelection = pioche->getTuile(indiceSelection);
    cout << "Tuile " << indiceSelection << " sélectionnée." << endl;
    return indiceSelection;  // Retourne l'indice de la tuile sélectionnée
}

void ActionSelectionTuile::annuler() {
    // Logique pour annuler l'action si nécessaire
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// ActionSelectionJeton
ActionSelectionJeton::ActionSelectionJeton(int indice, Pioche* p)
    : indiceSelection(indice), pioche(p), jetonSelection(Animal::Vide) {
}

ActionSelectionJeton::~ActionSelectionJeton() {}

int ActionSelectionJeton::executer() {
    if (indiceSelection < 0 || indiceSelection > 3 || !pioche->getJeton(indiceSelection)) {
        cout << "Indice invalide." << endl;
        return -1;  // Retourne un code d'erreur
    }
    Animal* jeton = pioche->getJeton(indiceSelection);
    jetonSelection = *jeton;
    cout << "Jeton " << indiceSelection << " sélectionné : "
        << AnimalFormateur{ jetonSelection, Format::Complet } << endl;
    return indiceSelection;  // Retourne l'indice du jeton sélectionné
}

void ActionSelectionJeton::annuler() {
    // Logique pour annuler l'action si nécessaire
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// ActionPlacerTuile
ActionPlacerTuile::ActionPlacerTuile(Tuile* t, const Position& p, Joueur* j)
    : tuile(t), pos(p), joueur(j) {
}

ActionPlacerTuile::~ActionPlacerTuile() {}

int ActionPlacerTuile::executer() {
    // Indiquer la position de la tuile et vérifier la validité avant de la placer
    TuilePlacee t(tuile, Animal::Vide, pos, 0); // Crée une tuile placée, la rotation peut être ajoutée ici
    joueur->getPlateau()->ajouterTuile(t);  // Ajoute la tuile au plateau
    cout << "Tuile placée à la position (" << pos.x << ", " << pos.y << ")." << endl;
    return 1;  // Indiquer que l'action a été exécutée avec succès
}

void ActionPlacerTuile::annuler() {
    // Logique pour annuler l'action si nécessaire
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// ActionPlacerJeton
ActionPlacerJeton::ActionPlacerJeton(Animal* j, TuilePlacee* c)
    : jeton(j), cible(c) {
}

ActionPlacerJeton::~ActionPlacerJeton() {}

int ActionPlacerJeton::executer() {
    // Vérifier si la tuile est déjà placée et si l'animal du jeton est valide pour cette tuile
    if (!cible || !jeton) {
        cout << "Tuile ou jeton invalide." << endl;
        return -1;  // Retourner une erreur si la tuile ou le jeton est invalide
    }

    /*
    // Exemple de logique pour vérifier la compatibilité du jeton avec la tuile
    if (//condition de validité du jeton ) {
        cout << "Jeton placé sur la tuile." << endl;
        return 1;  // Action réussie
    }
    else {
        cout << "Le jeton ne peut pas être placé sur cette tuile." << endl;
        return -1;  // Retourne une erreur si l'action échoue
    }
    */
}

void ActionPlacerJeton::annuler() {
    // Logique pour annuler l'action si nécessaire
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// ActionUtiliserJetonNature
ActionUtiliserJetonNature::ActionUtiliserJetonNature(Action* a) : actionCible(a) {}

ActionUtiliserJetonNature::~ActionUtiliserJetonNature() {}

int ActionUtiliserJetonNature::executer() {
    // Logique à définir pour cette classe abstraite
    return 0;  // Par défaut, renvoie 0
}

void ActionUtiliserJetonNature::annuler() {
    // Logique pour annuler l'action si nécessaire
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
