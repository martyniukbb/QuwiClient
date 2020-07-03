#ifndef PROJECTSDELEGATE_H
#define PROJECTSDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QListView>
#include <QUuid>

#include "quwiclient.h"
#include "quwiprojectlistitem.h"
#include "itemdatabase.h"

class ItemListModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    ItemListModel(QListView *view, ItemDatabase *itemDatabase);

    void loadFromDatabase();

    void refresh(int row);
    void clear();
    QuwiProjectListItem *appendItem(Item *item);

    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

    QVariant data(const QModelIndex &index, int role) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Item *itemFromIndex(QModelIndex index);

private:

    QListView *m_view;
    QVector<QuwiProjectListItem*> m_items;
    ItemDatabase *m_itemDatabase;

};

class ProjectsDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ProjectsDelegate(QSharedPointer<QuwiClient> quwiClient, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

private:
    QSharedPointer<QuwiClient> m_quwiClient;
};

#endif // PROJECTSDELEGATE_H
