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


//// used to convert from national to euro currency
//// used in hb_account.c :: only when convert account to euro
//// round option is to 0.5 case so 1.32 is 1.3, but 1.35 is 1.4

double hb_amount_to_euro(double amount)
{
    return hb_amount_round((amount * Preferences::Instance()->euro_value), Preferences::Instance()->minor_cur->frac_digits);
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
    QDir dir(Preferences::Instance()->path_hbbak);
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
    QDate date;
    QFileInfo fi(filename);

    basename = fi.baseName();
    rawfilename = hb_filename_new_without_extension(basename);

    date.fromJulianDay(HomeWallet::Instance()->today);
    newfilename = QString::asprintf("%s-%04d%02d%02d.bak",
                rawfilename.toStdString().c_str(),
                date.year(),
                date.month(),
                date.day()
                );

    newfilepath = QDir(Preferences::Instance()->path_hbbak).absoluteFilePath(newfilename);

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


///* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/


//gboolean hb_string_isdate(gchar *str)
//{
//gint d, m, y;

//	return(hb_date_parser_get_nums(str, &d, &m, &y));
//}


//gboolean hb_string_isdigit(gchar *str)
//{
//gboolean valid = TRUE;
//	while(*str && valid)
//		valid = g_ascii_isdigit(*str++);
//	return valid;
//}

///*
//gboolean hb_string_isprint(gchar *str)
//{
//gboolean valid = TRUE;
//	while(*str && valid)
//		valid = g_ascii_isprint(*str++);
//	return valid;
//}
//*/



//gboolean hb_string_isprint(gchar *str)
//{
//gboolean valid = TRUE;
//gchar *p;
//gunichar c;

//	if(g_utf8_validate(str, -1, NULL))
//	{
//		p = str;
//		while(*p && valid)
//		{
//			c = g_utf8_get_char(p);
//			valid = g_unichar_isprint(c);
//			p = g_utf8_next_char(p);
//		}
//	}
//	return valid;
//}


//gchar *hb_sprint_date(gchar *outstr, quint32 julian)
//{
//GDate date;

//	g_date_clear(&date, 1);
//	g_date_set_julian (&date, julian);
//	switch(Preferences::Instance()->dtex_datefmt)
//	{
//		//#2040010 change / to -
//		case PRF_DATEFMT_MDY:
//		{
//			g_sprintf(outstr, "%02d-%02d-%04d",
//				g_date_get_month(&date),
//				g_date_get_day(&date),
//				g_date_get_year(&date)
//				);
//		}
//		break;
//		case PRF_DATEFMT_DMY:
//		{
//			g_sprintf(outstr, "%02d-%02d-%04d",
//				g_date_get_day(&date),
//				g_date_get_month(&date),
//				g_date_get_year(&date)
//				);
//		}
//		break;
//		default:
//			g_sprintf(outstr, "%04d-%02d-%02d",
//				g_date_get_year(&date),
//				g_date_get_month(&date),
//				g_date_get_day(&date)
//				);
//			break;
//	}
//	return outstr;
//}


////used only in DB() macro !!
//void hb_print_date(quint32 jdate, gchar *label)
//{
//gchar buffer1[128];
//GDate *date;

//	date = g_date_new_julian(jdate);
//	g_date_strftime (buffer1, 128-1, "%x", date);
//	g_date_free(date);
//	g_print(" - %s %s\n", label != NULL ? label:"date is", buffer1);
//}



///*
//** parse a string an retrieve an iso date (dd-mm-yy(yy) or dd/mm/yy(yy))
//**
//*/
///* obsolete 4.5
//quint32 hb_date_get_julian_parse(gchar *str)
//{
//gchar **str_array = NULL;
//GDate *date;
//quint d, m, y;
//quint32 julian = HomeWallet::Instance()->today;

//	// try with - separator
//	if( g_strrstr(str, "-") != NULL )
//	{
//		str_array = g_strsplit (str, "-", 3);
//	}
//	else
//	{
//		if( g_strrstr(str, "/") != NULL )
//		{
//			str_array = g_strsplit (str, "/", 3);
//		}
//	}

//	if( g_strv_length( str_array ) == 3 )
//	{
//		d = atoi(str_array[0]);
//		m = atoi(str_array[1]);
//		y = atoi(str_array[2]);

//		//correct for 2 digits year
//		if(y < 1970)
//		{
//			if(y < 60)
//				y += 2000;
//			else
//				y += 1900;
//		}

//		//todo: here if month is > 12 then the format is probably mm/dd/yy(yy)
//		//or maybe check with g_date_valid_julian(julian)



//		date = g_date_new();
//		g_date_set_dmy(date, d, m, y);
//		julian = g_date_get_julian (date);
//		g_date_free(date);

//		qDebug("date: %s :: %d %d %d :: %d\n", str, d, m, y, julian ) );

//	}

//	g_strfreev (str_array);

//	return julian;
//}
//*/

///* -------------------- */

//#if MYDEBUG == 1

///*
//** hex memory dump
//*/
//#define MAX_DUMP 16
//void hex_dump(guchar *ptr, quint length)
//{
//guchar ascii[MAX_DUMP+4];
//quint i,j;

//	g_print("**hex_dump - %d bytes\n", length);

//	for(i=0;i<length;)
//	{
//        g_print("%08x: ", (quint)ptr+i);

//		for(j=0;j<MAX_DUMP;j++)
//		{
//			if(i >= length) break;

//			//store ascii value
//			if(ptr[i] >= 32 && ptr[i] <= 126)
//				ascii[j] = ptr[i];
//			else
//				ascii[j] = '.';

//			g_print("%02x ", ptr[i]);
//			i++;
//		}
//		//newline
//		ascii[j] = 0;
//		g_print(" '%s'\n", ascii);
//	}
//}

//#endif
