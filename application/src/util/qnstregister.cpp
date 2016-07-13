#include "qnstregister.h"

QnstRegister::QnstRegister()
{

}

QnstRegister::~QnstRegister()
{

}

void QnstRegister::add(QString name, QString value)
{
  entries.insert(name, value);
}

void QnstRegister::remove(QString name)
{
  entries.remove(name);
}

bool QnstRegister::contains(QString name)
{
  return entries.contains(name);
}

QString QnstRegister::value(QString name)
{
  return entries.value(name);
}

QMap<QString, QString> QnstRegister::values()
{
  return entries;
}

void QnstRegister::clean()
{
  entries.clear();
}
