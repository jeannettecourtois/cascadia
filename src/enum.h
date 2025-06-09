#pragma once
#include <initializer_list>
#include <iostream>

enum class Animal {
    Vide = 0,
    Ours = 1,
    Cerf = 2, 
    Saumon = 3,
    Aigle = 4,
    Renard = 5
};

enum class Habitat {
    Montagne,
    Foret,
    Prairie,
    Marais,
    Riviere
};

std::string toString(const Animal& a);
Animal fromStringAnimal(const std::string& s);

std::string toString(const Habitat& h);
Habitat fromStringHabitat(const std::string& s);

// Pour differencier l'affichage entre dans le jeu (affichage) et plateau (tuile)
enum class Format {
    Court,
    Complet
};

// Formatters
struct AnimalFormateur {
    const Animal& animal;
    Format format;
};

struct HabitatFormateur {
    const Habitat& habitat;
    Format format;
};

// Operateurs de sortie formates
std::ostream& operator<<(std::ostream& os, const AnimalFormateur& af);
std::ostream& operator<<(std::ostream& os, const HabitatFormateur& hf);
// Exemple d'utilisation : AnimalFormateur{ tuile->getJeton(), Format::Court }

//Quelques listes a devoirs utiliser pour
const std::initializer_list<Animal> animaux = {Animal::Aigle, Animal::Cerf, Animal::Ours, Animal::Renard, Animal::Saumon};
const std::initializer_list<Habitat> habitats = {Habitat::Riviere, Habitat::Foret, Habitat::Marais, Habitat::Montagne, Habitat::Prairie};
