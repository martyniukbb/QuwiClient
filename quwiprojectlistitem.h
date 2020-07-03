#ifndef QUWIPROJECTLISTITEM_H
#define QUWIPROJECTLISTITEM_H

#include "quwiclient.h"

class Item: public QuwiProject, public QObject
{
public:
    Item(QuwiProject project);

    QuwiProject operator=(QuwiProject&& other);
};

namespace Ui{

    class QuwiProjectListItem;

}

class QuwiProjectListItem: public QObject
{
    Q_OBJECT
public:

    QuwiProjectListItem(Item * quwiProject);
    ~QuwiProjectListItem();

    Item *item() const;
    void setItem(Item *quwiProject);

private:

    Item *m_item;

};

#endif // QUWIPROJECTLISTITEM_H
