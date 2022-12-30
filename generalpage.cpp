#include "generalpage.h"
#include "ui_generalpage.h"
#include <QRegularExpression>
#include <QSettings>
#include <spdlog/sinks/qt_sinks.h>
#include <spdlog/spdlog.h>

GeneralPage::GeneralPage(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::GeneralPage)
{
    ui->setupUi(this);
    laodOptions();
    connect(ui->dohUrl, &QLineEdit::editingFinished, this, [this]() {
        static const QRegularExpression exp(
            "^https://([-a-zA-Z0-9]*\\.)*([a-zA-Z0-9])*/[-a-zA-Z0-9/]*$");
        auto url = this->ui->dohUrl->text();
        auto res = exp.match(url);
        if (res.hasMatch()) {
            QSettings options;
            options.setValue("DoH_URL", url);
            ui->dohUrl->setStyleSheet("background:white;");
            spdlog::get("Logger")->info("DoH URL Changed, saved.");
        } else {
            ui->dohUrl->setStyleSheet("background:rgba(255,0,0,128);");
            spdlog::get("Logger")->error("!!!DoH URL is wrong!!!");
        }
    });
    connect(ui->dnsAddr, &QLineEdit::editingFinished, this, [this]() {
        static const QRegularExpression expv4(
            "^((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|["
            "0-"
            "1]?"
            "\\d{1,2})){3}$");
        static const QRegularExpression expv6(
            "\\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,"
            "4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)("
            "\\.("
            "25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:)"
            "{5}("
            "((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)"
            "(\\.("
            "25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:)"
            "{4}("
            "((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]"
            "\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){"
            "3}))|:))|("
            "([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{"
            "1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-"
            "4]\\d|"
            "1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-"
            "f]{"
            "1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|"
            "[1-"
            "9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-"
            "9A-Fa-f]"
            "{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:(("
            "25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|"
            "1\\d\\d|[1-9]?"
            "\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){"
            "0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|"
            "1\\d\\d|"
            "[1-9]?\\d)){3}))|:)))(%.+)?\\s*");
        auto ipAddr = this->ui->dnsAddr->text();
        auto v4res = expv4.match(ipAddr);
        auto v6res = expv6.match(ipAddr);
        if (v4res.hasMatch() || v6res.hasMatch()) {
            QSettings options;
            options.setValue("DNS_ADDR", ipAddr);
            ui->dnsAddr->setStyleSheet("background:white;");
            spdlog::get("Logger")->info("DNS Address Changed, saved.");
        } else {
            ui->dnsAddr->setStyleSheet("background:rgba(255,0,0,128);");
            spdlog::get("Logger")->error("!!!DNS Address is wrong!!!");
        }
    });
    connect(ui->allowLan, &QCheckBox::stateChanged, this, [this]() {
        QSettings options;
        options.setValue("allowLan", this->ui->allowLan->isChecked());
        spdlog::get("Logger")->info("Allow Lan Option Changed, saved.");
        emit reqConRestart();
    });
    connect(ui->enV4, &QCheckBox::stateChanged, this, [this]() {
        QSettings options;
        options.setValue("enV4", this->ui->enV4->isChecked());
        spdlog::get("Logger")->info("IPv4 Option Changed, saved.");
        emit reqConRestart();
    });
    connect(ui->enV6, &QCheckBox::stateChanged, this, [this]() {
        QSettings options;
        options.setValue("enV6", this->ui->enV6->isChecked());
        spdlog::get("Logger")->info("IPv6 Option Changed, saved.");
        emit reqConRestart();
    });
}

GeneralPage::~GeneralPage()
{
    delete ui;
}

void
GeneralPage::laodOptions()
{
    QSettings options;
    if (options.contains("DoH_URL")) {
        ui->dohUrl->setText(options.value("DoH_URL").toString());
    }
    if (options.contains("DNS_ADDR")) {
        ui->dnsAddr->setText(options.value("DNS_ADDR").toString());
    }
    if (options.contains("allowLan")) {
        ui->allowLan->setChecked(options.value("allowLan").toBool());
    }
    if (options.contains("enV4")) {
        ui->enV4->setChecked(options.value("enV4").toBool());
    }
    if (options.contains("enV6")) {
        ui->enV6->setChecked(options.value("enV6").toBool());
    }
}
