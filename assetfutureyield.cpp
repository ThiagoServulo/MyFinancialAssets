#include "assetfutureyield.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QEventLoop>
#include <QMessageBox>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

AssetFutureYield::AssetFutureYield(QDate exYieldDate, QDate paymentDate, YieldType type, double value, double ratio)
{
    this->exYieldDate = exYieldDate;
    this->paymentDate = paymentDate;
    this->type = type;
    this->value = value;
    this->ratio = ratio;
}

QDate AssetFutureYield::getExYieldDate() const
{
    return this->exYieldDate;
}

QDate AssetFutureYield::getPaymentDate() const
{
    return this->paymentDate;
}

YieldType AssetFutureYield::getYieldType() const
{
    return this->type;
}

double AssetFutureYield::getValue() const
{
    return this->value;
}

double AssetFutureYield::getRatio() const
{
    return this->ratio;
}

QList<AssetFutureYield> getFutureYieldsForTicker(Asset asset)
{
    QNetworkAccessManager manager;
    QList<AssetFutureYield> futureYields;
    QUrl url;

    // Determine the correct URL based on the asset type
    if(asset.getAssetType() == AssetType::ACAO)
    {
        url= "https://www.fundamentus.com.br/proventos.php?papel=" + asset.getTicker();
    }
    else if(asset.getAssetType() == AssetType::FUNDO)
    {
        url = "https://www.fundamentus.com.br/fii_proventos.php?papel=" + asset.getTicker();
    }
    else
    {
        // Return an empty list if the asset type is unsupported
        return futureYields;
    }

    QNetworkRequest request(url);

    // Perform the network request
    QNetworkReply *reply = manager.get(request);

    // Wait synchronously for the request to complete
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check if the request failed
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Request error: " << reply->errorString();
        return futureYields;
    }

    // Read and convert the response data to a QString
    QByteArray responseData = reply->readAll();
    QString htmlContent = QString::fromUtf8(responseData);

    // Locate the table containing the yield data
    int startIndex = htmlContent.indexOf("<table id=\"resultado\"");
    int endIndex = htmlContent.indexOf("</table>", startIndex);

    if (startIndex == -1 || endIndex == -1)
    {
        qDebug() << "Table HTML not found!";
        return futureYields;
    }

    // Extract only the table content
    QString tableContent = htmlContent.mid(startIndex, endIndex - startIndex + 7); // +7 to include </table>

    // Process data
    if(asset.getAssetType() == AssetType::ACAO)
    {
        // Regex to extract table rows with dividend information
        QRegularExpression rowRegex("<tr.*?>\\s*<td>(.*?)</td>\\s*<td>(.*?)</td>\\s*<td>(.*?)</td>\\s*<td>(.*?)</td>\\s*<td>(.*?)</td>\\s*</tr>",
                                    QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatchIterator it = rowRegex.globalMatch(tableContent);

        while (it.hasNext())
        {
            QRegularExpressionMatch match = it.next();
            if (match.hasMatch())
            {
                // Extract payment date and validate if it's a future yield
                QDate paymentDate = QDate::fromString(match.captured(4).trimmed(), "dd/MM/yyyy");

                if (paymentDate < QDate::currentDate())
                {
                    break; // Stop processing past yields
                }

                // Extract and convert relevant fields
                double value = match.captured(2).trimmed().replace(',', '.').toDouble();
                QString type = match.captured(3).trimmed();
                QDate exYieldDate = QDate::fromString(match.captured(1).trimmed(), "dd/MM/yyyy");
                int ratio = match.captured(5).trimmed().toInt();

                qDebug() << type;

                // Create an AssetFutureYield object and add it to the list
                AssetFutureYield yield = AssetFutureYield(exYieldDate, paymentDate, YieldType::DIVIDENDO, value, ratio);

                futureYields.append(yield);
            }
        }
    }
    else if(asset.getAssetType() == AssetType::FUNDO)
    {
        // Regex to extract table rows with yield information
        QRegularExpression rowRegex(R"(<tr.*?>\s*<td>(.*?)<\/td>\s*<td>(.*?)<\/td>\s*<td>(.*?)<\/td>\s*<td>(.*?)<\/td>)");
        QRegularExpressionMatchIterator matches = rowRegex.globalMatch(tableContent);

        while (matches.hasNext())
        {
            QRegularExpressionMatch match = matches.next();

            // Extract payment date and validate if it's a future yield
            QDate paymentDate = QDate::fromString(match.captured(3).trimmed(), "dd/MM/yyyy");

            if (paymentDate < QDate::currentDate())
            {
                // Stop processing past yields
                break;
            }

            // Extract and convert relevant fields
            QDate exYieldDate = QDate::fromString(match.captured(1).trimmed(), "dd/MM/yyyy");
            YieldType type = getYieldTypeFromString(match.captured(2).trimmed());
            double value = match.captured(4).trimmed().replace(",", ".").toDouble();

            // Create an AssetFutureYield object and add it to the list
            AssetFutureYield yield = AssetFutureYield(exYieldDate, paymentDate, type, value, 1);

            futureYields.append(yield);
        }
    }

    return futureYields;
}
