#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QInputDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDir>
#include <QComboBox>
#include <QVariant>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../table/pokus.db");
    db.open();

    model_players = new QSqlQueryModel(this);
    model_teams = new QSqlTableModel(this);
    model_classes = new QSqlTableModel(this);

    model_players->setQuery("SELECT players.id, players.name, teams.name, classes.name FROM players LEFT OUTER JOIN teams ON (players.id_team = teams.id OR players.id_team IS NULL AND teams.id IS NULL) LEFT OUTER JOIN classes ON (players.id_class = classes.id OR players.id_class IS NULL AND classes.id IS NULL)");
    model_teams->setTable("teams");
    model_classes->setTable("classes");

    model_players->setHeaderData(1, Qt::Horizontal, "Name");
    model_players->setHeaderData(2, Qt::Horizontal, "Team");
    model_players->setHeaderData(3, Qt::Horizontal, "Class");
    model_teams->setHeaderData(1, Qt::Horizontal, "Name");
    model_classes->setHeaderData(1, Qt::Horizontal, "Name");

    model_teams->select();
    model_classes->select();

    ui->players_view->setModel(model_players);
    ui->teams_view->setModel(model_teams);
    ui->classes_view->setModel(model_classes);

    ui->teams_view->hideColumn(0);
    ui->classes_view->hideColumn(0);
    ui->players_view->hideColumn(0);

    ui->classes_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->teams_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->players_view->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_teams_add_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add team", "Class team",QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if(!ok){
        return;
    }
    if(name.length() > 3){
        QSqlQuery dotaz;
        dotaz.prepare("INSERT INTO teams VALUES (NULL, :name)");
        dotaz.bindValue(":name", name);
        dotaz.exec();
        model_teams->select();
    }else{
        QMessageBox::warning(this, "Warning", "Name too short");
    }
}

void MainWindow::on_classes_add_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add class", "Class name",QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if(!ok){
        return;
    }
    if(name.length() > 3){
        QSqlQuery dotaz;
        dotaz.prepare("INSERT INTO classes VALUES (NULL, :name)");
        dotaz.bindValue(":name", name);
        dotaz.exec();
        model_classes->select();
    }else{
        QMessageBox::warning(this, "Warning", "Name too short");
    }
}

void MainWindow::on_players_add_clicked()
{
    Dialog d;
    QComboBox* team = d.getComboBox_team();
    QComboBox* classes = d.getComboBox_class();
    QSqlQuery dotaz;
    dotaz.prepare("SELECT * FROM teams");
    dotaz.exec();
    while(dotaz.next()){
        team->insertItem(0, dotaz.value("name").toString(), dotaz.value("id").toInt());
    }
    dotaz.prepare("SELECT * FROM classes");
    dotaz.exec();
    while(dotaz.next()){
        classes->insertItem(0, dotaz.value("name").toString(), dotaz.value("id").toInt());
    }
    if(d.exec() == QDialog::Accepted){
        dotaz.prepare("INSERT INTO players VALUES (NULL, :name, :team, :class)");
        dotaz.bindValue(":name", d.getName());
        dotaz.bindValue(":team", d.getTeam_id());
        dotaz.bindValue(":class", d.getClass_id());
        dotaz.exec();
    }
    model_players->setQuery("SELECT players.id, players.name, teams.name, classes.name FROM players LEFT OUTER JOIN teams ON (players.id_team = teams.id OR players.id_team IS NULL AND teams.id IS NULL) LEFT OUTER JOIN classes ON (players.id_class = classes.id OR players.id_class IS NULL AND classes.id IS NULL)");
}

void MainWindow::on_classes_del_clicked()
{
    QItemSelectionModel *selection = ui->classes_view->selectionModel();
    QString sql = "DELETE FROM classes WHERE id IN (";
    for(int i = 0; i<selection->selectedRows().count(); ++i)
    {
        QModelIndex index = selection->selectedRows().at(i);
        sql+= QString::number(model_classes->data(index).toInt());
        if(i+1 != selection->selectedRows().count())
        {
            sql+=", ";
        }
    }
    sql+=")";
    QSqlQuery dotaz;
    dotaz.prepare(sql);
    dotaz.exec();
    model_classes->select();
    model_players->setQuery("SELECT players.id, players.name, teams.name, classes.name FROM players LEFT OUTER JOIN teams ON (players.id_team = teams.id OR players.id_team IS NULL AND teams.id IS NULL) LEFT OUTER JOIN classes ON (players.id_class = classes.id OR players.id_class IS NULL AND classes.id IS NULL)");
}

void MainWindow::on_teams_del_clicked()
{
    QItemSelectionModel *selection = ui->teams_view->selectionModel();
    QString sql = "DELETE FROM teams WHERE id IN (";
    for(int i = 0; i<selection->selectedRows().count(); ++i)
    {
        QModelIndex index = selection->selectedRows().at(i);
        sql+= QString::number(model_teams->data(index).toInt());
        if(i+1 != selection->selectedRows().count())
        {
            sql+=", ";
        }
    }
    sql+=")";
    QSqlQuery dotaz;
    dotaz.prepare(sql);
    dotaz.exec();
    model_teams->select();
    model_players->setQuery("SELECT players.id, players.name, teams.name, classes.name FROM players LEFT OUTER JOIN teams ON (players.id_team = teams.id OR players.id_team IS NULL AND teams.id IS NULL) LEFT OUTER JOIN classes ON (players.id_class = classes.id OR players.id_class IS NULL AND classes.id IS NULL)");
}

void MainWindow::on_players_del_clicked()
{
    QItemSelectionModel *selection = ui->players_view->selectionModel();
    QString sql = "DELETE FROM players WHERE id IN (";
    for(int i = 0; i<selection->selectedRows().count(); ++i)
    {
        QModelIndex index = selection->selectedRows().at(i);
        sql+= QString::number(model_players->data(index).toInt());
        if(i+1 != selection->selectedRows().count())
        {
            sql+=", ";
        }
    }
    sql+=")";
    QSqlQuery dotaz;
    dotaz.prepare(sql);
    dotaz.exec();
    model_players->setQuery("SELECT players.id, players.name, teams.name, classes.name FROM players LEFT OUTER JOIN teams ON (players.id_team = teams.id OR players.id_team IS NULL AND teams.id IS NULL) LEFT OUTER JOIN classes ON (players.id_class = classes.id OR players.id_class IS NULL AND classes.id IS NULL)");
}
