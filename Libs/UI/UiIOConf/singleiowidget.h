#ifndef SINGLEIOWIDGET_H
#define SINGLEIOWIDGET_H

#include <QWidget>

namespace Ui {
class SingleIOWidget;
}

class SingleIOWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SingleIOWidget(QWidget *parent = 0);
    ~SingleIOWidget();
    void setIOStatus(bool en);
    void setIOName(const QString &ioName);
    void setIOFuncList(const QStringList &ioFuncList);
    void setPinName(const QString &pinName);
    void setIONote(const QString &ioNote);
    void setReverseStatus(bool en);
    void setFuncListEnable(bool en);
    void setFuncIndex(int index);
    void setBoxIndex(int index);
    void setCheckable(bool en);
    void setControlSrcList(const QStringList &srcList);
    void setControlSrc(int index);

    bool getReverseStatus();
    bool getIOStatus();
    int getFuncIndex();
    int getBoxIndex();
    int getControlSrc();

signals:
    void funcIndexChanged(int funcIndex, int preFuncIndex, int boxIndex);
    void reverseStatusChanged(bool checked, int boxIndex);
    void enStatusChanged(int index, bool en);
    void controlSrcChanged(int index);
private slots:
    void onBoxIndexChanged(int index);
    void onControlSrcChanged(int index);
    void onReverseBoxChanged(bool checked);
    void onEnStatusChanged(bool en);
private:
    Ui::SingleIOWidget *ui;
    int m_index;
    int m_preFuncIndex;
    int m_curFunIndex;
};

#endif // SINGLEIOWIDGET_H
