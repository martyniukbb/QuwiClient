#ifndef PROJECTS_H
#define PROJECTS_H

#include <QDialog>
#include <QSharedPointer>

#include "editproject.h"

#include "quwiclient.h"
#include "projectsdelegate.h"

namespace Ui {
class Projects;
}

class Projects : public QDialog
{
    Q_OBJECT

public:
    explicit Projects(QWidget *parent = 0) = delete;
    explicit Projects(QSharedPointer<QuwiClient> quwiClient, QWidget *parent = 0);
    ~Projects();

private slots:
    void on_pushButton_projects_released();

    void on_pushButton_logout_released();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::Projects *ui;
    QSharedPointer<QuwiClient> quwi;
    ItemDatabase *m_itemDatabase;
    ItemListModel *m_itemListModel;
    ProjectsDelegate *m_projectsDelegate;
};

#endif // PROJECTS_H
