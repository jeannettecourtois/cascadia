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
        Partie& partie = Partie::getInstance(nbPlayers);
        partie.initialiserPartie();
        // Jouer la partie
        partie.jouerTour();
        // Des qu'on sort, c'est que la partie est finie (pour le moment pas de quoi mettre en pause le jeu
        partie.getGagnant();
    }
    catch (const exception& e) { cerr << "Erreur lors de la creation de la partie : " << e.what() << endl; }
    catch (...) { cerr << "Erreur inconnue lors de la creation de la partie." << endl; }
}


int main() {
    cout << "Initalisation de la partie..." << endl;
    // ControleurGeneral CG;
    cout << "Bienvenue dans Cascadia - Version Console" << endl;
    try {
        if (askYesNo("Voulez-vous charger une partie sauvegardee ? (y/n) : ")) {
            // pour l'instant permet juste d'eviter une variable globale
            string filename = askFilename();
            try {
                FileHandler handler;
                if (handler.loadGame(filename)) {
                    cout << "Partie chargee avec succes !" << endl;
                    // ON LANCE LA PARTIE
                    createNewGame(); // Temporaire

                    // ON SAUVEGARDE LA PARTIE
                    cout << "Voulez-vous sauvegarder la partie ? (y/n) : ";
                    string saveChoice; cin >> saveChoice;
                    if (saveChoice == "y" || saveChoice == "Y") {
                        string saveFile = askFilename();
                        if (handler.saveGame(saveFile)) {
                            cout << "Partie sauvegardee avec succes dans " << saveFile << endl;
                        }
                        else {
                            cerr << "Erreur lors de la sauvegarde." << endl;
                        }
                    }
                    // ON LIBERE EVENTUELLEMENT LA PARTIE
                    //ControleurGeneral::freeInstance();
                }
                else { cerr << "Impossible de charger la partie." << endl; }

                // On indique ce qui a ete charge
                cout << "Chargement et lancement de la partie depuis : " << filename << endl;
            }
            catch (const exception& e) { cerr << "Exception lors du chargement de la partie : " << e.what() << endl; }
            catch (...) { cerr << "Erreur inconnue lors du chargement de la partie." << endl; }
        }
        // SINON ON CREER UNE NOUVELLE PARTIE
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