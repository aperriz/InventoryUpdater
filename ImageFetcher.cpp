// #include "ImageFetcher.h"

// #include <QObject>

// ImageFetcher::ImageFetcher(QUrl &searchUrl, QLabel* imgLabel, QWidget *parent) : QObject(parent){

//     // connect(&manager, &QNetworkAccessManager::finished, this, &ImageFetcher::onConnect);
//     connect(&manager, &QNetworkAccessManager::destroyed, this, &ImageFetcher::onManagerDestroy);
//     connect(&manager, &QNetworkAccessManager::sslErrors, this, &ImageFetcher::onManagerError);

//     QUrl url("127.0.0.1");

//     QNetworkRequest request(url);
//     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//     reply = manager.get(request);

//     if(!reply){
//         qDebug() << "Failed to get reply";
//     }

//     connect(reply, &QNetworkReply::finished, this, &ImageFetcher::onConnect);
//     connect(reply, &QNetworkReply::destroyed, this, &ImageFetcher::onReplyDestroy);
//     connect(reply, &QNetworkReply::sslErrors, this, &ImageFetcher::onReplyError);

// }

// void ImageFetcher::onConnect(QNetworkReply* reply){
//     int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//     qDebug() << status;
//     qDebug() << "Connected";
// }

// void ImageFetcher::onManagerDestroy(){
//     qDebug() << "manager destroyed";
// }

// void ImageFetcher::onReplyDestroy(){
//     qDebug() << "reply destroyed";
// }

// void ImageFetcher::onReplyError(){
//     qDebug() << "reply error";
// }

// void ImageFetcher::onManagerError(){
//     qDebug() << "manager destroyed";
// }

