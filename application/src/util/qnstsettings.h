#ifndef QNSTSETTINGS_H
#define QNSTSETTINGS_H

#include <QFile>
#include <QTextStream>
#include <QRegExp>

#include "qnstregister.h"

class QnstSettings : public QnstRegister
{
public:
  QnstSettings();
  QnstSettings(QString location);

  virtual ~QnstSettings();

  virtual void load(QString location);
  virtual void save(QString location);
};

#endif // QNSTSETTINGS_H
