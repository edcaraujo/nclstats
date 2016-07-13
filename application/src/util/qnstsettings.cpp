#include "qnstsettings.h"

QnstSettings::QnstSettings()
{

}

QnstSettings::QnstSettings(QString location)
{


  load(location);
}

QnstSettings::~QnstSettings()
{

}

void QnstSettings::load(QString location)
{
  QFile* file = new QFile(location);

  if (file->open(QIODevice::ReadOnly)){
    QRegExp regex; regex.setPattern("(.*)=(.*)");

    QTextStream* stream = new QTextStream(file);

    while(!stream->atEnd()){
      QString line = stream->readLine().trimmed();

      if (line.contains(regex)){
        QString name = (regex.cap(1)).trimmed();
        QString value = (regex.cap(2)).trimmed();

        entries.insert(name,value);
      }
    }

    delete stream;
  }

  delete file;
}

void QnstSettings::save(QString location)
{
  // TODO
}
