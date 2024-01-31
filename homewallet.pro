QT       += core gui sql widgets charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    account_mgr_dialog.cpp \
    account_sum_widget.cpp \
    bottom_future_lists_widget.cpp \
    bottom_lists_widget.cpp \
    bottom_remind_lists_widget.cpp \
    bottom_scheduled_lists_widget.cpp \
    category_mgr_dialog.cpp \
    hw-account.cpp \
    hw-archive.cpp \
    hw-assign.cpp \
    hw-category.cpp \
    hw-currency.cpp \
    hw-file.cpp \
    hw-filter.cpp \
    hw-group.cpp \
    hw-misc.cpp \
    hw-payee.cpp \
    hw-preferences.cpp \
    hw-split.cpp \
    hw-tag.cpp \
    hw-transaction.cpp \
    help_welcome_dialog.cpp \
    home_wallet.cpp \
    main.cpp \
    main_win.cpp \
    payee_mgr_dialog.cpp \
    time_chart_widget.cpp \
    total_chart_widget.cpp \
    transaction_add_dialog.cpp \
    transaction_mgr_dialog.cpp \
    ui-widgets-data.cpp

HEADERS += \
    account_mgr_dialog.h \
    account_sum_widget.h \
    bottom_future_lists_widget.h \
    bottom_lists_widget.h \
    bottom_remind_lists_widget.h \
    bottom_scheduled_lists_widget.h \
    category_mgr_dialog.h \
    config.h \
    enums.h \
    hw-account.h \
    hw-archive.h \
    hw-assign.h \
    hw-category.h \
    hw-currency.h \
    hw-file.h \
    hw-filter.h \
    hw-group.h \
    hw-misc.h \
    hw-payee.h \
    hw-preferences.h \
    hw-split.h \
    hw-tag.h \
    hw-transaction.h \
    help_welcome_dialog.h \
    home_wallet.h \
    main_win.h \
    payee_mgr_dialog.h \
    time_chart_widget.h \
    total_chart_widget.h \
    transaction_add_dialog.h \
    transaction_mgr_dialog.h

FORMS += \
    account_mgr_dialog.ui \
    account_sum_widget.ui \
    bottom_future_lists_widget.ui \
    bottom_lists_widget.ui \
    bottom_remind_lists_widget.ui \
    bottom_scheduled_lists_widget.ui \
    category_edit_dialog.ui \
    category_mgr_dialog.ui \
    help_welcome_dialog.ui \
    main_win.ui \
    payee_edit_dialog.ui \
    payee_mgr_dialog.ui \
    time_chart_widget.ui \
    total_chart_widget.ui \
    transaction_add_dialog.ui \
    transaction_mgr_dialog.ui

TRANSLATIONS += \
    homewallet_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/res.qrc
