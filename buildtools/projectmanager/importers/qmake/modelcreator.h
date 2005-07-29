/***************************************************************************
 *   Copyright (C) 2005 by Alexander Dymo                                  *
 *   adymo@kdevelop.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/
#ifndef MODELCREATOR_H
#define MODELCREATOR_H

#include "qmakeprojectmodel.h"

class QFileInfo;

namespace QMake {
    class AST;
}

namespace ModelCreator {

QMake::ProjectAST * buildProjectAST(const QString &projectDir);

//pass ast = 0 when you need to create it
void newFolderDom(ProjectFolderList &folderList,
    ProjectFolderDom &folderDom, QMake::AST *ast, QFileInfo *info = 0);

void newFileDom(QMakeFileList &fileList,
                QMakeFileDom &fileDom, QMake::AST *ast, QFileInfo *info = 0);

void addFileItem( const QString &field, const QString &icon, QMakeFolderDom &folder );
}

#endif
