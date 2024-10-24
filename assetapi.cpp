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
    // Init api key
    apiKey = "SAFNHJO46M4G14SI";
}

double AssetApi::getAssetCurrentPrice(QString ticker)
{
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
        changeApiKey();
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
        changeApiKey();
        return API_ERROR;
    }

    // Get json object
    QJsonObject jsonObj = jsonDoc.object();

    // Verify the answer
    if(!jsonObj.contains("Global Quote"))
    {
        qDebug() << "JSON received is invalid. 'Global Quote' is missing";
        changeApiKey();
        return API_ERROR;
    }

    QJsonObject globalQuote = jsonObj["Global Quote"].toObject();

    // Verify the answer
    if(!globalQuote.contains("05. price"))
    {
        qDebug() << "JSON received is invalid. '05. price' is missing";
        changeApiKey();
        return API_ERROR;
    }

    // Get the price
    QString priceStr = globalQuote["05. price"].toString();

    // Return the price
    changeApiKey();
    return priceStr.toDouble();
}

void AssetApi::changeApiKey()
{
    apiKey = (apiKey == "SAFNHJO46M4G14SI") ? "66O5L9PCQ9BFSEL4" : "SAFNHJO46M4G14SI";
}
