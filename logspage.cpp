#include "logspage.h"
#include "ui_logspage.h"
#include <QFileDialog>
#include <spdlog/sinks/qt_sinks.h>
#include <spdlog/spdlog.h>

LogsPage::LogsPage(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LogsPage)
{
    ui->setupUi(this);
    auto logger = spdlog::qt_logger_mt("Logger", ui->log);
    connect(ui->clear, &QPushButton::clicked, this, [this]() {
        this->ui->log->clear();
    });
    connect(ui->save, &QPushButton::clicked, this, [this]() {
        auto text = ui->log->toPlainText();
        auto fileName = QFileDialog::getSaveFileName(
            this, tr("Save Log"), "", "Log File (*.log)");
        QFile f(fileName);
        if (f.open(QFile::WriteOnly | QFile::Truncate)) {
            f.write(text.toUtf8());
            f.close();
            spdlog::get("Logger")->info("Save Log File success!");
        } else {
            spdlog::get("Logger")->error("!!!Cannot save to file!!!");
        }
    });
    hgl = new MyHighlighter(ui->log->document());
}

LogsPage::~LogsPage()
{
    delete ui;
}
