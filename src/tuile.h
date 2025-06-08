#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "enum.h"
#include <random>
#include "json.hpp"

using json = nlohmann::json;

class Tuile {
private:
    unsigned int id;
    unsigned int nbAnimaux;
    unsigned int nbHabitat;
    std::vector<Animal*> listeAnimaux;
    std::vector<Habitat*> listeHabitat;

    // Pour autoincremente ID de chaque tuile (static donc partage entre toutes les instances)
    static unsigned int idCounter;

public:
    Tuile();
    // Constructeur pour creer une tuile avec des animaux et habitats donnes
    //Utilise pour les tuiles de depart
    Tuile(unsigned int nbAnimaux, unsigned int nbHabitat, const Animal* animauxArr, const Habitat* habitatsArr);

    ~Tuile();

    int getNbHabitat() const { return nbHabitat; }
    int getNbAnimaux() const { return nbAnimaux; }
    Animal* getAnimal(unsigned int i) const { return listeAnimaux[i]; }
    Habitat* getHabitat(unsigned int i) const { return listeHabitat[i]; }
    // Renvoie un pointeur vers le premier element du vecteur (equivalent à l'ancien tableau)
    Animal** getListeAnimaux() const { return const_cast<Animal**>(listeAnimaux.data()); }
    Habitat** getListeHabitat() const { return const_cast<Habitat**>(listeHabitat.data()); }

    bool contientAnimal(const Animal& a) const;
    void afficherTuile(std::ostream& f = std::cout) const; // Affiche la tuile avec ses animaux et habitats
    void afficherTuileHexa(std::ostream& f = std::cout) const; // Affiche la tuile en format hexagonal
    // Pour Sauvegarder
    json toJson() const;
    static Tuile fromJson(const json& j);

};

/*class Keystone : public Tuile {
public:
    Keystone();
    ~Keystone() override;
    // Donne un jeton keyStone a joueur si pose animal sur bonne case keyStone
    void associationJeton() const override; // Où mettre joueur ?
    // on peut mettre dans ActionPlacerJeton car derniere action du tour
};*/
