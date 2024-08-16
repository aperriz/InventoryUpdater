#include "card.h"

Card::Card() {
    count = 0;
    name = "";
    printCode = "";
    setCode = "";
    foil = "";
}

Card::~Card(){

}

Card::Card(int c, std::string n, std::string pc, std::string set, std::string foil){
    count = c;
    name = n;
    printCode = pc;
    setCode = set;
    this->foil = foil;
}

bool Card::operator==(const Card& other){

    if(name == other.name){
        //std::cout << "card found" << std::endl;
        return true;
    }
    return false;
}

Card& Card::operator=(const Card& other){
    this->count = other.count;
    this->name = other.name;
    return *this;
}

std::string Card::getPrintCode(){
    return printCode;
}

int Card::getCount(){
    return count;
}

std::string Card::getName(){
    return name;
}

std::string Card::getSet(){
    return setCode;
}

std::string Card::getFoil(){
    return foil;
}

void Card::setCount(int c){
    count = c;
}

Card Card::copy(){
    return Card(count, name, printCode, setCode, foil);
}
