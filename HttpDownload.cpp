#include "HttpDownload.h"

//-----------------------------------------------------------------------------

HttpDownload::HttpDownload(QObject *parent) : QObject(parent)
{
   hash = new QCryptographicHash(QCryptographicHash::Md5);

   //------------------------

   timer.setSingleShot(true);
   timer.setInterval(timeout);

   connect(&timer, &QTimer::timeout,
           this, &HttpDownload::timerTimeout);
}

//-----------------------------------------------------------------------------

HttpDownload::~HttpDownload()
{
   delete hash;
}

//-----------------------------------------------------------------------------

bool HttpDownload::get(const QString &surl, const QString &pathfileName)
{
   hash->reset();

   reply = manager.get(QNetworkRequest(QUrl(surl)));

   //------------------------

   fileName = QString(pathfileName).append("_temp");
   file.setFileName(fileName);
   if ( file.exists() )
      file.remove();

   if ( file.open( QIODevice::WriteOnly) == false )
      return false;

   //------------------------

   connect(reply, &QNetworkReply::readyRead,
           this, &HttpDownload::readyRead);

   connect(reply, &QNetworkReply::downloadProgress,
           this, &HttpDownload::downloadProgress);

   connect(reply, &QNetworkReply::finished,
           this, &HttpDownload::finished);

   connect(reply, &QNetworkReply::errorOccurred,
           this, &HttpDownload::errorOccurred);

   //------------------------

   timer.start();

   return true;
}

//-----------------------------------------------------------------------------

void HttpDownload::abort()
{
   reply->abort();
}

//-----------------------------------------------------------------------------

QByteArray HttpDownload::md5()
{
   return hash->result();
}

//-----------------------------------------------------------------------------

void HttpDownload::readyRead()
{
   timer.start();

   QByteArray data = reply->readAll();

   if ( file.write(data) == -1 ){
      reply->abort();
      return;
   }

   hash->addData(data);
}

//-----------------------------------------------------------------------------

void HttpDownload::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
   emit fileProgress(bytesReceived, bytesTotal);
}

//-----------------------------------------------------------------------------

void HttpDownload::finished()
{
   timer.stop();

   file.close();

   if ( reply->error() == QNetworkReply::NoError )
   {
      if ( file.exists() )
      {
         fileName = fileName.replace("_temp", "");
         file.remove(fileName);
         file.rename(fileName);

         emit finishedOk();
      }
   }
   else
   {
      file.remove();

      emit finishedWithError();
   }

   reply->deleteLater();
}

//-----------------------------------------------------------------------------

void HttpDownload::errorOccurred(QNetworkReply::NetworkError code)
{
   Q_UNUSED(code)

   timer.stop();
}

//-----------------------------------------------------------------------------

void HttpDownload::timerTimeout()
{
   reply->abort();
}

//-----------------------------------------------------------------------------
