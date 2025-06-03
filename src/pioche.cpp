#include "pioche.h"
#include "enum.h"

/* SacTuile */
SacTuile::SacTuile(int nbTuiles) : nbTuile(nbTuiles) {
    ControleurGeneral& cg = ControleurGeneral::getInstance();
    for (int i = 0; i < nbTuiles; ++i) {
        tuiles.push_back(cg.getTuile());
    }
}

SacTuile::~SacTuile() {}

Tuile* SacTuile::Piocher() {
    if (tuiles.empty()) return nullptr;
    int index = rand() % tuiles.size();
    Tuile* t = tuiles[index];
    tuiles.erase(tuiles.begin() + index);
    return t;
} 

void SacTuile::mettreDansLeSac(Tuile* t) {
    tuiles.push_back(t);
}


/* SacJeton */
SacJeton::SacJeton(const std::vector<int>& repartition) : compteJetons(repartition), nbJetons(0) {
    for (int i : compteJetons) {
        nbJetons += i;
    }
}

SacJeton::~SacJeton() {}

Animal SacJeton::Piocher() {
    if (nbJetons == 0) return Animal::Vide;
    int index = rand() % nbJetons;

    for (int i = 0; i < compteJetons.size(); ++i) {
        if (index < compteJetons[i]) {
            compteJetons[i]--;
            nbJetons--;
            return static_cast<Animal>(i);
        }
        index -= compteJetons[i];
    }
    return Animal::Vide;
} 

void SacJeton::mettreDansLeSac(Animal a) {
    int index = static_cast<int>(a);
    if (index >= compteJetons.size()) {
        compteJetons.resize(index + 1, 0);
    }
    compteJetons[index]++;
    nbJetons++;
}

/* Pioche */
Pioche::Pioche(int nbJoueurs) {
    for (int i = 0; i < 4; ++i) {
        tuiles[i] = nullptr;
        jetons[i] = nullptr;
    }
    idTuileChoisis = -1;

    // calcul du nombre de tuiles selon le nombre de joueurs
    int nbTuiles;
    if (nbJoueurs == 1 || nbJoueurs == 2) nbTuiles = 43;
    else if (nbJoueurs == 3) nbTuiles = 63;
    else if (nbJoueurs == 4) nbTuiles = 83;

    sacDeTuiles = new SacTuile(nbTuiles);

    // repartition des jetons uniformement selon la liste d'animaux
    int nbJetonsTotal = nbJoueurs * 20;

    std::vector<int> repartition(6,0);
    int nbAnimaux = 0;
    for (Animal a : animaux) {
        repartition[static_cast<int>(a)] = nbJetonsTotal / 5;
        nbAnimaux++;
    }
    sacDeJetons = new SacJeton(repartition);
}

Pioche::~Pioche() {
    for (int i = 0; i < 4; ++i) {
        delete tuiles[i]; 
    }
    for (int i = 0; i < 4; ++i) {
    delete jetons[i];
    }
    delete sacDeTuiles;
    delete sacDeJetons;
}

void Pioche::preparerPioche() {
    // Preparer la pioche de tuiles et de jetons
    for (int i = 0; i < 4; ++i) {
        tuiles[i] = sacDeTuiles->Piocher();
        Animal a = sacDeJetons->Piocher();
        jetons[i] = new Animal(a);
    }
}

void Pioche::selectionnerTuile() {
    // Selectionner une tuile parmi celles disponibles
    idTuileChoisis = rand() % 4;
}

Animal* Pioche::selectionnerJeton() const {
    int idJetonChoisis = rand() % 4;
    return jetons[idJetonChoisis];
}



void Pioche::afficherTuilesDisponibles() const {
    std::cout << "Tuiles et jetons disponibles dans la pioche :\n\n";
    for (int i = 0; i < 4; ++i) {
        std::cout << "Tuile " << i << ": ";
        if (tuiles[i]) {
            tuiles[i]->afficherTuile();
        }
        else {
            std::cout << "Vide";
        }
    
        std::cout << " | Jeton: ";
        if (jetons[i] != nullptr) {
            std::cout << AnimalFormateur{ *jetons[i],Format::Complet };
        } else {
            std::cout << "Vide";
        }
        std::cout << "\n";
    } std::cout << std::endl;
}


void Pioche::completerPioche() {
    // on complete les tuiles manquants
    for (int i = 0; i < 4; ++i) {
        if (tuiles[i] == nullptr) {
            tuiles[i] = sacDeTuiles->Piocher();
        }
    }

    // on complete les jetons manquants
    for (int i = 0; i < 4; ++i) {
        if (jetons[i] == nullptr) {
            Animal a = sacDeJetons->Piocher();
            jetons[i] = new Animal(a);
        }
    }

    // gestion de la surpopulation : 3 ou 4 animaux identiques
    // creation d'un dictionnaire pour compter le nombre de fois ou le jeton apparait
    std::map<Animal, int> compteur;
    for (int i = 0; i < 4; ++i) {
        if (jetons[i]) {
            compteur[*jetons[i]]++;
        }
    }
    // parcours du dictionnaire
    for (const auto& pair : compteur) {
        if (pair.second >= 3) {
            std::cout << "Trois ou plus jetons identiques (" << AnimalFormateur{ pair.first,Format::Complet } << ") détectés. On les remplace tous.\n";

            // on remet les jetons dans sacJeton et on repioche
            for (int i = 0; i < 4; ++i) {
                if (jetons[i]) {
                    sacDeJetons->mettreDansLeSac(*jetons[i]);
                    delete jetons[i];
                    jetons[i] = new Animal(sacDeJetons->Piocher());
                }
            }
            break;
        }
    }
}
