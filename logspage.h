#ifndef LOGSPAGE_H
#define LOGSPAGE_H

#include "myhighlighter.h"
#include <QWidget>

namespace Ui {
class LogsPage;
}

class LogsPage : public QWidget
{
    Q_OBJECT

public:
    explicit LogsPage(QWidget* parent = nullptr);
    ~LogsPage();

private:
    Ui::LogsPage* ui;
    MyHighlighter* hgl;
};

#endif // LOGSPAGE_H
