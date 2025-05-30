#include "fileHandler.h"

using namespace std;

bool FileHandler::loadGame(bool debug, const string& filename) { return true; }
bool FileHandler::saveGame(bool debug, const string& filename) { return true; }
bool FileHandler::saveToFile(const string& filename) {
    ofstream fichier(filename, ios::out); // Ouvre le fichier en ecriture
    if (!fichier.is_open()) {
        cerr << "[Erreur] Impossible d'ouvrir le fichier '" << filename << "' pour l'ecriture.\n";
        return false;
    }

    try {
        //!! Pour l'instant un traitement basique
        fichier << "ICI on mettra les Donnee du programme (?)\n";
        fichier << "ICI on mettra les Donnee du programme (?)\n";
        fichier << "ICI on mettra les Donnee du programme (?)\n";

        if (fichier.fail()) {
            throw ios_base::failure("echec lors de l'ecriture dans le fichier.");
        }

        fichier.close();
        return true;
    }
    catch (const ios_base::failure& e) {
        cerr << "[Exception] " << e.what() << "\n";
        fichier.close();
        return false;
    }
}

bool FileHandler::loadFromFile(const string& filename) {
    ifstream fichier(filename, ios::in); // Ouvre le fichier en lecture
    if (!fichier.is_open()) {
        cerr << "[Erreur] Impossible d'ouvrir le fichier '" << filename << "' pour la lecture.\n";
        return false;
    }

    try {
        string ligne;
        while (getline(fichier, ligne)) {
            // Pour l'instant un traitement basique
            cout << "[Lecture] " << ligne << "\n";
        }

        if (fichier.bad()) {
            throw ios_base::failure("echec critique lors de la lecture du fichier.");
        }

        fichier.close();
        return true;
    }
    catch (const ios_base::failure& e) {
        cerr << "[Exception] " << e.what() << "\n";
        fichier.close();
        return false;
    }
}

bool askLoadGame() {
    string input;
    while (true) {
        cout << "Voulez-vous charger une partie sauvegardee ? (y/n) : ";
        cin >> input;
        if (input == "y" || input == "Y") return true;
        else if (input == "n" || input == "N") return false;
        else cout << "Reponse invalide, veuillez entrer 'y' ou 'n'." << endl;
    }
}
