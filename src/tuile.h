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
    std::vector<Animal> listeAnimaux;
    std::vector<Habitat> listeHabitat;

    bool starter = false;

    static unsigned int idCounter;
    
    // Seed fixe pour generation deterministe : choix arbitraire, mais constant
    static constexpr unsigned int BASE_SEED = 123456789u;

public:
    Tuile();
    Tuile(unsigned int nbAnimaux, unsigned int nbHabitat, const Animal* animauxArr, const Habitat* habitatsArr);
    ~Tuile() = default;

    static void resetIdCounter() { idCounter = 0; }
    void setStarter(bool b) { starter = b; }
    bool isStarterTile() const { return starter; }

    int getNbHabitat() const { return listeHabitat.size(); }
    int getNbAnimaux() const { return listeAnimaux.size(); }

    const Animal& getAnimal(unsigned int i) const { return listeAnimaux.at(i); }
    const Habitat& getHabitat(unsigned int i) const { return listeHabitat.at(i); }

    const std::vector<Animal>& getVectAnimaux() const { return listeAnimaux; }
    const std::vector<Habitat>& getVectHabitat() const { return listeHabitat; }

    unsigned int getId() const { return id; }

    bool contientAnimal(const Animal& a) const;
    void afficherTuile(std::ostream& f = std::cout) const;
    void afficherTuileHexa(std::ostream& f = std::cout) const;

    json toJson() const;
    static Tuile& fromJson(const json& j);
};



/*class Keystone : public Tuile {
public:
    Keystone();
    ~Keystone() override;
    // Donne un jeton keyStone a joueur si pose animal sur bonne case keyStone
    void associationJeton() const override; // Ou mettre joueur ?
    // on peut mettre dans ActionPlacerJeton car derniere action du tour
};*/
