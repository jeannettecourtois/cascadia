#include <sstream>
#include "tuile.h"

using namespace std;

unsigned int Tuile::idCounter = 0;

Tuile::Tuile() : id(idCounter++) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist3(1, 3);

    nbHabitat = dist3(gen);
    nbAnimaux = dist3(gen);

    // Prend de la place en prevision
    listeAnimaux.reserve(nbAnimaux);
    listeHabitat.reserve(nbHabitat);

    // Ajout unique des animaux
    std::vector<int> animauxIndices = {0, 1, 2, 3, 4};
    std::shuffle(animauxIndices.begin(), animauxIndices.end(), gen);
    for (unsigned int i = 0; i < nbAnimaux; ++i) {
        unsigned int unAnimal = animauxIndices[i];
        listeAnimaux.push_back(new Animal(*(std::next(animaux.begin(), unAnimal))));
    }

    // Ajout unique des habitats
    std::vector<int> habitatsIndices = {0, 1, 2, 3, 4};
    std::shuffle(habitatsIndices.begin(), habitatsIndices.end(), gen);
    for (unsigned int i = 0; i < nbHabitat; ++i) {
        unsigned int unHabitat = habitatsIndices[i];
        listeHabitat.push_back(new Habitat(*(std::next(habitats.begin(), unHabitat))));
    }
}

/*Tuile() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist3(1, 3);
    std::uniform_int_distribution<> dist5(0, 4);

    nbHabitat = dist3(gen);
    nbAnimaux = dist3(gen);

    // Prend de la place en prevision
    listeAnimaux.reserve(nbAnimaux);
    listeHabitat.reserve(nbHabitat);

    for (unsigned int i = 0; i < nbAnimaux; ++i) {
        unsigned int unAnimal = dist5(gen);
        listeAnimaux.push_back(new Animal(*(std::next(animaux.begin(), unAnimal))));
    }

    for (unsigned int i = 0; i < nbHabitat; ++i) {
        unsigned int unHabitat = dist5(gen);
        listeHabitat.push_back(new Habitat(*(std::next(habitats.begin(), unHabitat))));
    }
}*/

Tuile::Tuile(unsigned int nbAnimaux, unsigned int nbHabitat, const Animal* animauxArr, const Habitat* habitatsArr) : id(idCounter++) {
    // Verification de doublons pour les animaux
    for (unsigned int i = 0; i < nbAnimaux; ++i) {
        for (unsigned int j = i + 1; j < nbAnimaux; ++j) {
            if (animauxArr[i] == animauxArr[j]) {
                std::cerr << "Erreur : doublon d'animal detecte dans la tuile." << std::endl;
                throw std::invalid_argument("Doublon d'animal dans la tuile");
            }
        }
    }

    // Verification de doublons pour les habitats
    for (unsigned int i = 0; i < nbHabitat; ++i) {
        for (unsigned int j = i + 1; j < nbHabitat; ++j) {
            if (habitatsArr[i] == habitatsArr[j]) {
                std::cerr << "Erreur : doublon d'habitat detecte dans la tuile." << std::endl;
                throw std::invalid_argument("Doublon d'habitat dans la tuile");
            }
        }
    }

    this->nbAnimaux = nbAnimaux;
    this->nbHabitat = nbHabitat;
    listeAnimaux.reserve(nbAnimaux);
    listeHabitat.reserve(nbHabitat);

    for (unsigned int i = 0; i < nbAnimaux; ++i) {
        listeAnimaux.push_back(new Animal(animauxArr[i]));
    }
    for (unsigned int i = 0; i < nbHabitat; ++i) {
        listeHabitat.push_back(new Habitat(habitatsArr[i]));
    }
}


Tuile::~Tuile() {
    // Supprimer chaque Animal* et Habitat* puis laisser le vecteur se d�truire
    for (Animal* a : listeAnimaux)
        delete a;
    for (Habitat* h : listeHabitat)
        delete h;
    // Les vecteurs se lib�rent automatiquement
}

bool Tuile::contientAnimal(const Animal& a) const {
    for (unsigned int i = 0; i < nbAnimaux; ++i) {
        if (*listeAnimaux[i] == a) {
            return true;
        }
    }
    return false;
}

void Tuile::afficherTuile(std::ostream& f) const {
    f << "\nTuile avec " << nbAnimaux << " animaux et " << nbHabitat << " habitats.\n";
    f << " | Animaux : ";
    for (unsigned int i = 0; i < nbAnimaux; ++i) {
        f << AnimalFormateur{ *listeAnimaux[i], Format::Complet } << " ";
    }
    f << "\n | Habitats : ";
    for (unsigned int i = 0; i < nbHabitat; ++i) {
        f << HabitatFormateur{ *listeHabitat[i], Format::Complet } << " ";
    }
    f << std::endl;
}

void Tuile::afficherTuileHexa(std::ostream& f) const {
    std::ostringstream h1, h2, h3;
    if (nbHabitat > 0) h1 << HabitatFormateur{ *listeHabitat[0], Format::Court };
    if (nbHabitat > 1) h2 << HabitatFormateur{ *listeHabitat[1], Format::Court };
    if (nbHabitat > 2) h3 << HabitatFormateur{ *listeHabitat[2], Format::Court };

    std::ostringstream animaux;
    for (auto& a : listeAnimaux) {
        animaux << AnimalFormateur{ *a, Format::Court };
    }

    std::string animalStr = animaux.str();
    while (animalStr.length() < 5) animalStr = " " + animalStr;
    if (animalStr.length() > 5) animalStr = animalStr.substr(0, 5);

    f << "  /  "; f << h1.str(); f << " \\\n";
    f << " |"; f << h2.str(); f << "-1,1"; f << h2.str(); f << "|\n";
    f << " |"; f << h3.str(); f << animalStr; f << h3.str(); f << " |\n";
    f << "  \\_"; f << h1.str(); f << "_/\n";
}

json Tuile::toJson() const {
    json j;
    j["id"] = id;

    j["habitats"] = nlohmann::json::array();
    for (const Habitat* h : listeHabitat)
        j["habitats"].push_back(toString(*h));

    j["animaux"] = nlohmann::json::array();
    for (const Animal* a : listeAnimaux)
        j["animaux"].push_back(toString(*a));

    return j;
}

Tuile Tuile::fromJson(const nlohmann::json& j) {
    Tuile t;
    t.id = j.at("id");

    for (const auto& h : j.at("habitats"))
        t.listeHabitat.push_back(fromStringHabitat(h));

    for (const auto& a : j.at("animaux"))
        t.listeAnimaux.push_back(fromStringAnimal(a));

    return t;
}

/*
Keystone::Keystone() : Tuile() {}

Keystone::~Keystone() {}

// Donne un jeton keyStone
joueur si pose animal sur bonne case keyStone
void Keystone::associationJeton() const {}*/


