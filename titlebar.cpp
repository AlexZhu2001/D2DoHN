#include "titlebar.h"
#include "ui_titlebar.h"
#include <QDebug>
#include <QMouseEvent>

TitleBar::TitleBar(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TitleBar)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    connect(ui->close, &QPushButton::clicked, this, &TitleBar::closePressed);
    connect(
        ui->minimize, &QPushButton::clicked, this, &TitleBar::minimizePressed);
}

TitleBar::~TitleBar()
{
    delete ui;
}

void
TitleBar::mousePressEvent(QMouseEvent* event)
{
    switch (event->button()) {
        case Qt::LeftButton:
            isLeftButtonDown = true;
            lastMousePos = event->globalPos();
            break;
        default:
            QWidget::mousePressEvent(event);
            break;
    }
}

void
TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
    switch (event->button()) {
        case Qt::LeftButton:
            isLeftButtonDown = false;
            lastMousePos = QPoint();
            break;
        default:
            QWidget::mousePressEvent(event);
            break;
    }
}

void
TitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (isLeftButtonDown) {
        auto oldPos = lastMousePos;
        auto newPos = event->globalPos();
        auto deltaPos = newPos - oldPos;
        emit moveWindowPos(deltaPos);
        lastMousePos = newPos;
    }
}
