#include "quwiprojectlistitem.h"

Item::Item(QuwiProject project){

    this->id = project.id;
    this->name = project.name;
    this->uid = project.uid;
    this->logo_url = project.logo_url;
    this->position = project.position;
    this->is_active = project.is_active;
    this->is_owner_watcher = project.is_owner_watcher;
    this->users = project.users;
    this->logo = project.logo;
    this->spent_time_week = project.spent_time_week;
    this->spent_time_month = project.spent_time_month;
    this->spent_time_all = project.spent_time_all;

}

QuwiProject Item::operator=(QuwiProject&& other){

    other.id = this->id;
    other.name = this->name;
    other.uid = this->uid;
    other.logo_url = this->logo_url;
    other.position = this->position;
    other.is_active = this->is_active;
    other.is_owner_watcher = this->is_owner_watcher;
    other.users = this->users;
    other.logo = this->logo;
    other.spent_time_week = this->spent_time_week;
    other.spent_time_month = this->spent_time_month;
    other.spent_time_all = this->spent_time_all;

}

QuwiProjectListItem::QuwiProjectListItem(Item *quwiProject):
    m_item(quwiProject)
{

}

QuwiProjectListItem::~QuwiProjectListItem()
{
    m_item = nullptr;
}

Item *QuwiProjectListItem::item() const
{
    return m_item;
}

void QuwiProjectListItem::setItem(Item *quwiProject)
{
    m_item = quwiProject;
}
