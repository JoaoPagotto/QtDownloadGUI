#pragma once

#include <QObject>

#include <QCryptographicHash>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QFile>

#include <QTimer>

//-----------------------------------------------------------------------------

class HttpDownload : public QObject
{
   Q_OBJECT

public:
   explicit HttpDownload(QObject *parent = nullptr);
   ~HttpDownload();

   bool get(const QString &surl, const QString &pathfileName);
   void abort();
   QByteArray md5();

   int getTimeout() const { return timeout; }
   void setTimeout(int value) { timeout = value; }

private:
   QCryptographicHash *hash = nullptr;
   QNetworkAccessManager manager;
   QNetworkReply *reply = nullptr;
   QString fileName;
   QFile file;
   QTimer timer;

   int timeout = 5000;

private slots:
   void readyRead();
   void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
   void finished();
   void errorOccurred(QNetworkReply::NetworkError code);
   void timerTimeout();

signals:
   void fileProgress(qint64 bytesReceived, qint64 bytesTotal);
   void finishedOk();
   void finishedWithError();

};

//-----------------------------------------------------------------------------
