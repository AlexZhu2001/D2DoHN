#include "selector.h"
#include "ui_selector.h"

#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QTimer>
#include <QTransform>

Selector::Selector(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Selector)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    addBtnToGrp(0, ui->general, ui->logs, ui->about);
    connect(
        &btnGrp, &QButtonGroup::idClicked, this, &Selector::selectionChanged);
}

Selector::~Selector()
{
    delete ui;
}

void
Selector::mousePressEvent(QMouseEvent* event)
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
Selector::mouseReleaseEvent(QMouseEvent* event)
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
Selector::mouseMoveEvent(QMouseEvent* event)
{
    if (isLeftButtonDown) {
        auto oldPos = lastMousePos;
        auto newPos = event->globalPos();
        auto deltaPos = newPos - oldPos;
        emit moveWindowPos(deltaPos);
        lastMousePos = newPos;
    }
}

void
Selector::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
