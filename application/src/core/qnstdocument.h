#ifndef QNSTDOCUMENT_H
#define QNSTDOCUMENT_H

#include <QFile>
#include <QStack>
#include <QPair>
#include <QDomDocument>

//#include <QDebug>

#include "qnstentity.h"
#include "qnstlanguage.h"

class QnstDocument : public QnstEntity
{
public:
  QnstDocument();
  QnstDocument(QString location);

  virtual ~QnstDocument();

  void addEntity(QnstEntity* entity);
  void removeEntity(QnstEntity* entity);
  QMap<QString, QnstEntity*> getEntities();

  void load(QString location);
  void save(QString location);

  void setLanguage(QnstLanguage* language);

private:
  QnstLanguage* language;

  QMap<QString, QnstEntity*> entities;
};

#endif // QNSTDOCUMENT_H
