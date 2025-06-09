#include "tuilePlacee.h"
#include "controleurGeneral.h"
#include "enum.h"

json TuilePlacee::toJson() const {
    json j;
    j["tuile"] = { {"id", tuile->getId()} };
    j["position"] = { {"x", this->getX()}, {"y", this->getY()} };
    j["rotation"] = rotation;
    j["jeton"] = toString(jeton);
    return j;
}

TuilePlacee TuilePlacee::fromJson(const json& j) {
    // Tuile récupérer de CG par l'id
    unsigned int id = j.at("tuile").at("id");
    const Tuile* t = ControleurGeneral::getInstance().getTuileById(id);

    // Jeton
    Animal jt = fromStringAnimal(j["jeton"]);

    // Position
    Position p(j["position"]["x"], j["position"]["y"]);

    // Rotation
    int r = j.at("rotation");

    return TuilePlacee(t, jt, p, r);
}