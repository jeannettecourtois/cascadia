#include "partie.h"
#include "controleurGeneral.h"
#include "pioche.h"
#include "action.h"

using namespace std;

// Controleur Tour
ControleurTour::ControleurTour() {}

ControleurTour::~ControleurTour() {
    for (Action* a : listeActions) {
        delete a;
    }
    listeActions.clear();
}

void ControleurTour::executerAction(Action* a) {
    a->executer();
    listeActions.push_back(a);
}

void ControleurTour::annulerDerniereAction() {
    if (!listeActions.empty()) {
        listeActions.back()->annuler(); // annuler la derniere action
        delete listeActions.back(); // liberer la memoire
        listeActions.pop_back(); // supprimer l'action du vecteur
    }
}

void ControleurTour::afficherActions() const {
    cout << "\nListe des actions du tour : " << endl;
    for (Action* action : listeActions) {
        action->afficher();
    }
}



// PARTIE
//Partie avec plusieurs joueurs (max 4)
Partie::Partie(int nbJoueurs) : nbJoueur(nbJoueurs), joueurs(new Joueur* [nbJoueur]), pioche(new Pioche(nbJoueurs)), ctrlTour(new ControleurTour()), joueurCourant(0) {
    for (int i = 0; i < nbJoueur; ++i) {
        joueurs[i] = new Joueur(this);
    }
}

Partie::~Partie() {
    for (int i = 0; i < nbJoueur; ++i) {
        delete joueurs[i];
    }
    delete[] joueurs;
    delete pioche;
    delete ctrlTour;
}

bool Partie::estFini() const {
    return nbTour == 0;
}

Joueur* Partie::getGagnant() const {
    if (nbJoueur == 0) return nullptr;

    int maxPoints = joueurs[0]->calculScore();
    Joueur* gagnant = joueurs[0];
    bool egalite = false;

    for (int i = 1; i < nbJoueur; ++i) {
        int score = joueurs[i]->calculScore();
        if (score > maxPoints) {
            maxPoints = score;
            gagnant = joueurs[i];
            egalite = false;
        } else if (score == maxPoints) {
            egalite = true;
        }
    }
    if (egalite) {
        std::cout << "Il y a une egalite entre plusieurs joueurs." << std::endl;
        return nullptr;
    }
    return gagnant;
}


void Partie::initialiserCartesRegles() {
    static const Animal animaux[5] = { Animal::Aigle, Animal::Cerf, Animal::Ours, Animal::Renard, Animal::Saumon };
    ControleurGeneral& ctrl = ControleurGeneral::getInstance();
    for (int i = 0; i < 5; ++i) {
        cartesRegles[i] = ctrl.getCarteMarquageParAnimalAleatoire(animaux[i]);
    }
}

void Partie::initialiserPartie() {
    ControleurGeneral& ctrl = ControleurGeneral::getInstance();
    initialiserCartesRegles();
    cout << "Cartes de regles initialisees." << std::endl;

    // Selection et distribution d'une tuile de depart aleatoire differente pour chaque joueur
    for (int i = 0; i < nbJoueur; ++i) {
        TuileDepart* tuile = ctrl.getTuileDepartAleatoire(); // tirage aleatoire
        joueurs[i]->getPlateau()->ajouterTuileDepart(tuile);
    }
    cout << "Tuiles de depart distribuees a chaque joueur." << std::endl;

    //Mise a jour de la pioche
    if (!pioche) {
        cerr << "Erreur : pioche non initialisee !" << std::endl;
        return;
    }
    pioche->preparerPioche();
    cout << "Pioche preparee." << std::endl;
    // Reinitialisation du compteur de tours
    nbTour = 20;
    cout << "Initialisation de la partie terminee.\n" << std::endl;
}

void Partie::jouerTour() {
    if (estFini()) {
        cout << "\nLa partie est terminee." << endl;
        return;
    }

    ControleurTour* controleur = this->ctrlTour;
    if (!ctrlTour) {
        cerr << "Erreur : controleur de tour non initialise !" << endl;
        return;
    }
    if (joueurCourant < 0 || joueurCourant >= nbJoueur) {
        cerr << "Erreur : joueur courant invalide !" << endl;
        return;
    }
    Joueur* joueur = joueurs[joueurCourant];
    cout << "\nTour du joueur " << joueurCourant + 1 << endl;

    bool actionFinie = false;
    Tuile* tuileSelectionnee = nullptr;
    Animal* animalJetonSelectionne = nullptr;
    bool tuilePlacee = false;
    bool jetonPlace = false;

    // Etapes de gestion de l'historique
    vector<int> historiqueActions;  // Pour enregistrer l'historique des choix du joueur

    while (!actionFinie) {
        // On affiche le plateau
        cout << endl << "Plateau : " << endl;
        joueur->getPlateau()->afficherPlateau();
        cout << endl;

        // Affichage des actions possibles en fonction de l'etat actuel du jeu
        cout << "Que voulez-vous faire ?" << endl;
        if (!tuileSelectionnee) {cout << "1. Selectionner une tuile de la pioche" << endl;}
        if (tuileSelectionnee && !tuilePlacee) {cout << "2. Placer une tuile sur votre plateau" << endl;}
        if (tuilePlacee && !animalJetonSelectionne) { cout << "3. Selectionner un jeton faune" << endl; }
        if (animalJetonSelectionne && !jetonPlace) {cout << "4. Placer un jeton faune" << endl;}
        if (!historiqueActions.empty()) { cout << "5. Annuler la derniere action" << endl; }  // Pour ne pas afficher lors de la premiere 
        if (tuilePlacee && jetonPlace) {cout << "6. Terminer mon tour\n" << endl;}

        int choix;
        cin >> choix;

        // Gerer les actions en fonction du choix de l'utilisateur
        switch (choix) {
        case 1: { // Selectionner une tuile de la pioche
            int indiceTuile;
            pioche->afficherTuilesDisponibles(); // Affiche les tuiles disponibles dans la pioche
            pioche->afficherJetonsDisponibles(); // Affiche les jetons faune disponibles pour orienter le choix
            cout << "Quelle tuile souhaitez-vous prendre (indice 0-3) ? ";
            cin >> indiceTuile;

            Action* action1 = new ActionSelectionTuile(indiceTuile, pioche);
            int indiceSelection = action1->executer();
            if (indiceSelection == -1) {
                cout << "Action de selection de tuile echouee." << endl;
            }
            else {
                tuileSelectionnee = pioche->getTuile(indiceSelection);
                tuileSelectionnee->afficherTuile();
            }

            historiqueActions.push_back(1); // Enregistrer l'action
            break;
        }
        case 2: { // Placer une tuile sur le plateau
            if (!tuileSelectionnee) {
                cout << "Aucune tuile selectionnee. Veuillez d'abord selectionner une tuile." << endl;
                break;
            }

            int x, y;
            cout << "Ou souhaitez-vous placer la tuile ? (x,y) : ";
            cout << " x : ";
            cin >> x;
            cout << " y : ";
            cin >> y;
            Position pos(x, y);

            // Verification si une tuile est deja placee a cette position
            TuilePlacee* tuileExistante = joueur->getPlateau()->getTuilePlacee(pos);
            if (tuileExistante) {
                cout << "Il y a deja une tuile a cette position. Choisissez une autre position." << endl;
                break; // Retourner au menu des actions si la position est occupee
            }

            // Verification de l'adjacence : on regarde les positions adjacentes
            bool adjacente = false;
            vector<Position> positionsAdjacentes = {
                pos.getE(), pos.getSE(), pos.getSW(),
                pos.getW(), pos.getNW(), pos.getNE()
            };
            for (const auto& p : positionsAdjacentes) {
                TuilePlacee* tuileAdjacente = joueur->getPlateau()->getTuilePlacee(p);
                if (tuileAdjacente) {
                    adjacente = true;
                    break;
                }
            }
            if (!adjacente) {
                cout << "La tuile doit etre placee a côte d'une tuile deja existante." << endl;
                break; // Retourner au menu des actions si la position n'est pas adjacente a une tuile
            }

            Action* action = new ActionPlacerTuile(tuileSelectionnee, pos, joueur);
            controleur->executerAction(action);
            tuilePlacee = true;
            historiqueActions.push_back(2); // Enregistrer l'action
            break;
        }
        case 3: { // Selectionner un jeton faune
            if (!tuilePlacee) {
                cout << "Aucune tuile placee. Veuillez d'abord placer une tuile." << endl;
                break;
            }
            int indiceJeton;
            pioche->afficherJetonsDisponibles(); // Affiche les jetons faune disponibles
            cout << "Quel jeton faune souhaitez-vous prendre (indice 0-3) ? ";
            cin >> indiceJeton;

            Action* action1 = new ActionSelectionJeton(indiceJeton, pioche);
            int indiceSelection = action1->executer();
            if (indiceSelection == -1) {
                cout << "Action de selection de jeton echouee." << endl;
            }
            else {
                animalJetonSelectionne = pioche->getJeton(indiceSelection);
                cout << "Jeton faune selectionne : " << AnimalFormateur{ *animalJetonSelectionne, Format::Complet } << endl;
            }

            historiqueActions.push_back(3); // Enregistrer l'action
            break;
        }
        case 4: { // Placer un jeton faune
            if (!animalJetonSelectionne) {
                cout << "Aucun jeton faune selectionne. Veuillez d'abord selectionner un jeton." << endl;
                break;
            }
            int x, y;
            cout << "Sur quelle tuile souhaitez-vous placer le jeton faune ? (x,y) : " << AnimalFormateur{ *animalJetonSelectionne, Format::Complet } << endl;
            cout << " x : ";
            cin >> x;
            cout << " y : ";
            cin >> y;

            Position pos(x, y);
            TuilePlacee* tuilePlacee = joueur->getPlateau()->getTuilePlacee(pos);
            if (!tuilePlacee) {
                cout << "Aucune tuile placee a cette position. Veuillez selectionner une tuile deja placee." << endl;
                break;
            }
            // Verifier si le jeton peut etre place sur la tuile
            if (!tuilePlacee->getTuile()->contientAnimal(*animalJetonSelectionne)) {
                cout << "Le jeton faune ne peut pas etre place sur cette tuile." << endl;
                break;
            }

            Action* action = new ActionPlacerJeton(animalJetonSelectionne, tuilePlacee);
            controleur->executerAction(action);
            jetonPlace = true;
            historiqueActions.push_back(4); // Enregistrer l'action
            break;
        }
        case 5: { // Annuler la derniere action
            if (!historiqueActions.empty()) {
                controleur->annulerDerniereAction();
                int derniereAction = historiqueActions.back();
                historiqueActions.pop_back(); // Enlever la derniere action de l'historique
                cout << "Action annulee." << endl;
                switch (derniereAction) {
                    case 1: {
                        tuileSelectionnee = nullptr; // Annuler la selection de tuile
                        break;
                    }
                    case 2: {
                        tuilePlacee = false; // Annuler le placement de la tuile
                        break;
                    }
                    case 3: {
                        animalJetonSelectionne = nullptr; // Annuler la selection du jeton faune
                        break;
                    }
                    case 4: {
                        jetonPlace = false; // Annuler le placement du jeton faune
                        break;
                    }
                }
            }
            else {
                cout << "Aucune action a annuler." << endl;
            }
            break;
        }
        case 6: { // Terminer le tour
            if (tuilePlacee && jetonPlace) {
                cout << "Tour termine avec succes." << endl;
            }
            else if (!tuilePlacee) {
                cout << "Vous devez placer une tuile avant de terminer votre tour." << endl;
                continue; // Recommencer le tour si la tuile n'est pas placee
            }
            else if (!jetonPlace) {
                cout << "Vous devez placer un jeton faune avant de terminer votre tour." << endl;
                continue; // Recommencer le tour si le jeton n'est pas place
            }
            actionFinie = true;
            break;
        }
        default:
            cout << "Choix invalide." << endl;
        }
    }

    passerAuJoueurSuivant();
    nbTour--;
}


void Partie::passerAuJoueurSuivant() {
    joueurCourant = (joueurCourant + 1) % nbJoueur;
}


