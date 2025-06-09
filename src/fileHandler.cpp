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
        j["players"].push_back(joueur->toJson());

    // Regles de faune
    j["carteMarquageFaune"] = json::array();
    for (size_t i = 0; i < 5; i++) {
        CarteMarquageFaune* carte = partie.getCarteRegle(i);
        j["carteMarquageFaune"].push_back(carte->toJson());
    }

    // Pioche
    j["pioche"] = partie.getPioche()->toJson();

    // etat general
    j["gameState"] = {
        {"toursRestants", partie.getNbTour()},
        {"joueurCourant", partie.getJoueurCourant()},
        {"phase", partie.getPhaseCourante()}
    };

    // Historique d'actions
    j["actionHistory"] = partie.getControleurTour()->toJson();

    return j;
}

//
static void fromJsonPartie(const nlohmann::json& j, Partie& partie) {
    partie.reinitialiserPartie(); // reset : une option pour kill la partie en cours rapidement et fermer le porgramme/retourner au debut

    if (partie.getControleurTour()) {
        ControleurTour* ancienCtrl = partie.getControleurTour();
        for (Action* a : ancienCtrl->getListeActions()) {
            delete a;
        }
        ancienCtrl->getListeActions().clear();
        delete ancienCtrl;
        partie.setControleurTour(nullptr);
    }

    // Joueurs
    for (const auto& jjoueur : j["players"]) {
        Joueur* joueur = Joueur::fromJson(jjoueur, &partie);
        partie.ajouterJoueur(joueur);
    }

    // Cartes de marquage de faune
    int i = 0;
    for (const auto& jc : j["carteMarquageFaune"]) {
        CarteMarquageFaune* carte = &CarteMarquageFaune::fromJson(jc);
        partie.setCarteFaune(i++, carte);
    }

    // Pioche
    Pioche* nouvellePioche = new Pioche(j["players"].size());
    nouvellePioche->fromJson(j["pioche"]);
    partie.setPioche(nouvellePioche);

    // État du jeu
    partie.setNbTour(j["gameState"]["toursRestants"]);
    partie.setJoueurCourant(j["gameState"]["joueurCourant"]);

    // Contrôleur de tour neuf
    ControleurTour* nouveauCtrl = new ControleurTour();
    partie.setControleurTour(nouveauCtrl);

    // Historique d'actions : exécute les actions de sélection si valides
    for (const auto& ja : j["actionHistory"]) {
        try {
            Action* a = Action::fromJson(ja, &partie);

            // Exécuter uniquement les actions de sélection, ignorer si invalide
            if (dynamic_cast<ActionSelectionTuile*>(a) != nullptr || dynamic_cast<ActionSelectionJeton*>(a) != nullptr) {
                if (a->executer() == -1) {
                    delete a;
                    continue; // Ne pas ajouter au controleur
                }
            }
            nouveauCtrl->executerAction(a);
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur lors du chargement d'une action : " << e.what() << std::endl;
        }
    }
}


// Recupere la partie et load fromJsonPartie
bool FileHandler::loadGame(const std::string& filename) {
    ifstream file(filename);
    if (!file) return false;

    json j;
    file >> j;

    // Recupere la partie
    Partie& partie = Partie::getInstance();
    fromJsonPartie(j, partie);
    return true;
}

//
bool FileHandler::saveGame(const std::string& filename) {
    // On recupere la partie
    Partie& partie = Partie::getInstance();
    // On transforme la partie en json
    json j = toJsonPartie(partie);

    ofstream file(filename);
    if (!file) return false;

    // On le balance dans le fichier
    file << j.dump(4);  // indente correctement (cf. convention)
    return true;
}
