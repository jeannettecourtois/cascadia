#include "controleurGeneral.h"
#include "enum.h"

using namespace std;

// TuileDepart
TuileDepart::TuileDepart() { for (int i = 0; i < 3; ++i) tuiles[i] = nullptr; }
TuileDepart::~TuileDepart() { for (int i = 0; i < 3; ++i) delete tuiles[i]; }

TuileDepart::TuileDepart(const TuileDepart& other) {
    for (int i = 0; i < 3; ++i) {
        tuiles[i] = (other.tuiles[i] ? new TuilePlacee(*other.tuiles[i]) : nullptr);
    }
}

// ControleurGeneral
ControleurGeneral::ControleurGeneral()
    : nbTuilesHabitat(85),
    nbJetonFaune(100),
    nbJetonsNature(25),
    nbCartesMarquageFaune(15),
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
    for (size_t i = 0; i < 85; ++i) tabTuiles[i] = new Tuile();
    // Construction des Tuiles de depart (placeholder)
    for (size_t i = 85; i < 90; ++i) { tabTuiles[i] = nullptr; } // seront remplies dans initialiserTuilesDepart

    // Construction des Cartes de Marquage Faune
    for (unsigned int i = 0; i < 3; ++i) tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Aigle);
    for (unsigned int i = 3; i < 6; ++i) tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Cerf);
    for (unsigned int i = 6; i < 9; ++i) tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Ours);
    for (unsigned int i = 9; i < 12; ++i) tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Renard);
    for (unsigned int i = 12; i < 15; ++i) tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Saumon);

    // Construction des Tuiles de Depart
    initialiserTuilesDepart();
}

ControleurGeneral::~ControleurGeneral() {
    if (cartesRegles) {
        for (int i = 0; i < nbCartesRegles; ++i) delete cartesRegles[i];
        delete[] cartesRegles;
    }
    // Les vecteurs gerent leur propre memoire
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

/*CarteMarquageFaune* ControleurGeneral::getCarteMarquageFaune() {
    return tabCartesMarquage[distCartes(gen)];
}*/

CarteMarquageFaune* ControleurGeneral::getCarteMarquageParAnimalAleatoire(Animal a) {
    int debut = 0;
    switch (a) {
        case Animal::Aigle:  debut = 0;  break;
        case Animal::Cerf:   debut = 3;  break;
        case Animal::Ours:   debut = 6;  break;
        case Animal::Renard: debut = 9;  break;
        case Animal::Saumon: debut = 12; break;
        default: throw std::runtime_error("Animal inconnu pour la selection de carte.");
    }
    std::uniform_int_distribution<> distCarte(0, 2);
    return tabCartesMarquage[debut + distCarte(gen)];
}

void ControleurGeneral::initialiserTuilesDepart() {
    nbTuilesDepart = 5;
    tuilesDepart = new TuileDepart*[nbTuilesDepart];

    // TuileDepart 0
    {   
        Animal a1[1] = { Animal::Aigle };
        Habitat h1[1] = { Habitat::Marais };
        tabTuiles[85] = new Tuile(1, 1, a1, h1);
        tabTuiles[85]->setStarter(true);

        Animal a2[2] = { Animal::Ours, Animal::Renard };
        Habitat h2[2] = { Habitat::Montagne, Habitat::Prairie };
        tabTuiles[86] = new Tuile(2, 2, a2, h2);
        tabTuiles[86]->setStarter(true);

        Animal a3[3] = { Animal::Saumon, Animal::Cerf, Animal::Aigle };
        Habitat h3[2] = { Habitat::Riviere, Habitat::Foret };
        tabTuiles[87] = new Tuile(3, 2, a3, h3);
        tabTuiles[87]->setStarter(true);

        tuilesDepart[0] = new TuileDepart();
        tuilesDepart[0]->setTuile(0, new TuilePlacee(tabTuiles[85], Animal::Vide));
        tuilesDepart[0]->setTuile(1, new TuilePlacee(tabTuiles[86], Animal::Vide));
        tuilesDepart[0]->setTuile(2, new TuilePlacee(tabTuiles[87], Animal::Vide));
    }

    // TuileDepart 1
    {
        Animal a1[1] = { Animal::Ours };
        Habitat h1[1] = { Habitat::Montagne };
        tabTuiles[88] = new Tuile(1, 1, a1, h1);
        tabTuiles[88]->setStarter(true);

        Animal a2[2] = { Animal::Ours, Animal::Saumon };
        Habitat h2[2] = { Habitat::Prairie, Habitat::Riviere };
        tabTuiles[89] = new Tuile(2, 2, a2, h2);
        tabTuiles[89]->setStarter(true);

        Animal a3[3] = { Animal::Cerf, Animal::Aigle, Animal::Renard };
        Habitat h3[2] = { Habitat::Marais, Habitat::Foret };
        tabTuiles[90] = new Tuile(3, 2, a3, h3);
        tabTuiles[90]->setStarter(true);

        tuilesDepart[1] = new TuileDepart();
        tuilesDepart[1]->setTuile(0, new TuilePlacee(tabTuiles[88], Animal::Vide));
        tuilesDepart[1]->setTuile(1, new TuilePlacee(tabTuiles[89], Animal::Vide));
        tuilesDepart[1]->setTuile(2, new TuilePlacee(tabTuiles[90], Animal::Vide));
    }


    // TuileDepart 2
    {
        Animal a1[1] = { Animal::Cerf };
        Habitat h1[1] = { Habitat::Foret };
        tabTuiles[91] = new Tuile(1, 1, a1, h1);
        tabTuiles[91]->setStarter(true);

        Animal a2[2] = { Animal::Renard, Animal::Saumon };
        Habitat h2[2] = { Habitat::Prairie, Habitat::Marais };
        tabTuiles[92] = new Tuile(2, 2, a2, h2);
        tabTuiles[92]->setStarter(true);

        Animal a3[3] = { Animal::Aigle, Animal::Cerf, Animal::Ours };
        Habitat h3[2] = { Habitat::Montagne, Habitat::Riviere };
        tabTuiles[93] = new Tuile(3, 2, a3, h3);
        tabTuiles[93]->setStarter(true);

        tuilesDepart[2] = new TuileDepart();
        tuilesDepart[2]->setTuile(0, new TuilePlacee(tabTuiles[91], Animal::Vide));
        tuilesDepart[2]->setTuile(1, new TuilePlacee(tabTuiles[92], Animal::Vide));
        tuilesDepart[2]->setTuile(2, new TuilePlacee(tabTuiles[93], Animal::Vide));
    }


    // TuileDepart 3
    {
        Animal a1[1] = { Animal::Saumon };
        Habitat h1[1] = { Habitat::Riviere };
        tabTuiles[94] = new Tuile(1, 1, a1, h1);
        tabTuiles[94]->setStarter(true);

        Animal a2[2] = { Animal::Aigle, Animal::Renard };
        Habitat h2[2] = { Habitat::Montagne, Habitat::Riviere };
        tabTuiles[95] = new Tuile(2, 2, a2, h2);
        tabTuiles[95]->setStarter(true);

        Animal a3[3] = { Animal::Ours, Animal::Cerf, Animal::Saumon };
        Habitat h3[2] = { Habitat::Foret, Habitat::Prairie };
        tabTuiles[96] = new Tuile(3, 2, a3, h3);
        tabTuiles[96]->setStarter(true);

        tuilesDepart[3] = new TuileDepart();
        tuilesDepart[3]->setTuile(0, new TuilePlacee(tabTuiles[94], Animal::Vide));
        tuilesDepart[3]->setTuile(1, new TuilePlacee(tabTuiles[95], Animal::Vide));
        tuilesDepart[3]->setTuile(2, new TuilePlacee(tabTuiles[96], Animal::Vide));
    }


    // TuileDepart 4
    {
        Animal a1[1] = { Animal::Renard };
        Habitat h1[1] = { Habitat::Prairie };
        tabTuiles[97] = new Tuile(1, 1, a1, h1);
        tabTuiles[97]->setStarter(true);

        Animal a2[2] = { Animal::Ours, Animal::Cerf };
        Habitat h2[2] = { Habitat::Montagne, Habitat::Foret };
        tabTuiles[98] = new Tuile(2, 2, a2, h2);
        tabTuiles[98]->setStarter(true);

        Animal a3[3] = { Animal::Aigle, Animal::Saumon, Animal::Renard };
        Habitat h3[2] = { Habitat::Riviere, Habitat::Marais };
        tabTuiles[99] = new Tuile(3, 2, a3, h3);
        tabTuiles[99]->setStarter(true);

        tuilesDepart[4] = new TuileDepart();
        tuilesDepart[4]->setTuile(0, new TuilePlacee(tabTuiles[97], Animal::Vide));
        tuilesDepart[4]->setTuile(1, new TuilePlacee(tabTuiles[98], Animal::Vide));
        tuilesDepart[4]->setTuile(2, new TuilePlacee(tabTuiles[99], Animal::Vide));
    }
}

TuileDepart* ControleurGeneral::getTuileDepartAleatoire() {
    if (nbTuilesDepart == 0 || tuilesDepart == nullptr)
        throw std::runtime_error("Aucune tuile de depart disponible.");

    std::uniform_int_distribution<> dist(0, nbTuilesDepart - 1);
    return new TuileDepart(*tuilesDepart[dist(gen)]);
}