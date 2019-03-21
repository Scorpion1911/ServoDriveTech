#ifndef PTPLANMOTION_H
#define PTPLANMOTION_H

#include <QObject>
#include <QTimer>
#include <QVector>

class SevDevice;
class UiPTSLMotionData;

class PTPlanMotion : public QObject
{
    Q_OBJECT
public:
    explicit PTPlanMotion(quint16 axisInx, SevDevice *dev, UiPTSLMotionData *data, QObject *parent = 0);
    ~PTPlanMotion();
    QTimer *timer();
    void movePrepare();
    void move();
    void stop();
private:
    bool writeData(int index);

signals:
    void motionFinish(quint16 axisInx);
    void progressValueChanged(quint16 axisInx,int value);

public slots:
private slots:
    void onTimerOut();
private:
    QTimer m_timer;
    SevDevice *m_sev;
    UiPTSLMotionData *m_data;
    quint16 m_axisInx;
    QVector<qint32> m_timeVector;
    QVector<qint32> m_posVector;
    int m_count;
    int m_length;
};

#endif // PTPLANMOTION_H
