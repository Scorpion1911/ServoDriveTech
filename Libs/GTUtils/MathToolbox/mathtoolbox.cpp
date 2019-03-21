#include "mathtoolbox.h"
#include <QVector>
#include <QDebug>

MathToolbox::MathToolbox(QObject *parent) : QObject(parent)
{

}

void MathToolbox::differential(QVector<double> srcData, QVector<double> &resultData, double timeInterval, int startIndex, int step)
{
    double result = 0;
    for (int i = startIndex; i < srcData.size(); i = i + step) {
        if (i + step >= srcData.size()) {
            double result = resultData.at(i);
            for (int j = i + 1; j < srcData.size(); j++) {
                resultData.append(result);
            }
        } else {
            result = (srcData.at(i + step) - srcData.at(i)) / (step * timeInterval);
            if (i == startIndex) {
                for (int j = 0; j < startIndex + 1; j++) {
                    resultData.append(result);
                }
            }
            double ave = (result - resultData.at(i)) / step;
            for (int j = step; j > 0; j--) {
                resultData.append(result - (j - 1) * ave);
            }
        }
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

void MathToolbox::linearInterpolate(QVector<double> srcData, QVector<double> &resultData, int step)
{

    for (int i = 0; i < srcData.size(); i = i + step) {
        resultData.append(srcData.at(i));
        if (i + step >= srcData.size()) {
            for (int j = 1; j < step; j++) {
                if (i + j < srcData.size()) {
                    resultData.append(srcData.at(i + j));
                }
            }
        } else {
            for (int j = 1; j < step; j++) {
                double ave = (srcData.at(i + step) - srcData.at(i)) / step;
                resultData.append(srcData.at(i) + j * ave);
            }
        }
    }
}

