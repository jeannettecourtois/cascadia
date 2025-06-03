#pragma once
#include <iostream>
#include <string>
#include "enum.h"
#include <random>


class Tuile {
private:
    unsigned int nbAnimaux;
    unsigned int nbHabitat;
    Animal** listeAnimaux;
    Habitat** listeHabitat;

public:
    Tuile() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist3(1, 3);
        std::uniform_int_distribution<> dist5(0, 4);

        nbHabitat = dist3(gen);
        nbAnimaux = dist3(gen);

        listeAnimaux = new Animal*[nbAnimaux];
        listeHabitat = new Habitat*[nbHabitat];

        for (unsigned int i = 0; i < nbAnimaux; ++i) {
            unsigned int unAnimal = dist5(gen);
            listeAnimaux[i] = new Animal(*(std::next(animaux.begin(), unAnimal)));
        }

        for (unsigned int i = 0; i < nbHabitat; ++i) {
            unsigned int unHabitat = dist5(gen);
            listeHabitat[i] = new Habitat(*(std::next(habitats.begin(), unHabitat)));
        }
    }

    Tuile(unsigned int nbAnimaux, unsigned int nbHabitat, const Animal* animaux, const Habitat* habitats) {
        this->nbAnimaux = nbAnimaux;
        this->nbHabitat = nbHabitat;
        listeAnimaux = new Animal*[nbAnimaux];
        listeHabitat = new Habitat*[nbHabitat];
        for (unsigned int i = 0; i < nbAnimaux; ++i)
            listeAnimaux[i] = new Animal(animaux[i]);
        for (unsigned int i = 0; i < nbHabitat; ++i)
            listeHabitat[i] = new Habitat(habitats[i]);
    }


    ~Tuile();

    bool contientAnimal(const Animal& a) const;

    int getNbHabitat() const { return nbHabitat; }
    int getNbAnimaux() const { return nbAnimaux; }
    Animal* getAnimal(unsigned int i) const { return listeAnimaux[i]; }
    Habitat* getHabitat(unsigned int i) const { return listeHabitat[i]; }
    Habitat* getListeHabitat() const { return *listeHabitat; }
    Animal* getListeAnimaux() const { return *listeAnimaux; }

    void afficherTuile(std::ostream& f = std::cout) const {
        f << "Tuile avec " << nbAnimaux << " animaux et " << nbHabitat << " habitats.\n";
        f << " | Animaux : ";
        for (unsigned int i = 0; i < nbAnimaux; ++i) {
            f << AnimalFormateur{ *listeAnimaux[i], Format::Complet } << " ";
        }
        f << "\n | Habitats : ";
        for (unsigned int i = 0; i < nbHabitat; ++i) {
            f << HabitatFormateur{ *listeHabitat[i], Format::Complet } << " ";
        }
        f << std::endl;
    }
};


/*class Keystone : public Tuile {
public:
    Keystone();
    ~Keystone() override;
    // Donne un jeton keyStone a joueur si pose animal sur bonne case keyStone
    void associationJeton() const override; // Où mettre joueur ?
    // on peut mettre dans ActionPlacerJeton car derniere action du tour
};*/