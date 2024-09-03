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
#include "ImageFetcher.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
using namespace std;

//TODO: Seperate names into set, name, foil, etc. using find_first_of

void loadFileToList();
bool checkAgainstCollection();
void writeOutput();
int getCardIndex(Card *searchTerm);
bool alreadyExists(Card *searchTerm, vector<Card*> list);
void loadCardsToColumn();
void loadCards();
void setupCardTableWidgit();
bool loadSavedSettings();
void firstTimeSetup();
void firstLoadedScreen();

ifstream deckListFile, collectionFile;
ofstream outputFile;
QStringList filenames;
int row, activeRow;
string* settingsArray = new string[10];
QString foilString;
Card curC;


vector<Card*> deckListCards = vector<Card*>();
vector<Card*> collectionCards = vector<Card*>();
QPixmap* pic;
QLabel *cardImageLabel, *countLabel, *foilLabel, *nameLabel, *printCodeLabel, *setCodeLabel;
QGridLayout *cardGrid;
QTableWidget *cardTableWidget;
MainWindow* mainWindow;
QPushButton* decreaseCountButton, *increaseCountButton;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    cout << "init" << endl;

    mainWindow = this;
    if(loadSavedSettings()){
        filenames.append(QString().fromStdString(settingsArray[0]));

        std::string current_location_text = filenames[0].toLocal8Bit().constData();
        collectionFile = ifstream(current_location_text.c_str());

        string curLine;
        if(!collectionFile){
            cout << "File could not be opened." << endl;
            firstTimeSetup();
        }

    }
    else{
        firstTimeSetup();
    }

    ui->setupUi(this);
    cardTableWidget = ui->cardTableWidget;

    cardImageLabel = ui->cardImageLabel;

    cardImageLabel->setMinimumHeight(MainWindow::height()*.33);
    cardImageLabel->setMinimumWidth(MainWindow::width()*.65);

    cardGrid = ui->cardGrid;
    countLabel = ui->countLabel;
    foilLabel = ui->foilLabel;
    nameLabel = ui->nameLabel;
    printCodeLabel = ui->printCodeLabel;
    setCodeLabel = ui->setCodeLabel;
    decreaseCountButton = ui->decreaseCountButton;
    increaseCountButton = ui->increaseCountButton;

    manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("https://www.google.com")));
    setupCardTableWidgit();
    loadCards();

}

void MainWindow::showEvent(QShowEvent *event){
    QMainWindow::showEvent(event);

    firstLoadedScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void firstLoadedScreen(){
    countLabel->setText(QString(""));
    foilLabel->setText(QString(""));
    printCodeLabel->setText(QString(""));
    setCodeLabel->setText(QString(""));
    nameLabel->setText(QString(""));

    cardImageLabel->setMinimumHeight(mainWindow->height()-200);
    cardImageLabel->setMinimumWidth(mainWindow->width()*.65);
    cardImageLabel->setText("");

    decreaseCountButton->setVisible(false);
    increaseCountButton->setVisible(false);

    cardImageLabel->setVisible(false);
}

void firstTimeSetup(){
    filenames = QFileDialog::getOpenFileNames(mainWindow, mainWindow->tr("Chose Collection List"), "C://", "Text Files (*.txt)");
    ofstream newSettingsFile = ofstream("settings.txt");

    if(!filenames.isEmpty()){
        std::string current_location_text= filenames[0].toLocal8Bit().constData();
        collectionFile = ifstream(current_location_text.c_str());

        newSettingsFile << current_location_text << endl;

        string curLine;
        if(!collectionFile){
            cout << "File could not be opened." << endl;
        }
    }
    else{
        cout << "File not selected" << endl;
    }

}

void resizeImage(){

    cardImageLabel->setMinimumHeight(mainWindow->height()-200);
    cardImageLabel->setMinimumWidth(mainWindow->width()*.65);

    cardImageLabel->setMaximumHeight(mainWindow->height()-200);
    cardImageLabel->setMaximumWidth(mainWindow->width()*.65);

    if(pic != nullptr){
        cardImageLabel->setPixmap(pic->scaled(cardImageLabel->width(), cardImageLabel->height(), Qt::KeepAspectRatio));
    }
}

void MainWindow::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);
    int newWidth = (MainWindow::width() * 0.3) - 20;

    cardTableWidget->setMaximumWidth(MainWindow::width()*.3);
    cardTableWidget->setColumnWidth(0, newWidth*.25);
    cardTableWidget->setColumnWidth(1, newWidth*.75);

    resizeImage();
}


void setupCardTableWidgit(){
    QStringList headers = {"Count", "Name"};
    cardTableWidget->setColumnCount(2);
    cardTableWidget->setHorizontalHeaderLabels(headers);
    cardTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    cardTableWidget->setFocusPolicy(Qt::NoFocus);
    cardTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
}

bool loadSavedSettings(){

    ifstream settingsFile = ifstream("settings.txt");
    string curLine;

    if(settingsFile.is_open() && settingsFile.good()){
        cout << "Loaded file" << endl;
        getline(settingsFile, curLine);
        cout << curLine << endl;
        if(curLine != ""){
            cout << "empty file" << endl;
            settingsArray[0] = curLine;
            cout << settingsArray[0] << endl;
            int i = 1;
            while(getline(settingsFile, curLine)){
                settingsArray[i] = curLine;
                i++;
            }
            settingsFile.close();
            cout << "true" << endl;
            return true;
        }
        else{
            settingsFile.close();
        }
    }

    return false;

}

void loadCards(){


    if (collectionFile.is_open()) {
        loadFileToList();
        loadCardsToColumn();
    }

}

void loadCardsToColumn(){
    row = 0;

    for(Card *c : collectionCards){
        cout << c->getName() << endl;
        cardTableWidget->insertRow(row);
        QTableWidgetItem *countRow = new QTableWidgetItem(QString().fromStdString(to_string(c->getCount())));
        countRow->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        QTableWidgetItem *nameRow = new QTableWidgetItem(QString().fromStdString(c->getName()));

        cardTableWidget->setItem(row, 0, countRow);
        cardTableWidget->setItem(row, 1, nameRow);
        row++;
    }
}

void writeOutput() {
    //open output file
    outputFile = ofstream("out.txt");

    for(Card *c : collectionCards){
        outputFile << c->getCount() << " " << c->getName() << " (" << c->getSet() << ") " <<
            c->getPrintCode() << " " << c->getFoil() << endl;
    }

    outputFile.close();
}

void loadFileToList() {
    string curLine;
    int x = 0;

    //Save collection to list
    while (getline(collectionFile, curLine)) {
        int printIndex;
        string name, foil, set, temp, print;
        int count;

        size_t end = curLine.find_first_not_of("0123456789");
        temp = curLine.substr(end, curLine.length() - 1);
        name = temp.substr(1, temp.find_first_of("(")-2);
        set = temp.substr(temp.find_first_of("(")+1, 4);
        string fixedSet = "";


        for(char ch : set){
            if(isalnum(ch)){
                fixedSet.push_back(ch);
            }
        }

        set = fixedSet;

        if(temp.find_first_of("*") != variant_npos){
            foil = temp.substr(temp.find_last_of(" ") + 1, 10);
        }

        print = temp.substr(temp.find_first_of(")") + 2, 4);


        string num = curLine.substr(0, end);
        count = atoi(num.c_str());

        Card *tempCard = new Card(0, name, print, set, foil);

        if (alreadyExists(tempCard, collectionCards)) {
            collectionCards[getCardIndex(tempCard)]->setCount(collectionCards[getCardIndex(tempCard)]->getCount()+1);
        }
        else {
            //add card to vector
            collectionCards.push_back(new Card(count, name, print, set, foil));
        }
    }
    collectionFile.close();

}

int getCardIndex(Card *searchTerm) {
    auto pos = find(deckListCards.begin(), deckListCards.end(), searchTerm);

    if (pos != deckListCards.end()) {
        return pos - deckListCards.begin();
    }
    else {
        return -1;
    }
}

//For cleanliness
bool alreadyExists(Card *searchTerm, vector<Card*> list) {
    if (find(list.begin(), list.end(), searchTerm) != list.end()) {
        return true;
    }
    return false;
}

void MainWindow::on_cardTableWidget_cellClicked(int row, int column)
{
    cout << row << endl;
    if(column == 1){
        cardImageLabel->setVisible(true);
        activeRow = row;
        Card *c = collectionCards[activeRow];
        curC = *c;
        string name = "";

        for(char ch: c->getName()){
            if(isalnum(ch)){
                name.push_back(ch);
            }
        }

        string searchString = "https://api.scryfall.com/cards/search?q=\"" + c->getName() + "\"+set%3A\"" + c->getSet() + "\"+cn%3A\"" + c->getPrintCode() + "\"+is%3A";

        foilString = ("Foil: ");

        if(c->getFoil() == ""){
            foilString.append("No");
            searchString.append("nonfoil");
        }
        else if(c->getFoil() == "*E*"){
            foilString.append("Etched");
            searchString.append("etched");
        }
        else if(c->getFoil() == "*G*"){
            foilString.append("Glossy");
            searchString.append("glossy");
        }
        else if(c->getFoil() == "*F*"){
            foilString.append("Normal Foil");
            searchString.append("foil");
        }
        else{
            foilString.append("Yes");
            cout << c->getFoil() << endl;
        }


        QUrl searchUrl = QUrl(QString().fromStdString(searchString));

        queryImage(searchUrl);


    }
}

void MainWindow::queryImage(QUrl &searchUrl){

    manager = new QNetworkAccessManager();

    cout << searchUrl.toString().toStdString() << endl;

    QNetworkRequest request(searchUrl);

    reply = manager->get(request);

    if(!reply){
        qDebug() << "Failed to get reply";
    }

    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onConnect);

}

void MainWindow::onConnect(QNetworkReply *reply){
    qDebug() << "connected";

    //cout << reply->read(10240).toStdString() << endl;

    QByteArray jsonData = reply->readAll();

    //cout << jsonData.toStdString() << endl;

    //cout << endl << endl << endl;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    cout << jsonDoc.object().value("data").toString().toStdString();

    cout << endl << endl << endl;

    QJsonObject dataObj = jsonDoc.object();
    QJsonValue dataVal = dataObj.value("data");
    QJsonArray dataArr = dataVal.toArray();
    QJsonValue imgVal = dataArr.at(0);
    QJsonObject imgObj = imgVal.toObject();
    QString imageUrl = imgObj["image_uris"].toObject().value("png").toString();

    qDebug() << imageUrl;

    getImage(imageUrl);

}

void MainWindow::getImage(QString &imageUrl){
    disconnect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onConnect);

    reply = manager->get(QNetworkRequest(QUrl(imageUrl)));
    cout << "submitted request" << endl;
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::imageDownloaded);
}

void MainWindow::imageDownloaded(QNetworkReply *reply){
    cout << "got image" << endl;

    QByteArray imgData = reply->readAll();

    QImage img = QImage::fromData(imgData);
    if(pic){
        *pic = QPixmap::fromImage(img);
    }
    else{
        pic = new QPixmap();
        *pic = QPixmap::fromImage(img);
    }

    resizeImage();
    updateText();
}

void MainWindow::updateText(){
    countLabel->setText(QString().fromStdString("Count: " + to_string(collectionCards[activeRow]->getCount())));
    nameLabel->setText(QString().fromStdString(collectionCards[activeRow]->getName()));
    printCodeLabel->setText(QString().fromStdString("Collector Number: " + collectionCards[activeRow]->getPrintCode()));
    setCodeLabel->setText(QString().fromStdString("Set Code: " + collectionCards[activeRow]->getSet()));
    foilLabel->setText(foilString);

    if(!ui->decreaseCountButton->isVisible()){
        ui->decreaseCountButton->setVisible(true);
        ui->decreaseCountButton->setEnabled(true);
    }
    if(!ui->increaseCountButton->isVisible()){
        ui->increaseCountButton->setVisible(true);
        ui->increaseCountButton->setEnabled(true);
    }
}

void MainWindow::on_decreaseCountButton_clicked()
{
    collectionCards[activeRow]->setCount(collectionCards[activeRow]->getCount() - 1);

    if(collectionCards[activeRow]->getCount() > 0){
        QTableWidgetItem *countRow = new QTableWidgetItem(QString().fromStdString(to_string(collectionCards[activeRow]->getCount())));
        countRow->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        cardTableWidget->setItem(activeRow, 0, countRow);
        MainWindow::updateText();
    }
    else{
        cardTableWidget->removeRow(activeRow);

        auto it = std::find(collectionCards.begin(), collectionCards.end(), collectionCards[activeRow]);

        delete *it;
        collectionCards.erase(collectionCards.begin()+activeRow);
        cout << collectionCards[activeRow]->to_string() << endl;
        firstLoadedScreen();
        cardImageLabel->clear();
    }

    writeOutput();
}


void MainWindow::on_increaseCountButton_clicked()
{
    collectionCards[activeRow]->setCount(collectionCards[activeRow]->getCount() + 1);

    QTableWidgetItem *countRow = new QTableWidgetItem(QString().fromStdString(to_string(collectionCards[activeRow]->getCount())));
    countRow->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    cardTableWidget->setItem(activeRow, 0, countRow);
    MainWindow::updateText();

    writeOutput();
}
