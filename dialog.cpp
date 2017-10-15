#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

QComboBox *Dialog::getComboBox_team()
{
    return ui->comboBox_team;
}

QComboBox *Dialog::getComboBox_class()
{
    return ui->comboBox_class;
}

void Dialog::accept()
{
    name = ui->lineEdit_name->text();
    if(name.length() < 3){
        QMessageBox::warning(this, "Warning", "Name is too short");
        return;
    }
    class_id = ui->comboBox_class->currentData().toInt();
    team_id = ui->comboBox_team->currentData().toInt();
    QDialog::accept();
}

int Dialog::getTeam_id() const
{
    return team_id;
}

int Dialog::getClass_id() const
{
    return class_id;
}

QString Dialog::getName() const
{
    return name;
}

void Dialog::on_pushButton_ok_clicked()
{
    accept();
}

void Dialog::on_pushButton_2_clicked()
{
    reject();
}
