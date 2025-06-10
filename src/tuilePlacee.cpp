#include "tuilePlacee.h"
#include "controleurGeneral.h"
#include "enum.h"

json TuilePlacee::toJson() const {
    //!!! DEBUG
    verifierJeton("toJson()");
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
    // Tuile recuperer de CG par l'id
    unsigned int id = j.at("tuile").at("id");
    const Tuile* t = ControleurGeneral::getInstance().getTuileById(id);
    if (t == nullptr) {
        std::cerr << "[ERREUR] TuilePlacee::fromJson : pas de Tuile avec id=" << id << "\n";
    }


    // Jeton
    std::string jetonStr = j["jeton"];
    std::cout << "[DEBUG] jeton string recu = '" << jetonStr << "'\n";
    Animal jt = fromStringAnimal(j["jeton"]);
    // Garde-fou en attendant de trouver l'erreur
    if (!j.contains("jeton") || !j["jeton"].is_string()) {
        std::cerr << "[ERREUR] Champ 'jeton' manquant ou invalide dans TuilePlacee JSON.\n";
        jt = Animal::Vide;
    }
    else {
        try { // Si fromStringAnimal lance ou renvoie une valeur invalide, capturer/traiter.
            jt = fromStringAnimal(j["jeton"]);
        }
        catch (...) {
            std::cerr << "[ERREUR] TuilePlacee::fromJson : format 'jeton' invalide: " << j["jeton"] << "\n";
            jt = Animal::Vide;
        }
    }    

    // Position
    Position p(j["position"]["x"], j["position"]["y"]);

    // Rotation
    int r = j.at("rotation");

    return TuilePlacee(t, jt, p, r);
}