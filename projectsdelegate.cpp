#include "projectsdelegate.h"

#include <QPicture>

#define HINT_WIDTH  560
#define HINT_HEIGHT 100

ItemListModel::ItemListModel(QListView *view, ItemDatabase *itemDatabase): QAbstractItemModel()
{

    m_view = view;
    m_itemDatabase = itemDatabase;

    loadFromDatabase();

}

void ItemListModel::loadFromDatabase(){

    for(Item *item: m_itemDatabase->list()){

        appendItem(item);

    }

}

void ItemListModel::refresh(int row)
{

    QModelIndex left = createIndex(row, 0);

}

void ItemListModel::clear()
{

    if(m_items.length() > 0)
        removeRows(0, m_items.length());

}

QuwiProjectListItem *ItemListModel::appendItem(Item *item)
{

    int newIndex = m_items.length();
    beginInsertRows(QModelIndex(), newIndex, newIndex);

    m_items.append( new QuwiProjectListItem(nullptr) );
    QuwiProjectListItem *i = m_items.at(newIndex);
    i->setItem(item);

    endInsertRows();
    return i;

}

bool ItemListModel::insertRows(int position, int rows, const QModelIndex &parent)
{

    beginInsertRows(parent, position, position + rows - 1);

    for(int i = 0; i < rows; i++)
        m_items.insert(position + i, new QuwiProjectListItem(nullptr));

    endInsertRows();

    return true;
}

bool ItemListModel::removeRows(int position, int rows, const QModelIndex &parent)
{

    beginRemoveRows(parent, position, position + rows - 1);

    for(int i = 0; i < rows; i++){

        delete m_items[position];
        m_items[position] = nullptr;
        m_items.remove(position);

    }

    endRemoveRows();

    return true;

}

QVariant ItemListModel::data(const QModelIndex &index, int role) const
{

    return QVariant();

}

QModelIndex ItemListModel::index(int row, int column, const QModelIndex &parent) const
{

    if(!hasIndex(row, column, parent))
        return QModelIndex();

    if(row < m_items.length()){

        QModelIndex index = createIndex(row, column, m_items.at(row));
        return index;

    }
    else
        return QModelIndex();

}

QModelIndex ItemListModel::parent(const QModelIndex &child) const
{

    return QModelIndex();

}

int ItemListModel::rowCount(const QModelIndex &parent) const
{

    return m_items.length();

}

int ItemListModel::columnCount(const QModelIndex &parent) const
{

    return 1;

}

Item *ItemListModel::itemFromIndex(QModelIndex index)
{

    if(!index.isValid())
        return nullptr;

    QuwiProjectListItem *item = static_cast<QuwiProjectListItem*>(index.internalPointer());
    return item->item();

}

ProjectsDelegate::ProjectsDelegate(QSharedPointer<QuwiClient> quwiClient, QObject *parent) : QStyledItemDelegate(parent)
{
    m_quwiClient = quwiClient;
}

QString secondsToStringHMS(qint32 totalNumberOfSeconds){

    int seconds = totalNumberOfSeconds % 60;
    int minutes = (totalNumberOfSeconds / 60) % 60;
    int hours = (totalNumberOfSeconds / 60 / 60);

    QString timeString = QString("%1:%2:%3")
      .arg(hours, 2, 10, QChar('0'))
      .arg(minutes, 2, 10, QChar('0'))
      .arg(seconds, 2, 10, QChar('0'));

    return timeString;

}

void ProjectsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.column() == 0){

        qint32 startX = option.rect.width() / 2 - HINT_WIDTH / 2;
        if(startX < 0)
            startX = 0;

        QFont font = painter->font();

        QuwiProjectListItem *item = static_cast<QuwiProjectListItem*>( index.internalPointer() );

        QPixmap pixmap;
        pixmap.loadFromData(item->item()->logo);

        painter->setBrush(QBrush(QColor(255, 255, 255)));
        painter->fillRect(QRect(startX + 4, option.rect.y() + 4, HINT_WIDTH - 8, HINT_HEIGHT - 8), painter->brush());

        painter->setPen(QColor(210, 210, 210));
        painter->drawRect(startX + 4, option.rect.y() + 4, HINT_WIDTH - 8, HINT_HEIGHT - 8);
        painter->drawPixmap(QRect(startX + 20, option.rect.y() + HINT_HEIGHT/2 - 25, 50, 50), pixmap);

        font.setWeight(QFont::Bold);
        painter->setPen(QColor(0, 0, 0));
        painter->setFont(font);
        painter->drawText(QRectF(startX + 85, option.rect.y() + HINT_HEIGHT/2 - 5, 100, 15), item->item()->name);

        font.setWeight(QFont::Bold);
        painter->setPen(((item->item()->is_active == 1) ? QColor(20, 230, 20) : QColor(230, 20, 20)));
        painter->setFont(font);
        painter->drawText(QRectF(startX + 240, option.rect.y() + HINT_HEIGHT/2 - 5, 90 , 15), ((item->item()->is_active == 1) ? "Active" : "Not Active"));

        font.setWeight(QFont::Normal);
        painter->setPen(QColor(0, 0, 0));
        painter->setFont(font);

        painter->drawText(QRectF(startX + 340, option.rect.y() + HINT_HEIGHT/2 - 5 - 20, 100, 15), "time this week");
        painter->drawText(QRectF(startX + 340, option.rect.y() + HINT_HEIGHT/2 - 5, 100, 15), "this month");
        painter->drawText(QRectF(startX + 340, option.rect.y() + HINT_HEIGHT/2 - 5 + 20, 100, 15), "total");

        painter->drawText(QRectF(startX + 460, option.rect.y() + HINT_HEIGHT/2 - 5 - 20, 80, 15), secondsToStringHMS(item->item()->spent_time_week));
        painter->drawText(QRectF(startX + 460, option.rect.y() + HINT_HEIGHT/2 - 5, 80, 15), secondsToStringHMS(item->item()->spent_time_month));
        painter->drawText(QRectF(startX + 460, option.rect.y() + HINT_HEIGHT/2 - 5 + 20, 80, 15), secondsToStringHMS(item->item()->spent_time_all));


    }else{

        QStyledItemDelegate::paint(painter, option, index);

    }

}

QSize ProjectsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    return QSize(HINT_WIDTH, HINT_HEIGHT);

}
