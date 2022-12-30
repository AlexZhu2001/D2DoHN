#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "container.h"
#include "ui_mainwindow.h"
#include <AboutPage.h>
#include <GeneralPage.h>
#include <LogsPage.h>
#include <QMainWindow>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

template<typename T>
concept Page = requires(T a) {
                   {
                       a
                       } -> std::convertible_to<QWidget*>;
               };
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void showSystemTrayIcon();

private:
    template<Page P0, Page... P>
    void addToStack(P0 page, P... pages)
    {
        this->ui->pages->addWidget(page);
        if constexpr (sizeof...(pages) > 0) {
            addToStack(pages...);
        }
    }

private:
    Ui::MainWindow* ui;
    QSystemTrayIcon* trayIcon;
    QMenu* trayCtxMenu;
    Container* cont = nullptr;

private slots:
    void onMoveWindow(const QPoint& pos);
    void onRestartCont();
};
#endif // MAINWINDOW_H
