#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#define ORG_NAME "Alex"
#define ORG_DOMAIN "alex.org"
#define APP_NAME "D2DoHN"

int
main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName(ORG_NAME);
    QCoreApplication::setOrganizationDomain(ORG_DOMAIN);
    QCoreApplication::setApplicationName(APP_NAME);
    a.setQuitOnLastWindowClosed(false);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    qDebug() << uiLanguages;
    for (const QString& locale : uiLanguages) {
        const QString baseName = "D2DoHN_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    w.showSystemTrayIcon();
    return a.exec();
}
