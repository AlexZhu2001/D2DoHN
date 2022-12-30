#include "aboutpage.h"
#include "ui_aboutpage.h"

AboutPage::AboutPage(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::AboutPage)
{
    ui->setupUi(this);
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser_2->setOpenExternalLinks(true);
}

AboutPage::~AboutPage()
{
    delete ui;
}
