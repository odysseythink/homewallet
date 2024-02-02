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

#ifndef __HB_MISC__H__
#define __HB_MISC__H__

#include <QtGlobal>
#include <QStringList>

enum
{
	PRF_DATEFMT_MDY,
	PRF_DATEFMT_DMY,
	PRF_DATEFMT_YMD,
	NUM_PRF_DATEFMT
};




#define HBTK_CHART_FLAGS_NONE		0
#define HBTK_CHART_FLAGS_HOVER		1 << 1
#define HBTK_CHART_FLAGS_SHADED		1 << 2


//TODO: get rid of unused here
enum {
    BLACK,
    WHITE,
    GREY1,
    TEXT,
    XYLINES,
    THBASE,
    THTEXT
};


enum colmap
{
    CHART_COLMAP_HOMEBANK,
    CHART_COLMAP_MSMONEY,
    CHART_COLMAP_SAP,
    CHART_COLMAP_QUICKEN,
    CHART_COLMAP_OFFICE2010,
    CHART_COLMAP_OFFICE2013,
    CHART_COLMAP_ANALYTICS,
    CHART_COLMAP_YNAB,
    CHART_COLMAP_QUICKEN2017,
    CHART_COLMAP_MINT,
    CHART_COLMAP_MATERIAL,
    CHART_COLMAP_NORD,
};


enum {
    CHART_FONT_SIZE_TITLE,
    CHART_FONT_SIZE_SUBTITLE,
    CHART_FONT_SIZE_NORMAL,
    CHART_FONT_SIZE_SMALL
};



double hb_amount_round(const double x, int n);
double hb_amount_base(double value, qint32 kcur);
bool hb_amount_type_match(double amount, int type);
bool hb_amount_equal(double val1, double val2);
int hb_amount_compare(double val1, double val2);
QString hb_filename_new_with_extension(QString filename, const QString extension);
QString hb_filename_new_for_backup(const QString filename);
QStringList hb_filename_backup_list(const QString filename);

#endif
