#pragma once
#include <iostream>
#include <string>
#include "tuile.h"

class TuilePlaceeDeplacable; // declaration anticipee pour TuilePlacee

class Position {
public:
    int x;
    int y;
    Position(int a=0, const int b=0):x(a),y(b){}
    Position(const Position& pos):x(pos.x),y(pos.y){}
    ~Position() = default;
    void SetPosition(int a, int b){x=a; y=b;}
    void SetPosition(const Position& pos){x=pos.x; y=pos.y;}
    Position getE() const  { return Position(x + 1, y); }
    Position getSE() const { return Position(x, y + 1); }
    Position getSW() const { return Position(x - 1, y + 1); }
    Position getW() const  { return Position(x - 1, y); }
    Position getNW() const { return Position(x, y - 1); }
    Position getNE() const { return Position(x + 1, y - 1); }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

class TuilePlacee {
private:
    const Tuile* tuile;
    Animal jeton;
protected:
    Position pos;
    int rotation;
public:
    TuilePlacee();
    TuilePlacee(const TuilePlaceeDeplacable* t);
    TuilePlacee(const Tuile* tuile, Animal a) : tuile(tuile), jeton(a), rotation(0) {}
    TuilePlacee(const Tuile* tuile) : tuile(tuile), jeton(Animal::Vide), rotation(0) {}

    ~TuilePlacee();

    const Tuile* getTuile() const {return tuile;}
    Animal getJeton() const {return jeton;}
    virtual void deplacer(const Position& p);
    virtual void tourner(const int n);
    void ajouterJeton(const Animal a);
    Position getPosition() const { return pos; }
    void afficherTuilePlacee(std::ostream& f = std::cout) const {
        f << "Tuile avec " << tuile->getNbAnimaux() << " animaux et " << tuile->getNbHabitat() << " habitats.\n";
        f << "Animaux : ";
        for (unsigned int i = 0; i < tuile->getNbAnimaux(); ++i) {
            f << tuile->getAnimal(i) << " ";
        }
        f << "\nHabitats : ";
        for (unsigned int i = 0; i < tuile->getNbHabitat(); ++i) {
            f << tuile->getHabitat(i) << " ";
        }
        f << std::endl;
    }
};


class TuilePlaceeDeplacable : public TuilePlacee {
public:
    TuilePlaceeDeplacable() = default;
    ~TuilePlaceeDeplacable() = default;

    void deplacer(const Position& p2) override {pos.SetPosition(p2);}
    void tourner(int n) override {rotation = (rotation + n) % 6;}
    Position getPosition() const {return pos;}
    int getRotation() const {return rotation;}
};