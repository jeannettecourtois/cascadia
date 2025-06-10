#include "controleurTour.h"
#include "action.h"


ControleurTour::ControleurTour() {}

ControleurTour::~ControleurTour() {
    for (Action* a : listeActions) {
        delete a;
    }
    listeActions.clear();
}

void ControleurTour::executerAction(Action* a) {
    a->executer();
    listeActions.push_back(a);
}

void ControleurTour::annulerDerniereAction() {
    if (!listeActions.empty()) {
        listeActions.back()->afficher(); //!!! Pour debug -> observe que lorsque annule, 3. selection jeton faune: enft annule placement tuile 3
        listeActions.back()->annuler(); // annuler la derniere action
        delete listeActions.back(); // liberer la memoire
        listeActions.pop_back(); // supprimer l'action du vecteur
    }
}

void ControleurTour::afficherActions() const {
    cout << "\nListe des actions du tour : " << endl;
    for (Action* action : listeActions) {
        action->afficher();
    }
}

json ControleurTour::toJson() const {
    json j = json::array();
    for (const auto& action : listeActions)
        j.push_back(action->toJson());
    return j;
}

void ControleurTour::viderActions() {
    for (Action* a : listeActions) delete a;
    listeActions.clear();
}