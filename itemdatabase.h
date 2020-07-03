#ifndef ITEMDATABASE_H
#define ITEMDATABASE_H

#include <QSharedPointer>
#include <QList>

#include "quwiclient.h"
#include "quwiprojectlistitem.h"

class ItemDatabase : public QObject
{
    Q_OBJECT
public:
    ItemDatabase(QSharedPointer<QuwiClient> quwiClient);

    QList<Item*> list() const;
    int size() const;

    Item *addItem(Item *item);

    void removeItem(int index);
    void removeItem(Item *item);

    void loadItems();

signals:

public slots:

private:

    QSharedPointer<QuwiClient> m_quwiClient;
    QList<Item*> m_list;

};

#endif // ITEMDATABASE_H
