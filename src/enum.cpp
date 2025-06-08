#include "enum.h"
#include <iostream>
#include <string>
#include <stdexcept>

std::ostream& operator<<(std::ostream& f, const AnimalFormateur& af) {
    switch (af.format) {
    case Format::Complet:
        switch (af.animal) {
        case Animal::Vide:   f << "Vide"; break;
        case Animal::Ours:   f << "Ours"; break;
        case Animal::Cerf:   f << "Cerf"; break;
        case Animal::Saumon: f << "Saumon"; break;
        case Animal::Aigle:  f << "Aigle"; break;
        case Animal::Renard: f << "Renard"; break;
        }
        break;

    case Format::Court:
        switch (af.animal) {
        case Animal::Vide:   f << "V"; break;
        case Animal::Ours:   f << "O"; break;
        case Animal::Cerf:   f << "C"; break;
        case Animal::Saumon: f << "S"; break;
        case Animal::Aigle:  f << "A"; break;
        case Animal::Renard: f << "R"; break;
        }
        break;
    }
    return f;
}

// COnverti un animal vers un string
std::string toString(const Animal& a) {
    switch (a) {
    case Animal::Vide:   return "Vide";
    case Animal::Ours:   return "Ours";
    case Animal::Cerf:   return "Cerf";
    case Animal::Saumon: return "Saumon";
    case Animal::Aigle:  return "Aigle";
    case Animal::Renard: return "Renard";
    default: throw std::invalid_argument("Invalid Animal enum");
    }
}

// COnverti un string vers un animal
Animal* fromStringAnimal(const std::string& s) {
    if (s == "Vide")   return new Animal(Animal::Vide);
    if (s == "Ours")    return new Animal(Animal::Ours);
    if (s == "Cerf")    return new Animal(Animal::Cerf);
    if (s == "Saumon")   return new Animal(Animal::Saumon);
    if (s == "Aigle") return new Animal(Animal::Aigle);
    if (s == "Renard") return new Animal(Animal::Renard);
    throw std::invalid_argument("Unknown Animal string: " + s);
}

/* ### Habitat ### */

std::ostream& operator<<(std::ostream& f, const HabitatFormateur& hf) {
    switch (hf.format) {
    case Format::Complet:
        switch (hf.habitat) {
        case Habitat::Montagne: f << "Montagne"; break;
        case Habitat::Foret:    f << "Foret"; break;
        case Habitat::Prairie:  f << "Prairie"; break;
        case Habitat::Marais:   f << "Marais"; break;
        case Habitat::Riviere:   f << "Fleuve"; break;
        }
        break;

    case Format::Court:
        switch (hf.habitat) {
        case Habitat::Montagne: f << "M"; break;
        case Habitat::Foret:    f << "F"; break;
        case Habitat::Prairie:  f << "P"; break;
        case Habitat::Marais:   f << "M"; break;
        case Habitat::Riviere:   f << "R"; break;
        }
        break;
    }
    return f;
}

// Converti un habitat en chaîne de caractere
std::string toString(const Habitat& h) {
    switch (h) {
    case Habitat::Foret:     return "Foret";
    case Habitat::Montagne:  return "Montagne";
    case Habitat::Riviere:   return "Riviere";
    case Habitat::Prairie:   return "Prairie";
    case Habitat::Marais:    return "Marais";
    default: throw std::invalid_argument("Invalid Habitat enum");
    }
}

// Converti un string en habitat
Habitat* fromStringHabitat(const std::string& s) {
    if (s == "Foret")     return new Habitat(Habitat::Foret);
    if (s == "Montagne")  return new Habitat(Habitat::Montagne);
    if (s == "Riviere")   return new Habitat(Habitat::Riviere);
    if (s == "Prairie")   return new Habitat(Habitat::Prairie);
    if (s == "Marais")    return new Habitat(Habitat::Marais);
    throw std::invalid_argument("Unknown Habitat string: " + s);
}