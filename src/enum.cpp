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