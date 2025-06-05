#include "tuile.h"

Tuile::~Tuile() {
    // Supprimer chaque Animal* et Habitat* puis laisser le vecteur se détruire
    for (Animal* a : listeAnimaux)
        delete a;
    for (Habitat* h : listeHabitat)
        delete h;
    // Les vecteurs se libèrent automatiquement
}

bool Tuile::contientAnimal(const Animal& a) const {
    for (unsigned int i = 0; i < nbAnimaux; ++i) {
        if (*listeAnimaux[i] == a) {
            return true;
        }
    }
    return false;
}

/*
Keystone::Keystone() : Tuile() {}

Keystone::~Keystone() {}

// Donne un jeton keyStone
joueur si pose animal sur bonne case keyStone
void Keystone::associationJeton() const {}*/
