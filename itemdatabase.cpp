#include "itemdatabase.h"

ItemDatabase::ItemDatabase(QSharedPointer<QuwiClient> quwiClient) :
    m_quwiClient(quwiClient)
{

    loadItems();

}

QList<Item*> ItemDatabase::list() const
{

    return m_list;

}

int ItemDatabase::size() const
{

    return m_list.size();

}

Item *ItemDatabase::addItem(Item *item)
{

    m_list.prepend(item);

    return item;

}

void ItemDatabase::removeItem(int index)
{

    Item *item = m_list[index];
    m_list.removeAt(index);
    delete item;

}

void ItemDatabase::removeItem(Item *item)
{

    int index = m_list.indexOf(item);
    if(index >= 0)
        removeItem(index);
    else
        qDebug() << "Tried to remove item, ID == -1";

}

void ItemDatabase::loadItems()
{

    QMap<qint32, QuwiProject> projects = m_quwiClient->getProjects();
    for(auto project: projects){
        Item *item = new Item(project);
        addItem(item);
    }

}
