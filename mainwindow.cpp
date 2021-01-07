#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>

//-----------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   reset();

   connect(&hd, &HttpDownload::fileProgress,
           this, &MainWindow::fileProgress);

   connect(&hd, &HttpDownload::finishedOk,
           this, &MainWindow::finishedOk);

   connect(&hd, &HttpDownload::finishedWithError,
           this, &MainWindow::finishedWithError);

}

//-----------------------------------------------------------------------------

MainWindow::~MainWindow()
{
   delete ui;
}

//-----------------------------------------------------------------------------

void MainWindow::on_btnDownload_clicked()
{
   if ( ui->edURL->text().isEmpty() ){
      ui->lbInfo->setText("Please !!! Define URL");
      ui->edURL->setFocus();
   } else {
      hd.get(ui->edURL->text(), QFileInfo(ui->edURL->text()).fileName());

      ui->progressBar->setVisible(true);
      ui->btnDownload->setEnabled(false);
      ui->btnCancel->setEnabled(true);
   }
}

//-----------------------------------------------------------------------------

void MainWindow::on_btnCancel_clicked()
{
   hd.abort();
}

//-----------------------------------------------------------------------------

void MainWindow::fileProgress(qint64 bytesReceived, qint64 bytesTotal)
{
   if ( bytesTotal > 0 ) {
      ui->progressBar->setMaximum(bytesTotal);
      ui->progressBar->setValue(bytesReceived);

      ui->lbInfo->setText(QString("Receiving %1 of %2 bytes").arg(QString::number(bytesReceived), QString::number(bytesTotal)));
   } else {
      ui->progressBar->setRange(0, 0);

      ui->lbInfo->setText(QString("Receiving %1 bytes").arg(QString::number(bytesReceived)));
   }
}

//-----------------------------------------------------------------------------

void MainWindow::finishedOk()
{
   reset();

   ui->lbInfo->setText(QString("File downloaded. Checksum MD5:").append(hd.md5().toHex()));
}

//-----------------------------------------------------------------------------

void MainWindow::finishedWithError()
{
   reset();

   ui->lbInfo->setText("## Fail ##");
}

//-----------------------------------------------------------------------------

void MainWindow::reset()
{
   ui->progressBar->setVisible(false);

   ui->btnDownload->setEnabled(true);
   ui->btnCancel->setEnabled(false);
}

//-----------------------------------------------------------------------------
