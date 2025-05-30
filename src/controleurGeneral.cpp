#include "controleurGeneral.h"
#include "enum.h"
#include "affichable.h"
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

ControleurGeneral::ControleurGeneral(): nbTuilesHabitat(85), nbJetonFaune(100), nbJetonsNature(25), nbCartesMarquageFaune(21), gen(rd()), distTuiles(0, 84), distJetons(0, 99), distCartes(0, 15){
    //construction des jetons Faune
    //20 Ours 
    for(unsigned int i=0; i<20; ++i){ tabJetons[i] = new JetonFaune(Animal::Ours);}
    //20 Cerfs
    for(unsigned int i=20; i<40; ++i){ tabJetons[i] = new JetonFaune(Animal::Cerf);}
    //20 Saumons
    for(unsigned int i=40; i<60; ++i){ tabJetons[i] = new JetonFaune(Animal::Saumon);}
    //20 Aigles
    for(unsigned int i=60; i<80; ++i){ tabJetons[i] = new JetonFaune(Animal::Aigle);}
    //20 Renards 
    for(unsigned int i=80; i<100; ++i){ tabJetons[i] = new JetonFaune(Animal::Renard);}

    //construction des Tuiles 
    for(unsigned int i = 0; i <85; ++i){ tabTuiles[i] = new Tuile();}

    //constructeur de Cartes de Marquage Faune 
    for(unsigned int i = 0; i<3; ++i){tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Aigle);}
    for(unsigned int i = 3; i<6; ++i){tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Cerf);}
    for(unsigned int i = 6; i<9; ++i){tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Ours);}
    for(unsigned int i = 9; i<12; ++i){tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Renard);}
    for(unsigned int i = 12; i<15; ++i){tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Saumon);}

}



ControleurGeneral::~ControleurGeneral() {
    for(unsigned int i = 0; i<100; ++i){
        delete tabJetons[i];
    }

    for(unsigned int i = 0; i<85; ++i){
        delete tabTuiles[i];
    }

    for(unsigned int i = 0; i<15; ++i){
        delete tabCartesMarquage[i];
    }
}


// Env Test
namespace SINGLETON {
    ControleurGeneral::ControleurGeneral() : nbTuilesHabitat(85), nbJetonFaune(100), nbJetonsNature(25), nbCartesMarquageFaune(21), gen(rd()), distTuiles(0, 84), distJetons(0, 99), distCartes(0, 14) {
        //construction des jetons Faune
        //20 Ours 
        for (unsigned int i = 0; i < 20; ++i) { tabJetons[i] = new JetonFaune(Animal::Ours); }
        //20 Cerfs
        for (unsigned int i = 20; i < 40; ++i) { tabJetons[i] = new JetonFaune(Animal::Cerf); }
        //20 Saumons
        for (unsigned int i = 40; i < 60; ++i) { tabJetons[i] = new JetonFaune(Animal::Saumon); }
        //20 Aigles
        for (unsigned int i = 60; i < 80; ++i) { tabJetons[i] = new JetonFaune(Animal::Aigle); }
        //20 Renards 
        for (unsigned int i = 80; i < 100; ++i) { tabJetons[i] = new JetonFaune(Animal::Renard); }

        //construction des Tuiles 
        for (unsigned int i = 0; i < 85; ++i) { tabTuiles[i] = new Tuile(); }

        //constructeur de Cartes de Marquage Faune 
        for (unsigned int i = 0; i < 3; ++i) { tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Aigle); }
        for (unsigned int i = 3; i < 6; ++i) { tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Cerf); }
        for (unsigned int i = 6; i < 9; ++i) { tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Ours); }
        for (unsigned int i = 9; i < 12; ++i) { tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Renard); }
        for (unsigned int i = 12; i < 15; ++i) { tabCartesMarquage[i] = new CarteMarquageFaune(Animal::Saumon); }

    }



    ControleurGeneral::~ControleurGeneral() {
        for (unsigned int i = 0; i < 100; ++i) {
            delete tabJetons[i];
        }

        for (unsigned int i = 0; i < 85; ++i) {
            delete tabTuiles[i];
        }

        for (unsigned int i = 0; i < 15; ++i) {
            delete tabCartesMarquage[i];
        }
    }
}