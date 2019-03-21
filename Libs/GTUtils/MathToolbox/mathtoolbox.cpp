#include "mathtoolbox.h"
#include <QVector>
#include <QDebug>

MathToolbox::MathToolbox(QObject *parent) : QObject(parent)
{

}

void MathToolbox::differential(QVector<double> srcData, QVector<double> &resultData, double timeInterval, int startIndex)
{
    double result = 0;
    for (int i = startIndex; i < srcData.size(); i++) {
        result = (srcData.at(i) - srcData.at(i - 1)) / timeInterval;
        if (i == startIndex) {
            for (int j = 0; j < startIndex; j++) {
                resultData.append(result);
            }
        }
        resultData.append(result);
    }
}

void MathToolbox::smoothingFilter(QVector<double> srcData, QVector<double> &resultData, int width)
{
    double result = 0;
    for (int i = 0; i < srcData.size(); i++) {
        if (i >= width / 2 && i <= srcData.size() - (width / 2 + 1)) {
            result = 0;
            for (int j = 0; j < width; j++) {
                result = result + srcData.at(i - (width / 2) + j);
            }
            resultData.append(result / width);
        } else {
            resultData.append(srcData.at(i));
        }
    }
}

