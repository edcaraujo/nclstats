#include "qnstlanguage.h"

QnstLanguage::QnstLanguage()
{

}

QnstLanguage::QnstLanguage(QString location)
{
  load(location);
}

QnstLanguage::~QnstLanguage()
{
  clean();
}

void QnstLanguage::load(QString location)
{
  QFile* file = new QFile(location);

  if (file->open(QIODevice::ReadOnly)){
    QRegExp regex; regex.setPattern("(.*)=(.*)");

    QTextStream* stream = new QTextStream(file); clean();

    while(!stream->atEnd()){
      QString line = stream->readLine().trimmed();

      if (line.contains(regex)){
        QString name = regex.cap(1).trimmed();
        QString value = regex.cap(2).trimmed();

        entries.insert(name, value);
      }
    }

    delete stream;
  }

  delete file;
}

void QnstLanguage::save(QString location)
{
  // TODO
}
