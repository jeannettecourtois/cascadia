#pragma once
#include <string>

bool askYesNo(const std::string& s);
int  askNumberOfPlayers();
// Pour eviter d'avoir une variable globale -> return "save.json";
static std::string askFilename() { return "save.json"; }
