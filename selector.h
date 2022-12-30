#ifndef SELECTOR_H
#define SELECTOR_H

#include <QButtonGroup>
#include <QWidget>

template<typename T>
concept Button = requires(T a) {
                     {
                         a
                         } -> std::convertible_to<QAbstractButton*>;
                 };

namespace Ui {
class Selector;
}

class Selector : public QWidget
{
    Q_OBJECT

public:
    explicit Selector(QWidget* parent = nullptr);
    ~Selector();

private:
    Ui::Selector* ui;
    QButtonGroup btnGrp;
    bool isLeftButtonDown = false;
    QPoint lastMousePos;

private:
    template<Button T0, Button... T>
    void addBtnToGrp(int start, T0 btn, T... btns)
    {
        btnGrp.addButton(btn, start);
        if constexpr (sizeof...(btns) > 0) {
            addBtnToGrp(start + 1, btns...);
        }
    }

signals:
    void selectionChanged(int index);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

signals:
    void moveWindowPos(const QPoint& pos);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // SELECTOR_H
