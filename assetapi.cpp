#include "assetapi.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

AssetApi::AssetApi()
{

}

double AssetApi::getAssetCurrentPrice(QString ticker)
{
    // Init API key
    QString apiKey = "SAFNHJO46M4G14SI";

    // Create the correct symbol for API
    QString symbol = ticker + ".SA";

    // Build the URL for API
    QString urlStr = QString("https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=%1&apikey=%2").arg(symbol, apiKey);
    QUrl url(urlStr);
    QNetworkRequest request(url);

    qDebug() << "URL: " << urlStr;

    // Create the network manager
    QNetworkAccessManager manager;

    // Make the GET request
    QNetworkReply *reply = manager.get(request);

    // Create connection to API
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    // Wait until the response is received
    loop.exec();

    // Verify the API answer
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "API requisition error:" << reply->errorString();
        return API_ERROR;
    }

    // Read the answer
    QByteArray response = reply->readAll();
    reply->deleteLater();

    // Parse the JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    if(jsonDoc.isNull())
    {
        qDebug() << "Parse JSON error";
        return API_ERROR;
    }

    // Get json object
    QJsonObject jsonObj = jsonDoc.object();

    // Verify the answer
    if(!jsonObj.contains("Global Quote"))
    {
        qDebug() << "JSON received is invalid. 'Global Quote' is missing";
        return API_ERROR;
    }

    QJsonObject globalQuote = jsonObj["Global Quote"].toObject();

    // Verify the answer
    if(!globalQuote.contains("05. price"))
    {
        qDebug() << "JSON received is invalid. '05. price' is missing";
        return API_ERROR;
    }

    // Get the price
    QString priceStr = globalQuote["05. price"].toString();

    // Return the price
    return priceStr.toDouble();
}
