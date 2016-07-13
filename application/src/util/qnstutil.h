#ifndef QNSTUTIL_H
#define QNSTUTIL_H

#include <QString>
#include <QTextStream>
#include <QUuid>
#include <QDir>
#include <QFileInfo>

//#include <QDebug>

class QnstUtil
{
public:
  static QString uid();
  static QStringList list(QString location, QRegExp regex);
};

#endif // QNSTUTIL_H
