#include "enum.h"
#include <iostream>

std::ostream& operator<<(std::ostream& f, const Animal& animal) {
    switch (animal) {
    case Animal::Vide: f << "V"; break;
    case Animal::Ours: f << "O"; break;
    case Animal::Cerf: f << "C"; break;
    case Animal::Saumon: f << "S"; break;
    case Animal::Aigle: f << "A"; break;
    case Animal::Renard: f << "R"; break;
    }
    return f;
}

std::ostream& operator<<(std::ostream& os, const Habitat& habitat) {
    switch (habitat) {
    case Habitat::Montagne: os << "M"; break;
    case Habitat::Foret:    os << "F"; break;
    case Habitat::Prairie:  os << "P"; break;
    case Habitat::Marais:   os << "M"; break;
    case Habitat::Fleuve:   os << "F"; break;
    }
    return os;
}