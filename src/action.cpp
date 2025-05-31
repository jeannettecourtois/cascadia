#include "action.h"

using namespace std;

void Action::executer() {}
void Action::annuler() {}

ActionSelectionTuile::ActionSelectionTuile(int indice, Pioche* p) : indiceSelection(indice), pioche(p), tuileSelection(nullptr)  {}
ActionSelectionTuile::~ActionSelectionTuile() {}

void ActionSelectionTuile::executer() {
    if (indiceSelection < 0 || indiceSelection > 3 || !pioche->getTuile(indiceSelection)) {
        cout << "Indice invalide." << endl;
        return;
    }
    tuileSelection = pioche->getTuile(indiceSelection);
    cout << "Tuile " << indiceSelection << " sélectionnée." << endl;
}

void ActionSelectionTuile::annuler() {}

ActionSelectionJeton::ActionSelectionJeton() : jetonSelection(Animal::Vide) {
    // To do
}
ActionSelectionJeton::~ActionSelectionJeton() {}
void ActionSelectionJeton::executer() {}
void ActionSelectionJeton::annuler() {}

ActionPlacerTuile::ActionPlacerTuile(Tuile* t, const Position& p) : tuile(t), pos(p) {}
ActionPlacerTuile::~ActionPlacerTuile() {}
void ActionPlacerTuile::executer() {
    //indiquer la position de la tuile
    //donner un indice
}
void ActionPlacerTuile::annuler() {}

ActionPlacerJeton::ActionPlacerJeton(Animal* j, TuilePlacee* c) : jeton(j), cible(c) {}
ActionPlacerJeton::~ActionPlacerJeton() {}
void ActionPlacerJeton::executer() {
    //verifier si la tuile est deja placee
    // verifier si l'animal du jeton est valide pour la tuile
}
void ActionPlacerJeton::annuler() {}

ActionUtiliserJetonNature::ActionUtiliserJetonNature(Action* a) : actionCible(a) {}
ActionUtiliserJetonNature::~ActionUtiliserJetonNature() {}
void ActionUtiliserJetonNature::executer() {}
void ActionUtiliserJetonNature::annuler() {}