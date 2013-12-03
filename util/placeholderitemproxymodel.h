/*
 * Copyright 2013 Kevin Funk <kevin@kfunk.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KDEVPLATFORM_PLACEHOLDERITEMPROXYMODEL_H
#define KDEVPLATFORM_PLACEHOLDERITEMPROXYMODEL_H

#include "utilexport.h"

#include <KIdentityProxyModel>

#include <QScopedPointer>

namespace KDevelop {

/**
 * Proxy model adding a placeholder item for new entries
 *
 * This is mostly a KIdentityProxyModel, with one additional row added at the end
 *
 * Example use:
 *
 * @code
 * PlaceholderItemProxyModel* proxyModel = new PlaceholderItemProxyModel;
 * proxyModel->setSourceModel(new MyItemModel);
 * proxyModel->setHint("(Add new entry)");
 * connect(proxyModel, SIGNAL(dataInserted(...), SLOT(handleDataInserted(...));
 * @endcode
 *
 * In this case MyItemModel has exactly two entries, "Item1" and "Item2"
 *
 * This will end up in PlaceholderItemProxyModel holding the following indices:
 * - "Item1" (from source model)
 * - "Item2" (from source model)
 * - "(Add new entry)" (from PlaceholderItemProxyModel)
 *
 * In case the last entry is edited, and a non-empty value is supplied,
 * dataInserted() is emitted to notify the user about newly created rows.
 * The user then makes sure the requested change is handled in the source model.
 *
 * @see dataInserted
 *
 * @note WARNING: This implementation is only suitable for flat models
 * It will fall apart when you use a tree model as source
 */
class KDEVPLATFORMUTIL_EXPORT PlaceholderItemProxyModel : public KIdentityProxyModel
{
    Q_OBJECT

public:
    explicit PlaceholderItemProxyModel(QObject* parent = 0);
    virtual ~PlaceholderItemProxyModel();

    QVariant hint() const;
    void setHint(const QVariant& hint);

    int hintColumn() const;
    void setHintColumn(int column);

    virtual void setSourceModel(QAbstractItemModel* sourceModel);

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& proxyIndex, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual QModelIndex buddy(const QModelIndex& index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

    virtual QModelIndex mapToSource(const QModelIndex& proxyIndex) const;

    /**
     * Implement in subclass.
     *
     * @return True in case the input was valid, and the filter should notify
     *   external observers via the dataInserted signal.
     *
     * @sa dataInserted()
     */
    virtual bool validateRow(const QModelIndex& index, const QVariant& value) const = 0;

Q_SIGNALS:
    void dataInserted(const QVariant& values);

private:
    struct Private;
    QScopedPointer<Private> const d;
};

}

#endif // KDEVPLATFORM_PLACEHOLDERITEMPROXYMODEL_H
