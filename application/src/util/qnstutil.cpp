#include "qnstutil.h"

QString QnstUtil::uid()
{
  QString uid = QUuid::createUuid().toString();
  uid = uid.replace("{","");
  uid = uid.replace("}","");
  uid = uid.replace("-","");

  return uid;
}

QStringList QnstUtil::list(QString location, QRegExp regex)
{
  QDir directory(location);
  directory.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

  QStringList files;

  foreach(QFileInfo info, directory.entryInfoList())
  {
    QString file = info.absoluteFilePath();

    if (info.isDir())
    {
      files << list(file, regex);
    }
    else
    {
      if (file.contains(regex))
      {
        files << file;
      }
    }
  }

  return files;
}
