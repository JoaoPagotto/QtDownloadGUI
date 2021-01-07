#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "HttpDownload.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

private slots:
   void on_btnDownload_clicked();

   void on_btnCancel_clicked();

   void fileProgress(qint64 bytesReceived, qint64 bytesTotal);
   void finishedOk();
   void finishedWithError();

private:
   Ui::MainWindow *ui;

   HttpDownload hd;

   void reset();

};
#endif // MAINWINDOW_H
