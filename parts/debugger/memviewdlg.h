/***************************************************************************
                          memview.h  -  description                              
                             -------------------                                         
    begin                : Tue Oct 5 1999                                           
    copyright            : (C) 1999 by John Birch                         
    email                : jb.nz@writeme.com                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifndef _MEMVIEW_H_
#define _MEMVIEW_H_

#include <kdialog.h>

class KLineEdit;
class QMultiLineEdit;

/**
 * @author John Birch
 */

class MemoryView : public KDialog
{
    Q_OBJECT
    
public:
    MemoryView( QWidget *parent, const char *name );
    ~MemoryView();
    
signals:
    void disassemble(const QString &start, const QString &end);
    void memoryDump(const QString &start, const QString &end);
    void registers();
    void libraries();
    
public slots:
    void slotRawGDBMemoryView(char *buf);
    
private slots:
    void slotDisassemble();
    void slotMemoryDump();

private:
    KLineEdit *start_, *end_;
    QMultiLineEdit *output_;
};

#endif
