#include "controleurGeneral.h"
#include "enum.h"

using namespace std;

TuileDepart::TuileDepart() {
    for (int i = 0; i < 3; ++i) {
        tuiles[i] = nullptr;
    }
}

TuileDepart::TuileDepart(const TuileDepart& other) {
    for (int i = 0; i < 3; ++i) {
        if (other.tuiles[i])
            tuiles[i] = new TuilePlacee(*other.tuiles[i]); // Copie profonde
        else
            tuiles[i] = nullptr;
    }
}

TuileDepart::~TuileDepart() {
    for (int i = 0; i < 3; ++i) {
        delete tuiles[i];
    }
}


ControleurGeneral::ControleurGeneral()
    : nbTuilesHabitat(85),
    nbJetonFaune(100),
    nbJetonsNature(25),
    nbCartesMarquageFaune(21),
    nbCartesRegles(0),
    nbTuilesDepart(0),
    gen(rd()),
    distTuiles(0, 84),
    distJetons(0, 99),
    distCartes(0, 14) {

    // Construction des jetons Faune
    for (unsigned int i = 0; i < 20; ++i) tabJetons[i] = new JetonFaune(Animal::Ours);
    for (unsigned int i = 20; i < 40; ++i) tabJetons[i] = new JetonFaune(Animal::Cerf);
    for (unsigned int i = 40; i < 60; ++i) tabJetons[i] = new JetonFaune(Animal::Saumon);
    for (unsigned int i = 60; i < 80; ++i) tabJetons[i] = new JetonFaune(Animal::Aigle);
    for (unsigned int i = 80; i < 100; ++i) tabJetons[i] = new JetonFaune(Animal::Renard);

    // Construction des Tuiles
    for (unsigned int i = 0; i < 85; ++i) tabTuiles[i] = new Tuile();

    // Construction des Cartes de Marquage Faune
    for (unsigned int i = 0; i < 3; ++i) tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Aigle);
    for (unsigned int i = 3; i < 6; ++i) tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Cerf);
    for (unsigned int i = 6; i < 9; ++i) tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Ours);
    for (unsigned int i = 9; i < 12; ++i) tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Renard);
    for (unsigned int i = 12; i < 15; ++i) tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Saumon);
}

ControleurGeneral::~ControleurGeneral() {
    for (unsigned int i = 0; i < 100; ++i) delete tabJetons[i];
    for (unsigned int i = 0; i < 85; ++i) delete tabTuiles[i];
    for (unsigned int i = 0; i < 15; ++i) delete tabCartesMarquage[i];

    if (cartesRegles) {
        for (int i = 0; i < nbCartesRegles; ++i) delete cartesRegles[i];
        delete[] cartesRegles;
    }

    if (tuilesDepart) {
        for (int i = 0; i < nbTuilesDepart; ++i) delete tuilesDepart[i];
        delete[] tuilesDepart;
    }
}

ControleurGeneral& ControleurGeneral::getInstance() {
    static ControleurGeneral instance;
    return instance;
}

Tuile* ControleurGeneral::getTuile() {
    return tabTuiles[distTuiles(gen)];
}

JetonFaune* ControleurGeneral::getJetonFaune() {
    return tabJetons[distJetons(gen)];
}

CarteMarquageFaune* ControleurGeneral::getCarteMarquageFaune() {
    return tabCartesMarquage[distCartes(gen)];
}

CarteMarquageFaune* ControleurGeneral::getCarteRegleAleatoire() {
    if (nbCartesRegles == 0 || cartesRegles == nullptr)
        throw std::runtime_error("Aucune carte règle disponible.");
    std::uniform_int_distribution<> dist(0, nbCartesRegles - 1);
    return cartesRegles[dist(gen)];
}

TuileDepart** ControleurGeneral::getTuilesDepartAleatoires() {
    if (nbTuilesDepart == 0 || tuilesDepart == nullptr)
        throw std::runtime_error("Aucune tuile de départ disponible.");

    TuileDepart** selection = new TuileDepart * [3];
    for (int i = 0; i < 3; ++i) {
        int index = std::uniform_int_distribution<>(0, nbTuilesDepart - 1)(gen);
        selection[i] = new TuileDepart(*tuilesDepart[index]);
    }
    return selection;
}