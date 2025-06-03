#include "joueur.h"
#include "partie.h"
#include <vector>
#include <sstream>

Joueur::Joueur(Partie* p) : idJoueur(1), nbJetonNature(0), nomJoueur("Joueur 1"), plateau(new PlateauJoueur()), partie(p) {
    // Constructeur par defaut
}

Joueur :: ~Joueur() {
    delete plateau;
}
int Joueur::calculScore() {
    int score = 0;
    // Calculer le score en fonction des tuiles placees et des jetons selon les regles
    // Calcul des points d'environnement



    //Plus points majorité de couloir à ajouter (c'est à la partie de le faire, c'est elle qui peut avoir accès aux infos des différents joueurs)

    // Calcul des points des animaux (après la réu => car dépend de CarteMarquageFaune)
    for (int i = 0; i < 5; i++) {
        score += partie->getCarteRegle(i)->calculPoints(*plateau);
    }
    // Points jeton nature
    score += nbJetonNature;
    return score;
}


PlateauJoueur :: PlateauJoueur() : nbTuiles(0), tuiles(nullptr) {
    // Constructeur par defaut
}
PlateauJoueur :: ~PlateauJoueur() {
    for (int i = 0; i < nbTuiles; ++i) {
        delete tuiles[i];
    }
    delete[] tuiles;
}
void PlateauJoueur:: ajouterTuile(const TuilePlacee* tuile) {
    // Ajouter une tuile au plateau du joueur
    TuilePlacee** newTuiles = new TuilePlacee*[nbTuiles + 1]; //nouveau tableau de taille nbTuiles + 1
    for (int i = 0; i < nbTuiles; ++i) {
        newTuiles[i] = tuiles[i]; //copie des tuiles existantes
    }
    newTuiles[nbTuiles] = new TuilePlacee(*tuile); //copie de la tuile passe en parametre
    delete[] tuiles;
    tuiles = newTuiles; //nouveau tableau avec la nouvelle tuile placee a la fin
    nbTuiles++;
}
void PlateauJoueur::ajouterTuileDepart(const TuileDepart* set) {
    Position pos1(0, 0); //tuile 1 au centre (0,0)
    Position pos2 = pos1.getSE();  // (0, 1)
    Position pos3 = pos1.getSW(); // (-1, 1)

    set->getTuile(0)->deplacer(pos1);
    ajouterTuile(set->getTuile(0));

    set->getTuile(1)->deplacer(pos2);
    ajouterTuile(set->getTuile(1));

    set->getTuile(2)->deplacer(pos3);
    ajouterTuile(set->getTuile(2));
}

void PlateauJoueur::afficherPlateau() const {
    int min_x = 0, min_y = 0, max_x = 0, max_y = 0;
    int y_min_x = 0;//La valeur de y pour laquelle x est minimal
    for (int i = 0; i < nbTuiles; ++i) {
        const TuilePlacee* tuile = tuiles[i];
        if (tuile) {
            const Position& pos = tuile->getPosition();
            if (pos.x < min_x) min_x = pos.x, y_min_x = pos.y;
            if (pos.x > max_x) max_x = pos.x;
            if (pos.y < min_y) min_y = pos.y;
            if (pos.y > max_y) max_y = pos.y;
        }
    }
    const int nb_lignes = 2 * (max_y - min_y) + 2; // diviser tout par 4 plutôt que deux
    std::vector<std::ostringstream> lignes(nb_lignes);
    //Initialisation des espaces
    for (int j = max_y; j < min_y; j--) { // changer par (int j = max_y; j >= min_y; j--)
        if ((max_y - y_min_x) % 2)
        {
            lignes[2 * j + 0] << " ";
            lignes[2 * j + 3] << " ";
        }
        else {
            lignes[2 * j + 1] << " ";
            lignes[2 * j + 2] << " ";
        }
    }

    for (int i = min_x; i < max_x; i++) {
        for (int j = max_y; j < min_y; j--) { // changer par (int j = max_y; j >= min_y; j--)
            TuilePlacee* tuile = getTuilePlacee(Position(i, j));
            if (tuile) {
                std::ostringstream jetons;
                if (tuile->getJeton() == Animal::Vide) {
                    jetons << AnimalFormateur{ ((tuile->getTuile()->getNbAnimaux() >= 2) ? tuile->getTuile()->getListeAnimaux()[1] : Animal::Vide), Format::Court };
                    jetons << AnimalFormateur{ tuile->getTuile()->getListeAnimaux()[0], Format::Court };
                    jetons << AnimalFormateur{ ((tuile->getTuile()->getNbAnimaux() >= 3) ? tuile->getTuile()->getListeAnimaux()[2] : Animal::Vide), Format::Court };

                }
                else {
                    jetons << "|" << AnimalFormateur{ tuile->getJeton(), Format::Court } << "|";
                }
                lignes[2 * j + 0] << "/ " << HabitatFormateur{ tuile->getTuile()->getListeHabitat()[(0 + tuile->getRotation()) * 6 / tuile->getTuile()->getNbHabitat()], Format::Court } << " \\";
                lignes[2 * j + 1] << "/" << HabitatFormateur{ tuile->getTuile()->getListeHabitat()[(5 + tuile->getRotation()) * 6 / tuile->getTuile()->getNbHabitat()], Format::Court } << i % 10 << "," << j % 10 << HabitatFormateur{ tuile->getTuile()->getListeHabitat()[(1 + tuile->getRotation()) * 6 / tuile->getTuile()->getNbHabitat()], Format::Court } << "\\";
                lignes[2 * j + 2] << "\\" << HabitatFormateur{ tuile->getTuile()->getListeHabitat()[(4 + tuile->getRotation()) * 6 / tuile->getTuile()->getNbHabitat()], Format::Court } << jetons.str() << HabitatFormateur{ tuile->getTuile()->getListeHabitat()[(2 + tuile->getRotation()) * 6 / tuile->getTuile()->getNbHabitat()], Format::Court } << "/";
                lignes[2 * j + 3] << "\\_" << HabitatFormateur{ tuile->getTuile()->getListeHabitat()[(3 + tuile->getRotation()) * 6 / tuile->getTuile()->getNbHabitat()], Format::Court } << "_/";
            }
            else {
                lignes[2 * j + 0] << "     ";
                lignes[2 * j + 1] << "       ";
                lignes[2 * j + 2] << "       ";
                lignes[2 * j + 3] << "     ";
            }
        }
    }

    for (const auto& ligne : lignes) {
        std::cout << ligne.str() << '\n';
    }
}