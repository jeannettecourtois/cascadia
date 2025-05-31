#include "pioche.h"
#include "enum.h"

/* SacTuile */
SacTuile::SacTuile() {}
SacTuile::~SacTuile() {}
Tuile* SacTuile::Piocher() {
    return nullptr; //a changer, juste pour compiler coder
}
void SacTuile::mettreDansLeSac(Tuile&) {
    // To do
}

/* SacJeton */
SacJeton::SacJeton() {}
SacJeton::~SacJeton() {}
Animal* SacJeton::Piocher() {
    return nullptr; //a changer, juste pour compiler coder
}
void SacJeton::mettreDansLeSac(const Animal& a) {
    // To do
}

/* Pioche */
Pioche::Pioche() {
    for (int i = 0; i < 4; ++i) {
        tuiles[i] = nullptr;
        jetons[i] = nullptr;
    }
    idTuileChoisis = -1;
    sacDeTuiles = new SacTuile();
    sacDeJetons = new SacJeton();
}

Pioche::~Pioche() {
    for (int i = 0; i < 4; ++i) {
        delete tuiles[i]; 
    }
    delete sacDeTuiles;
    delete sacDeJetons;
}

void Pioche::preparerPioche() {
    // Preparer la pioche de tuiles et de jetons
    for (int i = 0; i < 4; ++i) {
        tuiles[i] = sacDeTuiles->Piocher();
        jetons[i] = sacDeJetons->Piocher();
    }
}

void Pioche::selectionnerTuile() {
    // Selectionner une tuile parmi celles disponibles
    idTuileChoisis = rand() % 4;
}

Animal Pioche::selectionnerJeton() const {
    int idJetonChoisis = rand() % 4;
    return *jetons[idJetonChoisis];
}

void Pioche::afficherTuilesDisponibles() const {
    std::cout << "Tuiles disponibles dans la pioche :\n";
    for (int i = 0; i < 4; ++i) {
        if (tuiles[i]) {
            std::cout << "Tuile " << i << ": "; tuiles[i]->afficherTuile(); cout << "\n";
        } else {
            std::cout << "Tuile " << i << ": Vide\n";
        }
    }
}