#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

using namespace std;

class FileHandler {
public:
    bool loadGame(const string& filename);
    bool saveGame(const string& filename);
    bool saveToFile(const string& filename);
    bool loadFromFile(const string& filename);
};
