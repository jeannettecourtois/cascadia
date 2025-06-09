#include "tuilePlacee.h"
#include "controleurGeneral.h"
#include "enum.h"

json TuilePlacee::toJson() const {
    if (static_cast<int>(jeton) < 0 || static_cast<int>(jeton) > 5) {
        std::cerr << "[BUG] jeton invalide dans TuilePlacee::toJson: " << static_cast<int>(jeton)
            << " (pos " << pos.x << "," << pos.y << ")\n";
    }
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
    // Garde-fou en attendant de trouver l'erreur
    if (!j.contains("jeton") || !j["jeton"].is_string()) {
        std::cerr << "[ERREUR] Champ 'jeton' manquant ou invalide dans TuilePlacee JSON.\n";
        jt = Animal::Vide;
    }
    else {
        jt = fromStringAnimal(j["jeton"]);
    }    

    // Position
    Position p(j["position"]["x"], j["position"]["y"]);

    // Rotation
    int r = j.at("rotation");

    return TuilePlacee(t, jt, p, r);
}