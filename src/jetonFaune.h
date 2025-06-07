#include "controleurGeneral.h"
#include "enum.h"

class JetonFaune {
    Animal ani;
public:
    JetonFaune(Animal a) : ani(a) {}
    ~JetonFaune() = default;
    //methodes et accesseurs en lecture 
    Animal getAnimal() const { return ani; }
}; 
