#include "util.h"
#include <iostream>

using namespace std;

// Ajouter aussi pour : "Sauvegarder et quitter ? (1=oui/0=non): "
bool askYesNo(const string& s) {
    string input;
    while (true) {
        cout << s;
        cin >> input;
        if (input == "y" || input == "Y") return true;
        else if (input == "n" || input == "N") return false;
        else cout << "Reponse invalide, veuillez entrer 'y' ou 'n'." << endl;
    }
}

int askNumberOfPlayers() {
    int nbPlayers = -1;
    while (nbPlayers < 0 || nbPlayers > 4) {
        cout << "Combien de joueurs ? (1 a 4) : ";
        cin >> nbPlayers;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entree invalide, veuillez entrer un nombre entre 1 et 4 inclus." << endl;
            nbPlayers = -1;
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // nettoyage buffer
    return nbPlayers;
}
