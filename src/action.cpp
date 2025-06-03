#include "action.h"

using namespace std;

void Action::executer() {}
void Action::annuler() {}


/* ActionSelectionTuile */
ActionSelectionTuile::ActionSelectionTuile(int indice, Pioche* p) : indiceSelection(indice), pioche(p), tuileSelection(nullptr)  {}
ActionSelectionTuile::~ActionSelectionTuile() {}
void ActionSelectionTuile::executer() {
    if (indiceSelection < 0 || indiceSelection > 3 || !pioche->getTuile(indiceSelection)) {
        cout << "Indice invalide." << endl;
        return;
    }
    tuileSelection = pioche->getTuile(indiceSelection);
    cout << "Tuile " << indiceSelection << " selectionnee." << endl;
}
void ActionSelectionTuile::annuler() {}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/* ActionSelectionJeton */
ActionSelectionJeton::ActionSelectionJeton(int indice, Pioche* p) : indiceSelection(indice), pioche(p), jetonSelection(Animal::Vide) {}
ActionSelectionJeton::~ActionSelectionJeton() {}
void ActionSelectionJeton::executer() {
    if (indiceSelection < 0 || indiceSelection > 3 || !pioche->getJeton(indiceSelection)) {
        cout << "Indice invalide." << endl;
        return;
    }
    Animal* jeton = pioche->getJeton(indiceSelection);
    jetonSelection = *jeton;
    cout << "Jeton " << indiceSelection << " selectionnee." << AnimalFormateur{ jetonSelection, Format::Complet } << endl;
}
void ActionSelectionJeton::annuler() {}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/* ActionPlacerTuile */
ActionPlacerTuile::ActionPlacerTuile(Tuile* t, const Position& p, Joueur* j) : tuile(t), pos(p), joueur(j) {}
ActionPlacerTuile::~ActionPlacerTuile() {}
void ActionPlacerTuile::executer() {
    //indiquer la position de la tuile
    //donner un indice pour permettre au joueur de les designer ? Genre la position ?
    TuilePlacee t(tuile, Animal::Vide, pos, 0); // on créer une tuile placee, faudrait fonction pour la rotation
    joueur->getPlateau()->ajouterTuile(t);
}
void ActionPlacerTuile::annuler() {}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/* ActionPlacerJeton */
ActionPlacerJeton::ActionPlacerJeton(Animal* j, TuilePlacee* c) : jeton(j), cible(c) {}
ActionPlacerJeton::~ActionPlacerJeton() {}
void ActionPlacerJeton::executer() {
    //verifier si la tuile est deja placee
    // verifier si l'animal du jeton est valide pour la tuile
}
void ActionPlacerJeton::annuler() {}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/* ActionUtiliserJetonNature */
ActionUtiliserJetonNature::ActionUtiliserJetonNature(Action* a) : actionCible(a) {}
ActionUtiliserJetonNature::~ActionUtiliserJetonNature() {}
void ActionUtiliserJetonNature::executer() {}
void ActionUtiliserJetonNature::annuler() {}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/