#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "projects.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    quwiClient = QSharedPointer<QuwiClient>(new QuwiClient);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    hide();

    QString login = ui->lineEdit_login->text();
    QString password = ui->lineEdit_password->text();
    
    if(quwiClient->auth(login, password)){
        
        Projects projects(quwiClient);
        projects.setModal(true);
        projects.exec();
        quwiClient->logout();
        
    }

    show();
}
