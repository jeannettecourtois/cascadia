#include "controleurTour.h"
#include <iostream>
using namespace std;

ControleurTour::ControleurTour() {}

ControleurTour::~ControleurTour() {
    viderActions();
}

void ControleurTour::executerAction(Action* a) {
    if (!a) return;
    a->executer();
    listeActions.push_back(a);
}

void ControleurTour::annulerDerniereAction() {
    if (!listeActions.empty()) {
        listeActions.back()->annuler();
        delete listeActions.back();
        listeActions.pop_back();
    }
}

void ControleurTour::afficherActions() const {
    cout << "Actions effectuees ce tour :\n";
    for (const auto& action : listeActions) {
        action->afficher();
    }
}

void ControleurTour::viderActions() {
    for (auto& a : listeActions) delete a;
    listeActions.clear();
}

json ControleurTour::toJson() const {
    json j;
    for (const auto& a : listeActions) {
        j.push_back(a->toJson());
    }
    return j;
}
