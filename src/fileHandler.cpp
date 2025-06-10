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

    // etat general
    j["gameState"] = {
        {"toursRestants", partie.getNbTour()},
        {"joueurCourant", partie.getJoueurCourant()},
        {"phase", partie.getPhaseCourante()}
    };

    // Regles de faune
    j["carteMarquageFaune"] = json::array();
    for (size_t i = 0; i < 5; i++) {
        CarteMarquageFaune* carte = partie.getCarteRegle(i);
        j["carteMarquageFaune"].push_back(carte->toJson());
    }

    // Joueurs
    j["players"] = json::array();
    for (const Joueur* joueur : partie.getJoueurs())
        j["players"].push_back(joueur->toJson());

    // Pioche
    j["pioche"] = partie.getPioche()->toJson();

    // Historique d'actions
    j["actionHistory"] = partie.getControleurTour()->toJson();

    // Si un jeton a ete selectionne sans etre encore place, on l’ajoute manuellement a la sauvegarde
    const auto& actions = partie.getControleurTour()->getListeActions();

    if (!actions.empty()) {
        const auto& actions = partie.getControleurTour()->getListeActions();

        bool jetonDejaPlace = false;
        for (const auto* a : actions) {
            if (dynamic_cast<const ActionPlacerJeton*>(a)) {
                jetonDejaPlace = true;
                break;
            }
        }

        if (!jetonDejaPlace) {
            // Rechercher la derniere tuile placee
            bool posTrouvee = false;
            Position posDerniereTuile;
            for (auto it = actions.rbegin(); it != actions.rend(); ++it) {
                if (auto* placer = dynamic_cast<ActionPlacerTuile*>(*it)) {
                    posDerniereTuile = placer->getPosition();
                    posTrouvee = true;
                    break;
                }
            }
            if (posTrouvee) {
                j["actionHistory"].push_back({
                    {"type", "PlacementJeton"},
                    {"TuileCible", {{"x", posDerniereTuile.x}, {"y", posDerniereTuile.y}}},
                    {"jeton", toString(partie.getAnimalJetonSelectionne())}
                });
            }/*
            if (posTrouvee) {
                Joueur* joueur = partie.getJoueur(partie.getJoueurCourant());
                TuilePlacee* cible = joueur->getPlateau()->getTuilePlacee(posDerniereTuile);
                if (cible) {
                    ActionPlacerJeton* action = new ActionPlacerJeton(partie.getAnimalJetonSelectionne(), cible, joueur);
                    action->executer();  // important : place le jeton dans la tuile
                    j["actionHistory"].push_back(action->toJson());
                    delete action;
                }
            }*/
        }
    }

    return j;
}

//
static void fromJsonPartie(const nlohmann::json& j, Partie& partie) {
    partie.reinitialiserPartie(); // reset : une option pour kill la partie en cours rapidement et fermer le porgramme/retourner au debut

    // Supprime
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

    // etat du jeu
    partie.setNbTour(j["gameState"]["toursRestants"]);
    partie.setJoueurCourant(j["gameState"]["joueurCourant"]);

    // Controleur de tour neuf
    ControleurTour* nouveauCtrl = new ControleurTour();
    partie.setControleurTour(nouveauCtrl);

    // Historique d'actions : execute les actions de selection si valides
    for (const auto& ja : j["actionHistory"]) {
        try {
            if (ja["type"] == "SelectionJeton") {
                auto* act = ActionSelectionJeton::fromJson(ja, partie.getPioche());
                nouveauCtrl->ajouterAction(act);
                continue;
            }
            Action* a = Action::fromJson(ja, &partie);
            if (auto* selT = dynamic_cast<ActionSelectionTuile*>(a)) { nouveauCtrl->ajouterAction(selT); }
            else if (auto* selJ = dynamic_cast<ActionSelectionJeton*>(a)) { nouveauCtrl->ajouterAction(selJ); }
            else if (auto* placerT = dynamic_cast<ActionPlacerTuile*>(a)) { nouveauCtrl->ajouterAction(placerT); }
            else if (auto* placerJ = dynamic_cast<ActionPlacerJeton*>(a)) { nouveauCtrl->ajouterAction(placerJ);}
        }
        catch (const std::exception& e) {
            std::cerr << "[ERREUR] Chargement d'une action echoue : " << e.what() << "\n";
        }
    }
    for (const auto& action : nouveauCtrl->getListeActions()) {
    if (auto selJ = dynamic_cast<ActionSelectionJeton*>(action)) {
        Animal a = selJ->getJetonSelection();
        if (a != Animal::Vide) {
            partie.setAnimalJetonSelectionne(a);
            std::cerr << "[INFO] Jeton selectionne restaure depuis ActionSelectionJeton: "
                      << static_cast<int>(a) << "\n";
        }
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
