#ifndef CARD_H
#define CARD_H
#include <string>

class Card
{
public:
    Card(int c, std::string n);
    Card();
    ~Card();
    int count;
    std::string name;
    bool operator==(const Card& other);
    Card& operator=(const Card& other);
};

#endif // CARD_H
