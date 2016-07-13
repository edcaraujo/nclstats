#ifndef QNSTREGISTER_H
#define QNSTREGISTER_H

#include <QString>
#include <QMap>

class QnstRegister
{
public:
  QnstRegister();
  virtual ~QnstRegister();

  void add(QString name, QString value);
  void remove(QString name);
  bool contains(QString name);
  QString value(QString name);
  QMap<QString, QString> values();

  virtual void load(QString location) = 0;
  virtual void save(QString location) = 0;

  virtual void clean();

protected:
  QMap<QString, QString> entries;
};

#endif // QNSTREGISTER_H
