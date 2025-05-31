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
    for (unsigned int i = 0; i < 85; ++i) tabTuiles[i] = new Tuile();

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

/*CarteMarquageFaune* ControleurGeneral::getCarteMarquageFaune() {
    return tabCartesMarquage[distCartes(gen)];
}*/

CarteMarquageFaune* ControleurGeneral::getCarteMarquageParAnimalAleatoire(Animal a) {
    int debut = 0;
    switch (a) {
        case Animal::Aigle:   debut = 0; break;
        case Animal::Cerf:   debut = 3; break;
        case Animal::Ours:   debut = 6; break;
        case Animal::Renard: debut = 9; break;
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
        Tuile* t1 = new Tuile(1, 1, a1, h1);

        Animal a2[2] = { Animal::Ours, Animal::Renard };
        Habitat h2[2] = { Habitat::Montagne, Habitat::Prairie };
        Tuile* t2 = new Tuile(2, 2, a2, h2);

        Animal a3[3] = { Animal::Saumon, Animal::Cerf, Animal::Aigle };
        Habitat h3[2] = { Habitat::Fleuve, Habitat::Foret };
        Tuile* t3 = new Tuile(3, 2, a3, h3);

        tuilesDepart[0] = new TuileDepart();
        tuilesDepart[0]->setTuile(0, new TuilePlacee(t1));
        tuilesDepart[0]->setTuile(1, new TuilePlacee(t2));
        tuilesDepart[0]->setTuile(2, new TuilePlacee(t3));
    }

    // TuileDepart 1
    {
        Animal a1[1] = { Animal::Ours };
        Habitat h1[1] = { Habitat::Montagne };
        Tuile* t1 = new Tuile(1, 1, a1, h1);

        Animal a2[2] = { Animal::Ours, Animal::Saumon };
        Habitat h2[2] = { Habitat::Prairie, Habitat::Fleuve };
        Tuile* t2 = new Tuile(2, 2, a2, h2);

        Animal a3[3] = { Animal::Cerf, Animal::Aigle, Animal::Renard };
        Habitat h3[2] = { Habitat::Marais, Habitat::Foret };
        Tuile* t3 = new Tuile(3, 2, a3, h3);

        tuilesDepart[1] = new TuileDepart();
        tuilesDepart[1]->setTuile(0, new TuilePlacee(t1));
        tuilesDepart[1]->setTuile(1, new TuilePlacee(t2));
        tuilesDepart[1]->setTuile(2, new TuilePlacee(t3));
    }

    // TuileDepart 2
    {
        Animal a1[1] = { Animal::Cerf };
        Habitat h1[1] = { Habitat::Foret };
        Tuile* t1 = new Tuile(1, 1, a1, h1);

        Animal a2[2] = { Animal::Renard, Animal::Saumon };
        Habitat h2[2] = { Habitat::Prairie, Habitat::Marais };
        Tuile* t2 = new Tuile(2, 2, a2, h2);

        Animal a3[3] = { Animal::Aigle, Animal::Cerf, Animal::Ours };
        Habitat h3[2] = { Habitat::Montagne, Habitat::Fleuve };
        Tuile* t3 = new Tuile(3, 2, a3, h3);

        tuilesDepart[2] = new TuileDepart();
        tuilesDepart[2]->setTuile(0, new TuilePlacee(t1));
        tuilesDepart[2]->setTuile(1, new TuilePlacee(t2));
        tuilesDepart[2]->setTuile(2, new TuilePlacee(t3));
    }

    // TuileDepart 3
    {
        Animal a1[1] = { Animal::Saumon };
        Habitat h1[1] = { Habitat::Fleuve };
        Tuile* t1 = new Tuile(1, 1, a1, h1);

        Animal a2[2] = { Animal::Aigle, Animal::Renard };
        Habitat h2[2] = { Habitat::Montagne, Habitat::Fleuve };
        Tuile* t2 = new Tuile(2, 2, a2, h2);

        Animal a3[3] = { Animal::Ours, Animal::Cerf, Animal::Saumon };
        Habitat h3[2] = { Habitat::Foret, Habitat::Prairie };
        Tuile* t3 = new Tuile(3, 2, a3, h3);

        tuilesDepart[3] = new TuileDepart();
        tuilesDepart[3]->setTuile(0, new TuilePlacee(t1));
        tuilesDepart[3]->setTuile(1, new TuilePlacee(t2));
        tuilesDepart[3]->setTuile(2, new TuilePlacee(t3));
    }

    // TuileDepart 4
    {
        Animal a1[1] = { Animal::Renard };
        Habitat h1[1] = { Habitat::Prairie };
        Tuile* t1 = new Tuile(1, 1, a1, h1);

        Animal a2[2] = { Animal::Ours, Animal::Cerf };
        Habitat h2[2] = { Habitat::Montagne, Habitat::Foret };
        Tuile* t2 = new Tuile(2, 2, a2, h2);

        Animal a3[3] = { Animal::Aigle, Animal::Saumon, Animal::Renard };
        Habitat h3[2] = { Habitat::Fleuve, Habitat::Marais };
        Tuile* t3 = new Tuile(3, 2, a3, h3);

        tuilesDepart[4] = new TuileDepart();
        tuilesDepart[4]->setTuile(0, new TuilePlacee(t1));
        tuilesDepart[4]->setTuile(1, new TuilePlacee(t2));
        tuilesDepart[4]->setTuile(2, new TuilePlacee(t3));
    }
}

TuileDepart* ControleurGeneral::getTuileDepartAleatoire() {
    if (nbTuilesDepart == 0 || tuilesDepart == nullptr)
        throw std::runtime_error("Aucune tuile de depart disponible.");

    std::uniform_int_distribution<> dist(0, nbTuilesDepart - 1);
    return new TuileDepart(*tuilesDepart[dist(gen)]);
}