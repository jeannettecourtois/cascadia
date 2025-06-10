#include "action.h"

using namespace std;

// Classe Action : implementation des methodes de base
int Action::executer() { return 0; }  // On peut definir un retour generique ici
void Action::annuler() {}

Action* Action::fromJson(const json& j, Partie* partie) {
    const std::string type = j.at("type");

    if (type == "SelectionTuile") {
        int index = j.at("indiceTuile");
        return new ActionSelectionTuile(index, partie->getPioche());
    }

    if (type == "SelectionJeton") {
        int index = j.at("indiceJeton");
        Animal jeton = Animal::Vide;
        if (j.contains("jeton") && j["jeton"].is_string()) {
            jeton = fromStringAnimal(j["jeton"].get<std::string>());
        }
        else {
            std::cerr << "[WARN] jeton non trouve ou mal type dans JSON, valeur forcee a Vide\n";
            jeton = Animal::Vide;
        }
        auto* action = new ActionSelectionJeton(index, partie->getPioche());
        action->setJetonSelection(jeton); // ici ça validera la valeur
        return action;
    }

    if (type == "PlacementTuile") {
        int x = j.at("position").at("x");
        int y = j.at("position").at("y");
        unsigned int idTuile = j.at("tuile").at("id");
        Tuile* tuile = ControleurGeneral::getInstance().getTuileById(idTuile);
        return new ActionPlacerTuile(tuile, Position{ x, y }, partie->getJoueur(partie->getJoueurCourant()));
    }

    if (type == "PlacementJeton") {
        Animal jeton = fromStringAnimal(j.at("jeton"));
        Position pos{
            j["TuileCible"]["x"],
            j["TuileCible"]["y"]
        };
        return new ActionPlacerJeton(jeton, pos, partie->getJoueur(partie->getJoueurCourant()));
    }

    throw std::runtime_error("Type d'action inconnu : " + type);
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
    if (!pioche || indiceSelection < 0 || indiceSelection > 3 || !pioche->getJeton(indiceSelection)) {
        std::cerr << "[ERROR] ActionSelectionJeton::executer() - pioche invalide ou indice incorrect\n";
        return -1;
    }
    jetonSelection = *pioche->getJeton(indiceSelection);
    return indiceSelection;
    /*
    if (indiceSelection < 0 || indiceSelection > 3 || !pioche->getJeton(indiceSelection)) {
        cout << "Indice invalide." << endl;
        return -1;  // Retourne un code d'erreur
    }
    Animal* jeton = pioche->getJeton(indiceSelection);
    jetonSelection = *jeton;
    cout << "Jeton " << indiceSelection << " selectionne : " << AnimalFormateur{ jetonSelection, Format::Complet } << endl;
    return indiceSelection;  // Retourne l'indice du jeton selectionne
    */
}

void ActionSelectionJeton::annuler() {
    // Logique pour annuler l'action si necessaire
}

json ActionSelectionJeton::toJson() const {
    return {
        {"type", "SelectionJeton"},
        {"indiceJeton", indiceSelection},
        {"jeton", toString(jetonSelection)}
    };
}
ActionSelectionJeton* ActionSelectionJeton::fromJson(const json& j, Pioche* p) {
    int index = j.at("indiceJeton");
    Animal a = fromStringAnimal(j.at("jeton"));
    auto* act = new ActionSelectionJeton(index, p);
    act->setJetonSelection(a);
    return act;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// ActionPlacerTuile
ActionPlacerTuile::ActionPlacerTuile(Tuile* t, const Position& p, Joueur* j)
    : tuile(t), pos(p), joueur(j) {
}

ActionPlacerTuile::~ActionPlacerTuile() {}

int ActionPlacerTuile::executer() {
    // Indiquer la position de la tuile et verifier la validite avant de la placer
    TuilePlacee tPlacee(tuile, Animal::Vide, pos, 0); // Cree une tuile placee, la rotation peut etre ajoutee ici
    joueur->getPlateau()->ajouterTuile(tPlacee);  // Ajoute la tuile au plateau
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
        {"position", {{"x", pos.x}, {"y", pos.y}}}
    };
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// ActionPlacerJeton
ActionPlacerJeton::~ActionPlacerJeton() {}

int ActionPlacerJeton::executer() {
    TuilePlacee* cible = joueur->getPlateau()->getTuilePlacee(posTuile);
    if (!cible) {
        std::cerr << "ActionPlacerJeton::executer -> tuile introuvable\n";
        return -1;
    }
    // Verifie si la tuile accepte ce jeton (animal)
    if (!cible->getTuile()->contientAnimal(jeton)) {
        std::cout << "Le jeton " << toString(jeton)
            << " ne peut pas etre place sur cette tuile." << std::endl;
        return -1;
    }
    // Verifie si un jeton est deja place
    if (cible->getJeton() != Animal::Vide) {
        std::cout << "Il y a deja un jeton sur cette tuile." << std::endl;
        return -1;
    }
    // Effectue le placement
    cible->ajouterJeton(jeton);
    std::cout << "Jeton " << toString(jeton) << " place avec succes." << std::endl;

    return 1;
}

void ActionPlacerJeton::annuler() {
    // Logique pour annuler l'action si necessaire
}

json ActionPlacerJeton::toJson() const {
    json j;
    j["type"] = "PlacementJeton";
    j["TuileCible"] = { {"x", posTuile.x}, {"y", posTuile.y} };
    j["jeton"] = toString(jeton);
    return j;
}
ActionPlacerJeton* ActionPlacerJeton::fromJson(const json& j, Partie* partie) {
    Joueur* joueur = partie->getJoueurCourant() >= 0 ? partie->getJoueur(partie->getJoueurCourant()) : nullptr;
    Animal a = fromStringAnimal(j.at("jeton"));
    Position pos(j.at("TuileCible").at("x"), j.at("TuileCible").at("y"));
    return new ActionPlacerJeton(a, pos, joueur);
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
