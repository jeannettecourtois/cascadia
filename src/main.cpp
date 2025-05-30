#include <iostream>
#include "carteMarquageFaune.h"
#include "controleurGeneral.h"
#include "enum.h"
#include "tuile.h"

#include <string>
#include <exception>
#include "fileHandler.h"

using namespace std;

static void createNewGame() {
    cout << "Création d'une nouvelle partie" << endl;
    int nbPlayers = askNumberOfPlayers();
    cout << "Nombre de joueurs sélectionné : " << nbPlayers << endl;
    try {
        cout << "Lancement du jeu ! " << endl;
        Partie partie(nbPlayers);
        partie.initialiserPartie();
    }
    catch (const exception& e) {cerr << "Erreur lors de la création de la partie : " << e.what() << endl;}
    catch (...) {cerr << "Erreur inconnue lors de la création de la partie." << endl;}
}


int main() {
    cout << "Initalisation de la partie..." << endl;
    // ControleurGeneral CG;
    cout << "Bienvenue dans Cascadia - Version Console" << endl;
    try {
        if (askLoadGame()) {
            // pour l'instant permet juste d'éviter une variable globale
            string filename = askFilename();
            try {                
                FileHandler handler;
                if (handler.loadGame(false, filename)) {
                    cout << "Partie chargée avec succès !" << endl;                    
                    // ON LANCE LA PARTIE
                    //ControleurGeneral::getInstance().startConsole();

                    // ON SAUVEGARDE LA PARTIE
                    cout << "Voulez-vous sauvegarder la partie ? (y/n) : ";
                    string saveChoice; cin >> saveChoice;
                    if (saveChoice == "y" || saveChoice == "Y") {
                        string saveFile = askFilename();
                        if (handler.saveGame(false, saveFile)) {
                            cout << "Partie sauvegardée avec succès dans " << saveFile << endl;
                        } else {
                            cerr << "Erreur lors de la sauvegarde." << endl;
                        }
                    }
                    // ON LIBERE EVENTUELLEMENT LA PARTIE
                    //ControleurGeneral::freeInstance();
                } else {cerr << "Impossible de charger la partie." << endl;}

                // On indique ce qui a été chargé
                cout << "Chargement et lancement de la partie depuis : " << filename << endl;
            }
            catch (const exception& e) {cerr << "Exception lors du chargement de la partie : " << e.what() << endl;}
            catch (...) {cerr << "Erreur inconnue lors du chargement de la partie." << endl;}
        }
        // SINON ON CREER UNE NOUVELLE PARTIE
        else {createNewGame();}
    }
    // On récupère ici toutes les éventuelles erreurs pour une sortie "propre" du programme
    catch (const exception& e) {
        cerr << "Erreur fatale : " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cerr << "Erreur fatale inconnue." << endl;
        return 1;
    }

    // AU REVOIR
    cout << "Merci d'avoir joué !" << endl;
    return 0;
}