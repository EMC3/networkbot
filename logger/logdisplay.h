#ifndef LOGDISPLAY_H
#define LOGDISPLAY_H

#include <QWidget>
#include "loggerbackend.h"
#include <QTreeWidgetItem>
#include <QIcon>
#include <QTimer>
namespace Ui {
class LogDisplay;
}

class LogDisplay : public QWidget, public LogReciever
{
    Q_OBJECT

public:
    explicit LogDisplay(QWidget *parent = 0);
    ~LogDisplay();
    virtual void onNewLog(LogEntry & e);
    void paintEvent(QPaintEvent *e);
public slots:
    void addTree(QTreeWidgetItem * it);

signals:
    void doScroll();
    void logArrived(QTreeWidgetItem * it);
private slots:
    void scrollDown();
    void on_toolButton_2_clicked();

    void on_toolButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::LogDisplay *ui;
    QIcon warn, error, info;
    QString filter;
    bool filterActive;
    QTimer * tm;
    void applyFilter(QTreeWidgetItem *it);
};

#endif // LOGDISPLAY_H
