#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

namespace Ui {
class TitleBar;
}

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget* parent = nullptr);
    ~TitleBar();

private:
    Ui::TitleBar* ui;
    bool isLeftButtonDown = false;
    QPoint lastMousePos;
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

signals:
    void moveWindowPos(const QPoint& pos);
    void closePressed();
    void minimizePressed();
};

#endif // TITLEBAR_H
