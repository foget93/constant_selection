#include "widget.h"
#include "cnt_reader.h"
#include <stat_reader.h>

#include <QApplication>
#include <iostream>

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    ReaderCNT aa("/home/dmitrii/QtExample/build-cnt_selection-Desktop_Qt_6_3_0_GCC_64bit-Debug/C212.CNT");
    //
    ReaderStat bb("/home/dmitrii/QtExample/build-cnt_selection-Desktop_Qt_6_3_0_GCC_64bit-Debug/A212-1.1");
//    int a = 0;
//    QApplication a(argc, argv);
//    Widget w;
//    w.show();
//    return a.exec();
}
