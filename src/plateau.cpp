#include "plateau.h"
#include "position.h"
#include "tuile.h"
#include "action.h"

PlateauJoueur::PlateauJoueur() {
    // Créer 40 positions en cercle autour de (0, 0) dans un motif hexagonal
    int rayon = 4;
    for (int q = -rayon; q <= rayon; ++q) {
        for (int r = std::max(-rayon, -q - rayon); r <= std::min(rayon, -q + rayon); ++r) {
            positionsDisponibles.emplace_back(q, r);
            if (positionsDisponibles.size() >= maxTuiles) return;
        }
    }
}

void PlateauJoueur:: ajouterTuilePlacee(Tuile* tuile, const Position& pos) {
    tuilesPlacees.emplace_back(tuile, pos);
}

void PlateauJoueur::afficherPositionsDisponibles() const {
	for (const auto& position : positionsDisponibles) {
		std::cout << "Position disponible: (" << position.getCol() << ", " << position.getRow()<< ")\n";
	}
}
