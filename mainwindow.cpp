#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <string>
#include <QFileDialog>
#include <vector>
#include <algorithm>
#include "card.h"
using namespace std;

void loadFileToList();
bool checkAgainstCollection();
void writeOutput();
int getCardIndex(Card searchTerm);
bool alreadyExists(Card searchTerm, vector<Card> list);
void loadCardsToColumn();
void loadCards();
void setupCardTableWidgit();

ifstream deckListFile, collectionFile;
ofstream outputFile;

int row;

vector<Card> deckListCards = vector<Card>();
vector<Card> collectionCards = vector<Card>();

QTableWidget *cardTableWidget;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    cout << "init" << endl;

    // QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Chose Deck List"), "C://", "Text Files (*.txt)");

    // if(!filenames.isEmpty()){
    //     std::string current_location_text= filenames[0].toLocal8Bit().constData();
    //     deckListFile = ifstream(current_location_text.c_str());

    //     string curLine;
    //     if(deckListFile){
    //         // while(getline(deckListFile, curLine)){
    //         //     cout << curLine << endl;
    //         // }
    //     }
    //     else{
    //         cout << "File could not be opened." << endl;
    //     }
    // }
    // else{
    //     cout << "File not selected" << endl;
    // }

    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Chose Collection List"), "C://", "Text Files (*.txt)");

    if(!filenames.isEmpty()){
        std::string current_location_text= filenames[0].toLocal8Bit().constData();
        collectionFile = ifstream(current_location_text.c_str());

        string curLine;
        if(collectionFile){
            // while(getline(collectionFile, curLine)){
            //     cout << curLine << endl;
            // }
        }
        else{
            cout << "File could not be opened." << endl;
        }
    }
    else{
        cout << "File not selected" << endl;
    }

    //cardListColumn;

    ui->setupUi(this);
    cardTableWidget = ui->cardTableWidget;

    setupCardTableWidgit();
    loadCards();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void setupCardTableWidgit(){
    QStringList headers = {"Number", "Name"};
    cardTableWidget->setColumnCount(2);
    cardTableWidget->setHorizontalHeaderLabels(headers);
    cardTableWidget->setColumnWidth(0, 50);
    cardTableWidget->setColumnWidth(1, 130);
}

void loadCards(){

    //if (deckListFile.is_open()) {
        if (collectionFile.is_open()) {
            loadFileToList();
            if (checkAgainstCollection()) {
                //writeOutput();
            }
            loadCardsToColumn();
        }
    //}

}

void loadCardsToColumn(){
    row = 0;
    for(Card c: collectionCards){
        cardTableWidget->insertRow(row);
        QTableWidgetItem *countRow = new QTableWidgetItem(QString().fromStdString(to_string(c.count)));
        countRow->setTextAlignment(Qt::AlignHCenter);

        QTableWidgetItem *nameRow = new QTableWidgetItem(QString().fromStdString(c.name));

        cardTableWidget->setItem(row, 0, countRow);
        cardTableWidget->setItem(row, 1, nameRow);
        row++;
    }
}

void writeOutput() {
    //open output file
    outputFile = ofstream("out.txt");

    //compare files
    for (int i = 0; i < collectionCards.size(); i++) {
        int index = getCardIndex(collectionCards[i]);
        if (index == -1) {
            outputFile << collectionCards[i].count << collectionCards[i].name << endl;
        }
        else {
            //Check if enough cards in collection.
            int newCount = collectionCards[i].count - deckListCards[index].count;
            if (newCount > 0) {
                outputFile << newCount << collectionCards[i].count << endl;
            }
        }
    }

}

void loadFileToList() {
    string curLine, temp;
    int count;
    int x = 0;

    //Save deck list to vector
    // while (getline(deckListFile, curLine)) {
    //     size_t end = curLine.find_first_not_of("0123456789");
    //     temp = curLine.substr(end, curLine.size() - 1);

    //     string num = curLine.substr(0, end);
    //     count = atoi(num.c_str());

    //     Card tempCard = Card(0, temp);

    //     //cout << count << endl << temp << endl;
    //     if (alreadyExists(tempCard, deckListCards)) {
    //         deckListCards[getCardIndex(tempCard)].count += count;
    //     }
    //     else {
    //         Card newCard = Card(count, temp);
    //         deckListCards.push_back(newCard);
    //     }
    // }

    //Save collection to vector
    while (getline(collectionFile, curLine)) {
        size_t end = curLine.find_first_not_of("0123456789");
        temp = curLine.substr(end, curLine.size() - 1);

        string num = curLine.substr(0, end);
        count = atoi(num.c_str());
        Card tempCard = Card(0, temp);

        //cout << count << endl << temp << endl;
        if (alreadyExists(tempCard, collectionCards)) {
            collectionCards[getCardIndex(tempCard)].count += count;
        }
        else {
            //add card to vector
            Card newCard = Card(count, temp);
            collectionCards.push_back(newCard);
        }
    }

    // deckListFile.close();
    collectionFile.close();

}

bool checkAgainstCollection() {
    // for (int i = 0; i < collectionCards.size(); i++) {
    //     Card c = deckListCards[i];
    //     if (find(collectionCards.begin(), collectionCards.end(), c) == collectionCards.end()) {
    //         cout << "Card" << c.name << " does not exist in the collection!" << endl;
    //         return false;
    //     }
    // }
    return true;
}

int getCardIndex(Card searchTerm) {
    auto pos = find(deckListCards.begin(), deckListCards.end(), searchTerm);

    if (pos != deckListCards.end()) {
        return pos - deckListCards.begin();
    }
    else {
        return -1;
    }
}

//For cleanliness
bool alreadyExists(Card searchTerm, vector<Card> list) {
    if (find(list.begin(), list.end(), searchTerm) != list.end()) {
        return true;
    }
    return false;
}
