#include "projects.h"
#include "ui_projects.h"

#include "projectsdelegate.h"

Projects::Projects(QSharedPointer<QuwiClient> quwiClient, QWidget *parent):
    quwi(quwiClient),
    QDialog(parent),
    ui(new Ui::Projects)
{
    ui->setupUi(this);

    m_itemDatabase = new ItemDatabase(quwiClient);
    m_itemListModel = new ItemListModel(ui->listView, m_itemDatabase);
    m_projectsDelegate = new ProjectsDelegate(quwiClient);
    ui->listView->setModel(m_itemListModel);
    ui->listView->setItemDelegate(m_projectsDelegate);

}

Projects::~Projects()
{
    delete m_projectsDelegate;
    delete m_itemListModel;
    delete m_itemDatabase;
    delete ui;
}

void Projects::on_pushButton_projects_released()
{
    m_itemListModel->clear();
    int size = m_itemDatabase->size();

    for(int i = 0; i < size; ++i){
        m_itemDatabase->removeItem(0);
    }

    m_itemDatabase->loadItems();

    m_itemListModel->loadFromDatabase();

}

void Projects::on_pushButton_logout_released()
{
    quwi->logout();
    close();
}

void Projects::on_listView_clicked(const QModelIndex &index)
{
    EditProject editProject(quwi, static_cast<QuwiProjectListItem*>( index.internalPointer() ));
    editProject.setModal(true);
    editProject.exec();
    ui->listView->repaint();
}
