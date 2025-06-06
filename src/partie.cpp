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
        listeActions.back()->afficher(); //!!! Pour debug -> observe que lorsque annule, 3. selection jeton faune: enft annule placement tuile 3
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
        return; // Ici on retourne dans le main qui va Partie::getGagnant()
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
    cout << "\nTour " << 21 - nbTour << " du joueur " << joueurCourant + 1 << endl;

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
        cout << "Environnements : F : Forêt, M : Montagne, P : Prairie, M : Marais, R : Riviere" << endl;
        cout << "Animaux : A : Aigle, C : Cerf, O : Ours, R : Renard, S : Saumon" << endl;
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

            Action* action = new ActionSelectionTuile(indiceTuile, pioche);
            int indiceSelection = action->executer();
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

            int refX, refY;
            string direction;
            cout << "A quelle tuile existante sur votre plateau souhaitez-vous coller la tuile ? (x,y) : ";
            cout << " x : ";
            cin >> refX;
            cout << " y : ";
            cin >> refY;
            Position posRef(refX, refY);

            // Verification si une tuile existe a cette position
            TuilePlacee* tuileExistante = joueur->getPlateau()->getTuilePlacee(posRef);
            if (!tuileExistante) {
                cout << "Il n'y a pas de tuile a cette position. Choisissez une autre position." << endl;
                break; // Retourner au menu des actions si la position est innocupee
            }

            cout << "A quelle position (NW, NE, E, SE, SW, W) souhaitez-vous positionner la nouvelle tuile par rapport a la tuile : " << endl;
            tuileExistante->getTuile()->afficherTuile();
            //afficher la tuile en mode plateau pour savoir comment la positionner par rapport au plateau

            cin >> direction;
            transform(direction.begin(), direction.end(), direction.begin(), ::toupper); // Met la chaine en majuscule
            Position pos;
            if (direction == "NW") pos = posRef.getNW();
            else if (direction == "NE") pos = posRef.getNE();
            else if (direction == "E") pos = posRef.getE();
            else if (direction == "SE") pos = posRef.getSE();
            else if (direction == "SW") pos = posRef.getSW();
            else if (direction == "W") pos = posRef.getW();
            else {
                cout << "Direction invalide." << endl;
                break;
            }
            // Verification si une tuile est deja placee a cette position
            TuilePlacee* tuileExistante2 = joueur->getPlateau()->getTuilePlacee(pos);
            if (tuileExistante2) {
                cout << "Il y a deja une tuile a cette position. Choisissez une autre position." << endl;
                break; // Retourner au menu des actions si la position est occupee
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

            Action* action = new ActionSelectionJeton(indiceJeton, pioche);
            int indiceSelection = action->executer();
            if (indiceSelection == -1) {
                cout << "Action de selection de jeton echouee." << endl;
            }
            else {
                animalJetonSelectionne = pioche->getJeton(indiceSelection);
                controleur->executerAction(action);
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
                controleur->annulerDerniereAction(); //!!! Supprime la tuile même lorsqu'annule la sélectiondu jeton car trigger ActionPlacerTuile::annuler() !!!! à Corriger
                int derniereAction = historiqueActions.back();
                historiqueActions.pop_back(); // Enlever la derniere action de l'historique
                cout << "Action annulee." << endl;
                switch (derniereAction) {
                    case 1: {tuileSelectionnee = nullptr; break;}   // Annuler la selection de tuile
                    case 2: {tuilePlacee = false; break;}   // Annuler le placement de la tuile
                    case 3: {animalJetonSelectionne = nullptr; break;}  // Annuler la selection du jeton faune
                    case 4: {jetonPlace = false; break;}    // Annuler le placement du jeton faune
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
    // Update la pioche
    pioche->completerPioche(animalJetonSelectionne, tuileSelectionnee);
    // Passe au joueur suivant
    passerAuJoueurSuivant();
}

// Change le joueur courant, diminue le nbTour, et relance JouerTour
void Partie::passerAuJoueurSuivant() {
    // Fonctionne aussi avec un seul joueur
    joueurCourant = (joueurCourant + 1) % nbJoueur;
    if (joueurCourant == 0) { nbTour--; } // un tour de moins
    this->jouerTour();
}


