#include <iostream>
//Usage des coordonnées axiales pour les positions dans un jeu de stratégie hexagonal
// (q, r) pour les coordonnées axiales
using namespace std; 

class Position {
private: 
	int col; //correspond à q
	int row; // correspond à r
public: 
	Position(int c = 0, int r = 0) : col(c), row(r) {}
	//accesseurs de lecture 
	int getCol() const { return col; }
	int getRow() const { return row; }
	void set(int c, int r) {
		c = col; 
		r = row;
	}
	//accesseurs de modification
	//Déplacements(6 directions d’un hexagone)
	Position getEst() const { return Position(col + 1, row); }
	Position getNE() const { return Position(col + 1, row - 1); }
	Position getNO() const { return Position(col, row - 1); }
	Position getOuest() const { return Position(col - 1, row); }
	Position getSO() const { return Position(col - 1, row + 1); }
	Position getSE() const { return Position(col, row + 1); }
	//distance entre deux Tuiles 
	int distanceTo(const Position& other) const {
		int dq = col - other.col;
		int dr = row - other.row;
		return (abs(dq) + abs(dq + dr) + abs(dr)) / 2;
	}
	bool operator==(const Position& other);
	void afficher(std::ostream& f = std::cout);
}; 


