#pragma once
#include<vector>
#include "action.h"
#include "partie.h"
using namespace std;


class Pioche;
class ControleurTour;
class TuileDepart;
class TuilePlacee;
class Joueur;
class CarteMarquageFaune;
class Action;




class ControleurTour {
    vector<Action*> listeActions;
public:
    ControleurTour();
    ~ControleurTour();
    void executerAction(Action* a);
    void annulerDerniereAction();
    void afficherActions() const;
};


class Partie {
private:
    CarteMarquageFaune* cartesRegles[5];
    int nbJoueur;
    Joueur** joueurs;
    Pioche* pioche;
    ControleurTour* ctrlTour;
    int nbTour = 20;
    int joueurCourant=0;
public:
    Partie(int nbJoueurs);
    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;
    ~Partie();

    int getNbJoueur() const {return nbJoueur;}
    int getNbTour() const {return nbTour;}
    Joueur* getJoueur(int i) const {return joueurs[i];}
        Pioche* getPioche() const { return pioche; }
    int getJoueurCourant() const { return joueurCourant; }
    CarteMarquageFaune* getCarteRegle(int i) const { return cartesRegles[i]; }

    bool estFini() const;
    Joueur* getGagnant() const;
    void initialiserPartie();
    void initialiserCartesRegles();
    void jouerTour();
    void passerAuJoueurSuivant();
};