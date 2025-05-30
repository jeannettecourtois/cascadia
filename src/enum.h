#pragma once
#include <initializer_list>
#include <iostream>
//Mieux d'utiliser "Animal" que "animal"
//Même chose pour "Habitat" que "habitat"
enum class Animal {
    Vide = 0,
    Ours = 1,
    Cerf, 
    Saumon,
    Aigle,
    Renard
};

std::ostream& operator<<(std::ostream& f, const Animal& animal) {
    switch (animal) {
    case Animal::Vide: f << "Vide"; break;
    case Animal::Ours: f << "Ours"; break;
    case Animal::Cerf: f << "Cerf"; break;
    case Animal::Saumon: f << "Saumon"; break;
    case Animal::Aigle: f << "Aigle"; break;
    case Animal::Renard: f << "Renard";break;
    }
    return f;
}

enum class Habitat {
    Montagne,
    Foret,
    Prairie,
    Marais,
    Fleuve
};

enum class presenceAnimal {
    PasRenard = -5,
    PasAigle,
    PasSaumon,
    PasCerf,
    PasOurs,
    Ours = 1,
    Cerf,
    Saumon,
    Aigle,
    Renard
};


//Quelques listes à devoirs utiliser pour

const std::initializer_list<Animal> animaux = {Animal::Aigle, Animal::Cerf, Animal::Ours, Animal::Renard, Animal::Saumon};
const std::initializer_list<Habitat> habitats = {Habitat::Fleuve, Habitat::Foret, Habitat::Marais, Habitat::Montagne, Habitat::Prairie};
