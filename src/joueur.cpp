#include "joueur.h"
#include "partie.h"
#include <vector>
#include <sstream>

class tuilePlacee;

Joueur::Joueur(Partie* p)
    : idJoueur(1), nbJetonNature(0), nomJoueur("Joueur 1"), plateau(new PlateauJoueur()), partie(p) {
    // Constructeur par defaut
}

Joueur::~Joueur() {
    delete plateau;
}

int Joueur::calculScore() {
    int score = 0;
    // Calcul des points selon les cartes de marquage de faune
    for (int i = 0; i < 5; i++) {
        score += partie->getCarteRegle(i)->calculPoints(*plateau);
    }
    // Points des jetons nature
    score += nbJetonNature;
    return score;
}

void PlateauJoueur::ajouterTuile(const TuilePlacee& tuile) {
    tuiles.push_back(tuile);  // Copie locale, gestion memoire automatique
}

void PlateauJoueur::ajouterTuileDepart(const TuileDepart* set) {
    Position pos1(0, 0); // tuile 1 au centre (0,0)
    Position pos2 = pos1.getSE();  // (0, 1)
    Position pos3 = pos1.getSW(); // (-1, 1)

    set->getTuile(0)->deplacer(pos1);
    ajouterTuile(*set->getTuile(0));

    set->getTuile(1)->deplacer(pos2);
    ajouterTuile(*set->getTuile(1));

    set->getTuile(2)->deplacer(pos3);
    ajouterTuile(*set->getTuile(2));
}

void PlateauJoueur::supprimerTuile(const Position& pos) {
    for (auto it = tuiles.begin(); it != tuiles.end(); ++it) {
        if (it->getPosition() == pos) {
            tuiles.erase(it);
            return;
        }
    }
}

void PlateauJoueur::afficherPlateau() const {
    int min_x = 0, min_y = 0, max_x = 0, max_y = 0;
    int y_min_x = 0;

    for (const auto& tuile : tuiles) {
        const Position& pos = tuile.getPosition();
        if (pos.x < min_x) min_x = pos.x, y_min_x = pos.y;
        if (pos.x > max_x) max_x = pos.x;
        if (pos.y < min_y) min_y = pos.y;
        if (pos.y > max_y) max_y = pos.y;
    }

    const int nb_lignes = 4 * (max_y - min_y + 1);
    std::vector<std::ostringstream> lignes(nb_lignes);

    for (int j = max_y; j >= min_y; j--) {
        if ((max_y - y_min_x) % 2) {
            lignes[2 * j + 0] << " ";
            lignes[2 * j + 3] << " ";
        }
        else {
            lignes[2 * j + 1] << " ";
            lignes[2 * j + 2] << " ";
        }
    }

    for (int i = min_x; i < max_x; i++) {
        for (int j = max_y; j >= min_y; j--) {
            int index = (max_y - j) * 4;
            const TuilePlacee* tuile = getTuilePlacee(Position(i, j));
            if (tuile) {
                std::ostringstream jetons;
                if (tuile->getJeton() == Animal::Vide) {
                    jetons << AnimalFormateur{ (tuile->getTuile()->getNbAnimaux() >= 2) ? *tuile->getTuile()->getListeAnimaux()[1] : Animal::Vide, Format::Court } << AnimalFormateur{ *tuile->getTuile()->getListeAnimaux()[0], Format::Court } << AnimalFormateur{ (tuile->getTuile()->getNbAnimaux() >= 3) ? *tuile->getTuile()->getListeAnimaux()[2] : Animal::Vide, Format::Court };
                }
                else {
                    jetons << "|" << AnimalFormateur{ tuile->getJeton(), Format::Court } << "|";
                }
                int nbH = tuile->getTuile()->getNbHabitat();
                int rot = tuile->getRotation();
                lignes[index + 0] << "/ " << HabitatFormateur{ *tuile->getTuile()->getListeHabitat()[(0 + rot) % nbH], Format::Court } << " \\";
                lignes[index + 1] << "/" << HabitatFormateur{ *tuile->getTuile()->getListeHabitat()[(5 + rot) % nbH], Format::Court } << i % 10 << "," << j % 10 << HabitatFormateur{ *tuile->getTuile()->getListeHabitat()[(1 + rot) % nbH], Format::Court } << "\\";
                lignes[index + 2] << "\\" << HabitatFormateur{ *tuile->getTuile()->getListeHabitat()[(4 + rot) % nbH], Format::Court } << jetons.str() << HabitatFormateur{ *tuile->getTuile()->getListeHabitat()[(2 + rot) % nbH], Format::Court } << "/";
                lignes[index + 3] << "\\_" << HabitatFormateur{ *tuile->getTuile()->getListeHabitat()[(3 + rot) % nbH], Format::Court } << "_/";
            }
            else {
                lignes[index + 0] << "     ";
                lignes[index + 1] << "       ";
                lignes[index + 2] << "       ";
                lignes[index + 3] << "     ";
            }
        }
    }

    for (const auto& ligne : lignes) {
        std::cout << ligne.str() << '\n';
    }
}
