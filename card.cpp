#include "card.h"
#include <iostream>

Card::Card() {
    count = 0;
    name = "";
}

Card::~Card(){

}

Card::Card(int c, std::string n){
    count = c;
    name = n;
}

bool Card::operator==(const Card& other){

    if(name == other.name){
        std::cout << "card found" << std::endl;
        return true;
    }

    return false;
}

Card& Card::operator=(const Card& other){
    this->count = other.count;
    this->name = other.name;
    return *this;
}
