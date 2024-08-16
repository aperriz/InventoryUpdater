#ifndef CARD_H
#define CARD_H
#include <string>

class Card
{
private:
    int count;
    std::string name, setCode, foil, printCode;
public:
    Card(int c, std::string n, std::string pc, std::string set, std::string foil);
    Card();
    ~Card();
    Card copy();
    bool operator==(const Card& other);
    Card& operator=(const Card& other);
    int getCount();
    std::string getPrintCode();
    std::string getName();
    std::string getSet();
    std::string getFoil();
    void setCount(int i);
};

#endif // CARD_H
