#include "joueur.h"
#include "partie.h"

Joueur :: Joueur() : idJoueur(1), nbJetonNature(0), nomJoueur("Joueur 1"), plateau(new PlateauJoueur()) {
    // Constructeur par defaut
}
Joueur :: ~Joueur() {
    delete plateau;
}
int Joueur :: calculScore() {
    int score = 0;
    // Calculer le score en fonction des tuiles placees et des jetons selon les regles
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
    /*Affichage du plateau du joueur en vrac pour le moment,
    * la rotation des tuiles n'est pas prise en compte/!\
    * faudra ajouter le design d'Alexandre pour que les tuiles soient correctement affichees*/

    for (int i = 0; i < nbTuiles; ++i) {
        const TuilePlacee* tuile = tuiles[i];
        if (tuile) {
            const Position& pos = tuile->getPosition();  // Getter ajoute dans tuilePlacee.h
            const Tuile* t = tuile->getTuile();
            Animal animal = tuile->getJeton();

            std::cout << "Tuile " << i + 1 << " : ";
            std::cout << "Position (" << pos.x << ", " << pos.y << "), ";
            std::cout << "Animal: " << animal << std::endl; // Surcharge dans enum.h
        }
    }
}