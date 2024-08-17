#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <string>
#include <QFileDialog>
#include <vector>
#include <algorithm>
#include "card.h"
#include <QPixmap>
using namespace std;

//TODO: Seperate names into set, name, foil, etc. using find_first_of


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
QPixmap* pic;
QLabel *cardImageLabel;
QGridLayout *cardGrid;
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

    ui->setupUi(this);
    cardTableWidget = ui->cardTableWidget;
    pic = new QPixmap("C:/Users/alecp/Documents/Qt Projects/InventoryUpdater/cmr-170-court-of-ire.jpg");
    cardImageLabel = ui->cardImageLabel;

    cardImageLabel->setMinimumHeight(MainWindow::height()*.33);
    cardImageLabel->setMinimumWidth(MainWindow::width()*.65);

    cardGrid = ui->cardGrid;

    setupCardTableWidgit();
    loadCards();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);
    int newWidth = (MainWindow::width() * 0.3) - 20;

    cardTableWidget->setMaximumWidth(MainWindow::width()*.3);
    cardTableWidget->setColumnWidth(0, newWidth*.25);
    cardTableWidget->setColumnWidth(1, newWidth*.75);

    cardImageLabel->setMinimumHeight(MainWindow::height()-100);
    cardImageLabel->setMinimumWidth(MainWindow::width()*.65);
    cardImageLabel->setPixmap(pic->scaled(cardImageLabel->width(), cardImageLabel->height(), Qt::KeepAspectRatio));

    MainWindow::setMinimumSize(0,0);
    MainWindow::setMaximumSize(16777215, 16777215);
}

void setupCardTableWidgit(){
    QStringList headers = {"Count", "Name"};
    cardTableWidget->setColumnCount(2);
    cardTableWidget->setHorizontalHeaderLabels(headers);


    cardImageLabel->setPixmap(pic->scaled(cardImageLabel->width(), cardImageLabel->height(), Qt::KeepAspectRatio));
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

    for(Card c : collectionCards){
        cout << c.getName() << endl;
        cardTableWidget->insertRow(row);
        QTableWidgetItem *countRow = new QTableWidgetItem(QString().fromStdString(to_string(c.getCount())));
        countRow->setTextAlignment(Qt::AlignHCenter);

        QTableWidgetItem *nameRow = new QTableWidgetItem(QString().fromStdString(c.getName()));

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
            outputFile << collectionCards[i].getCount() << collectionCards[i].getName() << endl;
        }
        else {
            //Check if enough cards in collection.
            int newCount = collectionCards[i].getCount() - deckListCards[index].getCount();
            if (newCount > 0) {
                outputFile << newCount << collectionCards[i].getCount() << endl;
            }
        }
    }

}

void loadFileToList() {
    string curLine;
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
    //         deckListCards[getCardIndex(tempCard)].getCount() += count;
    //     }
    //     else {
    //         Card newCard = Card(count, temp);
    //         deckListCards.push_back(newCard);
    //     }
    // }

    //Save collection to vector
    while (getline(collectionFile, curLine)) {
        int printIndex;
        string name, foil, set, temp, print;
        int count;

        size_t end = curLine.find_first_not_of("0123456789");
        temp = curLine.substr(end, curLine.length() - 1);
        name = temp.substr(0, temp.find_first_of("("));
        set = temp.substr(temp.find_first_of("("), 5);

        if(temp.find_first_of("*") != variant_npos){
            foil = temp.substr(temp.find_first_of(")") + 4, 10);
        }

        print = temp.substr(temp.find_first_of(")") + 1, 3);


        string num = curLine.substr(0, end);
        count = atoi(num.c_str());

        Card tempCard = Card(0, name, print, set, foil);

        if (alreadyExists(tempCard, collectionCards)) {
            collectionCards[getCardIndex(tempCard)].setCount(collectionCards[getCardIndex(tempCard)].getCount()+1);
        }
        else {
            //add card to vector
            Card newCard = Card(count, name, print, set, foil);
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
    //         cout << "Card" << c.getName() << " does not exist in the collection!" << endl;
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
