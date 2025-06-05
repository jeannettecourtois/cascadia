#include "tuile.h"

Tuile::~Tuile() {
    for(unsigned int i = 0; i<nbAnimaux; ++i){
        delete listeAnimaux[i]; 
        /*!!!On a une erreur critique ici dès qu'un tour est fini : 
        * Exception levée : violation d'accès en lecture. this->listeAnimaux a été 0x111011101110111.
        * le pointeur listeAnimaux ne pointe pas vers une mémoire valide au moment du delete[]
        */
    }
    delete[] listeAnimaux;
    for(unsigned int i = 0; i<nbHabitat; ++i){
        delete listeHabitat[i];
    }
    delete[] listeHabitat;
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
