#include "qnstanalyser.h"

QnstAnalyser::QnstAnalyser()
{

}

QnstAnalyser::~QnstAnalyser()
{
    estats.clear();
    astats.clear();
    vstats.clear();
    mtstats.clear();

    elemUsage.clear();
    attrUsage.clear();

    docs_loc.clear();

    destats.clear();
    dastats.clear();
    dvstats.clear();
    drustats.clear();
    dmtstats.clear();
}

QnstLanguage* QnstAnalyser::getLanguage()
{
  return language;
}

void QnstAnalyser::setLanguage(QnstLanguage* language)
{
  this->language = language;
}

QnstSettings* QnstAnalyser::getSettings()
{
  return settings;
}

void QnstAnalyser::setSettings(QnstSettings* settings)
{
  this->settings = settings;
}

void QnstAnalyser::load(QString location)
{
  // TODO
}

void QnstAnalyser::save(QString location)
{

  qDebug() << "======== N" << docs_loc.size();
  qDebug() << "Make dir";

  QDir dir;
  dir.mkpath(location);
  dir.mkpath(location+"/css");
//  dir.mkpath(location+"");

  qDebug() << "Remove old";

  if (QFile::exists(location+"/index.html"))
  {
    QFile::remove(location+"/index.html");
  }

  qDebug() << "Copy index";
  if (!QFile::copy(settings->value(":system:template")+"/index.html", location+"/index.html"))
  {
      qDebug() << settings->value(":system:template");
      qDebug() << location;
  }


  QFile tpl_overview(location+"/index.html");

  if (tpl_overview.open(QIODevice::ReadOnly)){

      QTextStream tpl_stream(&tpl_overview);

      QString txt = tpl_stream.readAll();

      txt = txt.replace("[:system:application]",settings->value(":system:application"));
      txt = txt.replace("[:system:version]",settings->value(":system:version"));
      txt = txt.replace("[:system:organization]",settings->value(":system:organization"));
      txt = txt.replace("[:system:site]",settings->value(":system:site"));
      txt = txt.replace("[:system:date]",settings->value(":system:date"));
      txt = txt.replace("[:system:#:files]",QString::number(docs_loc.size()));

      long long int nelem = 0;
      long long int nattr = 0;
      long long int nval = 0;

      foreach(int v, estats.values())
      {
        nelem += v;
      }

      foreach(int v, astats.values())
      {
        nattr += v;
      }

      foreach(int v, vstats.values())
      {
        nval += v;
      }

      txt = txt.replace("[:system:#:elements]",QString::number(nelem));
      txt = txt.replace("[:system:#:attributes]",QString::number(nattr));
      txt = txt.replace("[:system:#:values]",QString::number(nval));

      QFile out_overview(location+"/index.html");

      if (out_overview.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out_stream(&out_overview);

        out_stream << txt;
      }

      out_overview.close();
  }

  tpl_overview.close();

  qDebug() << "Format overview";
  formatOverview(location);

  formatLanguage(location);

  qDebug() << "Format Complete";
  formatComplete(location);

  qDebug() << "Format File List";
  formatFilesList(location);

  qDebug() << "Format Files";
  formatFiles(location);

  QFile::copy(settings->value(":system:template")+"/css/default.css", location+"/css/default.css");

  qDebug() << "NMAX" << nmax;
  qDebug() << "NMAX UNBOUNDED" << nmaxUnbouded;

  qDebug() << qualifier;
}

void QnstAnalyser::formatOverview(QString location)
{
  int MAX = 0;

  foreach (int v, estats.values())
  {
    if (v > MAX) MAX = v;
  }

  foreach (int v, astats.values())
  {
    if (v > MAX) MAX = v;
  }

  foreach (int v, vstats.values())
  {
    if (v > MAX) MAX = v;
  }

  MAX = (MAX/120+1)*120;
//  MAX = 600;

  int MOSTLENGTH = 5;

//  qDebug() << "=====================" << MAX;

  QTextStream stream(new QString(""));

  int MUEMIN;


  if (estats.size() > 0)
  {
  QList<QPair<QString,long long int> > topElements;

  QMap<QString,long long int> elementdata = estats;

  foreach(QString key, elementdata.keys())
  {
    topElements << QPair<QString,long long int>(key, elementdata.value(key));
  }

  qSort(topElements.begin(), topElements.end(), QPairSecondComparer());

  // Overview
  stream << "<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>" << endl;

  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
  stream << "google.setOnLoadCallback(draw5MustUsedElements);" << endl;
  stream << "function draw5MustUsedElements() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Elements', 'Quantities']" << endl;

  MUEMIN = MOSTLENGTH; if (topElements.size() < MUEMIN) MUEMIN = topElements.size();

  for(int i=0;i<MUEMIN;i++)
  {
    QPair<QString,long long int> p = topElements.at(i);

    stream << ",['<" << p.first << ">'," << p.second << "]" << endl;
  }

  stream << "]);" << endl;

  stream << "var options = {" << endl;
  stream << "chartArea: {top: 0, width: 500, height:" << MUEMIN*25 << "}," << endl;
  stream << "fontSize: 12," << endl;
  stream << "hAxis: {title: 'Quantity', viewWindowMode: 'explicit', viewWindow: {min: 0, max:" << MAX << "}}," << endl;
  stream << "vAxis: {title: 'Elements',gridlines: {count: 3}}" << endl;
  stream << "};" << endl;

  stream << "var  chart = new google.visualization.BarChart(document.getElementById('overview_data_mostusedelements'));" << endl;
  stream << "chart.draw(data, options);" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;
}

// ============================================= MOST USED ATTRIBUTES

  int MUAMIN;

  //

  if (astats.size() > 0)
  {
  QList<QPair<QString,long long int> > topAttributes;

  QMap<QString,long long int> attributedata = astats;

  foreach(QString key, attributedata.keys())
  {
    topAttributes << QPair<QString,long long int>(key, attributedata.value(key));
  }

  qSort(topAttributes.begin(), topAttributes.end(), QPairSecondComparer());

    //


  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
  stream << "google.setOnLoadCallback(draw5MostUsedAttributes);" << endl;
  stream << "function draw5MostUsedAttributes() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Attributes', 'Quantities']" << endl;

  MUAMIN = MOSTLENGTH; if (topAttributes.size() < MUAMIN) MUAMIN = topAttributes.size();

  for(int i=0;i<MUAMIN;i++)
  {
    QPair<QString,long long int> p = topAttributes.at(i);

    stream << ",['" << p.first << "=\\\"\\\"'," << p.second << "]" << endl;
  }

  stream << "]);" << endl;

  stream << "var options = {" << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
  stream << "chartArea: {top:0, width: 500, height:" << MUAMIN*25 << "}," << endl;
  stream << "fontSize: 12," << endl;
  stream << "hAxis: {title: 'Quantity',viewWindowMode: 'explicit', viewWindow: {min: 0,  max: " << MAX << "}}," << endl;
  stream << "vAxis: {title: 'Attributes',gridlines: {count: 3}}" << endl;
  stream << "};" << endl;

  stream << "var  chart = new google.visualization.BarChart(document.getElementById('overview_data_mostusedattributes'));" << endl;
  stream << "chart.draw(data, options);" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;
}
  //

QList<QPair<QString,long long int> > topValues;

QMap<QString,long long int> valuedata = vstats;

foreach(QString key, valuedata.keys())
{
  topValues << QPair<QString,long long int>(key, valuedata.value(key));
}

qSort(topValues.begin(), topValues.end(), QPairSecondComparer());


//

int MUVMIN;

if (vstats.size() > 0)
{
stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
stream << "google.setOnLoadCallback(draw5MostUsedValues);" << endl;
stream << "function draw5MostUsedValues() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Values', 'Quantities']" << endl;

MUVMIN = MOSTLENGTH; if (topValues.size() < MUVMIN) MUVMIN = topValues.size();

for(int i=0;i<MUVMIN;i++)
{
QPair<QString,long long int> p = topValues.at(i);

stream << ",['\\\""<< p.first << "\\\"'," << p.second << "]" << endl;
}

stream << "]);" << endl;

stream << "var options = {" << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
stream << "chartArea: {top:0, width: 500, height:" << MUVMIN*25 << "}," << endl;
stream << "fontSize: 12," << endl;
stream << "hAxis: {title: 'Quantity',viewWindowMode: 'explicit', viewWindow: {min: 0, max: " << MAX << "}}," << endl;
stream << "vAxis: {title: 'Values',gridlines: {count: 3}}" << endl;
stream << "};" << endl;

stream << "var  chart = new google.visualization.BarChart(document.getElementById('overview_data_mostusedvalues'));" << endl;
stream << "chart.draw(data, options);" << endl;
stream << "}" << endl;
stream << "</script>" << endl;

}
int MUMTMIN ;

if (mtstats.size() > 0)
{
QList<QPair<QString,long long int> > topMediaType;

QMap<QString,long long int> typedata = mtstats;

foreach(QString key, typedata.keys())
{
  topMediaType << QPair<QString,long long int>(key, typedata.value(key));
}

qSort(topMediaType.begin(), topMediaType.end(), QPairSecondComparer());


//


stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
stream << "google.setOnLoadCallback(draw5MostUsedMediaType);" << endl;
stream << "function draw5MostUsedMediaType() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Values', 'Quantities']" << endl;

MUMTMIN = MOSTLENGTH;

if (topMediaType.size() < MUMTMIN) MUMTMIN = topMediaType.size();

for(int i=0;i<MUMTMIN;i++)
{
QPair<QString,long long int> p = topMediaType.at(i);

stream << ",['."<< p.first << "'," << p.second << "]" << endl;
}

stream << "]);" << endl;

stream << "var options = {" << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
stream << "chartArea: {top:0, width: 500, height:" << MUMTMIN*25 << "}," << endl;
stream << "fontSize: 12," << endl;
stream << "hAxis: {title: 'Quantity', viewWindowMode: 'explicit', viewWindow: {min: 0, max: " << MAX << "}}," << endl;
stream << "vAxis: {title: 'Media Types', gridlines: {count: 3}}" << endl;
stream << "};" << endl;

stream << "var  chart = new google.visualization.BarChart(document.getElementById('overview_data_mostusedmediatypes'));" << endl;
stream << "chart.draw(data, options);" << endl;
stream << "}" << endl;
stream << "</script>" << endl;
}
//
//

int refer = 0;
int descriptor = 0;
int region = 0;
int rule = 0;
int transInOut = 0;
int xconnector = 0;
int total = 0;
int done = 0;

foreach(QString docUID, drustats.keys())
{
QnstStatistics localrustats = drustats.value(docUID);

foreach(QString key, localrustats.keys())
{
  if (key.startsWith("refer"))
  {
    refer += localrustats.value(key);
  }
  else if (key.startsWith("descriptor"))
  {
    descriptor += localrustats.value(key) - 1;
  }
  else if (key.startsWith("region"))
  {
    region += localrustats.value(key) - 1;
  }
  else if (key.startsWith("rule"))
  {
    rule += localrustats.value(key) - 1;
  }
  else if (key.startsWith("transIn") || key.startsWith("transOut"))
  {
    transInOut += localrustats.value(key) - 1;
  }
  else if (key.startsWith("xconnector"))
  {
    xconnector += localrustats.value(key) - 1;
  }

  total += localrustats.value(key);
}

}

done += refer +  descriptor + region + rule + transInOut + xconnector;

if (done > 0)
{

stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
stream << "google.setOnLoadCallback(drawReusePie);" << endl;
stream << "function drawReusePie() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Type', 'Quantities']" << endl;

if (refer)
{
stream << ",['Node reuse'," << refer << "]" << endl;
}
if (descriptor)
{
stream << ",['Descriptor reuse'," << descriptor << "]" << endl;
}
if (region)
{
stream << ",['Region reuse'," << region << "]" << endl;
}
if (rule)
{
stream << ",['Rule reuse'," << rule << "]" << endl;
}
if (transInOut)
{
stream << ",['Transition reuse'," << transInOut << "]" << endl;
}
if (xconnector)
{
stream << ",['Connector reuse'," << xconnector << "]" << endl;
}

stream << "]);" << endl;

stream << "var options = {" << endl;
stream << "chartArea: {top:0, height: 300}" << endl;
stream << "};" << endl;

stream << "var  chart = new google.visualization.PieChart(document.getElementById('overview_data_reuserate'));" << endl;
stream << "chart.draw(data, options);" << endl;
stream << "}" << endl;
stream << "</script>" << endl;
}

//
//
//

  QFile tpl_overview(location+"/index.html");

  if (tpl_overview.open(QIODevice::ReadOnly)){

      QTextStream tpl_stream(&tpl_overview);

      QString txt = tpl_stream.readAll();

      txt = txt.replace("[:system:scripts:overview]", stream.readAll());

      if (estats.size() > 0)
        txt = txt.replace("[:system:data:overview:mostusedelements]","<div id=\"overview_data_mostusedelements\" style=\"height: " + QString::number((MUEMIN*25 + 25)) + "px;\"></div>");
      else
        txt = txt.replace("[:system:data:overview:mostusedelements]","<div id=\"overview_data_mostusedelements\">No element found!</div>");

      if (astats.size() > 0)
        txt = txt.replace("[:system:data:overview:mostusedattributes]","<div id=\"overview_data_mostusedattributes\" style=\"height: " + QString::number((MUAMIN*25 + 25)) + "px;\"></div>");
      else
        txt = txt.replace("[:system:data:overview:mostusedattributes]","<div id=\"overview_data_mostusedattributes\">No attribute found!</div>");

      if (vstats.size() > 0)
        txt = txt.replace("[:system:data:overview:mostusedvalues]","<div id=\"overview_data_mostusedvalues\" style=\"height: " + QString::number((MUVMIN*25 + 25)) + "px;\"></div>");
      else
        txt = txt.replace("[:system:data:overview:mostusedvalues]","<div id=\"overview_data_mostusedvalues\">No value found!</div>");

      if (mtstats.size() > 0)
        txt = txt.replace("[:system:data:overview:mostusedmediatypes]","<div id=\"overview_data_mostusedmediatypes\" style=\"height: " + QString::number((MUMTMIN*25 + 25)) + "px;\"></div>");
      else
        txt = txt.replace("[:system:data:overview:mostusedmediatypes]","<div id=\"overview_data_mostusedmediatypes\">No media type found!</div>");

      if (done > 0)
      {
        txt = txt.replace("[:system:#:reuse]",QString::number(done));
        txt = txt.replace("[:system:+:reuse]",QString::number(total));
        txt = txt.replace("[:system:%:reuse]",QString::number(done*100/total)+"%");
        txt = txt.replace("[:system:data:overview:reuse]","<div id=\"overview_data_reuserate\" style=\"height: 300px;\"></div>");
      }
      else
      {
        txt = txt.replace("[:system:#:reuse]",QString::number(done));
        txt = txt.replace("[:system:+:reuse]",QString::number(total));
        txt = txt.replace("[:system:%:reuse]","0%");
        txt = txt.replace("[:system:data:overview:reuse]","<div>No reuse found!</div>");
      }



      QFile out_overview(location+"/index.html");

      if (out_overview.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out_stream(&out_overview);

        out_stream << txt;
      }

      out_overview.close();
      tpl_overview.close();
  }
}

void QnstAnalyser::formatSimple(QString location)
{
  QTextStream stream(new QString(""));

  QList<QPair<QString,long long int> > topElements;

  QMap<QString,long long int> elementdata = estats;

  foreach(QString key, elementdata.keys())
  {
    topElements << QPair<QString,long long int>(key, elementdata.value(key));
  }

  qSort(topElements.begin(), topElements.end(), QPairSecondComparer());


  // Overview
  stream << "<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>" << endl;

  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
  stream << "google.setOnLoadCallback(draw10MustUsedElements);" << endl;
  stream << "function draw10MustUsedElements() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Elements', 'Quantities']" << endl;


  for(int i=0;i<10;i++)
  {
    QPair<QString,long long int> p = topElements.at(i);

    stream << ",['<" << p.first << ">'," << p.second << "]" << endl;
  }

  stream << "]);" << endl;

  stream << "var options = {" << endl;
  stream << "title: '10 most used elements'," << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
  stream << "chartArea: {width: 500, height:" << 10*25 << "}," << endl;
  stream << "fontSize: 12," << endl;
  stream << "hAxis: {viewWindowMode: 'explicit', viewWindow: {max: 600}}" << endl;
  stream << "};" << endl;

  stream << "var  chart = new google.visualization.BarChart(document.getElementById('10mostusedelements'));" << endl;
  stream << "chart.draw(data, options);" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;

//

  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
  stream << "google.setOnLoadCallback(draw10LessUsedElements);" << endl;
  stream << "function draw10LessUsedElements() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Elements', 'Quantities']" << endl;

  for(int i=topElements.size()-1;i>=topElements.size()-10;i--)
  {
    QPair<QString,long long int> p = topElements.at(i);

    stream << ",['<" << p.first << ">'," << p.second << "]" << endl;
  }

  stream << "]);" << endl;

  stream << "var options = {" << endl;
  stream << "title: '10 less used elements'," << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
  stream << "chartArea: {width: 500, height:" << 10*25 << "}," << endl;
  stream << "fontSize: 12," << endl;
  stream << "hAxis: {viewWindowMode: 'explicit', viewWindow: {max: 600}}" << endl;
  stream << "};" << endl;

  stream << "var  chart = new google.visualization.BarChart(document.getElementById('10lessusedelements'));" << endl;
  stream << "chart.draw(data, options);" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;

  //

  QList<QPair<QString,long long int> > topAttributes;

  QMap<QString,long long int> attributedata = astats;

  foreach(QString key, attributedata.keys())
  {
    topAttributes << QPair<QString,long long int>(key, attributedata.value(key));
  }

  qSort(topAttributes.begin(), topAttributes.end(), QPairSecondComparer());

    //


  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
  stream << "google.setOnLoadCallback(draw10MostUsedAttributes);" << endl;
  stream << "function draw10MostUsedAttributes() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Attributes', 'Quantities']" << endl;

  for(int i=0;i<10;i++)
  {
    QPair<QString,long long int> p = topAttributes.at(i);

    stream << ",['" << p.first << "=\\\"\\\"'," << p.second << "]" << endl;
  }

  stream << "]);" << endl;

  stream << "var options = {" << endl;
  stream << "title: '10 most used attributes'," << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
  stream << "chartArea: {width: 500, height:" << 10*25 << "}," << endl;
  stream << "fontSize: 12," << endl;
  stream << "hAxis: {viewWindowMode: 'explicit', viewWindow: {max: 600}}" << endl;
  stream << "};" << endl;

  stream << "var  chart = new google.visualization.BarChart(document.getElementById('10MostUsedAttributes'));" << endl;
  stream << "chart.draw(data, options);" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;

  //


stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
stream << "google.setOnLoadCallback(draw10LessUsedAttributes);" << endl;
stream << "function draw10LessUsedAttributes() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Attributes', 'Quantities']" << endl;

for(int i=topAttributes.size()-1;i>=topAttributes.size()-10;i--)
{
  QPair<QString,long long int> p = topAttributes.at(i);

  stream << ",['" << p.first << "=\\\"\\\"'," << p.second << "]" << endl;
}

stream << "]);" << endl;

stream << "var options = {" << endl;
stream << "title: '10 less used attributes'," << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
stream << "chartArea: {width: 500, height:" << 10*25 << "}," << endl;
stream << "fontSize: 12," << endl;
stream << "hAxis: {viewWindowMode: 'explicit', viewWindow: {max: 600}}" << endl;
stream << "};" << endl;

stream << "var  chart = new google.visualization.BarChart(document.getElementById('10LessUsedAttributes'));" << endl;
stream << "chart.draw(data, options);" << endl;
stream << "}" << endl;
stream << "</script>" << endl;
  //

QList<QPair<QString,long long int> > topValues;

QMap<QString,long long int> valuedata = vstats;

foreach(QString key, valuedata.keys())
{
  topValues << QPair<QString,long long int>(key, valuedata.value(key));
}

qSort(topValues.begin(), topValues.end(), QPairSecondComparer());


//


stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
stream << "google.setOnLoadCallback(draw10MostUsedValues);" << endl;
stream << "function draw10MostUsedValues() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Values', 'Quantities']" << endl;

for(int i=0;i<10;i++)
{
QPair<QString,long long int> p = topValues.at(i);

stream << ",['\\\""<< p.first << "\\\"'," << p.second << "]" << endl;
}

stream << "]);" << endl;

stream << "var options = {" << endl;
stream << "title: '10 most used values'," << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
stream << "chartArea: {width: 500, height:" << 10*25 << "}," << endl;
stream << "fontSize: 12," << endl;
stream << "hAxis: {viewWindowMode: 'explicit', viewWindow: {max: 600}}" << endl;
stream << "};" << endl;

stream << "var  chart = new google.visualization.BarChart(document.getElementById('10MostUsedValues'));" << endl;
stream << "chart.draw(data, options);" << endl;
stream << "}" << endl;
stream << "</script>" << endl;

//


stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
stream << "google.setOnLoadCallback(draw10LessUsedValues);" << endl;
stream << "function draw10LessUsedValues() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Values', 'Quantities']" << endl;

for(int i=topValues.size()-1;i>=topValues.size()-10;i--)
{
QPair<QString,long long int> p = topValues.at(i);

stream << ",['\\\"" << p.first << "\\\"'," << p.second << "]" << endl;
}

stream << "]);" << endl;

stream << "var options = {" << endl;
stream << "title: '10 less used values'," << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
stream << "chartArea: {width: 500, height:" << 10*25 << "}," << endl;
stream << "fontSize: 12," << endl;
stream << "hAxis: {viewWindowMode: 'explicit', viewWindow: {max: 600}}" << endl;
stream << "};" << endl;

stream << "var  chart = new google.visualization.BarChart(document.getElementById('10LessUsedValues'));" << endl;
stream << "chart.draw(data, options);" << endl;
stream << "}" << endl;
stream << "</script>" << endl;
//

  QFile tpl_overview(settings->value(":system:template")+"/simple.html");

  if (tpl_overview.open(QIODevice::ReadOnly)){

      QTextStream tpl_stream(&tpl_overview);

      QString txt = tpl_stream.readAll();

      txt = txt.replace("[:system:scripts]", stream.readAll());
      txt = txt.replace("[:system:chart:element]",
                        "<div id=\"10mostusedelements\" style=\" height: " + QString::number((10*25 + 100)) + "px;\"></div>" +
                        "<div id=\"10lessusedelements\" style=\" height: " + QString::number((10*25 + 100)) + "px;\"></div>");

      txt = txt.replace("[:system:chart:attribute]",
                        "<div id=\"10MostUsedAttributes\" style=\" height: " + QString::number((10*25 + 100)) + "px;\"></div>" +
                        "<div id=\"10LessUsedAttributes\" style=\" height: " + QString::number((10*25 + 100)) + "px;\"></div>");

      txt = txt.replace("[:system:chart:value]",
                        "<div id=\"10MostUsedValues\" style=\" height: " + QString::number((10*25 + 100)) + "px;\"></div>" +
                        "<div id=\"10LessUsedValues\" style=\" height: " + QString::number((10*25 + 100)) + "px;\"></div>");

      QFile out_overview(location+"/simple.html");

      if (out_overview.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out_stream(&out_overview);

        out_stream << txt;
      }

      out_overview.close();
      tpl_overview.close();
  }
}

void QnstAnalyser::formatComplete(QString location)
{
  QTextStream stream(new QString(""));

  if (estats.size() > 0)
  {
  QList<QPair<QString,long long int> > topElements;

  QMap<QString,long long int> elementdata = estats;

  foreach(QString key, elementdata.keys())
  {
    topElements << QPair<QString,long long int>(key, elementdata.value(key));
  }

  qSort(topElements.begin(), topElements.end(), QPairSecondComparer());


  // Overview
  stream << "<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>" << endl;

  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"table\"]});" << endl;
  stream << "google.setOnLoadCallback(drawCMustUsedElements);" << endl;
  stream << "function drawCMustUsedElements() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Elements', 'Quantities']" << endl;


  for(int i=0;i<topElements.size();i++)
  {
    QPair<QString,long long int> p = topElements.at(i);

    stream << ",['<" << p.first << ">'," << p.second << "]" << endl;
  }

  foreach(QString k, language->values().keys())
  {
    if (k.contains(QRegExp("^:element.*")))
    {
      if (!elementdata.contains(k.mid(9)))
      {
        stream << ",['<" << k.mid(9) << ">'," << 0 << "]" << endl;
      }
    }
  }

  stream << "]);" << endl;

  stream << "var  chart = new google.visualization.Table(document.getElementById('details_data_elements'));" << endl;
  stream << "chart.draw(data, {showRowNumber: true});" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;
}
//

//

  if (astats.size() > 0)
  {
  QList<QPair<QString,long long int> > topAttributes;

  QMap<QString,long long int> attributedata = astats;

  foreach(QString key, attributedata.keys())
  {
    topAttributes << QPair<QString,long long int>(key, attributedata.value(key));
  }

  qSort(topAttributes.begin(), topAttributes.end(), QPairSecondComparer());

    //


  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"table\"]});" << endl;
  stream << "google.setOnLoadCallback(drawCMostUsedAttributes);" << endl;
  stream << "function drawCMostUsedAttributes() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Attributes', 'Quantities']" << endl;

  for(int i=0;i<topAttributes.size();i++)
  {
    QPair<QString,long long int> p = topAttributes.at(i);

    stream << ",['" << p.first << "=\\\"\\\"'," << p.second << "]" << endl;
  }

  foreach(QString k, language->values().keys())
  {
    if (k.contains(QRegExp("^:attribute.*")))
    {
      if (!attributedata.contains(k.mid(11)))
      {
        stream << ",['" << k.mid(11) << "=\\\"\\\"'," << 0 << "]" << endl;
      }
    }
  }

  stream << "]);" << endl;


  stream << "var  chart = new google.visualization.Table(document.getElementById('details_data_attributes'));" << endl;
  stream << "chart.draw(data, {showRowNumber: true});" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;
  }

//

//

  if (vstats.size() > 0)
  {
QList<QPair<QString,long long int> > topValues;

QMap<QString,long long int> valuedata = vstats;

foreach(QString key, valuedata.keys())
{
  topValues << QPair<QString,long long int>(key, valuedata.value(key));
}

qSort(topValues.begin(), topValues.end(), QPairSecondComparer());


//


stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"table\"]});" << endl;
stream << "google.setOnLoadCallback(drawCMostUsedValues);" << endl;
stream << "function drawCMostUsedValues() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Values', 'Quantities']" << endl;

int N = 500;
if (topValues.size() < 500) N = topValues.size();

for(int i=0;i<N;i++)
{
QPair<QString,long long int> p = topValues.at(i);

stream << ",['\\\""<< p.first << "\\\"'," << p.second << "]" << endl;
}

stream << "]);" << endl;


stream << "var  chart = new google.visualization.Table(document.getElementById('details_data_values'));" << endl;
stream << "chart.draw(data, {showRowNumber: true});" << endl;
stream << "}" << endl;
stream << "</script>" << endl;
  }
//

//

  if (mtstats.size() > 0)
  {
QList<QPair<QString,long long int> > topMTypes;

QMap<QString,long long int> typedata = mtstats;

foreach(QString key, typedata.keys())
{
  topMTypes << QPair<QString,long long int>(key, typedata.value(key));
}

qSort(topMTypes.begin(), topMTypes.end(), QPairSecondComparer());


//


stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"table\"]});" << endl;
stream << "google.setOnLoadCallback(drawCMostUsedMediaTypes);" << endl;
stream << "function drawCMostUsedMediaTypes() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Values', 'Quantities']" << endl;

for(int i=0;i<topMTypes.size();i++)
{
QPair<QString,long long int> p = topMTypes.at(i);

stream << ",['."<< p.first << "'," << p.second << "]" << endl;
}

stream << "]);" << endl;


stream << "var  chart = new google.visualization.Table(document.getElementById('details_data_mediatypes'));" << endl;
stream << "chart.draw(data, {showRowNumber: true});" << endl;
stream << "}" << endl;
stream << "</script>" << endl;
  }

//
//


  QFile tpl_overview(location+"/index.html");

  if (tpl_overview.open(QIODevice::ReadOnly)){

      QTextStream tpl_stream(&tpl_overview);

      QString txt = tpl_stream.readAll();

      txt = txt.replace("[:system:scripts:details]", stream.readAll());

      if (estats.size() > 0)
        txt = txt.replace("[:system:data:details:elements]", QString("<div id=\"details_data_elements\"></div>"));
      else
        txt = txt.replace("[:system:data:details:elements]", QString("<div id=\"details_data_elements\">No element found!</div>"));

      if (astats.size() > 0)
        txt = txt.replace("[:system:data:details:attributes]", QString("<div id=\"details_data_attributes\"></div>"));
      else
        txt = txt.replace("[:system:data:details:attributes]", QString("<div id=\"details_data_attributes\">No attribute found!</div>"));

      if (vstats.size() > 0)
        txt = txt.replace("[:system:data:details:values]", QString("<div id=\"details_data_values\"></div>"));
      else
        txt = txt.replace("[:system:data:details:values]", QString("<div id=\"details_data_values\">No value found!</div>"));

      if (mtstats.size() > 0)
        txt = txt.replace("[:system:data:details:mediatypes]", QString("<div id=\"details_data_mediatypes\"></div>"));
      else
        txt = txt.replace("[:system:data:details:mediatypes]", QString("<div id=\"details_data_mediatypes\">No media type found!</div>"));


      QFile out_overview(location+"/index.html");

      if (out_overview.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out_stream(&out_overview);

        out_stream << txt;
      }

      out_overview.close();
      tpl_overview.close();
  }
}


void QnstAnalyser::clean()
{
  // TODO
}

void QnstAnalyser::analyse(QnstDocument* document)
{

  if (document != NULL)
  {
    QnstStatistics localestats;
    QnstStatistics localastats;
    QnstStatistics localvstats;
    QnstStatistics localmtstats;
    QnstStatistics localrustats;

    QnstStatistics localElemUsage;
    QnstStatistics localAttrUsage;

    foreach(QnstEntity* entity, document->getEntities().values())
    {
      if (localestats.contains(entity->getName()))
      {
        localestats.insert(entity->getName(),localestats.value(entity->getName()) + 1);
      }
      else
      {
        localestats.insert(entity->getName(), 1);
      }

      if (estats.contains(entity->getName()))
      {
        estats.insert(entity->getName(),estats.value(entity->getName()) + 1);
      }
      else
      {
        estats.insert(entity->getName(), 1);
      }

      if (!localElemUsage.contains(entity->getName()))
      {
        localElemUsage.insert(entity->getName(), 1);
      }

      QMap<QString, QString> attrs = entity->getAttributes();

      foreach(QString name, attrs.keys())
      {
        if (localastats.contains(name))
        {
          localastats.insert(name,localastats.value(name) + 1);
        }
        else
        {
          localastats.insert(name, 1);
        }

        if (astats.contains(name))
        {
          astats.insert(name,astats.value(name) + 1);
        }
        else
        {
          astats.insert(name, 1);
        }

        QString attvalue = attrs.value(name);

        if (localvstats.contains(attvalue))
        {
          localvstats.insert(attvalue,localvstats.value(attvalue) + 1);
        }
        else
        {
          localvstats.insert(attvalue, 1);
        }

        if (vstats.contains(attvalue))
        {
          vstats.insert(attvalue,vstats.value(attvalue) + 1);
        }
        else
        {
          vstats.insert(attvalue, 1);
        }

        if (entity->getName() == "media" && name == "src")
        {
          QString type = attvalue.section('.',-1,-1);

          if (localmtstats.contains(type))
          {
            localmtstats.insert(type,localmtstats.value(type) + 1);
          }
          else
          {
            localmtstats.insert(type, 1);
          }

          if (mtstats.contains(type))
          {
            mtstats.insert(type,mtstats.value(type) + 1);
          }
          else
          {
            mtstats.insert(type, 1);
          }
        }

        QString elemattr = entity->getName()+":"+name;

        if (attrUsage.contains(elemattr))
        {
          attrUsage.insert(elemattr,attrUsage.value(elemattr) + 1);
        }
        else
        {
          attrUsage.insert(elemattr, 1);
        }

        if (name == "max")
        {
          nmax++;

          if (attvalue == "unbounded")
          {
            nmaxUnbouded++;
          }
        }

        if (name == "qualifier")
        {
          if (qualifier.contains(attvalue))
          {
            qualifier.insert(attvalue,qualifier.value(attvalue) + 1);
          }
          else
          {
            qualifier.insert(attvalue, 1);
          }
        }

        // REUSED INFOS!
        if (name == "refer" ||
            name == "descriptor" ||
            name == "region" ||
            name == "rule" ||
            name == "transIn" ||
            name == "transOut" ||
            name == "xconnector")
        {
          QString comb = name + ":" + attvalue;

          if (localrustats.contains(comb))
          {
            localrustats.insert(comb,localrustats.value(comb) + 1);
          }
          else
          {
            localrustats.insert(comb, 1);
          }
        }
      }
    }

    QString loc = document->getAttributes().value("location");

    QString md5 = QString(QCryptographicHash::hash(loc.toStdString().c_str(),QCryptographicHash::Md5).toHex());

//    qDebug() << md5;

    docs_loc.insert(md5, loc);

//    qDebug() << document->getUid() << " -> " << docs_loc[document->getUid()];


    foreach(QString s, localElemUsage.keys())
    {
      if (elemUsage.contains(s))
      {
        elemUsage.insert(s,elemUsage.value(s) + 1);
      }
      else
      {
        elemUsage.insert(s, 1);
      }
    }

    destats.insert(md5, localestats);
    dastats.insert(md5, localastats);
    dvstats.insert(md5, localvstats);
    drustats.insert(md5, localrustats);
    dmtstats.insert(md5, localmtstats);
  }
}

void QnstAnalyser::formatOverviewFile(QString docUID , QString location)
{
  int MAX = 0;

  foreach (int v, destats.value(docUID).values())
  {
    if (v > MAX) MAX = v;
  }

  foreach (int v, dastats.value(docUID).values())
  {
    if (v > MAX) MAX = v;
  }

  foreach (int v, dvstats.value(docUID).values())
  {
    if (v > MAX) MAX = v;
  }



  MAX = (MAX/120 + 1)*120;

  QTextStream stream(new QString(""));

  int MOSTLENGTH = 5;

  int MUEMIN;

  if (destats.value(docUID).size() > 0)
  {
  QList<QPair<QString,long long int> > topElements;

  QMap<QString,long long int> elementdata = destats.value(docUID);

  foreach(QString key, elementdata.keys())
  {
    topElements << QPair<QString,long long int>(key, elementdata.value(key));
  }

  qSort(topElements.begin(), topElements.end(), QPairSecondComparer());

  // Overview
  stream << "<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>" << endl;

  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
  stream << "google.setOnLoadCallback(draw5MustUsedElements);" << endl;
  stream << "function draw5MustUsedElements() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Elements', 'Quantities']" << endl;

  MUEMIN = MOSTLENGTH; if (topElements.size() < MUEMIN) MUEMIN = topElements.size();

  for(int i=0;i<MUEMIN;i++)
  {
    QPair<QString,long long int> p = topElements.at(i);

    stream << ",['<" << p.first << ">'," << p.second << "]" << endl;
  }

  stream << "]);" << endl;

  stream << "var options = {" << endl;
  stream << "chartArea: {top: 0, width: 500, height:" << MUEMIN*25 << "}," << endl;
  stream << "fontSize: 12," << endl;
  stream << "hAxis: {title: 'Quantity', viewWindowMode: 'explicit', viewWindow: {min: 0, max:" << MAX << "}}," << endl;
  stream << "vAxis: {title: 'Elements',gridlines: {count: 3}}" << endl;
  stream << "};" << endl;

  stream << "var  chart = new google.visualization.BarChart(document.getElementById('overview_data_mostusedelements'));" << endl;
  stream << "chart.draw(data, options);" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;
}

// ============================================= MOST USED ATTRIBUTES

  int MUAMIN;

  //

  if (dastats.value(docUID).size() > 0)
  {
  QList<QPair<QString,long long int> > topAttributes;

  QMap<QString,long long int> attributedata = dastats.value(docUID);

  foreach(QString key, attributedata.keys())
  {
    topAttributes << QPair<QString,long long int>(key, attributedata.value(key));
  }

  qSort(topAttributes.begin(), topAttributes.end(), QPairSecondComparer());

    //


  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
  stream << "google.setOnLoadCallback(draw5MostUsedAttributes);" << endl;
  stream << "function draw5MostUsedAttributes() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Attributes', 'Quantities']" << endl;

  MUAMIN = MOSTLENGTH; if (topAttributes.size() < MUAMIN) MUAMIN = topAttributes.size();

  for(int i=0;i<MUAMIN;i++)
  {
    QPair<QString,long long int> p = topAttributes.at(i);

    stream << ",['" << p.first << "=\\\"\\\"'," << p.second << "]" << endl;
  }

  stream << "]);" << endl;

  stream << "var options = {" << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
  stream << "chartArea: {top:0, width: 500, height:" << MUAMIN*25 << "}," << endl;
  stream << "fontSize: 12," << endl;
  stream << "hAxis: {title: 'Quantity',viewWindowMode: 'explicit', viewWindow: {min: 0,  max: " << MAX << "}}," << endl;
  stream << "vAxis: {title: 'Attributes',gridlines: {count: 3}}" << endl;
  stream << "};" << endl;

  stream << "var  chart = new google.visualization.BarChart(document.getElementById('overview_data_mostusedattributes'));" << endl;
  stream << "chart.draw(data, options);" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;
}
  //

QList<QPair<QString,long long int> > topValues;

QMap<QString,long long int> valuedata = dvstats.value(docUID);

foreach(QString key, valuedata.keys())
{
  topValues << QPair<QString,long long int>(key, valuedata.value(key));
}

qSort(topValues.begin(), topValues.end(), QPairSecondComparer());


//

int MUVMIN;

if (dvstats.value(docUID).size() > 0)
{
stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
stream << "google.setOnLoadCallback(draw5MostUsedValues);" << endl;
stream << "function draw5MostUsedValues() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Values', 'Quantities']" << endl;

MUVMIN = MOSTLENGTH; if (topValues.size() < MUVMIN) MUVMIN = topValues.size();

for(int i=0;i<MUVMIN;i++)
{
QPair<QString,long long int> p = topValues.at(i);

stream << ",['\\\""<< p.first << "\\\"'," << p.second << "]" << endl;
}

stream << "]);" << endl;

stream << "var options = {" << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
stream << "chartArea: {top:0, width: 500, height:" << MUVMIN*25 << "}," << endl;
stream << "fontSize: 12," << endl;
stream << "hAxis: {title: 'Quantity',viewWindowMode: 'explicit', viewWindow: {min: 0, max: " << MAX << "}}," << endl;
stream << "vAxis: {title: 'Values',gridlines: {count: 3}}" << endl;
stream << "};" << endl;

stream << "var  chart = new google.visualization.BarChart(document.getElementById('overview_data_mostusedvalues'));" << endl;
stream << "chart.draw(data, options);" << endl;
stream << "}" << endl;
stream << "</script>" << endl;

}
int MUMTMIN ;

if (dmtstats.value(docUID).size() > 0)
{
QList<QPair<QString,long long int> > topMediaType;

QMap<QString,long long int> typedata = dmtstats.value(docUID);

foreach(QString key, typedata.keys())
{
  topMediaType << QPair<QString,long long int>(key, typedata.value(key));
}

qSort(topMediaType.begin(), topMediaType.end(), QPairSecondComparer());


//


stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
stream << "google.setOnLoadCallback(draw5MostUsedMediaType);" << endl;
stream << "function draw5MostUsedMediaType() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Values', 'Quantities']" << endl;

MUMTMIN = MOSTLENGTH;

if (topMediaType.size() < MUMTMIN) MUMTMIN = topMediaType.size();

for(int i=0;i<MUMTMIN;i++)
{
QPair<QString,long long int> p = topMediaType.at(i);

stream << ",['."<< p.first << "'," << p.second << "]" << endl;
}

stream << "]);" << endl;

stream << "var options = {" << endl;
//    stream << "chartArea: {left: 300, top: 10, bottom: 0}," << endl;
stream << "chartArea: {top:0, width: 500, height:" << MUMTMIN*25 << "}," << endl;
stream << "fontSize: 12," << endl;
stream << "hAxis: {title: 'Quantity', viewWindowMode: 'explicit', viewWindow: {min: 0, max: " << MAX << "}}," << endl;
stream << "vAxis: {title: 'Media Types', gridlines: {count: 3}}" << endl;
stream << "};" << endl;

stream << "var  chart = new google.visualization.BarChart(document.getElementById('overview_data_mostusedmediatypes'));" << endl;
stream << "chart.draw(data, options);" << endl;
stream << "}" << endl;
stream << "</script>" << endl;
}
//
//

int refer = 0;
int descriptor = 0;
int region = 0;
int rule = 0;
int transInOut = 0;
int xconnector = 0;
int total = 0;
int done = 0;

QnstStatistics localrustats = drustats.value(docUID);

foreach(QString key, localrustats.keys())
{
  if (key.startsWith("refer"))
  {
    refer += localrustats.value(key);
  }
  else if (key.startsWith("descriptor"))
  {
    descriptor += localrustats.value(key) - 1;
  }
  else if (key.startsWith("region"))
  {
    region += localrustats.value(key) - 1;
  }
  else if (key.startsWith("rule"))
  {
    rule += localrustats.value(key) - 1;
  }
  else if (key.startsWith("transIn") || key.startsWith("transOut"))
  {
    transInOut += localrustats.value(key) - 1;
  }
  else if (key.startsWith("xconnector"))
  {
    xconnector += localrustats.value(key) - 1;
  }

  total += localrustats.value(key);
}

done += refer +  descriptor + region + rule + transInOut + xconnector;

if (done > 0)
{

stream << "<script type=\"text/javascript\">" << endl;
stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
stream << "google.setOnLoadCallback(drawReusePie);" << endl;
stream << "function drawReusePie() {" << endl;

stream << "var data = google.visualization.arrayToDataTable([" << endl;
stream << "['Type', 'Quantities']" << endl;

if (refer)
{
  stream << ",['Node reuse'," << refer << "]" << endl;
  }
  if (descriptor)
  {
  stream << ",['Descriptor reuse'," << descriptor << "]" << endl;
  }
  if (region)
  {
  stream << ",['Region reuse'," << region << "]" << endl;
  }
  if (rule)
  {
  stream << ",['Rule reuse'," << rule << "]" << endl;
  }
  if (transInOut)
  {
  stream << ",['Transition reuse'," << transInOut << "]" << endl;
  }
  if (xconnector)
  {
  stream << ",['Connector reuse'," << xconnector << "]" << endl;
}

stream << "]);" << endl;

stream << "var options = {" << endl;
stream << "chartArea: {top:0, height: 300}" << endl;
stream << "};" << endl;

stream << "var  chart = new google.visualization.PieChart(document.getElementById('overview_data_reuserate'));" << endl;
stream << "chart.draw(data, options);" << endl;
stream << "}" << endl;
stream << "</script>" << endl;
}

//
//
//

  QFile tpl_overview(location+"/"+docUID+".html");

  if (tpl_overview.open(QIODevice::ReadOnly)){

      QTextStream tpl_stream(&tpl_overview);

      QString txt = tpl_stream.readAll();

      txt = txt.replace("[:system:scripts:overview]", stream.readAll());

      if (estats.size() > 0)
        txt = txt.replace("[:system:data:overview:mostusedelements]","<div id=\"overview_data_mostusedelements\" style=\"height: " + QString::number((MUEMIN*25 + 25)) + "px;\"></div>");
      else
        txt = txt.replace("[:system:data:overview:mostusedelements]","<div id=\"overview_data_mostusedelements\">No element found!</div>");

      if (astats.size() > 0)
        txt = txt.replace("[:system:data:overview:mostusedattributes]","<div id=\"overview_data_mostusedattributes\" style=\"height: " + QString::number((MUAMIN*25 + 25)) + "px;\"></div>");
      else
        txt = txt.replace("[:system:data:overview:mostusedattributes]","<div id=\"overview_data_mostusedattributes\">No attribute found!</div>");

      if (vstats.size() > 0)
        txt = txt.replace("[:system:data:overview:mostusedvalues]","<div id=\"overview_data_mostusedvalues\" style=\"height: " + QString::number((MUVMIN*25 + 25)) + "px;\"></div>");
      else
        txt = txt.replace("[:system:data:overview:mostusedvalues]","<div id=\"overview_data_mostusedvalues\">No value found!</div>");

      if (mtstats.size() > 0)
        txt = txt.replace("[:system:data:overview:mostusedmediatypes]","<div id=\"overview_data_mostusedmediatypes\" style=\"height: " + QString::number((MUMTMIN*25 + 25)) + "px;\"></div>");
      else
        txt = txt.replace("[:system:data:overview:mostusedmediatypes]","<div id=\"overview_data_mostusedmediatypes\">No media type found!</div>");

      if (done > 0)
      {
        txt = txt.replace("[:system:#:reuse]",QString::number(done));
        txt = txt.replace("[:system:+:reuse]",QString::number(total));
        txt = txt.replace("[:system:%:reuse]",QString::number(done*100/total)+"%");
        txt = txt.replace("[:system:data:overview:reuse]","<div id=\"overview_data_reuserate\" style=\"height: 300px;\"></div>");
      }
      else
      {
        txt = txt.replace("[:system:#:reuse]",QString::number(done));
        txt = txt.replace("[:system:+:reuse]",QString::number(total));
        txt = txt.replace("[:system:%:reuse]","0%");
        txt = txt.replace("[:system:data:overview:reuse]","<div>No reuse found!</div>");
      }



      QFile out_overview(location+"/"+docUID+".html");

      if (out_overview.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out_stream(&out_overview);

        out_stream << txt;
      }

      out_overview.close();
      tpl_overview.close();
  }
}

void QnstAnalyser::formatFilesList(QString location)
{
  QFile tpl_overview(location+"/index.html");

  if (tpl_overview.open(QIODevice::ReadOnly)){

      QTextStream tpl_stream(&tpl_overview);

      QString txt = tpl_stream.readAll();

      QString replace = "<ul>";

      foreach(QString sDoc, docs_loc.keys())
      {
        QString CODE = sDoc;
        CODE = CODE.replace("{","");
        CODE = CODE.replace("}","");
        CODE = CODE.replace("-","");


        QString loc = docs_loc.value(sDoc);

        replace += "<li><a target=\"_blank\" href=\""+CODE+".html\">"+loc+"</a></li>";
      }

      replace += "</ul>";

      if (docs_loc.size() > 0)
        txt = txt.replace("[:system:data:files]",replace);
      else
        txt = txt.replace("[:system:data:files]","<p>No file found!</p>");

      QFile out_overview(location+"/index.html");

      if (out_overview.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out_stream(&out_overview);

        out_stream << txt;
      }

      out_overview.close();
      tpl_overview.close();
  }
}

void QnstAnalyser::formatFiles(QString location)
{
  QTextStream stream(new QString(""));

  int G = 0;

  foreach (QString sDoc, destats.keys())
  {
//    if (++G > 1) break;

    QString CODE = sDoc;
    CODE = CODE.replace("{","");
    CODE = CODE.replace("}","");
    CODE = CODE.replace("-","");

    QList<QPair<QString,long long int> > topElements;

    QMap<QString,long long int> elementdata = destats.value(sDoc);

    if (elementdata.size() > 0)
    {

    foreach(QString key, elementdata.keys())
    {
      topElements << QPair<QString,long long int>(key, elementdata.value(key));
    }

    qSort(topElements.begin(), topElements.end(), QPairSecondComparer());


    // Overview
    stream << "<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>" << endl;

    stream << "<script type=\"text/javascript\">" << endl;
    stream << "google.load(\"visualization\", \"1\", {packages:[\"table\"]});" << endl;
    stream << "google.setOnLoadCallback(drawE"+CODE+");" << endl;
    stream << "function drawE"+CODE+"() {" << endl;

    stream << "var data = google.visualization.arrayToDataTable([" << endl;
    stream << "['Elements', 'Quantities']" << endl;


    for(int i=0;i<topElements.size();i++)
    {
      QPair<QString,long long int> p = topElements.at(i);

      stream << ",['<" << p.first << ">'," << p.second << "]" << endl;
    }

    foreach(QString k, language->values().keys())
    {
      if (k.contains(QRegExp("^:element.*")))
      {
        if (!elementdata.contains(k.mid(9)))
        {
          stream << ",['<" << k.mid(9) << ">'," << 0 << "]" << endl;
        }
      }
    }

    stream << "]);" << endl;

    stream << "var  chart = new google.visualization.Table(document.getElementById('details_data_elements'));" << endl;
    stream << "chart.draw(data, {showRowNumber: true});" << endl;
    stream << "}" << endl;
    stream << "</script>" << endl;
}  //

  //

    QList<QPair<QString,long long int> > topAttributes;

    QMap<QString,long long int> attributedata = dastats.value(sDoc);;

    if (attributedata.size() > 0)
    {
    foreach(QString key, attributedata.keys())
    {
      topAttributes << QPair<QString,long long int>(key, attributedata.value(key));
    }

    qSort(topAttributes.begin(), topAttributes.end(), QPairSecondComparer());

      //


    stream << "<script type=\"text/javascript\">" << endl;
    stream << "google.load(\"visualization\", \"1\", {packages:[\"table\"]});" << endl;
    stream << "google.setOnLoadCallback(drawA"+CODE+");" << endl;
    stream << "function drawA"+CODE+"() {" << endl;

    stream << "var data = google.visualization.arrayToDataTable([" << endl;
    stream << "['Attributes', 'Quantities']" << endl;

    for(int i=0;i<topAttributes.size();i++)
    {
      QPair<QString,long long int> p = topAttributes.at(i);

      stream << ",['" << p.first << "=\\\"\\\"'," << p.second << "]" << endl;
    }

    foreach(QString k, language->values().keys())
    {
      if (k.contains(QRegExp("^:attribute.*")))
      {
        if (!attributedata.contains(k.mid(11)))
        {
          stream << ",['" << k.mid(11) << "=\\\"\\\"'," << 0 << "]" << endl;
        }
      }
    }

    stream << "]);" << endl;


    stream << "var  chart = new google.visualization.Table(document.getElementById('details_data_attributes'));" << endl;
    stream << "chart.draw(data, {showRowNumber: true});" << endl;
    stream << "}" << endl;
    stream << "</script>" << endl;
}
  //

  //

  QList<QPair<QString,long long int> > topValues;

  QMap<QString,long long int> valuedata = dvstats.value(sDoc);;

  if (valuedata.size() > 0)
  {
  foreach(QString key, valuedata.keys())
  {
    topValues << QPair<QString,long long int>(key, valuedata.value(key));
  }

  qSort(topValues.begin(), topValues.end(), QPairSecondComparer());


  //


  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"table\"]});" << endl;
  stream << "google.setOnLoadCallback(drawV"+CODE+");" << endl;
  stream << "function drawV"+CODE+"() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Values', 'Quantities']" << endl;

  int N = 500;
  if (topValues.size() < 500) N = topValues.size();

  for(int i=0;i<N;i++)
  {
  QPair<QString,long long int> p = topValues.at(i);

  stream << ",['\\\""<< p.first << "\\\"'," << p.second << "]" << endl;
  }

  stream << "]);" << endl;


  stream << "var  chart = new google.visualization.Table(document.getElementById('details_data_values'));" << endl;
  stream << "chart.draw(data, {showRowNumber: true});" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;
}
//
//
//

  QList<QPair<QString,long long int> > topMTypes;

  QMap<QString,long long int> typedata = dmtstats.value(CODE);

  if (typedata.size() > 0)
  {
  foreach(QString key, typedata.keys())
  {
    topMTypes << QPair<QString,long long int>(key, typedata.value(key));
  }

  qSort(topMTypes.begin(), topMTypes.end(), QPairSecondComparer());


  if (topMTypes.size() > 0)
  {

  //


  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"table\"]});" << endl;
  stream << "google.setOnLoadCallback(drawMT"+CODE+");" << endl;
  stream << "function drawMT"+CODE+"() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Values', 'Quantities']" << endl;

  for(int i=0;i<topMTypes.size();i++)
  {
  QPair<QString,long long int> p = topMTypes.at(i);

  stream << ",['."<< p.first << "'," << p.second << "]" << endl;
  }

  stream << "]);" << endl;


  stream << "var  chart = new google.visualization.Table(document.getElementById('details_data_mediatypes'));" << endl;
  stream << "chart.draw(data, {showRowNumber: true});" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;
}
  }

  //

  //

  QString final_loc = location+"/"+CODE+".html";

  if (QFile::exists(final_loc))
    QFile::remove(final_loc);

   QFile::copy(settings->value(":system:template")+"/item.html", final_loc);

   formatOverviewFile(CODE, location);

   QFile tpl_overview(final_loc);

   if (tpl_overview.open(QIODevice::ReadOnly))
   {
       QTextStream tpl_stream(&tpl_overview);

       QString txt = tpl_stream.readAll();

       txt = txt.replace("[:system:scripts:details]", stream.readAll());

       if (elementdata.size() > 0)
         txt = txt.replace("[:system:data:details:elements]", QString("<div id=\"details_data_elements\"></div>"));
       else
         txt = txt.replace("[:system:data:details:elements]", QString("<div id=\"details_data_elements\">No element found!</div>"));

       if (attributedata.size() > 0)
         txt = txt.replace("[:system:data:details:attributes]", QString("<div id=\"details_data_attributes\"></div>"));
       else
         txt = txt.replace("[:system:data:details:attributes]", QString("<div id=\"details_data_attributes\">No attribute found!</div>"));

       if (valuedata.size() > 0)
         txt = txt.replace("[:system:data:details:values]", QString("<div id=\"details_data_values\"></div>"));
       else
         txt = txt.replace("[:system:data:details:values]", QString("<div id=\"details_data_values\">No value found!</div>"));

       if (typedata.size() > 0)
         txt = txt.replace("[:system:data:details:mediatypes]", QString("<div id=\"details_data_mediatypes\"></div>"));
       else
         txt = txt.replace("[:system:data:details:mediatypes]", QString("<div id=\"details_data_mediatypes\">No media type found!</div>"));

       txt = txt.replace("[:system:application]",settings->value(":system:application"));
       txt = txt.replace("[:system:version]",settings->value(":system:version"));
       txt = txt.replace("[:system:organization]",settings->value(":system:organization"));
       txt = txt.replace("[:system:site]",settings->value(":system:site"));
       txt = txt.replace("[:system:date]",settings->value(":system:date"));
       txt = txt.replace("[:system:#:files]",QString::number(docs_loc.size()));

       long long int nelem = 0;
       long long int nattr = 0;
       long long int nval = 0;

       foreach(int v, elementdata.values())
       {
         nelem += v;
       }

       foreach(int v, attributedata.values())
       {
         nattr += v;
       }

       foreach(int v, valuedata.values())
       {
         nval += v;
       }

       txt = txt.replace("[:system:#:elements]",QString::number(nelem));
       txt = txt.replace("[:system:#:attributes]",QString::number(nattr));
       txt = txt.replace("[:system:#:values]",QString::number(nval));

       txt = txt.replace("[:system:file]","<a href=\""+docs_loc.value(CODE)+"\">"+docs_loc.value(CODE)+"</a>");

       QFile out_overview(final_loc);

       if (out_overview.open(QIODevice::WriteOnly | QIODevice::Text)){
         QTextStream out_stream(&out_overview);

         out_stream << txt;
       }

       out_overview.close();
       tpl_overview.close();
   }
  }
}

void QnstAnalyser::formatLanguage(QString location)
{
  QTextStream stream(new QString(""));

  stream << "<script type=\"text/javascript\">" << endl;
  stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
  stream << "google.setOnLoadCallback(drawLanguageUsage);" << endl;
  stream << "function drawLanguageUsage() {" << endl;

  stream << "var data = google.visualization.arrayToDataTable([" << endl;
  stream << "['Element', 'Rate']" << endl;

  int NELEM = 0;

  foreach (QString str, language->values().keys())
  {
      if (str.startsWith(":element"))
      {
        QString name = str.section(':',-1,-1);

        qreal r = 0;

        if (docs_loc.size() > 0)
          r = ((qreal)elemUsage.value(name))/docs_loc.size();

        stream << ",['<" << name << ">', "<< r*100 <<"]" << endl;

        NELEM++;
      }
  }

  stream << "]);" << endl;

  stream << "var options = {" << endl;
  stream << "allowHtml: true," << endl;
  stream << "chartArea: {top: 0, width: 500, height:" << NELEM*25 << "}," << endl;
  stream << "fontSize: 12," << endl;
  stream << "hAxis: {title: 'Rate (%)',viewWindowMode: 'explicit', viewWindow: {min: 0,  max: " << 120 << "}}," << endl;
  stream << "vAxis: {title: 'Elements',gridlines: {count: 3}}" << endl;
  stream << "};" << endl;

  stream << "var formatter = new google.visualization.NumberFormat(" << endl;
  stream << "{suffix: '%'});" << endl;
  stream << "formatter.format(data, 1);" << endl;

  stream << "var  chart = new google.visualization.BarChart(document.getElementById('language_data_elements'));" << endl;
  stream << "chart.draw(data, options);" << endl;
  stream << "}" << endl;
  stream << "</script>" << endl;

  QMap<QString, QString> elemDIvs;

  foreach (QString lkey, language->values().keys())
  {
    if (lkey.startsWith(":element:"))
    {
      QString element = lkey.section(':',-1,-1);

      stream << "<script type=\"text/javascript\">" << endl;
      stream << "google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});" << endl;
      stream << "google.setOnLoadCallback(draw"<< element << "LanguageUsage);" << endl;
      stream << "function draw"<< element << "LanguageUsage() {" << endl;

      stream << "var data = google.visualization.arrayToDataTable([" << endl;
      stream << "['Attribute', 'Rate']" << endl;

      int nattrs = 0;

      foreach (QString lrkey, language->values().keys())
      {
        if (lrkey.startsWith(":relation:"))
        {
           QString selem = lrkey.section(':',2,2);
           QString sattr = lrkey.section(':',3,3);

           if (selem == element)
           {
             qreal r =  0;

             if (attrUsage.contains(selem+":"+sattr))
             {
               if (estats.value(element,0) > 0)
               {
                 r = ((qreal)attrUsage.value(selem+":"+sattr))/estats.value(element);
               }
             }

             stream << ",['" << sattr << "=\\\"\\\"', "<< r*100 <<"]" << endl;

             nattrs++;
           }

        }
      }

      stream << "]);" << endl;

      stream << "var options = {" << endl;
      stream << "allowHtml: true," << endl;
      stream << "chartArea: {top: 0, width: 500, height:" << nattrs*25 << "}," << endl;
      stream << "fontSize: 12," << endl;
      stream << "hAxis: {title: 'Rate (%)',viewWindowMode: 'explicit', viewWindow: {min: 0,  max: " << 120 << "}}," << endl;
      stream << "vAxis: {title: 'Attributes',gridlines: {count: 3}}" << endl;
      stream << "};" << endl;

      stream << "var formatter = new google.visualization.NumberFormat(" << endl;
      stream << "{suffix: '%'});" << endl;
      stream << "formatter.format(data, 1);" << endl;

      stream << "var  chart = new google.visualization.BarChart(document.getElementById('language_data_attributes_"<< element << "'));" << endl;
      stream << "chart.draw(data, options);" << endl;
      stream << "}" << endl;
      stream << "</script>" << endl;

      if (nattrs > 0)
      {
        elemDIvs.insert(element, "<h4>Attributes of element &lt;"+ element + "&gt; ("+ QString::number(estats.value(element)) +")</h4><p style=\"font-style: italic;\">Usage rate of attributes in "+ QString::number(estats.value(element)) +" &lt;"+ element + "&gt; element(s) analysed.</p><div id=\"language_data_attributes_" + element+ "\" style=\" height: " + QString::number((nattrs*25 + 25)) + "px;\"></div><br/>");
      }
      else
      {
        elemDIvs.insert(element, "<h4>"+ element +" ("+ QString::number(estats.value(element)) +")</h4><div><p>No attribute found!</p></div><br/>");
      }

    }
  }

  QFile tpl_overview(location+"/index.html");

  if (tpl_overview.open(QIODevice::ReadOnly)){

      QTextStream tpl_stream(&tpl_overview);

      QString txt = tpl_stream.readAll();

      txt = txt.replace("[:system:scripts:language]", stream.readAll());

      txt = txt.replace("[:system:data:language:elements]", "<div id=\"language_data_elements\" style=\"height: " + QString::number((NELEM*25 + 25)) + "px;\"></div>");

      QString divs = "";

      foreach (QString div, elemDIvs.values())
      {
        divs += div;
      }

       txt = txt.replace("[:system:data:language:attributes]",divs);

      QFile out_overview(location+"/index.html");

      if (out_overview.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out_stream(&out_overview);

        out_stream << txt;
      }

      out_overview.close();
      tpl_overview.close();
  }
}

void QnstAnalyser::formatLanguageAttr(QString location)
{

}

void QnstAnalyser::report()
{

}
