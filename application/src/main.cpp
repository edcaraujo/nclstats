#include <QCoreApplication>

#include <QMap>
#include <QString>
#include <QTextStream>
#include <QDateTime>

#include "qnstsettings.h"
#include "qnstdocument.h"
#include "qnstanalyser.h"
#include "qnstutil.h"

QTextStream stream(stdout, QIODevice::WriteOnly);

void error()
{
  stream << "nclstats: missing operand." << endl;
  stream << "Try `nclstats --help' for more information." << endl;
}

void help()
{
  stream << "Usage: nclstats [OPTION]... [FILE]... [DIRECTORY]..." << endl;
  stream << endl;
  stream << "Avaiable options:" << endl;
  stream << endl;
  stream << "    -o output            set output directory" << endl;
  stream << "    --help               display this help and exit" << endl;
  stream << "    --version            display version information and exit" << endl;
  stream << endl;
  stream << "Usage examples:" << endl;
  stream << endl;
  stream << "    nclstats example.ncl" << endl;
  stream << "    nclstats examples/" << endl;
  stream << "    nclstats -o report/ example.ncl examples/" << endl;
  stream << endl;
  stream << "Report nclstats bugs to edcaraujo@telemidia.puc-rio.br" << endl;
  stream << "More information: http://www.telemidia.puc-rio.br/~edcaraujo/nclstats/" << endl;
}

void version()
{
  stream << "nclstats version " << QString(VERSION) << endl;
}

int main(int argc, char *argv[])
{
  QCoreApplication application(argc, argv);
  application.setApplicationName("nclstats");
  application.setApplicationVersion(QString(VERSION));
  application.setOrganizationName("telemidia");
  application.setOrganizationDomain("telemidia.puc-rio.br");

  QnstSettings* settings = new QnstSettings();
  settings->add(":system:application",application.applicationName());
  settings->add(":system:version", application.applicationVersion());
  settings->add(":system:organization", application.organizationName());
  settings->add(":system:site", "http://www.telemidia.puc-rio.br/~edcaraujo/nclstats/");
  settings->add(":system:template", QCoreApplication::applicationDirPath()+"/templates/default");
  settings->add(":system:date", QDateTime::currentDateTime().toString());

  settings->load(application.applicationDirPath()+"/settings/default.settings");

  QStringList files; bool analyse = true;

  if (application.arguments().size() > 1)
  {
    for (int n = 1; n < application.arguments().size(); n++)
    {
      QString arg = application.arguments().at(n);

      // settings output directory
      if (arg == "-o")
      {
        if (n+1 < application.arguments().size())
        {
          settings->add(":system:output", application.arguments().at(++n));
        }
        else
        {
          error(); analyse = false; break;
        }
      }
      // display help
      if (arg == "--help")
      {
        help(); analyse = false; break;
      }
      // display version
      if (arg == "--version")
      {
        version(); analyse = false; break;
      }
      // settings files
      else
      {
        if (QFile::exists(arg))
        {
          QFileInfo info(arg);

          if (info.isDir())
          {
            files << QnstUtil::list(arg,QRegExp("^.*\\.ncl$"));
          }
          else
          {
            files << info.absoluteFilePath();
          }
        }
      }
    }
  }
  else
  {
    error(); analyse = false;
  }

  if (analyse)
  {
    QnstLanguage* language = new QnstLanguage(QCoreApplication::applicationDirPath()+"/languages/ncl.lang");


    QnstAnalyser* analyser = new QnstAnalyser();
    analyser->setLanguage(language);
    analyser->setSettings(settings);

    foreach (QString file, files)
    {
      stream << "Analysing '" << file << "'..." << endl;
      QnstDocument* d = new QnstDocument();
      d->setLanguage(language);
      d->load(file);
      analyser->analyse(d);

      delete d;
    }

    stream << endl;
    stream << "Generating report to '" << settings->value(":system:output") << "'..." << endl;
    analyser->save(settings->value(":system:output"));
    stream << endl;
    stream << "Done!" << endl;

    delete analyser;
    delete language;
  }

  delete settings;

  return 0;
}
