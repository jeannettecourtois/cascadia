#include "fileHandler.h"
#include "partie.h"

using namespace std;

//
static json toJsonPartie(const Partie& partie) {
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
static void fromJsonPartie(const nlohmann::json& j, Partie& partie) {
    partie.reinitialiserPartie(); // reset : une option pour kill la partie en cours rapidement et fermer le porgramme/retourner au debut

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
    Pioche* nouvellePioche = new Pioche(j["players"].size());
    nouvellePioche->fromJson(j["pioche"]);
    partie.setPioche(nouvellePioche);

    // Game state
    partie.setNbTour(j["gameState"]["toursRestants"]);
    partie.setJoueurCourant(j["gameState"]["joueurCourant"]);
    partie.setPhase(j["gameState"]["phase"]); //!!! Pas ecnore reflechi à comment l'utiliser intelligement : pour l'instant utilise jeu commutateur pour passer entre les options. integrer ?

    partie.setControleurTour(new ControleurTour());
    
    /* Chgt */
    //!!! Rejouer les actions ??
    for (const auto& ja : j["actionHistory"]) {
        Action* a = Action::fromJson(ja, &partie);
        partie.getControleurTour()->executerAction(a);
        // peut stocker a si on a besoin de le garder
    }

}


// Récupère la partie et load fromJsonPartie
bool FileHandler::loadGame(const std::string& filename) {
    ifstream file(filename);
    if (!file) return false;

    json j;
    file >> j;

    // Récupère la partie
    Partie& partie = Partie::getInstance();
    fromJsonPartie(j, partie);
    return true;
}

//
bool FileHandler::saveGame(const std::string& filename) {
    // On récupère la partie
    Partie& partie = Partie::getInstance();
    // On transforme la partie en json
    json j = toJsonPartie(partie);

    std::ofstream file(filename);
    if (!file) return false;

    // On le balance dans le fichier
    file << j.dump(4);  // indente correctement (cf. convention)
    return true;
}
