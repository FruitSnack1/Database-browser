#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_teams_add_clicked();

    void on_classes_add_clicked();

    void on_players_add_clicked();

    void on_classes_del_clicked();

    void on_teams_del_clicked();

    void on_players_del_clicked();

private:
    Ui::MainWindow *ui;
    //Dialog d;
    QSqlQueryModel *model_players;
    QSqlTableModel *model_teams;
    QSqlTableModel *model_classes;
};

#endif // MAINWINDOW_H
