/*  HomeBank -- Free, easy, personal accounting for everyone.
 *  Copyright (C) 1995-2023 Maxime DOYEN
 *
 *  This file is part of HomeBank.
 *
 *  HomeBank is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  HomeBank is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __HB_SPLIT_H__
#define __HB_SPLIT_H__

#include <QtGlobal>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QWidget>
#include <QList>

//for the record, quicken is limited to 250
#define TXN_MAX_SPLIT 62
#define G_ASCII_DTOSTR_BUF_SIZE 39


class Split: public QObject
{
    Q_OBJECT
public:
    Split(QObject* parent = nullptr);
    ~Split();

public:
    QString		kcat;
    double		amount;
    QString memo;
	//unsaved data
    unsigned short		pos;
};





#endif
