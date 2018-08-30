#ifndef DFTDIALOG_H
#define DFTDIALOG_H

#include <QDialog>

class ICurve;
class DFTPrivate;
class QTableWidgetItem;

namespace Ui {
class DFTDialog;
}

typedef enum {
    ANALYSIS_SIGNAL = 0,
    ANALYSIS_SYSTEM = 1
} AnalysisType;

typedef enum {
    COL_FFT_SHOW = 0,
    COL_FFT_NAME = 1,
    COL_FFT_NOTE = 2
} COL_FFT_CURVE;

typedef struct {
    AnalysisType fftType;
    int inputIndex;
    int outputIndex;
} FFTInfo;

typedef struct {
    QVector<double> freq;
    QVector<double> amp;
    QVector<double> phase;
} FFTResult;

class DFTDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DFTDialog(QWidget *parent = 0);
    ~DFTDialog();

    void setCurveList(const QList<ICurve *> &curveList);
    void setSamplingPeriod(double period);
    void setTimeRange(double min, double max);
public slots:
    void onFFTTimeChanged(qreal x1, qreal x2, qreal dx);
    void onComboBoxActivated(int index);
signals:
    void fftDialogClosed();
    void sendSaveMsg(int value, const QString &msg);
protected:
    void closeEvent(QCloseEvent *event);
private:
    void initUi();
    void enableAllUi(bool enabled);
    void setConnections();
    void addCurve(const QString &name, const QString &fullName, const QString &note, const QColor &color, bool isShow, FFTResult &fftData);
    void fftAnalysis(FFTInfo &fftInfo);
private slots:
    void onBtnAmpClicked(bool checked);
    void onBtnPhaClicked(bool checked);
    void onBtnSignalClicked();
    void onBtnSettingClicked();
    void onAxisRangeChanged(QVector<double> limitList);
    void onBtnFitClicked();
    void onBtnOpenClicked();
    void onBtnSaveClicked();
    void onBtnAllClicked();
    void onBtnClearClicked();
    void onBtnRemoveClicked();
    void onBtnAddClicked();

    void onPlotAmplitudeMouseMoved(QMouseEvent *event);
    void onPlotPhaseMouseMoved(QMouseEvent *event);

    void onTableItemClicked(QTableWidgetItem * item);
    void onTableWidgetCellEnteredMoreDetail(int row, int col);

    void getSaveMsg(int value, const QString &msg);
private:
    Ui::DFTDialog *ui;
    DFTPrivate *dd;
};

#endif // DFTDIALOG_H
