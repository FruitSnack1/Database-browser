#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <mainwindow.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QComboBox* getComboBox_team();
    QComboBox* getComboBox_class();
    void accept();
    QString name;
    int class_id;
    int team_id;

    QString getName() const;

    int getClass_id() const;

    int getTeam_id() const;

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
