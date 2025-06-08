#include "position.h"




bool Position::operator==(const Position& other){
	return (this->col == other.col && this->row == other.row);
}

void Position::afficher(std::ostream& f) {
	f << "La position est: \n";
	f << "Colonne q = : " << col << "\n";
	f << "Ligne: r = " << row << "\n";
}