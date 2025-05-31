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



// PARTIE
//Partie avec plusieurs joueurs (max 4)
Partie::Partie(int nbJoueurs) : nbJoueur(nbJoueurs), joueurs(new Joueur*[nbJoueur]), pioche(new Pioche(nbJoueurs)), ctrlTour(new ControleurTour()), joueurCourant(0) {
    for (int i = 0; i < nbJoueur; ++i) {
        joueurs[i] = new Joueur();
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
    cout << "Initialisation de la partie terminee." << std::endl;
}

void Partie::jouerTour() {
    if (estFini()) {
        cout << "La partie est terminee." << endl;
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
    cout << "Tour du joueur " << joueurCourant << endl;
    bool actionFinie = false;
    Tuile* tuileSelectionnee = nullptr;
    Animal* animalJetonSelectionne = nullptr;
    bool tuilePlacee = false;
    bool jetonPlace = false;

    while (!actionFinie) {
        cout << "Que voulez-vous faire ?" << endl;
        cout << "1. Selectionner une tuile de la pioche" << endl;
        cout << "2. Placer une tuile sur votre plateau" << endl;
        cout << "3. Placer un jeton faune" << endl;
        cout << "4. Annuler derniere action" << endl;
        cout << "5. Terminer mon tour" << endl;

        int choix;
        cin >> choix;

        switch (choix) {
            case 1: { // Selectionner une tuile de la pioche
                int indiceTuile;
                pioche->afficherTuilesDisponibles(); // Affiche les tuiles disponibles dans la pioche
                cout << "Quelle tuile souhaitez-vous prendre (indice 0-3) ? ";
                cin >> indiceTuile;
                if (indiceTuile < 0 || indiceTuile > 3 || !pioche->getTuile(indiceTuile)) {
                    cout << "Indice invalide." << endl;
                    break;
                }
                tuileSelectionnee = pioche->getTuile(indiceTuile);
                animalJetonSelectionne = pioche->getJeton(indiceTuile); // Recupere le jeton associe a la tuile selectionnee
                cout << "Tuile selectionnee : ";
                tuileSelectionnee->afficherTuile();
                cout << "Jeton selectionne : " << animalJetonSelectionne << endl;

                Action* action = new ActionSelectionTuile();
                controleur->executerAction(action);
                break;
            }
            case 2: { // Placer une tuile sur le plateau
                if (!tuileSelectionnee) {
                    cout << "Aucune tuile selectionnee. Veuillez d'abord selectionner une tuile." << endl;
                    break;
                }
                int x, y;
                cout << "Ou souhaitez-vous placer la tuile ? (x,y) : "; // pas avec la position mais avec les cardinaux d'un id de tuile deja placee
                cout << " x : ";
                cin >> x;
                cout << " y : ";
                cin >> y;
                Position pos(x, y);
                Action* action = new ActionPlacerTuile(tuileSelectionnee, pos); // recupere la tuile selectionnee dans case 1
                controleur->executerAction(action);
                tuilePlacee = true;
                break;
            }
            case 3: { // Placer un jeton faune
                if (!animalJetonSelectionne){ // Test si le pointeur est nul
                    cout << "Aucun jeton faune selectionne. Veuillez d'abord selectionner un jeton." << endl;
                    break;
                }
                int x,y;
                cout << "Sur quelle tuile souhaitez-vous placer le jeton faune ? (x,y) : " << animalJetonSelectionne << endl;
                cout << " x : ";
                cin >> x;
                cout << " y : ";
                cin >> y;

                Position pos(x, y);
                TuilePlacee* tuilePlacee = joueur->getPlateau()->getTuilePlacee(pos); // Recupere la tuile placee a la position donnee
                if (!tuilePlacee) {
                    cout << "Aucune tuile placee a cette position. Veuillez selectionner un tuile deja placee." << endl;
                    break;
                }
                // Verifier si le jeton peut être place sur la tuile
                if (!tuilePlacee->getTuile()->contientAnimal(*animalJetonSelectionne)) {
                    cout << "Le jeton faune ne peut pas être place sur cette tuile." << endl;
                    break;
                }

                Action* action = new ActionPlacerJeton(animalJetonSelectionne, tuilePlacee); //recuperer l'animal selectionne dans la pioche et la tuile placee correspondante a la position
                controleur->executerAction(action);
                jetonPlace = true;
                break;
            }
            case 4: { // Annuler la derniere action
                controleur->annulerDerniereAction();
                break;
            }
            case 5: { // Terminer le tour
                if (tuilePlacee && jetonPlace) {
                    cout << "Tour termine avec succes." << endl;
                } else if (!tuilePlacee) {
                    cout << "Vous devez placer une tuile avant de terminer votre tour." << endl;
                    continue; // Recommencer le tour si la tuile n'est pas placee
                } else if (!jetonPlace) {
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




    // Exemple : appeler le controleur de tour pour jouer une action
    // ctrlTour->executerAction(...); // a adapter selon ton systeme d’action

    // Diminuer le compteur de tour
    nbTour--;
    passerAuJoueurSuivant();
}

void Partie::passerAuJoueurSuivant() {
    joueurCourant = (joueurCourant + 1) % nbJoueur;
}


