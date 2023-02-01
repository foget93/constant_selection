#include "widget.h"

#include "cnt_reader.h"
#include "stat_reader.h"
#include "data_calculator.h"

#include <QApplication>
#include <iostream>

using namespace std;

std::vector<Axes<int>> VecToAxes(std::vector<std::vector<int>> vec) {
    std::vector<Axes<int>> result;
        for(int j {0}; j < static_cast<int>(vec[0].size()); ++j) {
            result.push_back(Axes{vec[0][j],
                                  vec[1][j],
                                  vec[2][j]}
                             );
        }

    return result;
}

int main(/*int argc, char *argv[]*/)
{
    ReaderCNT cnt("/home/dmitrii/QtExample/build-cnt_selection-Desktop_Qt_6_3_0_GCC_64bit-Debug/C212.CNT");
    //
    ReaderStat stat("/home/dmitrii/QtExample/build-cnt_selection-Desktop_Qt_6_3_0_GCC_64bit-Debug/A212-1.4");

    Axes<int> sum_192 {stat.GetSum192()};
    std::vector<Axes<int>> R = stat.GetMtInfo();
    std::vector<std::vector<int>> constants = cnt.GetConstantsMT();
    std::vector<Axes<int>> b = VecToAxes(constants);

    DataCalculator calc(b, R, sum_192, stat.GetGeoCfs());
    std::vector<Axes<double>> x = calc.CalculationMT();
    //DataCalculator(const std::vector<Axes>& cnt, const std::vector<Axes>& r, Axes sum192)

//    int a = 0;
//    QApplication a(argc, argv);
//    Widget w;
//    w.show();
//    return a.exec();
    return 0;
}
