#include "addwindow.h"
#include "ui_addwindow.h"
#include "constants.h"
#include <QIntValidator>
#include <QMessageBox>

AddWindow::AddWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddWindow)
{
    ui->setupUi(this);

    // Add assets types
    ui->comboBox_type->addItem(getAssetTypeString(AssetType::ACAO));
    ui->comboBox_type->addItem(getAssetTypeString(AssetType::FUNDO));
    ui->comboBox_type->setCurrentIndex(-1);

    // Disable fields
    ui->label_quantity->setEnabled(false);
    ui->label_value->setEnabled(false);
    ui->lineEdit_quantity->setEnabled(false);
    ui->lineEdit_value->setEnabled(false);

    // Clear label
    ui->label_value->setText("");

    // Creating validators
    QIntValidator *validator = new QIntValidator(0, 9999, ui->lineEdit_quantity);
    ui->lineEdit_quantity->setValidator(validator);
}

AddWindow::~AddWindow()
{
    delete ui;
}

void AddWindow::on_pushButton_save_clicked()
{
    if(ui->comboBox_operationType->currentIndex() != -1 &&
       ui->comboBox_type->currentIndex() != -1)
    {

    }

    if(isValidOperationType(ui->comboBox_operationType->currentText()))
    {
        QMessageBox::about(this, "aaa", "aaa");
    }
    else
    {
        QMessageBox::about(this, "333", ui->comboBox_operationType->currentText());
    }
}

void AddWindow::on_comboBox_type_textActivated(const QString &arg1)
{
    // Clear fields
    ui->lineEdit_value->clear();
    ui->lineEdit_quantity->clear();
    ui->label_value->setText("");

    // Disable fields
    ui->label_quantity->setEnabled(false);
    ui->label_value->setEnabled(false);
    ui->lineEdit_quantity->setEnabled(false);
    ui->lineEdit_value->setEnabled(false);

    // Clear comboBox
    ui->comboBox_operationType->clear();
    ui->comboBox_operationType->addItem(getOperationTypeString(OperationType::COMPRA));
    ui->comboBox_operationType->addItem(getOperationTypeString(OperationType::VENDA));

    // Check asset type
    if(arg1 == getAssetTypeString(AssetType::ACAO))
    {
        // Add operations
        ui->comboBox_operationType->addItem(getOperationTypeString(OperationType::DIVIDENDOS));
        ui->comboBox_operationType->addItem(getOperationTypeString(OperationType::JCP));
    }
    else // Fundo
    {
        // Add operations
        ui->comboBox_operationType->addItem(getOperationTypeString(OperationType::RENDIMENTOS));
    }

    // Uncheck comboBox
     ui->comboBox_operationType->setCurrentIndex(-1);
}


void AddWindow::on_comboBox_operationType_textActivated(const QString &arg1)
{
    // Clear fields
    ui->lineEdit_value->clear();
    ui->lineEdit_quantity->clear();

    // Enable value
    ui->label_value->setEnabled(true);
    ui->lineEdit_value->setEnabled(true);

    // Check operation type
    if(arg1 == getOperationTypeString(OperationType::COMPRA) || arg1 == getOperationTypeString(OperationType::VENDA))
    {
        // Enable quantity
        ui->label_quantity->setEnabled(true);
        ui->lineEdit_quantity->setEnabled(true);

        // Update label value
        ui->label_value->setText("Preço médio");

    }
    else // JCP, Dividendos e Rendimentos
    {
        // Disable quantity
        ui->label_quantity->setEnabled(false);
        ui->lineEdit_quantity->setEnabled(false);

        // Update label value
        ui->label_value->setText("Valor total");
    }
}

