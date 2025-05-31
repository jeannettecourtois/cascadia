#pragma once
#include <iostream>
#include <string>
#include "tuile.h"

class Position {
public:
    int x;
    int y;
    Position(int a = 0, const int b = 0) : x(a), y(b) {}
    Position(const Position& pos) : x(pos.x), y(pos.y) {}
    ~Position() = default;

    void SetPosition(int a, int b) { x = a; y = b; }
    void SetPosition(const Position& pos) { x = pos.x; y = pos.y; }

    Position getE() const { return Position(x + 1, y); }
    Position getSE() const { return Position(x, y + 1); }
    Position getSW() const { return Position(x - 1, y + 1); }
    Position getW() const { return Position(x - 1, y); }
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
    TuilePlacee() : tuile(nullptr), jeton(Animal::Vide), pos(0, 0), rotation(0) {}

    TuilePlacee(const Tuile* tuile, Animal a, const Position& p, int rot = 0)
        : tuile(tuile), jeton(a), pos(p), rotation(rot) {
    }

    TuilePlacee(const Tuile* tuile, Animal a)
        : tuile(tuile), jeton(a), pos(0, 0), rotation(0) {
    }

    TuilePlacee(const Tuile* tuile)
        : tuile(tuile), jeton(Animal::Vide), pos(0, 0), rotation(0) {
    }

    virtual ~TuilePlacee() = default;

    const Tuile* getTuile() const { return tuile; }
    Animal getJeton() const { return jeton; }
    virtual Position getPosition() const { return pos; }
    virtual int getRotation() const { return rotation; }

    virtual void deplacer(const Position& p) {}
    virtual void tourner(const int n) {}

    void ajouterJeton(const Animal a) { jeton = a; }

    void afficherTuilePlacee(std::ostream& f = std::cout) const {
        if (!tuile) {
            f << "Aucune tuile placée.\n";
            return;
        }

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

    TuilePlaceeDeplacable(const Tuile* tuile, Animal a, const Position& p, int rot = 0)
        : TuilePlacee(tuile, a, p, rot) {
    }

    ~TuilePlaceeDeplacable() = default;

    void deplacer(const Position& p2) override { pos.SetPosition(p2); }
    void tourner(int n) override { rotation = (rotation + n) % 6; }

    Position getPosition() const override { return pos; }
    int getRotation() const override { return rotation; }
};
