/*
    This file is part of libkcal.
    Copyright (c) 2002 Cornelius Schumacher <schumacher@kde.org>

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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "compat.h"

#include <kdebug.h>

#include <qregexp.h>

using namespace KCal;

Compat *CompatFactory::createCompat( const QString &productId )
{
  kdDebug(5800) << "CompatFactory::createCompat(): '" << productId << "'"
                << endl;

  Compat *compat = 0;

  int korg = productId.find( "KOrganizer" );
  kdDebug() << "korg: " << korg << endl;
  if ( korg >= 0 ) {
    int versionStart = productId.find( " ", korg );
    kdDebug() << "versionStart: " << versionStart << endl;
    if ( versionStart >= 0 ) {
      int versionStop = productId.find( QRegExp( "[ /]" ), versionStart + 1 );
      kdDebug() << "versionStop: " << versionStop << endl;
      if ( versionStop >= 0 ) {
        QString version = productId.mid( versionStart,
                                         versionStop - versionStart );
        kdDebug(5800) << "Found KOrganizer version: " << version << endl;
        
        int versionNum = version.section( ".", 0, 0 ).toInt() * 10000 +
                         version.section( ".", 1, 1 ).toInt() * 100 +
                         version.section( ".", 2, 2 ).toInt();
                         
        kdDebug(5800) << "Numerical version: " << versionNum << endl;
        
        if ( versionNum < 30100 ) {
          compat = new CompatPre31;
        }
      }
    }
  }
  
  if ( !compat ) compat = new Compat;

  return compat;
}

void CompatPre31::fixFloatingEnd( QDate &endDate )
{
  endDate = endDate.addDays( 1 );
}
