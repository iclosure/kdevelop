/***************************************************************************
 *   This file is part of KDevelop                                         *
 *   Copyright 2007 Andreas Pakulat <apaku@gmx.de>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "vcsdiff.h"

#include <QString>
#include <QByteArray>
#include <QSharedData>

namespace KDevelop
{

class VcsDiffPrivate : public QSharedData
{
public:
    QHash<VcsLocation,QByteArray> leftBinaries;
    QHash<VcsLocation,QByteArray> rightBinaries;
    QHash<VcsLocation,QString> leftTexts;
    QHash<VcsLocation,QString> rightTexts;
    QUrl baseDiff;
    QString diff;
    VcsDiff::Type type = VcsDiff::DiffDontCare;
    VcsDiff::Content content = VcsDiff::Text;
    uint depth = 0;
};

VcsDiff::VcsDiff()
    : d(new VcsDiffPrivate)
{
}

VcsDiff::~VcsDiff() = default;

VcsDiff::VcsDiff( const VcsDiff& rhs )
    : d(rhs.d)
{
}

bool VcsDiff::isEmpty() const
{
    return d->diff.isEmpty() && d->leftBinaries.isEmpty() && d->rightBinaries.isEmpty()
                             && d->leftTexts.isEmpty() && d->rightTexts.isEmpty();
}

VcsDiff::Type VcsDiff::type() const
{
    return d->type;
}

VcsDiff::Content VcsDiff::contentType() const
{
    return d->content;
}

QHash<VcsLocation, QByteArray> VcsDiff::leftBinaries() const
{
    return d->leftBinaries;
}

QHash<VcsLocation, QByteArray> VcsDiff::rightBinaries() const
{
    return d->rightBinaries;
}


QHash<VcsLocation, QString> VcsDiff::leftTexts() const
{
    return d->leftTexts;
}

QHash<VcsLocation, QString> VcsDiff::rightTexts() const
{
    return d->rightTexts;
}

QString VcsDiff::diff() const
{
    return d->diff;
}


void VcsDiff::setDiff( const QString& s )
{
    d->diff = s;
}

void VcsDiff::addLeftBinary( const VcsLocation& loc, const QByteArray& b )
{
    d->leftBinaries[loc] = b;
}

void VcsDiff::addRightBinary( const VcsLocation& loc, const QByteArray& b )
{
    d->rightBinaries[loc] = b;
}

void VcsDiff::removeLeftBinary( const VcsLocation& loc )
{
    d->leftBinaries.remove( loc );
}

void VcsDiff::removeRightBinary( const VcsLocation& loc )
{
    d->rightBinaries.remove( loc );
}

void VcsDiff::addLeftText( const VcsLocation& loc, const QString& b )
{
    d->leftTexts[loc] = b;
}

void VcsDiff::addRightText( const VcsLocation& loc, const QString& b )
{
    d->rightTexts[loc] = b;
}

void VcsDiff::removeLeftText( const VcsLocation& loc )
{
    d->leftTexts.remove( loc );
}

void VcsDiff::removeRightText( const VcsLocation& loc )
{
    d->rightTexts.remove( loc );
}

void VcsDiff::setType( VcsDiff::Type t )
{
    d->type = t;
}

void VcsDiff::setContentType( VcsDiff::Content c )
{
    d->content = c;
}

VcsDiff& VcsDiff::operator=( const VcsDiff& rhs)
{
    d = rhs.d;
    return *this;
}

QUrl VcsDiff::baseDiff() const
{
    return d->baseDiff;
}

uint VcsDiff::depth() const
{
    return d->depth;
}

void VcsDiff::setBaseDiff(const QUrl& url)
{
    d->baseDiff=url;
}

void VcsDiff::setDepth(const uint depth)
{
    d->depth = depth;
}


}

