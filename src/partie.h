#pragma once
#include<vector>
#include "action.h"
#include "partie.h"
using namespace std;


class Pioche;
class TuileDepart;
class TuilePlacee;
class Joueur;
class CarteMarquageFaune;
class Action;

// ControleurTour : gere les actions du tour de jeu
class ControleurTour {
    vector<Action*> listeActions; // Liste des actions effectuees durant le tour du joueur
public:
    ControleurTour();
    ~ControleurTour();
    void executerAction(Action* a);
    void annulerDerniereAction();
    void afficherActions() const;
    vector<Action*> getListeActions() { return listeActions; }
};

// Partie : gere la partie avec plusieurs joueurs
class Partie {
private:
    static Partie* instance;

    CarteMarquageFaune* cartesRegles[5];
    int nbJoueur;
    vector<Joueur*> joueurs;
    Pioche* pioche;
    ControleurTour* ctrlTour;
    int nbTour = 20;
    int joueurCourant = 0;

    string phase = "SELECTION_TUILE";;

    Partie(int nbJoueurs); // constructeur prive (singleton)
    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

public:
    ~Partie();

    // Singleton
    static Partie& getInstance(int nbJoueurs = 2);
    static void libererInstance();

    // Getters
    int getNbJoueur() const { return nbJoueur; }
    int getNbTour() const { return nbTour; }
    Joueur* getJoueur(int i) const { return joueurs[i]; }
    vector<Joueur*> getJoueurs() const { return joueurs; }
    Pioche* getPioche() const { return pioche; }
    int getJoueurCourant() const { return joueurCourant; }
    CarteMarquageFaune* getCarteRegle(int i) const { return cartesRegles[i]; }
    ControleurTour* getControleurTour() const { return ctrlTour; }

    // etat de la partie
    bool estFini() const;
    Joueur* getGagnant() const;
    string getPhase() const { return phase; }

    // Setters pour charger partie
    void ajouterJoueur(Joueur* j) {joueurs.push_back(j);}
    void setCarteFaune(int index, CarteMarquageFaune* carte);
    void setNbTour(int tour) { nbTour = tour; }
    void setJoueurCourant(int courant) { joueurCourant = courant; }
    void setPhase(const string& s) { phase = s; }



    // Moteur de jeu
    void initialiserPartie();
    void initialiserCartesRegles();
    void jouerTour();
    void passerAuJoueurSuivant();
};