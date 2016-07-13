#ifndef QNSTLANGUAGE_H
#define QNSTLANGUAGE_H

#include <QFile>
#include <QTextStream>
#include <QRegExp>

#include "qnstregister.h"

class QnstLanguage : public QnstRegister
{
public:
  QnstLanguage();
  QnstLanguage(QString location);

  virtual ~QnstLanguage();

  virtual void load(QString location);
  virtual void save(QString location);
};

#endif // QNSTLANGUAGE_H
