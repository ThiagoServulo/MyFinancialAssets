#include "addwindow.h"
#include "ui_addwindow.h"

AddWindow::AddWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddWindow)
{
    ui->setupUi(this);

    // Add operations
    ui->comboBox_operationType->addItem("Compra");
    ui->comboBox_operationType->addItem("Venda");
    ui->comboBox_operationType->addItem("Dividendo");
    ui->comboBox_operationType->addItem("JCP");
    ui->comboBox_operationType->setCurrentIndex(-1);

    // Add assets types
    ui->comboBox_type->addItem("Ação");
    ui->comboBox_type->addItem("Fundo");

    // Disable fields
    ui->label_quantity->setEnabled(false);
    ui->label_value->setEnabled(false);
    ui->lineEdit_quantity->setEnabled(false);
    ui->lineEdit_value->setEnabled(false);

    // Clear label
    ui->label_value->setText("");
}

AddWindow::~AddWindow()
{
    delete ui;
}

void AddWindow::on_pushButton_save_clicked()
{
    ui->label_value->setText("xxxx");
}

void AddWindow::on_comboBox_type_textActivated(const QString &arg1)
{
    if(arg1 == "Ação")
    {
        ui->label_value->setText("Ação");
    }
    else
    {
        ui->label_value->setText("Fundo");
    }

}

