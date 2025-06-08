#include <string>
#include <exception>
#include <iostream>
#include "carteMarquageFaune.h"
#include "controleurGeneral.h"
#include "enum.h"
#include "tuile.h"
#include "fileHandler.h"
#include "util.h"

using namespace std;

static void createNewGame() {
    cout << "Creation d'une nouvelle partie" << endl;
    int nbPlayers = askNumberOfPlayers();
    cout << "Nombre de joueurs selectionne : " << nbPlayers << endl;
    try {
        cout << "Lancement du jeu ! " << endl;
        Partie& partie = Partie::getInstance();
        partie.initialiserPartie(nbPlayers);
        // Jouer la partie
        partie.jouerTour();
        // Des qu'on sort, c'est que la partie est finie (pour le moment pas de quoi mettre en pause le jeu
        partie.getGagnant();
    }
    catch (const exception& e) { cerr << "Erreur lors de la creation de la partie : " << e.what() << endl; }
    catch (...) { cerr << "Erreur inconnue lors de la creation de la partie." << endl; }
}

int main() {
    cout << "Bienvenue dans Cascadia - Version Console" << endl;
    try {
        if (askYesNo("Souhaitez-vous reprendre la derniere partie sauvegardee ? (y/n) : ")) {
            FileHandler handler;
            string save = askFilename();
            if (handler.loadGame(save)) {
                cout << "Partie chargee depuis la sauvegarde !" << endl;
                Partie::getInstance().jouerTour();
            }
            else {
                cerr << "Aucune sauvegarde trouvee." << endl;
                createNewGame();
            }
        }
        else { createNewGame(); }
    }
    // On recupere ici toutes les eventuelles erreurs pour une sortie "propre" du programme
    catch (const exception& e) {
        cerr << "Erreur fatale : " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cerr << "Erreur fatale inconnue." << endl;
        return 1;
    }

    // AU REVOIR
    cout << "Merci d'avoir joue !" << endl;
    return 0;
}