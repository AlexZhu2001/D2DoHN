#ifndef GENERALPAGE_H
#define GENERALPAGE_H

#include <QWidget>

namespace Ui {
class GeneralPage;
}

class GeneralPage : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralPage(QWidget* parent = nullptr);
    ~GeneralPage();

private:
    void laodOptions();

private:
    Ui::GeneralPage* ui;

signals:
    void reqConRestart();
};

#endif // GENERALPAGE_H
