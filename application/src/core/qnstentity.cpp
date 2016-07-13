#include "qnstentity.h"

QnstEntity::QnstEntity(QnstEntity* parent)
  : QObject(parent)
{
  setUid(QnstUtil::uid());
  setParent(parent);
}

QnstEntity::~QnstEntity()
{
  foreach(QnstEntity* e, children.values()) delete e; children.clear(); attributes.clear();
}

QString QnstEntity::getUid()
{
  return uid;
}

void QnstEntity::setUid(QString uid)
{
  this->uid = uid;
}

QString QnstEntity::getName()
{
  return name;
}

void QnstEntity::setName(QString name)
{
  this->name = name;
}

void QnstEntity::addAttribute(QString name, QString value)
{
  attributes.insert(name, value);
}

void QnstEntity::removeAttribute(QString name)
{
  if (attributes.contains(name)){
    attributes.remove(name);
  }
}

QMap<QString, QString> QnstEntity::getAttributes()
{
  return attributes;
}

QnstEntity* QnstEntity::getParent()
{
  return parent;
}

void QnstEntity::setParent(QnstEntity* parent)
{
  this->parent = parent;
}

void QnstEntity::addChild(QnstEntity* child)
{
  if (child != NULL)
  {
    if (!children.contains(child->getUid()))
    {
      children.insert(child->getUid(), child);
    }
  }
}

void QnstEntity::removeChild(QnstEntity* child)
{
  if (child != NULL)
  {
    if (children.contains(child->getUid()))
    {
      children.remove(child->getUid());
    }
  }
}

QMap<QString, QnstEntity*> QnstEntity::getChildren()
{
  return children;
}
