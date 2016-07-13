#ifndef QNSTENTITY_H
#define QNSTENTITY_H

#include <QObject>
#include <QString>
#include <QMap>

#include "qnstutil.h"

class QnstEntity : public QObject
{
public:
  QnstEntity(QnstEntity* parent = 0);
  virtual ~QnstEntity();

  QString getUid();
  void setUid(QString uid);

  QString getName();
  void setName(QString name);

  QnstEntity* getParent();
  void setParent(QnstEntity* parent);

  void addChild(QnstEntity* child);
  void removeChild(QnstEntity* child);
  QMap<QString, QnstEntity*> getChildren();

  void addAttribute(QString name, QString value);
  void removeAttribute(QString name);
  QMap<QString, QString> getAttributes();

private:
  QString uid;
  QString name;

  QnstEntity* parent;
  QMap<QString, QnstEntity*> children;

  QMap<QString, QString> attributes;
};

#endif // QNSTENTITY_H
