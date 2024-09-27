#include "newreorganizationwindow.h"
#include "ui_newreorganizationwindow.h"
#include "constants.h"

NewReorganizationWindow::NewReorganizationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewReorganizationWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(370, 271);
    this->setMinimumSize(370, 271);

    // Add assets types
    ui->comboBox_assetType->addItem(getAssetTypeString(AssetType::ACAO));
    ui->comboBox_assetType->addItem(getAssetTypeString(AssetType::FUNDO));
    ui->comboBox_assetType->setCurrentIndex(-1);

    // Add reorganization types
    ui->comboBox_reorganizationType->addItem(getReorganizationTypeString(ReorganizationType::DESDOBRAMENTO));
    ui->comboBox_reorganizationType->addItem(getReorganizationTypeString(ReorganizationType::GRUPAMENTO));
    ui->comboBox_reorganizationType->setCurrentIndex(-1);

    // Creating ratio validator
    QRegularExpression regexRatio("[0-9]*");
    QRegularExpressionValidator *validatorRatio = new QRegularExpressionValidator(regexRatio, ui->lineEdit_ratio);
    ui->lineEdit_ratio->setValidator(validatorRatio);

    // Creating ticker validator
    QRegularExpression regexTicker("[a-zA-Z0-9]*");
    QRegularExpressionValidator *validatorTicker = new QRegularExpressionValidator(regexTicker, ui->lineEdit_ticker);
    ui->lineEdit_ticker->setValidator(validatorTicker);

    // Setting current date
    QDate currentDate = QDate::currentDate();
    ui->dateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->dateEdit->setDate(currentDate);
}

NewReorganizationWindow::~NewReorganizationWindow()
{
    delete ui;
}

void NewReorganizationWindow::on_pushButton_save_clicked()
{

}

void NewReorganizationWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

