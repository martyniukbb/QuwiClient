#ifndef EDITPROJECT_H
#define EDITPROJECT_H

#include <QDialog>

#include "quwiprojectlistitem.h"
#include "quwiclient.h"

namespace Ui {
class EditProject;
}

class EditProject : public QDialog
{
    Q_OBJECT

public:
    explicit EditProject(QSharedPointer<QuwiClient> quwiClient, QuwiProjectListItem* listItem, QWidget *parent = 0);
    ~EditProject();

private slots:
    void on_pushButton_ok_released();

private:
    Ui::EditProject *ui;
    QuwiProjectListItem *quwiProjectListItem;
    QSharedPointer<QuwiClient> quwi;
};

#endif // EDITPROJECT_H
