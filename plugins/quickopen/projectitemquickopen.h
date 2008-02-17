/* This file is part of the KDE libraries
   Copyright (C) 2007 David Nolden <david.nolden.kdevelop@art-master.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef PROJECT_ITEM_QUICKOPEN
#define PROJECT_ITEM_QUICKOPEN

#include "duchainitemquickopen.h"

class ProjectItemDataProvider : public DUChainItemDataProvider {
public:
  enum ItemTypes {
    NoItems = 0,
    Classes = 1,
    Functions = 2,
    AllItemTypes = Classes + Functions
  };
  
  ProjectItemDataProvider( KDevelop::IQuickOpen* quickopen );

  virtual void enableData( const QStringList& items, const QStringList& scopes );

  virtual void reset();

  static QStringList supportedItemTypes();
private:
  ItemTypes m_itemTypes;
};

#endif
