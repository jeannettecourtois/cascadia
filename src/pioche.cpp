#include "pioche.h"
#include "enum.h"

/* SacTuile */
SacTuile::SacTuile(int nbTuiles) : nbTuile(nbTuiles) {
    ControleurGeneral& cg = ControleurGeneral::getInstance();
    for (int i = 0; i < nbTuiles; ++i) {
        tuiles.push_back(cg.getTuile());
    }
}

SacTuile::~SacTuile() {}

Tuile* SacTuile::Piocher() {
    if (tuiles.empty()) return nullptr;

    // Filtrer uniquement les tuiles valides (non-starters)
    std::vector<Tuile*> candidates;
    for (Tuile* t : tuiles) {
        if (!t->isStarterTile()) {
            candidates.push_back(t);
        }
    }

    if (candidates.empty()) return nullptr; // Plus rien à piocher d'utile

    int index = rand() % candidates.size();
    Tuile* selected = candidates[index];

    // Trouver l'index original pour l'enlever du sac
    auto it = std::find(tuiles.begin(), tuiles.end(), selected);
    if (it != tuiles.end()) {
        tuiles.erase(it);
    }

    return selected;
}


void SacTuile::mettreDansLeSac(Tuile* t) {
    if (t->isStarterTile()) {
        std::cerr << "[WARN] Tuile starter ignorée lors de l'ajout au sac (id = " << t->getId() << ").\n";
        return; // Ne pas ajouter les tuiles de départ
    }
    tuiles.push_back(t);
}


/* SacJeton */
SacJeton::SacJeton(const std::vector<int>& repartition) : compteJetons(repartition), nbJetons(0) {
    for (int i : compteJetons) {
        nbJetons += i;
    }
}

SacJeton::~SacJeton() {}

Animal SacJeton::Piocher() {
    if (nbJetons == 0) return Animal::Vide;
    int index = rand() % nbJetons;

    for (int i = 0; i < compteJetons.size(); ++i) {
        if (index < compteJetons[i]) {
            compteJetons[i]--;
            nbJetons--;
            return static_cast<Animal>(i);
        }
        index -= compteJetons[i];
    }
    return Animal::Vide;
} 

void SacJeton::mettreDansLeSac(Animal a) {
    int index = static_cast<int>(a);
    if (index >= compteJetons.size()) {
        compteJetons.resize(index + 1, 0);
    }
    compteJetons[index]++;
    nbJetons++;
}

/* Pioche */
Pioche::Pioche(int nbJoueurs) {
    for (int i = 0; i < 4; ++i) {
        tuiles[i] = nullptr;
        jetons[i] = nullptr;
    }
    idTuileChoisis = -1;

    // calcul du nombre de tuiles selon le nombre de joueurs
    int nbTuiles;
    if (nbJoueurs == 1 || nbJoueurs == 2) nbTuiles = 43;
    else if (nbJoueurs == 3) nbTuiles = 63;
    else if (nbJoueurs == 4) nbTuiles = 83;

    sacDeTuiles = new SacTuile(nbTuiles);

    // repartition des jetons uniformement selon la liste d'animaux
    int nbJetonsTotal = nbJoueurs * 20;

    std::vector<int> repartition(6,0);
    int nbAnimaux = 0;
    for (Animal a : animaux) {
        repartition[static_cast<int>(a)] = nbJetonsTotal / 5;
        nbAnimaux++;
    }
    sacDeJetons = new SacJeton(repartition);
}

Pioche::~Pioche() {
    for (int i = 0; i < 4; ++i) {
        delete tuiles[i]; 
    }
    for (int i = 0; i < 4; ++i) {
    delete jetons[i];
    }
    delete sacDeTuiles;
    delete sacDeJetons;
}

void Pioche::preparerPioche() {
    // Preparer la pioche de tuiles et de jetons
    for (int i = 0; i < 4; ++i) {
        tuiles[i] = sacDeTuiles->Piocher();
        Animal a = sacDeJetons->Piocher();
        jetons[i] = new Animal(a);
    }
}

void Pioche::selectionnerTuile() {
    // Selectionner une tuile parmi celles disponibles
    idTuileChoisis = rand() % 4;
}

Animal* Pioche::selectionnerJeton() const {
    int idJetonChoisis = rand() % 4;
    return jetons[idJetonChoisis];
}



void Pioche::afficherTuilesDisponibles() const {
    std::cout << "Tuiles disponibles dans la pioche :\n\n";
    for (int i = 0; i < 4; ++i) {
        std::cout << "Tuile " << i << ": ";
        if (tuiles[i]) {
            tuiles[i]->afficherTuile();
        }
        else {
            std::cout << "Vide";
        }
        std::cout << "\n";
    } std::cout << std::endl;
}

void Pioche::afficherJetonsDisponibles() const {
    std::cout << "Jetons disponibles dans la pioche :\n\n";
    for (int i = 0; i < 4; ++i) {
        std::cout << "Jeton " << i << ": ";
        if (jetons[i] != nullptr) {
            std::cout << AnimalFormateur{ *jetons[i],Format::Complet };
        }
        else {
            std::cout << "Vide";
        }
        std::cout << "\n";
    } std::cout << std::endl;
}

// Retirer les jetons/tuiles qui ont ete selecionnes et en mettre de nouveaux
void Pioche::completerPioche(const Animal* a, const Tuile* t) {
    // on change la tuile
    for (int i = 0; i < 4; ++i) {
        if (tuiles[i] == t) {
            tuiles[i] = sacDeTuiles->Piocher();
        }
    }

    // on change le jeton animal
    for (int i = 0; i < 4; ++i) {
        if (jetons[i] == a) {
            Animal a = sacDeJetons->Piocher();
            jetons[i] = new Animal(a);
        }
    }

    // gestion de la surpopulation : 3 ou 4 animaux identiques
    // creation d'un dictionnaire pour compter le nombre de fois ou le jeton apparait
    std::map<Animal, int> compteur;
    for (int i = 0; i < 4; ++i) {
        if (jetons[i]) {
            compteur[*jetons[i]]++;
        }
    }
    // parcours du dictionnaire
    for (const auto& pair : compteur) {
        if (pair.second >= 3) {
            std::cout << "Trois ou plus jetons identiques (" << AnimalFormateur{ pair.first,Format::Complet } << ") detectes. On les remplace tous.\n";

            // on remet les jetons dans sacJeton et on repioche
            for (int i = 0; i < 4; ++i) {
                if (jetons[i]) {
                    sacDeJetons->mettreDansLeSac(*jetons[i]);
                    delete jetons[i];
                    jetons[i] = new Animal(sacDeJetons->Piocher());
                }
            }
            break;
        }
    }
}

// pioche.cpp
json Pioche::toJson() const {
    json j;

    // Sac tuiles : en supposant que vous avez un identifiant ou un toJson simple
    j["sacTuiles"] = nlohmann::json::array();
    for (const Tuile* t : sacDeTuiles->getTuiles()) {
        j["sacTuiles"].push_back(t->toJson()); //bug free
    }

    // Sac jetons : enum -> string
    json j_jetons = json::array();
    const auto& compte = sacDeJetons->getCompteJetons();

    for (size_t i = 0; i < compte.size(); ++i) {
        Animal a = static_cast<Animal>(i);
        int nb = compte[i];
        for (int k = 0; k < nb; ++k) {
            j_jetons.push_back(toString(a));
        }
    }
    j["sacJetons"] = j_jetons;

    // tuiles dans pioche
    j["tuilesPioche"] = json::array();
    for (size_t i = 0; i < 4; i++){
        Tuile* t = tuiles[i];
        j["tuilesPioche"].push_back(t->toJson());
    }

    //  jetons dans pioche
    j["jetonsPioche"] = nlohmann::json::array();
    for (size_t i = 0; i < 4; i++) {
        Animal* a = jetons[i];
        j["jetonsPioche"].push_back(toString(*a));
    }

    return j;
}

void Pioche::fromJson(const json& j) {
    ControleurGeneral& controleur = ControleurGeneral::getInstance();

    // --- Chargement du sac de tuiles ---
    auto& sac = sacDeTuiles->getTuiles();
    sac.clear();  // On part d’un sac vide
    for (const auto& tuileJson : j.at("sacTuiles")) {
        unsigned int id = tuileJson.at("id");
        Tuile* t = controleur.getTuileById(id);
        if (!t) {
            throw std::runtime_error("Tuile introuvable dans le controleur pour l'id: " + std::to_string(id));
        }
        sac.push_back(t);
    }

    // --- Chargement des tuiles de la pioche ---
    for (int i = 0; i < 4; ++i) {
        unsigned int id = j["tuilesPioche"][i].at("id");
        tuiles[i] = controleur.getTuileById(id);
    }

    // --- Chargement des jetons du sac ---
    std::vector<int> compte(6, 0);
    for (const auto& jn : j.at("sacJetons")) {
        Animal a = fromStringAnimal(jn);
        ++compte[static_cast<int>(a)];
    }
    delete sacDeJetons;
    sacDeJetons = new SacJeton(compte);

    // --- Chargement des 4 jetons visibles ---
    for (int i = 0; i < 4; ++i) {
        Animal a = fromStringAnimal(j["jetonsPioche"][i]);
        delete jetons[i]; // sécurisation mémoire si un ancien pointeur existe
        jetons[i] = new Animal(a);
    }
}