#include "logdisplay.h"
#include "ui_logdisplay.h"
#include <QFile>
#include "logger/log.h"
#include "logger/loggerbackend.h"
#include <QDebug>
#include <QScrollBar>
LogDisplay::LogDisplay(QWidget *parent) :
    QWidget(parent), info(":/icons/icons/Info.png"), warn(":/icons/icons/Warning.png"), error(":/icons/icons/Error.png"),
    ui(new Ui::LogDisplay)
{
    this->setAutoFillBackground(true);
    QFile ss(":/qdarkstyle/style.qss");
    ss.open(QFile::ReadOnly);
    QString styleSheet = ss.readAll();

    ss.close();

    ui->setupUi(this);

    ui->lineEdit->setVisible(false);
    filterActive = false;

    this->setStyleSheet(styleSheet);
    this->setAutoFillBackground(true);
    LoggerBackend * be = Logger::getBackend();

    be->registerReciever(this);

    LOG << "Registered";

    tm = new QTimer(this);
    tm->setSingleShot(true);
    tm->setInterval(1);
    connect(this, SIGNAL(logArrived(QTreeWidgetItem*)), this, SLOT(addTree(QTreeWidgetItem*)));
    connect(tm, SIGNAL(timeout()), this, SLOT(scrollDown()));

    for(LogEntry e : be->getCurrentEntries()){
        onNewLog(e);
    }


}

LogDisplay::~LogDisplay()
{
    LoggerBackend * be = Logger::getBackend();
    be->removeReciever(this);
    delete ui;
}

void LogDisplay::onNewLog(LogEntry &e)
{
    QStringList entries;
    entries << QString::fromStdString(e.timestamp);
    entries << (e.file > 0 ? (e.file+QLatin1String(":")+QString::number(e.line)) : "UNKNOWN");
    entries << QString::fromStdString(e.string);
    QTreeWidgetItem * it = new QTreeWidgetItem(entries);
    if(e.level == 1)it->setIcon(2, warn);
    else if(e.level == 2)it->setIcon(2, error);
    else it->setIcon(2, info);
    emit logArrived(it);
}


#include<QPainter>
void  LogDisplay::paintEvent(QPaintEvent * e){
    QPainter f(this);
    // Manually fill background because of some shitty bug somewhere else...
    f.fillRect(QRect(0,0, this->width(), this->height()),QColor(0x30, 0x2F, 0x2F));
}

void LogDisplay::addTree(QTreeWidgetItem *it)
{
    QScrollBar * vert = ui->treeWidget->verticalScrollBar();
    bool scroll = false;
    if(vert->value() >= vert->maximum()-1)scroll = true;
    if(filterActive)applyFilter(it);
    ui->treeWidget->addTopLevelItem(it);
    if(scroll){
        tm->start(1);
    }
}

void LogDisplay::scrollDown(){
    QScrollBar * vert = ui->treeWidget->verticalScrollBar();
    vert->setValue(vert->maximum());
}

void LogDisplay::applyFilter(QTreeWidgetItem * it){
    if(filter.length() == 0 || it->text(1).contains(filter, Qt::CaseInsensitive))it->setHidden(false);
    else it->setHidden(true);
}

void LogDisplay::on_toolButton_2_clicked()
{
    ui->lineEdit->setVisible(!ui->lineEdit->isVisible());

    filterActive = ui->lineEdit->isVisible();
    filter = filterActive ? ui->lineEdit->text() : "";
    int n = ui->treeWidget->topLevelItemCount();
    for(int i = 0; i < n; i++){
        applyFilter(ui->treeWidget->topLevelItem(i));
    }

}

void LogDisplay::on_toolButton_clicked()
{
    ui->treeWidget->clear();
}

void LogDisplay::on_lineEdit_textChanged(const QString &arg1)
{
    filter = arg1;
    int n = ui->treeWidget->topLevelItemCount();
    for(int i = 0; i < n; i++){
        applyFilter(ui->treeWidget->topLevelItem(i));
    }
}
