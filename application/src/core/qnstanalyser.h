#ifndef QNSTANALYSER_H
#define QNSTANALYSER_H

#include <QDir>
#include <QFile>

#include <QCryptographicHash>

#include <QDebug>

#include "qnstdocument.h"
#include "qnstlanguage.h"
#include "qnstsettings.h"

typedef QMap<QString, long long int> QnstStatistics;

class QnstAnalyser
{
public:
  struct QPairSecondComparer
  {
      template<typename T1, typename T2>
      bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
      {
          return a.second > b.second;
      }
  };

  QnstAnalyser();
  virtual ~QnstAnalyser();

  QnstLanguage* getLanguage();
  void setLanguage(QnstLanguage* language);

  QnstSettings* getSettings();
  void setSettings(QnstSettings* settings);

  void load(QString location);
  void save(QString location);
  void clean();

  virtual void analyse(QnstDocument* document);
  virtual void report();

private:
  void formatOverview(QString location);
  void formatLanguage(QString location);
  void formatSimple(QString location);
  void formatComplete(QString location);
  void formatFilesList(QString location);
  void formatOverviewFile(QString docUID, QString location);
  void formatFiles(QString location);
  void formatLanguageAttr(QString location);

  QnstLanguage* language;
  QnstSettings* settings;

  QnstStatistics estats;
  QnstStatistics astats;
  QnstStatistics vstats;
  QnstStatistics mtstats;

  QnstStatistics elemUsage;
  QnstStatistics attrUsage;

  int nmax;
  int nmaxUnbouded;

  QMap<QString, QString> docs_loc;

  QMap<QString, int> qualifier;

  QMap<QString, QnstStatistics> destats;
  QMap<QString, QnstStatistics> dastats;
  QMap<QString, QnstStatistics> dvstats;
  QMap<QString, QnstStatistics> drustats;
  QMap<QString, QnstStatistics> dmtstats;
};

#endif // QNSTANALYSER_H
