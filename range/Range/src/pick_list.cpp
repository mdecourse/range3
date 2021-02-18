/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   pick_list.cpp                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   31-st October 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Pick list class definition                          *
 *********************************************************************/

#include <QMap>

#include "pick_list.h"

PickList::PickList(QObject *parent)
    : QObject(parent)
    , multipleSelection(true)
{
    R_LOG_TRACE;
}

bool PickList::getMultipleSelection() const
{
    R_LOG_TRACE;
    return this->multipleSelection;
}

void PickList::setMultipleSelection(bool multipleSelection)
{
    R_LOG_TRACE;
    this->multipleSelection = multipleSelection;
}

const QVector<PickItem> &PickList::getItems(void) const
{
    R_LOG_TRACE;
    return this->items;
}

QVector<PickItem> PickList::getItems(uint modelID) const
{
    R_LOG_TRACE;
    QVector<PickItem> modelItems;
    QVector<PickItem>::const_iterator iter = this->items.begin();

    while (iter != this->items.end())
    {
        const PickItem *pPickItem = iter;
        if (pPickItem->getEntityID().getMid() == modelID)
        {
            modelItems.append(*iter);
        }
        ++iter;
    }

    return modelItems;
}

uint PickList::getNItems(PickItemType type, uint modelID) const
{
    R_LOG_TRACE;
    uint nItems = 0;
    QVector<PickItem>::const_iterator iter = this->items.begin();

    while (iter != this->items.end())
    {
        const PickItem *pPickItem = iter;
        if (pPickItem->getItemType() == type)
        {
            if (modelID == RConstants::eod || modelID == pPickItem->getEntityID().getMid())
            {
                nItems++;
            }
        }
        ++iter;
    }
    return nItems;
}

bool PickList::isEmpty(void) const
{
    R_LOG_TRACE;
    return this->items.isEmpty();
}

bool PickList::isEmpty(uint modelID, REntityGroupType elementGroupType) const
{
    R_LOG_TRACE;
    QVector<PickItem>::const_iterator iter = this->items.begin();

    while (iter != this->items.end())
    {
        const PickItem *pPickItem = iter;
        if (pPickItem->getEntityID().getMid() == modelID)
        {
            if (elementGroupType == R_ENTITY_GROUP_NONE || pPickItem->getEntityID().getType() == elementGroupType)
            {
                return false;
            }
        }
        ++iter;
    }
    return true;
}

QList<uint> PickList::getModelIDs(void) const
{
    R_LOG_TRACE;
    QList<uint> modelIDs;
    QVector<PickItem>::const_iterator iter;
    QMap<uint,uint> modelIDcnt;
    uint modelID;

    iter = this->items.begin();
    while (iter != this->items.end())
    {
        const PickItem *pPickItem = iter;
        modelID = pPickItem->getEntityID().getMid();
        modelIDcnt[modelID]++;
        if (modelIDcnt[modelID] == 1)
        {
            modelIDs.append(modelID);
        }
        ++iter;
    }

    return modelIDs;
}

bool PickList::registerItem(const PickItem &pickItem)
{
    R_LOG_TRACE;
    QVector<PickItem>::iterator iter = this->items.begin();

    bool itemErased = false;

    while (iter != this->items.end())
    {
        PickItem *pPickItem = iter;
        if ((*pPickItem) == pickItem)
        {
            this->items.erase(iter);
            itemErased = true;
        }
        else
        {
            ++iter;
        }
    }

    if (!this->multipleSelection)
    {
        iter = this->items.begin();
        while (iter != this->items.end())
        {
            PickItem *pPickItem = iter;
            if (pPickItem->getEntityID().getMid() == pickItem.getEntityID().getMid())
            {
                iter = this->items.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }

    if (!itemErased)
    {
        this->items.append(pickItem);
    }

    emit this->pickListChanged();

    return (!itemErased);
}

void PickList::addItem(const PickItem &pickItem)
{
    R_LOG_TRACE;
    QVector<PickItem>::iterator iter = this->items.begin();

    while (iter != this->items.end())
    {
        PickItem *pPickItem = iter;
        if ((*pPickItem) == pickItem)
        {
            return;
        }
        else
        {
            ++iter;
        }
    }

    if (!this->multipleSelection)
    {
        iter = this->items.begin();
        while (iter != this->items.end())
        {
            PickItem *pPickItem = iter;
            if (pPickItem->getEntityID().getMid() == pickItem.getEntityID().getMid())
            {
                iter = this->items.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }

    this->items.append(pickItem);

    emit this->pickListChanged();
}

void PickList::removeItem(const PickItem &pickItem)
{
    R_LOG_TRACE;
    QVector<PickItem>::iterator iter = this->items.begin();

    while (iter != this->items.end())
    {
        PickItem *pPickItem = iter;
        if ((*pPickItem) == pickItem)
        {
            this->items.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    emit this->pickListChanged();
}

void PickList::removeItems(uint modelID)
{
    R_LOG_TRACE;
    QVector<PickItem>::iterator iter = this->items.begin();

    while (iter != this->items.end())
    {
        PickItem *pPickItem = iter;
        if (pPickItem->getEntityID().getMid() == modelID)
        {
            this->items.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    emit this->pickListChanged();
}

bool PickList::hasItem(const PickItem &pickItem)
{
    R_LOG_TRACE;
    QVector<PickItem>::iterator iter = this->items.begin();

    while (iter != this->items.end())
    {
        PickItem *pPickItem = iter;
        if ((*pPickItem) == pickItem)
        {
            return true;
        }
    }
    return false;
}

void PickList::clear(void)
{
    R_LOG_TRACE;
    this->items.clear();

    emit this->pickListChanged();
}
