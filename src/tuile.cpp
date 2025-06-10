#include <sstream>
#include "tuile.h"
#include "ControleurGeneral.h"

using namespace std;

unsigned int Tuile::idCounter = 0;

Tuile::Tuile() : id(idCounter++) {
    // Gen deterministe basee sur l'ID et BASE_SEED
    mt19937 gen(BASE_SEED + id);
    uniform_int_distribution<> dist3(1, 3);

    unsigned int nbHabitat = dist3(gen);
    unsigned int nbAnimaux = dist3(gen);

    // Ajout unique des animaux
    vector<int> animauxIndices = { 1, 2, 3, 4, 5 }; // pas de 0 car vide
    shuffle(animauxIndices.begin(), animauxIndices.end(), gen);
    for (unsigned int i = 0; i < nbAnimaux; ++i) {
        listeAnimaux.push_back(static_cast<Animal>(animauxIndices[i]));
    }

    // Ajout unique des habitats
    vector<int> habitatsIndices = { 0, 1, 2, 3, 4 }; // attribue automatiquement
    shuffle(habitatsIndices.begin(), habitatsIndices.end(), gen);
    for (unsigned int i = 0; i < nbHabitat; ++i) {
        listeHabitat.push_back(static_cast<Habitat>(habitatsIndices[i]));
    }
}



Tuile::Tuile(unsigned int nbAnimaux, unsigned int nbHabitat, const Animal* animauxArr, const Habitat* habitatsArr)
    : id(idCounter++) {

    for (unsigned int i = 0; i < nbAnimaux; ++i) {
        for (unsigned int j = i + 1; j < nbAnimaux; ++j) {
            if (animauxArr[i] == animauxArr[j])
                throw invalid_argument("Doublon d'animal dans la tuile");
        }
        listeAnimaux.push_back(animauxArr[i]);
    }

    for (unsigned int i = 0; i < nbHabitat; ++i) {
        for (unsigned int j = i + 1; j < nbHabitat; ++j) {
            if (habitatsArr[i] == habitatsArr[j])
                throw invalid_argument("Doublon d'habitat dans la tuile");
        }
        listeHabitat.push_back(habitatsArr[i]);
    }
}

bool Tuile::contientAnimal(const Animal& a) const { return find(listeAnimaux.begin(), listeAnimaux.end(), a) != listeAnimaux.end(); }


void Tuile::afficherTuile(ostream& f) const {
    f << "\nTuile avec " << getNbAnimaux() << " animaux et " << getNbHabitat() << " habitats.\n";
    f << " | Animaux : ";
    for (unsigned int i = 0; i < getNbAnimaux(); ++i) {
        f << AnimalFormateur{ listeAnimaux[i], Format::Complet } << " ";
    }
    f << "\n | Habitats : ";
    for (unsigned int i = 0; i < getNbHabitat(); ++i) {
        f << HabitatFormateur{ listeHabitat[i], Format::Complet } << " ";
    }
    f << endl;
}

void Tuile::afficherTuileHexa(ostream& f) const {
    ostringstream h1, h2, h3;
    if (getNbHabitat() > 0) h1 << HabitatFormateur{ listeHabitat[0], Format::Court };
    if (getNbHabitat() > 1) h2 << HabitatFormateur{ listeHabitat[1], Format::Court };
    if (getNbHabitat() > 2) h3 << HabitatFormateur{ listeHabitat[2], Format::Court };

    ostringstream animaux;
    for (auto& a : listeAnimaux) {
        animaux << AnimalFormateur{ a, Format::Court };
    }

    string animalStr = animaux.str();
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
    j["starter"] = starter;

    j["habitats"] = json::array();
    for (const Habitat h : getVectHabitat())
        j["habitats"].push_back(toString(h));

    j["animaux"] = json::array();
    for (const Animal a : getVectAnimaux())
        j["animaux"].push_back(toString(a));

    return j;
}

Tuile& Tuile::fromJson(const json& j) {
    Tuile& t = *ControleurGeneral::getInstance().getTuileById(j.at("id"));

    for (const auto& h : j.at("habitats")) {
        string h_str = h.get<string>();
        t.listeHabitat.push_back(fromStringHabitat(h_str));
    }

    for (const auto& a : j.at("animaux")) {
        string a_str = a.get<string>();
        t.listeAnimaux.push_back(fromStringAnimal(a_str));
    }

    return t;
}


/*
Keystone::Keystone() : Tuile() {}

Keystone::~Keystone() {}

// Donne un jeton keyStone
joueur si pose animal sur bonne case keyStone
void Keystone::associationJeton() const {}*/


