#include "enum.h"
#include <iostream>

std::ostream& operator<<(std::ostream& f, const Animal& animal) {
    switch (animal) {
    case Animal::Vide: f << "Vide"; break;
    case Animal::Ours: f << "Ours"; break;
    case Animal::Cerf: f << "Cerf"; break;
    case Animal::Saumon: f << "Saumon"; break;
    case Animal::Aigle: f << "Aigle"; break;
    case Animal::Renard: f << "Renard"; break;
    }
    return f;
}

std::ostream& operator<<(std::ostream& os, const Habitat& habitat) {
    switch (habitat) {
    case Habitat::Montagne: os << "Montagne"; break;
    case Habitat::Foret:    os << "Foret";    break;
    case Habitat::Prairie:  os << "Prairie";  break;
    case Habitat::Marais:   os << "Marais";   break;
    case Habitat::Fleuve:   os << "Fleuve";   break;
    }
    return os;
}