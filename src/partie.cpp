#include "partie.h"
#include "controleurGeneral.h"
#include "pioche.h"
#include "action.h"
#include "fileHandler.h"
#include "util.h"

using namespace std;

static Position askPosition(const string& prompt) {
    int x, y;
    while (true) {
        cout << prompt << endl;
        cout << " x : ";
        if (!(cin >> x)) {
            cout << "Valeur invalide pour x. Veuillez entrer un entier.\n";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cout << " y : ";
        if (!(cin >> y)) {
            cout << "Valeur invalide pour y. Veuillez entrer un entier.\n";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        return Position(x, y);
    }
}


// PARTIE

Partie* Partie::instance = nullptr;

// Constructeur qui initialise avec des valeurs sûres, appeles slmt au tout premier getInstance, apres utiliser initialiserPartie()
Partie::Partie() : nbJoueur(1), nbTour(20), joueurCourant(0), pioche(nullptr), ctrlTour(nullptr) {
    for (int i = 0; i < 5; i++)
        cartesRegles[i] = nullptr;
}

Partie::~Partie() {
    for (int i = 0; i < nbJoueur; ++i) {
        delete joueurs[i];
    }
    delete pioche;
    delete ctrlTour;
}

// Singleton
Partie& Partie::getInstance() {
    if (!instance)
        instance = new Partie();
    return *instance;
}

void Partie::libererInstance() {
    delete instance;
    instance = nullptr;
}

void Partie::reinitialiserPartie() {
    // Liberation des joueurs
    for (Joueur* j : joueurs)
        delete j;
    joueurs.clear();

    // Liberation de la pioche
    delete pioche;
    pioche = nullptr;

    // Liberation du controleur de tour
    delete ctrlTour;
    ctrlTour = nullptr;

    // Reinitialisation des cartes
    for (int i = 0; i < 5; ++i)
        cartesRegles[i] = nullptr;

    nbTour = 20;
    joueurCourant = 0;
}

string Partie::getPhaseCourante() const {
    int nbActions = ctrlTour ? ctrlTour->getListeActions().size() : 0;

    switch (nbActions) {
    case 0: return "SELECTION_TUILE";
    case 1: return "PLACEMENT_TUILE";
    case 2: return "SELECTION_JETON";
    case 3: return "PLACEMENT_JETON";
    case 4: return "TERMINER";
    default: return "INCONNUE";
    }
}


void Partie::initialiserPartie(int nbJoueurs) {
    reinitialiserPartie();

    this->nbJoueur = nbJoueurs;
    ControleurGeneral& ctrl = ControleurGeneral::getInstance();

    for (int i = 0; i < nbJoueur; ++i)
        joueurs.push_back(new Joueur(this, i));

    initialiserCartesRegles();

    for (int i = 0; i < nbJoueur; ++i) {
        TuileDepart* tuile = ctrl.getTuileDepartAleatoire();
        joueurs[i]->getPlateau()->ajouterTuileDepart(tuile);
    }

    pioche = new Pioche(nbJoueurs);
    pioche->preparerPioche();
    ctrlTour = new ControleurTour();
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

void Partie::setCarteFaune(int index, CarteMarquageFaune* carte) {
    if (index >= 0 && index < 5)
        cartesRegles[index] = carte;
}

void Partie::setPioche(Pioche* nouvellePioche) {
    if (pioche != nullptr)
        delete pioche;
    pioche = nouvellePioche;
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
    this->setAnimalJetonSelectionne(Animal::Vide);
    bool tuilePlacee = false;
    bool jetonPlace = false;

    vector<int> historiqueActions;  // Pour enregistrer l'historique des choix du joueur


    // Initialisation a partir de l'historique qd chargement
    for (Action* a : controleur->getListeActions()) {
        if (auto selT = dynamic_cast<ActionSelectionTuile*>(a)) {
            tuileSelectionnee = selT->getTuileSelectionnee();
            historiqueActions.push_back(1);
        }
        else if (dynamic_cast<ActionPlacerTuile*>(a)) {
            tuilePlacee = true;
            historiqueActions.push_back(2);
        }
        else if (auto selJ = dynamic_cast<ActionSelectionJeton*>(a)) {
            Animal jetonLu = selJ->getJetonSelection();
            if (jetonLu != Animal::Vide) {
                this->setAnimalJetonSelectionne(jetonLu);
                historiqueActions.push_back(3);
            }
            else {
                std::cerr << "[LOGIC WARNING] JetonSelection invalide ignore ("
                    << static_cast<int>(jetonLu) << ") @ "
                    << static_cast<const void*>(&jetonLu) << "\n";
            }
        }

        else if (dynamic_cast<ActionPlacerJeton*>(a)) {
            jetonPlace = true;
            historiqueActions.push_back(4);
        }
    }

    while (!actionFinie) {
        // On affiche le plateau
        cout << endl << "Plateau : " << endl;
        cout << "Environnements : F : Foret, M : Montagne, P : Prairie, M : Marais, R : Riviere" << endl;
        cout << "Animaux : A : Aigle, C : Cerf, O : Ours, R : Renard, S : Saumon" << endl;
        joueur->getPlateau()->afficherPlateau();
        cout << endl;

        const string phase = getPhaseCourante();
        cout << "Phase actuelle : " << phase << "\n";

        // Affichage des actions possibles en fonction de l'etat actuel du jeu
        cout << "Que voulez-vous faire ?" << endl;
        if (phase == "SELECTION_TUILE") {cout << "1. Selectionner une tuile de la pioche" << endl;}
        if (phase == "PLACEMENT_TUILE") {cout << "2. Placer une tuile sur votre plateau" << endl;}
        if (phase == "SELECTION_JETON") { cout << "3. Selectionner un jeton faune" << endl; }
        if (phase == "PLACEMENT_JETON") { cout << "4. Placer un jeton faune" << endl;}
        if (!historiqueActions.empty()) {cout << "5. Annuler la derniere action" << endl;}
        if (phase == "TERMINER") {cout << "6. Terminer mon tour" << endl;}
        cout << "7. Sauvegarder et quitter la partie" << endl;

        int choix;
        cin >> choix;

        switch (choix) {
        case 1: { // Selectionner une tuile de la pioche
            if (tuileSelectionnee) break;  // deja fait
            int indiceTuile;
            pioche->afficherTuilesDisponibles(); // Affiche les tuiles disponibles dans la pioche
            pioche->afficherJetonsDisponibles(); // Affiche les jetons faune disponibles pour orienter le choix
            cout << "Quelle tuile souhaitez-vous prendre (indice 0-3) ? ";
            cin >> indiceTuile;

            Action* action = new ActionSelectionTuile(indiceTuile, pioche);
            int indiceSelection = action->executer();
            if (indiceSelection == -1) {
                cout << "Action de selection de tuile echouee." << endl;
                delete action;
            }
            else {
                tuileSelectionnee = pioche->getTuile(indiceSelection);
                tuileSelectionnee->afficherTuileHexa();
                controleur->executerAction(action);
                historiqueActions.push_back(1);
            }
            break;
        }
        case 2: { // Placer une tuile sur le plateau
            if (!tuileSelectionnee || tuilePlacee) break;
            Position posRef = askPosition("A quelle tuile existante sur votre plateau souhaitez - vous coller la tuile ? ");
            // Verification si une tuile existe a cette position
            TuilePlacee* tuileExistante = joueur->getPlateau()->getTuilePlacee(posRef);
            if (!tuileExistante) {
                cout << "Il n'y a pas de tuile a cette position. Choisissez une autre position." << endl;
                break;
            }

            cout << "A quelle position (NW, NE, E, SE, SW, W) souhaitez-vous positionner la nouvelle tuile par rapport a la tuile : " << endl;
            tuileExistante->getTuile()->afficherTuile();

            cout << "position : " << endl;
            string direction;
            cin >> direction;
            transform(direction.begin(), direction.end(), direction.begin(), ::toupper);
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
            if (joueur->getPlateau()->getTuilePlacee(pos)) {
                cout << "Il y a deja une tuile a cette position. Choisissez une autre position." << endl;
                break;
            }

            Action* action = new ActionPlacerTuile(tuileSelectionnee, pos, joueur);
            controleur->executerAction(action);
            tuilePlacee = true;
            historiqueActions.push_back(2);
            break;
        }
        case 3: { // Selectionner un jeton faune
            if (!tuilePlacee || jetonPlace) break;
            int indiceJeton;
            pioche->afficherJetonsDisponibles();
            cout << "Quel jeton faune souhaitez-vous prendre (indice 0-3) ? ";
            cin >> indiceJeton;

            Action* action = new ActionSelectionJeton(indiceJeton, pioche);
            int indiceSelection = action->executer();
            if (indiceSelection == -1) {
                cout << "Action de selection de jeton echouee." << endl;
                delete action;
            }
            else {
                this->setAnimalJetonSelectionne(*pioche->getJeton(indiceSelection));
                controleur->executerAction(action);
                historiqueActions.push_back(3);
            }
            break;
        }
        case 4: { // Placer un jeton faune
            if (jetonPlace) break;
            Position pos = askPosition("Sur quelle tuile souhaitez-vous placer le jeton faune ? " + toString(animalJetonSelectionne));
            TuilePlacee* tuilePlaceePtr = joueur->getPlateau()->getTuilePlacee(pos);
            if (!tuilePlaceePtr) {
                cout << "Aucune tuile placee a cette position. Veuillez selectionner une tuile deja placee." << endl;
                break;
            }
            if (!tuilePlaceePtr->getTuile()->contientAnimal(animalJetonSelectionne)) {
                cout << "Le jeton faune ne peut pas etre place sur cette tuile." << endl;
                break;
            }

            // On cree dynamiquement une nouvelle copie, pour respecter le constructeur attendu.
            Action* action = new ActionPlacerJeton(animalJetonSelectionne, tuilePlaceePtr, joueur);
            controleur->executerAction(action);
            jetonPlace = true;
            historiqueActions.push_back(4);
            break;
        }
        case 5: { // Annuler la derniere action
            if (!historiqueActions.empty()) {
                controleur->annulerDerniereAction();
                int derniereAction = historiqueActions.back();
                historiqueActions.pop_back();
                cout << "Action annulee." << endl;
                switch (derniereAction) {
                case 1: tuileSelectionnee = nullptr; break;
                case 2: tuilePlacee = false; break;
                case 3: {
                    this->setAnimalJetonSelectionne(Animal::Vide);
                    break;
                }
                case 4: 
                    jetonPlace = false;
                    break;
                }
            }
            else {
                cout << "Aucune action a annuler." << endl;
            }
            break;
        }
        case 6: { // Terminer le tour
            if (!tuilePlacee || !jetonPlace) {
                cout << "Vous devez placer une tuile et un jeton faune avant de terminer votre tour." << endl;
                continue;
            }
            actionFinie = true;
            cout << "Tour termine avec succes." << endl;
            break;
        }
        case 7: {
            cout << "Sauvegarde en cours..." << endl;
            if (FileHandler().saveGame(askFilename())) {
                cout << "Partie sauvegardee ! Fin de la partie." << endl;
                exit(0); // pour ne pas update la pioche
            }
            else
                cerr << "Error !" << endl;
            break;
        }
        default:
            cout << "Choix invalide." << endl;
        }
    }
    // Update la pioche
    pioche->completerPioche(&getAnimalJetonSelectionne(), tuileSelectionnee);
    // Passe au joueur suivant
    passerAuJoueurSuivant();
}


// Change le joueur courant, diminue le nbTour, et relance JouerTour, gestion ctrl de tour
void Partie::passerAuJoueurSuivant() {
    // Fonctionne aussi avec un seul joueur
    joueurCourant = (joueurCourant + 1) % nbJoueur;
    if (joueurCourant == 0) { nbTour--; } // un tour de moins
    delete ctrlTour; // On supprime le controleur de tour pour en creer un nouveau
    ctrlTour = new ControleurTour(); // On cree un nouveau controleur de tour pour le joueur suivant
    this->jouerTour();
}


