#include "carteMarquageFaune.h"
#include "joueur.h"

CarteMarquageFaune::CarteMarquageFaune(const Animal& a) : animalCarte(a), id(idCounter++) {}
CarteMarquageFaune::~CarteMarquageFaune() {}

unsigned int CarteMarquageFaune::idCounter = 0;

int CarteMarquageFaune::calculPoints(PlateauJoueur&) {
    return 0; 
}


CarteMarquageFauneParMasque::CarteMarquageFauneParMasque(const Animal& a):CarteMarquageFaune(a) {}
CarteMarquageFauneParMasque::~CarteMarquageFauneParMasque() {}

int CarteMarquageFauneParMasque::calculPoints(PlateauJoueur&) {
    return 0; 
}


CarteMarquageFauneParDiversiteDeType::CarteMarquageFauneParDiversiteDeType(const Animal& a) : CarteMarquageFaune(a) {}
CarteMarquageFauneParDiversiteDeType::~CarteMarquageFauneParDiversiteDeType() {}

int CarteMarquageFauneParDiversiteDeType::calculPoints(PlateauJoueur&) {
    return 0; 
}

json CarteMarquageFaune::toJson() const {
    return {
        {"id", id},
        {"animal", toString(animalCarte)},
    };
}
