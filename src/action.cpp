#include "action.h"
#include "joueur.h"

using namespace std;

// Classe Action : implementation des methodes de base
int Action::executer() { return 0; }  // On peut definir un retour generique ici
void Action::annuler() {}

Action* Action::fromJson(const json& j, Partie* partie) {
    // Recupere l'action courante
    const string type = j.at("type");

    if (type == "SelectionTuile") {
        int index = j.at("index");
        return new ActionSelectionTuile(index, partie->getPioche());
    }
    else if (type == "PlacementTuile") { //!!!! attentio à peut-etre adapter !!
        Tuile* tuile = new Tuile(Tuile::fromJson(j.at("tuile")));
        Position pos{ j["position"]["x"], j["position"]["y"] };
        Joueur* jr = new Joueur(partie, j.at("joueur"));
        return new ActionPlacerTuile(tuile, pos, jr);
    }
    else if (type == "SelectionJeton") {
        int a = j.at("indiceJeton");
        return new ActionSelectionJeton(a, partie->getPioche());
    }
    else if (type == "PlacementJeton") {
        Animal* a = fromStringAnimal(j.at("jeton"));
        TuilePlacee* t = new TuilePlacee(TuilePlacee::fromJson(j.at("TuileCible")));
        return new ActionPlacerJeton(a, t);
    }

    throw runtime_error("Action inconnue : " + type);
}

// ActionSelectionTuile : selection d'une tuile dans la pioche
ActionSelectionTuile::ActionSelectionTuile(int indice, Pioche* p)
    : indiceSelection(indice), pioche(p), tuileSelection(nullptr) {
}

ActionSelectionTuile::~ActionSelectionTuile() {}

// Valider la selection de la tuile
int ActionSelectionTuile::executer() {
    if (indiceSelection < 0 || indiceSelection > 3 || !pioche->getTuile(indiceSelection)) {
        cout << "Indice invalide." << endl;
        return -1;  // Retourne un code d'erreur
    }
    tuileSelection = pioche->getTuile(indiceSelection);
    cout << "Tuile " << indiceSelection << " selectionnee :" << endl;
    return indiceSelection;  // Retourne l'indice de la tuile selectionnee
}

void ActionSelectionTuile::annuler() {
    if (tuileSelection) {
        cout << "Annulation : selection de la tuile " << indiceSelection << " annulee." << endl;
        tuileSelection = nullptr;
    }
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
    cout << "Jeton " << indiceSelection << " selectionne : " << AnimalFormateur{ jetonSelection, Format::Complet } << endl;
    return indiceSelection;  // Retourne l'indice du jeton selectionne
}

void ActionSelectionJeton::annuler() {
    // Logique pour annuler l'action si necessaire
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// ActionPlacerTuile
ActionPlacerTuile::ActionPlacerTuile(Tuile* t, const Position& p, Joueur* j)
    : tuile(t), pos(p), joueur(j) {
}

ActionPlacerTuile::~ActionPlacerTuile() {}

int ActionPlacerTuile::executer() {
    // Indiquer la position de la tuile et verifier la validite avant de la placer
    TuilePlacee t(tuile, Animal::Vide, pos, 0); // Cree une tuile placee, la rotation peut etre ajoutee ici
    joueur->getPlateau()->ajouterTuile(t);  // Ajoute la tuile au plateau
    cout << "Tuile placee a la position (" << pos.x << ", " << pos.y << ")." << endl;
    return 1;  // Indiquer que l'action a ete executee avec succes
}

void ActionPlacerTuile::annuler() {
    // Logique pour annuler l'action si necessaire
    if (joueur) {
        // Supprimer la tuile du plateau du joueur a la position correspondante
        joueur->getPlateau()->supprimerTuile(pos);
        cout << "Annulation du placement de la tuile." << endl;
    }
}

json ActionPlacerTuile::toJson() const {
    return {
        {"type", "PlacementTuile"},
        {"tuile", tuile->toJson()},
        {"position", {{"x", pos.x}, {"y", pos.y}}},
        { "joueur", joueur->getIdJoueur() }
    };
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// ActionPlacerJeton
ActionPlacerJeton::ActionPlacerJeton(Animal* j, TuilePlacee* c)
    : jeton(j), cible(c) {
}

ActionPlacerJeton::~ActionPlacerJeton() {}

int ActionPlacerJeton::executer() {
    // Verifier si la tuile est deja placee et si l'animal du jeton est valide pour cette tuile
    if (!cible || !jeton) {
        cout << "Tuile ou jeton invalide." << endl;
        return -1;  // Retourner une erreur si la tuile ou le jeton est invalide
    }

    /*
    // Exemple de logique pour verifier la compatibilite du jeton avec la tuile
    if (//condition de validite du jeton ) {
        cout << "Jeton place sur la tuile." << endl;
        return 1;  // Action reussie
    }
    else {
        cout << "Le jeton ne peut pas etre place sur cette tuile." << endl;
        return -1;  // Retourne une erreur si l'action echoue
    }
    */
}

void ActionPlacerJeton::annuler() {
    // Logique pour annuler l'action si necessaire
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// ActionUtiliserJetonNature
ActionUtiliserJetonNature::ActionUtiliserJetonNature(Action* a) : actionCible(a) {}

ActionUtiliserJetonNature::~ActionUtiliserJetonNature() {}

int ActionUtiliserJetonNature::executer() {
    // Logique a definir pour cette classe abstraite
    return 0;  // Par defaut, renvoie 0
}

void ActionUtiliserJetonNature::annuler() {
    // Logique pour annuler l'action si necessaire
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
