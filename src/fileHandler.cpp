#include "fileHandler.h"
#include "partie.h"

using namespace std;
json toJsonPartie(const Partie& partie);
void fromJsonPartie(const nlohmann::json& j, Partie& partie);
//
bool FileHandler::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return false;

    nlohmann::json j;
    file >> j;

    Partie& partie = Partie::getInstance();
    fromJsonPartie(j, partie);
    return true;
}

//
bool FileHandler::saveGame(const std::string& filename) {
    Partie& partie = Partie::getInstance();
    nlohmann::json j = toJsonPartie(partie);

    std::ofstream file(filename);
    if (!file) return false;

    file << j.dump(4);  // indente correctement (cf. convention)
    return true;
}

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

//
json toJsonPartie(const Partie& partie) {
    json j;

    // Meta
    j["metadata"] = {
        {"game", "Cascadia"},
        {"version", "1.0"},
        {"saveFormatVersion", 1}
    };

    // Joueurs
    j["players"] = json::array();
    for (const Joueur* joueur : partie.getJoueurs())
        j["players"].push_back(joueur->toJson()); // toJson à coder

    // Regles de faune
    j["carteMarquageFaune"] = json::array();
    for (size_t i = 0; i < 5; i++) {
        CarteMarquageFaune* carte = partie.getCarteRegle(i);
        j["carteMarquageFaune"].push_back(carte->toJson()); // toJson à coder
    }

    // Pioche
    j["pioche"] = partie.getPioche()->toJson(); // toJson à coder

    // etat general
    j["gameState"] = {
        {"toursRestants", partie.getNbTour()},
        {"joueurCourant", partie.getJoueurCourant()},
        {"phase", partie.getPhase()}
    };

    // Historique d'actions
    j["actionHistory"] = json::array();
    for (Action* a : partie.getControleurTour()->getListeActions())
        j["actionHistory"].push_back(a->toJson()); // toJson à coder

    return j;
}

//
void fromJsonPartie(const nlohmann::json& j, Partie& partie) {
    /*
    partie.reinitialiser(); // reset : une option pour kill la partie en cours rapidement et fermer le porgramme/retourner au debut
    */

    // Joueurs
    for (const auto& jjoueur : j["players"]) {
        Joueur* joueur = Joueur::fromJson(jjoueur, &partie);
        partie.ajouterJoueur(joueur);
    }

    // Cartes
    int i = 0;
    for (const auto& jc : j["carteMarquageFaune"]) {
        CarteMarquageFaune* carte = &CarteMarquageFaune::fromJson(jc);
        partie.setCarteFaune(i++, carte);
    }

    // Pioche
    partie.getPioche()->fromJson(j["pioche"]);

    // Game state
    partie.setNbTour(j["gameState"]["toursRestants"]);
    partie.setJoueurCourant(j["gameState"]["joueurCourant"]);
    partie.setPhase(j["gameState"]["phase"]); //!!! Pas ecnore reflechi à comment l'utiliser intelligement : pour l'instant utilise jeu commutateur pour passer entre les options. integrer ?

    /*
    // Rejouer les actions ??
    for (const auto& ja : j["actionHistory"]) {
        Action a = Action::fromJson(ja);
        partie.getControleurTour().executerAction(a.get());
        // peut stocker a si on a besoin de le garder
    }
    */
}
