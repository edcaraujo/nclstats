#include "qnstdocument.h"

QnstDocument::QnstDocument()
{

}

QnstDocument::QnstDocument(QString location)
{

}

QnstDocument::~QnstDocument()
{
  entities.clear();
}

void QnstDocument::setLanguage(QnstLanguage* language)
{
  this->language = language;
}

void QnstDocument::addEntity(QnstEntity* entity)
{
  if (entity != NULL)
  {
    if (!entities.contains(entity->getUid()))
    {
      entities.insert(entity->getUid(), entity);
    }
  }
}

void QnstDocument::removeEntity(QnstEntity* entity)
{
  if (entity != NULL)
  {
    if (entities.contains(entity->getUid()))
    {
      entities.remove(entity->getUid());
    }
  }
}

QMap<QString, QnstEntity*> QnstDocument::getEntities()
{
  return entities;
}

void QnstDocument::load(QString location)
{
  QFile* file = new QFile(location);

  if (file->open(QIODevice::ReadOnly))
  {
    QDomDocument* document = new QDomDocument();
    document->setContent(file);

    QStack<QPair<QDomElement,QnstEntity*> > elements;
    elements.push(QPair<QDomElement,QnstEntity*> (document->firstChildElement(),0));

    while(!elements.empty())
    {
      QPair<QDomElement,QnstEntity*>  pair = elements.pop();

      QnstEntity* parent = pair.second;

      QDomElement element = pair.first;
      QDomNamedNodeMap attributes = element.attributes();

      if (language->contains(":element:"+element.nodeName()))
      {
        QnstEntity* entity = new QnstEntity(parent);
        entity->setName(element.nodeName());

        for (int i=0;i<attributes.size();i++)
        {
          if (language->contains(":attribute:"+attributes.item(i).nodeName()))
          {
            entity->addAttribute(attributes.item(i).nodeName(), attributes.item(i).nodeValue());
          }
          else
          {
            // qWarning() << "[NST]" << ":"<< "[!]" << ":" << "Ignoring attribute " << attributes.item(i).nodeName() << "." << endl;
          }
        }

        if (parent == 0)
        {
          addChild(entity);
        }

        addEntity(entity);

        QDomNodeList children = element.childNodes();
        for (int i=children.length()-1;i>=0;--i)
        {
          if (children.item(i).isElement())
          {
            elements.push(QPair<QDomElement,QnstEntity*>(children.item(i).toElement(),entity));
          }
        }
      }
      else
      {
        // qWarning() << "[NST]" << ":"<< "[!]" << ":" << "Ignoring element " << element.nodeName() << "." << endl;
      }
    }

    delete document;

    addAttribute("location", location);
  }

  delete file;
}

void QnstDocument::save(QString location)
{
  // TODO
}
