# Cascadia

🎮 **Jeu original :** https://cascadiagame.github.io/


📖 **Règles officielles du jeu (PDF) :** https://www.alderac.com/wp-content/uploads/2021/08/Cascadia-Rules.pdf

**Branche principale : `VF-architecture`**

## Installation & Prérequis

Ce projet est en C++. Pour compiler et exécuter le projet, vous devez avoir un compilateur C++ installé.

### Avec Visual Studio

Visual Studio n'installe pas le compilateur C++ par défaut. Il faut installer la charge de travail dédiée :

1. Ouvrez le **Visual Studio Installer**
2. Sélectionnez la charge de travail **"Développement Desktop en C++"**
3. Installez / Modifiez

Cela installe le compilateur **MSVC**, les outils CMake et le débogueur nécessaires.

### Avec Visual Studio Code

⚠️ **Non recommandé pour ce projet.** VS Code n'est qu'un éditeur : il faut installer et configurer un compilateur séparément (MSVC via les Build Tools, ou MinGW-w64/MSYS2), ce qui est plus complexe à mettre en place. Préférez Visual Studio.

### Commandes de compilation

Avec **g++** (MinGW) :
```bash
g++ -std=c++17 -o cascadia main.cpp *.cpp
./cascadia
```

Avec **MSVC** (cl.exe, depuis la Developer Command Prompt) :
```bash
cl /EHsc /std:c++17 main.cpp *.cpp /Fe:cascadia.exe
cascadia.exe
```

## Répartition

- [x] Action			Jeanne
- [x] CarteMarquageFaune	Alexandre
- [x] ControleurGeneral	Jeanette
- [x] Joueur			Corentin
- [x] Partie			Jeanne
- [x] Pioche			Alexandre
- [x] TuilePlacee		Corentin
- [x] Tuile			Yan
- [x] Debbug de départ 	Yan
- [x] README Yan

## UML
![](UML.png)