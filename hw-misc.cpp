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


//nota: this file should be renamed hb-utils

#include "home_wallet.h"
#include "hw-misc.h"
#include "hw-preferences.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDate>

///* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

static const double fac[9] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };

double hb_amount_round(const double d, int digits)
{
    double out;

    //fixed 5.6 MIN, not MAX... + #1977796
    digits = qMin(digits, 8);

    //initial formula
        //out = floor((d * fac[digits]) + 0.5) / fac[digits];
    //#1977796 fix rounding -0.5 : +0.5...
        //out = round(d * fac[digits]) / fac[digits];
    //#2018206 fix rounding -0.00...
        //out = ((long) (d < 0 ? (d * fac[digits]) - 0.5 : (d * fac[digits]) + 0.5)) / fac[digits];

    //#2022049 overflow on windows cause compiled 32bits long is int32 4 bytes...
    out = ((qint64) (d < 0 ? (d * fac[digits]) - 0.5 : (d * fac[digits]) + 0.5)) / fac[digits];
    //qDebug(" in:%17g out:%17g\n", d, out) );

    return out;
}


static QString hb_filename_new_without_extension(QString filename)
{
    QFileInfo fi(filename);
    QString sf = fi.completeSuffix();
    return filename.left(filename.size()-sf.size()-1);
}


//static gint hb_filename_backup_list_sort_func(gchar **a, gchar **b)
//{
//gint da = atoi( *a  + strlen(*a) - 12);
//gint db = atoi( *b  + strlen(*b) - 12);

//	return db - da;
//}


QStringList hb_filename_backup_list(const QString filename)
{
    QString dirname, basename;
    QString rawfilename, pattern;
    const QString tmpname;
//    GPatternSpec *pspec;
    QStringList array;

    qDebug("\n[util] filename backup list\n");
    QFileInfo fi(filename);
    dirname = fi.filePath();
    basename = fi.baseName();

    qDebug(" dir='%s' base='%s'\n", dirname.toStdString().c_str(), basename.toStdString().c_str());

    rawfilename = hb_filename_new_without_extension(basename);
    pattern = QString::asprintf("%s-????????.bak", rawfilename.toStdString().c_str());
	
//    pspec = g_pattern_spec_new(pattern);


    qDebug(" pattern='%s'\n", pattern.toStdString().c_str());
    QDir dir(Preferences::Instance()->BackupPath());
    QStringList filters;
     filters << pattern;
    dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
    dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式（后缀为.jpeg等图片格式）
    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
        if(mfi.isFile())
        {
            qDebug("matched file '%s'", mfi.fileName().toStdString().c_str());
            array << mfi.fileName();
        }
    }
    return array;
}


//gchar *
//hb_filename_backup_get_filtername(gchar *filename)
//{
//gchar *basename;
//gchar *rawfilename, *pattern;

//	qDebug("\n[util] filename backup get filtername\n") );

//	basename = g_path_get_basename(filename);

//	rawfilename = hb_filename_new_without_extension(basename);

//    pattern = QString::asprintf("%s*.[Bb][Aa][Kk]", rawfilename);

//	g_free(rawfilename);
//	g_free(basename);
	
//	return pattern;
//}


QString hb_filename_new_for_backup(const QString filename)
{
    QString basename, rawfilename, newfilename, newfilepath;
    QDate date = QDate::currentDate();
    QFileInfo fi(filename);

    basename = fi.baseName();
    rawfilename = hb_filename_new_without_extension(basename);

    newfilename = QString::asprintf("%s-%04d%02d%02d.bak",
                rawfilename.toStdString().c_str(),
                date.year(),
                date.month(),
                date.day()
                );

    newfilepath = QDir(Preferences::Instance()->BackupPath()).absoluteFilePath(newfilename);

    return newfilepath;
}


QString hb_filename_new_with_extension(QString filename, const QString extension)
{
    QString rawfilename, newfilename;

    qDebug("\n[util] filename new with extension\n");

    rawfilename = hb_filename_new_without_extension(filename);
    newfilename = QString("%1.%2").arg(rawfilename).arg(extension);

    qDebug(" - '%s' => '%s'\n", filename.toStdString().c_str(), newfilename.toStdString().c_str());

    return newfilename;
}



