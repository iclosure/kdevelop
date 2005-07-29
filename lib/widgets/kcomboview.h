/* This file is part of the KDE project
   Copyright (C) 2003 Alexander Dymo <cloudtemple@mksat.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef KCOMBOVIEW_H
#define KCOMBOVIEW_H

#include <kcompletion.h>

#include "qcomboview.h"

/**
@file kcomboview.h
KComboView class.
*/

/**
KComboView - a combo with a QListView as a popup widget.
KComboView provides text completion.
@sa QComboView for a description.
*/
class KComboView: public QComboView
{
    Q_OBJECT
public:
    KComboView( bool rw, int defaultWidth = 100, QWidget* parent=0, const char* name=0 );

    virtual void addItem(QListViewItem *it);
    virtual void removeItem(QListViewItem *it);
    virtual void renameItem(QListViewItem *it, const QString &newName);

    virtual void setDefaultText(const QString &text);

    virtual void clear();

    int defaultWidth();
private:
    KCompletion m_comp;
    int m_defaultWidth;
    QString m_defaultText;
};

#endif
