#include <QtCore/QCoreApplication>
#include <QObject>
#include <QMetaType>
#include <msgfnc.h>
#include <Pipe/MsgPipe.h>
#include <Pipe/UpdatePipe.h>

#include "ParserTrack.h"
#include "TrackSocketReader.h"
#include "ParserPlan.h"
#include "Structs.h"
#include "STCA.h"
#include "STCATasksXMLReader.h"
#include "STCAParamsXMLReader.h"
#include "Point4DInfo.h"

#include <config.h>

#include <getopt.h>

using namespace std;

POINTS_MAP points;  // map точек (S,Sv) всех самолетов
Parameters params;
QMap<int, int> task_flags;
HistoryForMofN historyMofN;
QTextStream* outStream = 0;

int the_current = STCA::ALL;
int the_other = STCA::ALL;

static quint16 port = 0;// = 20016;

Separation<Point4D>* pSep = 0;

void emulateSTCA();
//void parseCmd(int argc, char *argv[]);

int parseCmd(int argc, char *argv[]) {

    QString xmlParams, xsdParams, xmlTaskflags, xsdTaskflags;
    QString xmlNormlist, xsdNormlist;
    //-----------------------------------------------------------------------------------------------------

    const struct option longopts[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"params", no_argument, 0, 'p'},
        {"taskflags", no_argument, 0, 't'},
        {"normlist", no_argument, 0, 'n'},
        {"port", required_argument, 0, 'P'},
        {0, 0, 0, 0}
    };
    char ch;
    int longind;
    while ((ch = getopt_long(argc, argv, "?hvp:t:n:P:", longopts, &longind)) != -1) {
        switch (ch) {
            case '?':
            case 'h':
                QTextStream(stdout) << argv[0] << ":\n";
                QTextStream(stdout) << "\t--" << longopts[0].name << ", -" << (char) longopts[0].val << "\t\t\t\tprint this message and exit\n";
                QTextStream(stdout) << "\t--" << longopts[1].name << ", -" << (char) longopts[1].val << "\t\t\t\tprint version and exit\n";
                QTextStream(stdout) << "\t--" << longopts[2].name << " <filename>, -" << (char) longopts[2].val << " <filename>\t\tset params-file name (without extention)\n";
                QTextStream(stdout) << "\t--" << longopts[4].name << " <filename>, -" << (char) longopts[4].val << " <filename>\t\tset taskfalgs-file name (without extention)\n";
                QTextStream(stdout) << "\t--" << longopts[3].name << " <filename>, -" << (char) longopts[3].val << " <filename>\t\tset normlist-file name (without extention)\n";
                QTextStream(stdout) << "\t--" << longopts[5].name << " <port_number>, -" << (char) longopts[5].val << " <port_number>\t\tset port number\n";
                return 0;
            case 'v':
                QTextStream(stdout) << argv[0] << " built from rev. " << SVN_REV << "\n";
                return 0;
            case 'p':
                xmlParams.append(QString(optarg) + ".xml");
                xsdParams.append(QString(optarg) + ".xsd");
                break;
            case 't':
                xmlTaskflags.append(QString(optarg) + ".xml");
                xsdTaskflags.append(QString(optarg) + ".xsd");
                break;
            case 'n':
                xmlNormlist.append(QString(optarg) + ".xml");
                xsdNormlist.append(QString(optarg) + ".xsd");
                break;
            case 'P':
                if (sscanf(optarg, "%hu", &port) <= 0) {
                    QTextStream(stdout) << "warning: incorrect option argument " << optarg << " - ignored\n";
                }
                break;
        }
    }

    //-----------------------------------------------------------------------------------------------------
    if (port == 0) {
        QTextStream(stdout) << "port not specified, exiting\n";
        return 1;
    }

    /*-----------------------------------------------------------------------------------*/
    /* Чтение xml файлов Параметры и Флаги заданий                                       */
    QTextStream(stdout) << xmlTaskflags << "\n" << xsdTaskflags << "\n";
    //STCATasks::STCATasksXMLReader xmlTasksReader(task_flags, "/tmp/tasks_flags.xsd", "/tmp/tasks_flags.xml");
    STCATasks::STCATasksXMLReader xmlTasksReader(task_flags, xsdTaskflags, xmlTaskflags);
    xmlTasksReader.parse();

    QTextStream(stdout) << xmlParams << "\n" << xsdParams << "\n";
    //STCAParams::STCAParamsXMLReader xmlParamsReader(params, "/tmp/tempParams.xsd", "/tmp/tempParams.xml");
    STCAParams::STCAParamsXMLReader xmlParamsReader(params, xsdParams, xmlParams);
    xmlParamsReader.parse();

    QTextStream(stdout) << xmlNormlist << "\n" << xsdNormlist << "\n";
    //Separation<Point4D> sep("../NormViolations/normlistTemp.xsd", "../NormViolations/normlistTemp.xml");
    pSep = new Separation<Point4D>(xsdNormlist, xmlNormlist);

    if(pSep == 0) {
        QTextStream(stdout) << "Error while creating Separation<Point4D> object!\n";
        return -1;
    }

    return 1;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    openmkfu();

    outStream = new QTextStream(stdout);

    std::map<int, int> cflMap;

    int res = parseCmd(argc, argv);
    if(res <= 0) {
        return res;
    }

    /*-----------------------------------------------------------------------------------*/
    /* данные 4D траектории                                                              */
    char updatePipePlanName[MAX_PIPENAME] = "/sintez/sintez/pipe/plan";

    ParserPlan parserPlan(cflMap, outStream);
    Pipe::UpdatePipe updatePipePlan(updatePipePlanName);
    qRegisterMetaType<MS_update>();
    QObject::connect(&updatePipePlan, SIGNAL(newMessage(MS_update)), &parserPlan, SLOT(newMSMessage(MS_update)));

    outStream->flush();

    /*-----------------------------------------------------------------------------------*/
    /* фактические данные */
    ParserTrack parserTrack(cflMap, outStream);
    TrackSocketReader socketReader;

    //quint16 port = 20016;
    if (socketReader.bind(port)) {
        qDebug("port %d bound ok", port);
        QObject::connect(&socketReader, SIGNAL(newTrackMessage(const Track&)), &parserTrack, SLOT(newMessage(const Track&)));
    } else {
        qDebug("port %d bind failed, exiting", port);
        return 1;
    }

    *outStream <<  "start\n";
    outStream->flush();

    //doSTCA(Point4DInfo(0,Point4D(0,0,0,0), Rate3D(0,0,0)));


    //emulateSTCA();


    return a.exec();
}

void analysePoints(POINTS_MAP& points, const Point4DInfo& ptInfo, bool extrapolate) {
    static map<int, unsigned int> storeExtra;

    //*outStream << "---------------------------- " << ptInfo.id << "\n";
    //outStream->flush();

    if(points.find(ptInfo.id) == points.end()) {
        points.insert(make_pair<int, Point4DInfo>(ptInfo.id, ptInfo));
    } else {
        points.erase(ptInfo.id);
        points.insert(make_pair<int, Point4DInfo>(ptInfo.id, ptInfo));
        //points[ptInfo.id] = Point4DInfoptInfo;
    }

    const unsigned int EXTRA_LIMIT = 4;

    if(extrapolate) {
        storeExtra[ptInfo.id] = storeExtra[ptInfo.id] + 1;

        if(storeExtra[ptInfo.id] >= EXTRA_LIMIT) {
            storeExtra.erase(ptInfo.id);
            points.erase(ptInfo.id);
            /*map<int, unsigned int> :: iterator it = storeExtra.find(ptInfo.id);
            storeExtra.erase(it);
            POINTS_MAP::iterator it2 = points.find(ptInfo.id);
            points.erase(it2);*/
        }
    } else {
        storeExtra[ptInfo.id] = 0;
    }
}


void emulateSTCA() {
    //---------------------------------------------------------
    Point4DInfo pt1(1, Point4D(2000,0,1000,10), Rate3D(1000,1000,0));
    points.insert(make_pair<int,Point4DInfo>(pt1.id, pt1));

    Point4DInfo pt2(2, Point4D(0,10,20,10), Rate3D(1,-1,-2));
   // points.insert(make_pair<int,Point4DInfo>(pt2.id, pt2));

    Point4DInfo pt3(3, Point4D(0,0,14,10), Rate3D(6,0,5));
    //points.insert(make_pair<int,Point4DInfo>(pt3.id, pt3));

    Point4DInfo pt4(4, Point4D(1500,8000,1000,10), Rate3D(1000,-500,0));
    points.insert(make_pair<int,Point4DInfo>(pt4.id, pt4));

    Point4DInfo pt5(5, Point4D(0,0,0,10), Rate3D(1,0,1));
    pt5.CFL = 50;
//    points.insert(make_pair<int,Point4DInfo>(pt5.id, pt5));

    //---------------------------------------------------------

    Point4DInfo* pCurPt = 0;

    int dt = 1;
    int i=0;
    do {

        if(i!=0)
            for(POINTS_MAP::iterator it = points.begin(); it != points.end(); ++it) {
                Point4DInfo pi = it->second;

                if(it->first == the_current) {
                    pCurPt = &it->second;
                }

                Point4D p = Point4D(pi.pt.x() + pi.rate.xRate() * dt, pi.pt.y() + pi.rate.yRate() * dt, pi.pt.altitude() + pi.rate.zRate() * dt, pi.pt.time() + dt);

                if(pi.CFL != Point4DInfo::CFL_INVALID_VALUE && p.altitude() == pi.CFL) {
                    Point4DInfo pt(pi.id, p, Rate3D(pi.rate.xRate(), pi.rate.yRate(), 0));
                    pt.CFL = pi.CFL;

                    points.erase(pi.id);
                    points.insert(make_pair(pt.id, pt));
                } else {
                    Point4DInfo pt(pi.id, p, Rate3D(pi.rate.xRate(), pi.rate.yRate(), pi.rate.zRate()));
                    pt.CFL = pi.CFL;

                    points.erase(pi.id);
                    points.insert(make_pair(pt.id, pt));
                }


/*
                Point4D p = Point4D(pi.pt.x() + pi.rate.xRate() * dt, pi.pt.y() + pi.rate.yRate() * dt, pi.pt.altitude() + pi.rate.zRate() * dt, pi.pt.time() + dt);

                Point4DInfo pt(pi.id, p, Rate3D(pi.rate.xRate(), pi.rate.yRate(), pi.rate.zRate()));
                pt.CFL = pi.CFL;

                points.erase(pi.id);
                points.insert(make_pair(pt.id, pt));
*/

            }
        else
            for(POINTS_MAP::iterator it = points.begin(); it != points.end(); ++it) {
                if(it->first == the_current) {
                    pCurPt = &it->second;
                }
            }

        doSTCA(*pCurPt, false);

        i++;

    } while (i < 7);

    int end = 1;

}


void doSTCA(const Point4DInfo& curPt, bool extrapolate) {

    // Манипуляция с треками (сделать алгоритм анализа треков):
    // Добавление новых, удаление тех, что уже сели или пропали
    analysePoints(points, curPt, extrapolate);

    // устновка M, N для подтверждения соответствующих фильтров, где это используется
    std::map<int, std::pair<unsigned int, unsigned int> > mn;
    mn[STCATasks::CONFIRMATION_LINEAR_FILTER] = make_pair<int, int>(params.linearPredictionCycleCount, params.linearPredictionConflictCount);
    mn[STCATasks::CONFIRMATION_PROXIMITY_FILTER] = make_pair<int, int>(params.currentProximityCycleCount, params.currentProximityConflictCount);
    mn[STCATasks::CONFIRMATION_TURNING__FILTER] = make_pair<int, int>(params.turningPredictionCycleCount, params.turningPredictionConflictCount);
    historyMofN.setParams(mn);

    /*QTextStream(stdout) << xmlNormlist << "\n" << xsdNormlist << "\n";
    //Separation<Point4D> sep("../NormViolations/normlistTemp.xsd", "../NormViolations/normlistTemp.xml");
    Separation<Point4D> sep(xsdNormlist, xmlNormlist);*/

    // создаем класс STCA для проверки самолета в точке curPt на конфликтность
    // со всеми самолетами из points
    STCA stca(points, curPt, params, task_flags, historyMofN, outStream, *pSep);

    // Самолеты, с которыми конфликтует рассматриваемое судно, собираются в контейнер
    std::vector<int> conflictNums;

    //*outStream << "Pt.size = " << points.size() << "\n";
    //outStream->flush();

    stca.doAllFilters(conflictNums);

    *outStream << "conflictNums.size = " << conflictNums.size() << "\n";
    outStream->flush();
}

