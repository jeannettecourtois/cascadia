#include "enum.h"
#include <iostream>

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

std::ostream& operator<<(std::ostream& f, const HabitatFormateur& hf) {
    switch (hf.format) {
    case Format::Complet:
        switch (hf.habitat) {
        case Habitat::Montagne: f << "Montagne"; break;
        case Habitat::Foret:    f << "Foret"; break;
        case Habitat::Prairie:  f << "Prairie"; break;
        case Habitat::Marais:   f << "Marais"; break;
        case Habitat::Fleuve:   f << "Fleuve"; break;
        }
        break;

    case Format::Court:
        switch (hf.habitat) {
        case Habitat::Montagne: f << "M"; break;
        case Habitat::Foret:    f << "F"; break;
        case Habitat::Prairie:  f << "P"; break;
        case Habitat::Marais:   f << "M"; break;
        case Habitat::Fleuve:   f << "F"; break;
        }
        break;
    }
    return f;
}