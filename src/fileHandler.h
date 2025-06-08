#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "partie.h"

using json = nlohmann::json;

using namespace std;

static json toJsonPartie(const Partie& partie);
static void fromJsonPartie(const nlohmann::json& j, Partie& partie);

class FileHandler {
public:
    bool loadGame(const string& filename);
    bool saveGame(const string& filename);
};
