#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "enum.h"
#include <random>

class Tuile {
private:
    unsigned int nbAnimaux;
    unsigned int nbHabitat;
    std::vector<Animal*> listeAnimaux;
    std::vector<Habitat*> listeHabitat;

public:
    Tuile() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist3(1, 3);

        nbHabitat = dist3(gen);
        nbAnimaux = dist3(gen);

        // Prend de la place en prevision
        listeAnimaux.reserve(nbAnimaux);
        listeHabitat.reserve(nbHabitat);

        // Ajout unique des animaux
        std::vector<int> animauxIndices = {0, 1, 2, 3, 4};
        std::shuffle(animauxIndices.begin(), animauxIndices.end(), gen);
        for (unsigned int i = 0; i < nbAnimaux; ++i) {
            unsigned int unAnimal = animauxIndices[i];
            listeAnimaux.push_back(new Animal(*(std::next(animaux.begin(), unAnimal))));
        }

        // Ajout unique des habitats
        std::vector<int> habitatsIndices = {0, 1, 2, 3, 4};
        std::shuffle(habitatsIndices.begin(), habitatsIndices.end(), gen);
        for (unsigned int i = 0; i < nbHabitat; ++i) {
            unsigned int unHabitat = habitatsIndices[i];
            listeHabitat.push_back(new Habitat(*(std::next(habitats.begin(), unHabitat))));
        }
    }

    /*Tuile() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist3(1, 3);
        std::uniform_int_distribution<> dist5(0, 4);

        nbHabitat = dist3(gen);
        nbAnimaux = dist3(gen);

        // Prend de la place en prevision
        listeAnimaux.reserve(nbAnimaux);
        listeHabitat.reserve(nbHabitat);

        for (unsigned int i = 0; i < nbAnimaux; ++i) {
            unsigned int unAnimal = dist5(gen);
            listeAnimaux.push_back(new Animal(*(std::next(animaux.begin(), unAnimal))));
        }

        for (unsigned int i = 0; i < nbHabitat; ++i) {
            unsigned int unHabitat = dist5(gen);
            listeHabitat.push_back(new Habitat(*(std::next(habitats.begin(), unHabitat))));
        }
    }*/

    // Constructeur pour créer une tuile avec des animaux et habitats donnés
    //Utilisé pour les tuiles de départ
    Tuile(unsigned int nbAnimaux, unsigned int nbHabitat, const Animal* animauxArr, const Habitat* habitatsArr) {
        // Verification de doublons pour les animaux
        for (unsigned int i = 0; i < nbAnimaux; ++i) {
            for (unsigned int j = i + 1; j < nbAnimaux; ++j) {
                if (animauxArr[i] == animauxArr[j]) {
                    std::cerr << "Erreur : doublon d'animal détecté dans la tuile." << std::endl;
                    throw std::invalid_argument("Doublon d'animal dans la tuile");
                }
            }
        }

         // Vérification de doublons pour les habitats
        for (unsigned int i = 0; i < nbHabitat; ++i) {
            for (unsigned int j = i + 1; j < nbHabitat; ++j) {
                if (habitatsArr[i] == habitatsArr[j]) {
                    std::cerr << "Erreur : doublon d'habitat détecté dans la tuile." << std::endl;
                    throw std::invalid_argument("Doublon d'habitat dans la tuile");
                }
            }
        }

        this->nbAnimaux = nbAnimaux;
        this->nbHabitat = nbHabitat;
        listeAnimaux.reserve(nbAnimaux);
        listeHabitat.reserve(nbHabitat);

        for (unsigned int i = 0; i < nbAnimaux; ++i) {
            listeAnimaux.push_back(new Animal(animauxArr[i]));
        }
        for (unsigned int i = 0; i < nbHabitat; ++i) {
            listeHabitat.push_back(new Habitat(habitatsArr[i]));
        }
    }

    ~Tuile();

    bool contientAnimal(const Animal& a) const;

    int getNbHabitat() const { return nbHabitat; }
    int getNbAnimaux() const { return nbAnimaux; }
    Animal* getAnimal(unsigned int i) const { return listeAnimaux[i]; }
    Habitat* getHabitat(unsigned int i) const { return listeHabitat[i]; }

    // Renvoie un pointeur vers le premier élément du vecteur (équivalent à l'ancien tableau)
    Animal** getListeAnimaux() const { return const_cast<Animal**>(listeAnimaux.data()); }
    Habitat** getListeHabitat() const { return const_cast<Habitat**>(listeHabitat.data()); }

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
