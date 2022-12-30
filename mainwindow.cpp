#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , trayIcon(new QSystemTrayIcon{ this })
    , trayCtxMenu(new QMenu(this))
{
    ui->setupUi(this);
    // Set Window Properties
    setFixedSize(QSize(986, 625));
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("D2DoH NG");
    setWindowIcon(QIcon(":/icons/imgs/dns.png"));
    // Build 3 Pages
    GeneralPage* gPage = new GeneralPage;
    LogsPage* lPage = new LogsPage;
    AboutPage* aPage = new AboutPage;
    addToStack(gPage, lPage, aPage);
    // Init System Tray
    trayIcon->setIcon(QIcon(":/icons/imgs/dns.png"));
    trayIcon->setToolTip("D2DoH NG");
    trayIcon->setContextMenu(trayCtxMenu);
    // Init Tray Menu
    trayCtxMenu->addAction(
        tr("Show D2DoH NG"), this, [this]() { this->show(); });
    trayCtxMenu->addAction(
        tr("Exit App"), this, []() { QApplication::exit(); });
    cont = new Container;
    // Connect all signals and slots
    connect(ui->selector,
            &Selector::selectionChanged,
            ui->pages,
            &QStackedWidget::setCurrentIndex);
    connect(ui->titleBar,
            &TitleBar::moveWindowPos,
            this,
            &MainWindow::onMoveWindow);
    connect(ui->selector,
            &Selector::moveWindowPos,
            this,
            &MainWindow::onMoveWindow);
    connect(ui->titleBar, &TitleBar::minimizePressed, this, [this]() {
        this->setWindowState(Qt::WindowMinimized);
    });
    connect(ui->titleBar, &TitleBar::closePressed, this, [this]() {
        this->close();
    });
    connect(trayIcon,
            &QSystemTrayIcon::activated,
            this,
            [this](QSystemTrayIcon::ActivationReason reason) {
                switch (reason) {
                    case QSystemTrayIcon::ActivationReason::Trigger:
                        this->show();
                        break;
                    default:
                        break;
                }
            });
    connect(
        gPage, &GeneralPage::reqConRestart, this, &MainWindow::onRestartCont);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cont;
}

void
MainWindow::showSystemTrayIcon()
{
    this->trayIcon->show();
}

void
MainWindow::onMoveWindow(const QPoint& pos)
{
    auto leftTopPos = mapToGlobal(this->rect().topLeft());
    leftTopPos += pos;
    move(leftTopPos);
}

void
MainWindow::onRestartCont()
{
    delete cont;
    cont = new Container;
}
