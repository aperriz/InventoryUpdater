#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent* event);
    void showEvent(QShowEvent* event);
private:
    void queryImage(QUrl &searchUrl);
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    void getImage(QString &imageUrl);
    void updateText();
private slots:
    void on_cardTableWidget_cellClicked(int row, int column);
    void onConnect(QNetworkReply *reply);
    void imageDownloaded(QNetworkReply *reply);



    void on_decreaseCountButton_clicked();

    void on_increaseCountButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
