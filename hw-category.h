#ifndef __HB_CATEGORY_H__
#define __HB_CATEGORY_H__

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QWidget>
#include <QList>
#include <QQueue>
#include <QUuid>
#include <QStandardItem>
#include "hw-file.h"

class Category: public IHWFileIO
{
    Q_OBJECT
public:
    Category(QObject* parent = nullptr):IHWFileIO(parent){}
    Category(Category* other);
    ~Category();
    virtual QString CreateTableSql() override;
    virtual QString DataSql() override;
    virtual QString Tabname() override;
    virtual void LoadFromSqlQuery(QSqlQuery& query) override;
    static QList<QSharedPointer<Category> > get_all_by_type(const QString type="expense");
    static bool add_to_local(Category *item);
    static bool del_from_local(const QString& id);
    static bool update_type_to_local(const QString& id, const QString& type);
    static QMap<QString, QString> get_all_fullnames(const QString& type);
    static QList<QStandardItem*> ParseCategoriesToItems(const QString parentname,QList<QSharedPointer<Category> > &list, QMap<QString, QString>* fullnames = nullptr);

public:
    QString type;
    QString		id;
    QString		parent;
	unsigned short		flags;
	unsigned short		_pad1;
    QString name;
	double		budget[13];	//0:is same value, 1 ..12 are months

	/* unsaved datas */
    QString fullname;
	//bool	flt_select;
	quint16		nb_use_txn;
	quint16		nb_use_txncat;	//cat usage only
	quint16		nb_use_all;
	quint16		nb_use_allcat;	//cat usage only
};

#define GF_SUB		(1<<0)		//? useful as we can check item->parent= 0
#define GF_INCOME	(1<<1)
#define GF_CUSTOM	(1<<2)
#define GF_BUDGET	(1<<3)
#define GF_FORCED	(1<<4)
#define GF_MIXED	(1<<5)		//#1740368 if cat has mixed exp/inc type

#define GF_HIDDEN	(1<<8)		//hidden by user


#endif
