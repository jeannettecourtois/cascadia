#include "tuilePlacee.h"
#include "enum.h"


TuilePlacee::TuilePlacee():tuile(nullptr),jeton(Animal::Vide),pos(0,0),rotation(0){}

TuilePlacee::TuilePlacee(const TuilePlaceeDeplacable* t):tuile(t->getTuile()),jeton(t->getJeton()),pos(t->getPosition()),rotation(t->getRotation()){}

TuilePlacee::~TuilePlacee(){}

void TuilePlacee::deplacer(const Position& p) {
    // to do
}

void TuilePlacee::tourner(int n) {
    // to do
}

void TuilePlacee::ajouterJeton(Animal a) {
    jeton = a;
}