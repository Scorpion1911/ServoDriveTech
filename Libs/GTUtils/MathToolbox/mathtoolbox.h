#ifndef MATHTOOLBOX_H
#define MATHTOOLBOX_H

#include "gtutils_global.h"

#include <QObject>

class GTUTILSSHARED_EXPORT MathToolbox : public QObject
{
    Q_OBJECT
public:
    explicit MathToolbox(QObject *parent = 0);
    static void differential(QVector<double> srcData, QVector<double> &resultData, double timeInterval, int startIndex, int step);
    static void smoothingFilter(QVector<double> srcData, QVector<double> &resultData, int width);
    static void linearInterpolate(QVector<double> srcData, QVector<double> &resultData, int step);


signals:

public slots:
};

#endif // MATHTOOLBOX_H
