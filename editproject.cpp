#include "editproject.h"
#include "ui_editproject.h"

EditProject::EditProject(QSharedPointer<QuwiClient> quwiClient, QuwiProjectListItem *listItem, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditProject),
    quwiProjectListItem(listItem),
    quwi(quwiClient)
{
    ui->setupUi(this);

    QPixmap pixmap;
    pixmap.loadFromData(quwiProjectListItem->item()->logo);

    QLabel *label_image = ui->label_image;

    label_image->setPixmap(pixmap);
    //label_image->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //label_image->setFixedSize(130, 130);
    QRegion *region = new QRegion(0, 0, label_image->width(), label_image->height(), QRegion::Ellipse);
    label_image->setScaledContents(true);
    label_image->setMask(*region);

    ui->pushButton_active->setChecked(quwiProjectListItem->item()->is_active);
    ui->pushButton_watcher->setChecked(quwiProjectListItem->item()->is_owner_watcher);
    ui->lineEdit_name->setText(quwiProjectListItem->item()->name);

}

EditProject::~EditProject()
{
    delete ui;
}

void EditProject::on_pushButton_ok_released()
{
    hide();

    QuwiProject quwiProject = *quwiProjectListItem->item();
    quwiProject.is_active = qint32(ui->pushButton_active->isChecked());
    quwiProject.is_owner_watcher = qint32(ui->pushButton_watcher->isChecked());
    quwiProject.name = ui->lineEdit_name->text();

    if(quwi->upgradeProject(quwiProject).id == quwiProject.id){

        quwiProjectListItem->item()->is_active = quwiProject.is_active;
        quwiProjectListItem->item()->is_owner_watcher = quwiProject.is_owner_watcher;
        quwiProjectListItem->item()->name = quwiProject.name;

    }

    close();
}
