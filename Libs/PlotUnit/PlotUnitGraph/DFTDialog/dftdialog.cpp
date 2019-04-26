#include <QDataStream>
#include <QDebug>

#include "dftdialog.h"
#include "ui_dftdialog.h"
#include "gtplot.h"
#include "icurve.h"
#include "gtutils.h"
#include "Option"
#include "dftsettingdialog.h"
#include "ServoDriverAlgorithmDll.h"
#include "qcustomplot.h"

#define ICON_FFT_AMP "plot_fft_amp.png"
#define ICON_FFT_PHA "plot_fft_pha.png"
#define ICON_FFT_SET "plot_config.png"
#define ICON_FFT_FIT "plot_fit.png"
#define ICON_FFT_OPEN "plot_open.png"
#define ICON_FFT_SAVE "plot_save.png"
#define ICON_FFT_ADD "plot_add.png"
#define ICON_FFT_ALL "plot_all.png"
#define ICON_FFT_REMOVE "plot_remove.png"
#define ICON_FFT_CLEAR "plot_clear.png"

class DFTPrivate {
public:
    QList<ICurve*> m_curveList;
    DFTSettingDialog *m_setDialog;
    AnalysisType m_type;
    FFTInfo m_fftInfo;
    FFTResult m_fftResult;
    double m_period;
    double m_piRadio;
    QVector<bool> m_showVector;
    QStringList m_curveFullName;
};

DFTDialog::DFTDialog(QWidget *parent) :
    QDialog(parent),
    dd(new DFTPrivate),
    ui(new Ui::DFTDialog)
{
    ui->setupUi(this);
    dd->m_piRadio = 57.3;
    initUi();
    setConnections();
}

DFTDialog::~DFTDialog()
{
    delete ui;
}

void DFTDialog::setCurveList(const QList<ICurve *> &curveList)
{
    dd->m_curveList = curveList;
    ui->comboBox_fft_input->clear();
    ui->comboBox_fft_output->clear();
    for (int i = 0; i < dd->m_curveList.count(); i++) {
        ICurve* c =dd->m_curveList.at(i);
        QString curveStr = "(" + tr("Axis_%1").arg(c->axisInx() + 1) + ")" + c->name() + "(" + c->curUnitName() + ")";
        ui->comboBox_fft_input->addItem(curveStr);
        ui->comboBox_fft_output->addItem(curveStr);
    }
}

void DFTDialog::setSamplingPeriod(double period)
{
    dd->m_period = period;
}

void DFTDialog::setTimeRange(double min, double max)
{
    ui->doubleSpinBox_fft_start->setMinimum(min);
    ui->doubleSpinBox_fft_terminal->setMaximum(max);
}

void DFTDialog::onFFTTimeChanged(qreal x1, qreal x2, qreal dx)
{
    Q_UNUSED(dx);
    ui->doubleSpinBox_fft_start->setValue(x1);
    ui->doubleSpinBox_fft_terminal->setValue(x2);
}

void DFTDialog::closeEvent(QCloseEvent *event)
{
    emit fftDialogClosed();
    event->accept();
}

void DFTDialog::initUi()
{
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    QSize iconSize(24,24);
    OptFace *face = dynamic_cast<OptFace *>(OptContainer::instance()->optItem("optface"));
    QString iconPath = GTUtils::customPath() + "option/style/" + face->css() + "/icon/";
    ui->tbtn_fft_add->setIcon(QIcon(QPixmap(iconPath + ICON_FFT_ADD)));
    ui->tbtn_fft_add->setIconSize(iconSize);
    ui->tbtn_fft_clear->setIcon(QIcon(QPixmap(iconPath + ICON_FFT_CLEAR)));
    ui->tbtn_fft_clear->setIconSize(iconSize);
    ui->tbtn_fft_fit->setIcon(QIcon(QPixmap(iconPath + ICON_FFT_FIT)));
    ui->tbtn_fft_fit->setIconSize(iconSize);
    ui->tbtn_fft_amp->setIcon(QIcon(QPixmap(iconPath + ICON_FFT_AMP)));
    ui->tbtn_fft_amp->setIconSize(iconSize);
    ui->tbtn_fft_open->setIcon(QIcon(QPixmap(iconPath + ICON_FFT_OPEN)));
    ui->tbtn_fft_open->setIconSize(iconSize);
    ui->tbtn_fft_phase->setIcon(QIcon(QPixmap(iconPath + ICON_FFT_PHA)));
    ui->tbtn_fft_phase->setIconSize(iconSize);
    ui->tbtn_fft_remove->setIcon(QIcon(QPixmap(iconPath + ICON_FFT_REMOVE)));
    ui->tbtn_fft_remove->setIconSize(iconSize);
    ui->tbtn_fft_save->setIcon(QIcon(QPixmap(iconPath + ICON_FFT_SAVE)));
    ui->tbtn_fft_save->setIconSize(iconSize);
    ui->tbtn_fft_selectAll->setIcon(QIcon(QPixmap(iconPath + ICON_FFT_ALL)));
    ui->tbtn_fft_selectAll->setIconSize(iconSize);
    ui->tbtn_fft_setting->setIcon(QIcon(QPixmap(iconPath + ICON_FFT_SET)));
    ui->tbtn_fft_setting->setIconSize(iconSize);

    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);
    ui->label_fft_processInfo->setVisible(false);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setMouseTracking(true);

    onBtnSignalClicked();

    ui->plot_amp->xAxis->setRange(1, 10000);
    ui->plot_amp->yAxis->setRange(-50, 50);
    ui->plot_phase->xAxis->setRange(1, 10000);
    ui->plot_phase->yAxis->setRange(-180, 180);

    ui->plot_amp->replot();
    ui->plot_phase->replot();
}

void DFTDialog::enableAllUi(bool enabled)
{
    ui->widget_fft_left->setEnabled(enabled);
    ui->widget_fft_right->setEnabled(enabled);
    ui->widget_fft_up->setEnabled(enabled);
}

void DFTDialog::setConnections()
{
    connect(ui->tbtn_fft_amp, SIGNAL(clicked(bool)), this, SLOT(onBtnAmpClicked(bool)));
    connect(ui->tbtn_fft_phase, SIGNAL(clicked(bool)), this, SLOT(onBtnPhaClicked(bool)));
    connect(ui->radioBtn_signal, SIGNAL(clicked()), this, SLOT(onBtnSignalClicked()));
    connect(ui->radioBtn_system, SIGNAL(clicked()), this, SLOT(onBtnSignalClicked()));
    connect(ui->tbtn_fft_setting, SIGNAL(clicked()), this, SLOT(onBtnSettingClicked()));
    connect(ui->tbtn_fft_fit, SIGNAL(clicked()), this, SLOT(onBtnFitClicked()));
    connect(ui->tbtn_fft_open, SIGNAL(clicked()), this, SLOT(onBtnOpenClicked()));
    connect(ui->tbtn_fft_save, SIGNAL(clicked()), this, SLOT(onBtnSaveClicked()));
    connect(ui->tbtn_fft_add, SIGNAL(clicked()), this, SLOT(onBtnAddClicked()));
    connect(ui->tbtn_fft_remove, SIGNAL(clicked()), this, SLOT(onBtnRemoveClicked()));
    connect(ui->tbtn_fft_clear, SIGNAL(clicked()), this, SLOT(onBtnClearClicked()));
    connect(ui->tbtn_fft_selectAll, SIGNAL(clicked()), this, SLOT(onBtnAllClicked()));
    connect(ui->comboBox_fft_input, SIGNAL(activated(int)), this, SLOT(onComboBoxActivated(int)));

    connect(ui->plot_amp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onPlotAmplitudeMouseMoved(QMouseEvent*)));
    connect(ui->plot_phase, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onPlotPhaseMouseMoved(QMouseEvent*)));

    connect(ui->tableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableItemClicked(QTableWidgetItem*)));
    connect(ui->tableWidget, SIGNAL(cellEntered(int, int)), this, SLOT(onTableWidgetCellEnteredMoreDetail(int, int)));

    connect(this, SIGNAL(sendSaveMsg(int,QString)), this, SLOT(getSaveMsg(int,QString)));
}

void DFTDialog::addCurve(const QString &name, const QString &fullName, const QString &note, const QColor &color, bool isShow, FFTResult &fftData)
{
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(name);
    item->setTextColor(color);
    item->setFlags(Qt::NoItemFlags);
    ui->tableWidget->setItem(rowCount, COL_FFT_NAME, item);

    QString showText;
    if(isShow)
      showText = tr("on");
    else
      showText = tr("off");
    item = new QTableWidgetItem();
    item->setText(showText);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setFlags(Qt::NoItemFlags);
    ui->tableWidget->setItem(rowCount, COL_FFT_SHOW, item);

    item = new QTableWidgetItem();
    item->setText(note);
    ui->tableWidget->setItem(rowCount, COL_FFT_NOTE, item);

//    item=new QTableWidgetItem();
//    item->setText(QString::number(id));
//    ui->tableWidget->setItem(rowCount, 3, item);

    dd->m_showVector.append(isShow);
    dd->m_curveFullName.append(fullName);

    ui->plot_amp->addGraph();
    ui->plot_amp->graph(rowCount)->setPen(QPen(color));
    ui->plot_amp->graph(rowCount)->addData(fftData.freq, fftData.amp);
    ui->plot_amp->rescaleAxes(true);

    ui->plot_phase->addGraph();
    ui->plot_phase->graph(rowCount)->setPen(QPen(color));
    ui->plot_phase->graph(rowCount)->addData(fftData.freq, fftData.phase);
    ui->plot_phase->rescaleAxes(true);

//    QPen pen;
//    pen.setColor(Qt::yellow);
//    pen.setWidth(4);
//    ui->plot_amp->graph(rowCount)->setSelectedPen(pen);
//    ui->plot_phase->graph(rowCount)->setSelectedPen(pen);

    if(isShow == false)
    {
        ui->plot_amp->graph(rowCount)->setVisible(false);
        ui->plot_phase->graph(rowCount)->setVisible(false);
    }
    ui->plot_amp->replot();
    ui->plot_phase->replot();
}

void DFTDialog::fftAnalysis(FFTInfo &fftInfo)
{
    dd->m_fftResult.amp.clear();
    dd->m_fftResult.freq.clear();
    dd->m_fftResult.phase.clear();

    double start = ui->doubleSpinBox_fft_start->value();
    double end = ui->doubleSpinBox_fft_terminal->value();

    double t1 = dd->m_curveList.at(fftInfo.inputIndex)->sData()->keys.first();
    double t2 = dd->m_curveList.at(fftInfo.inputIndex)->sData()->keys.last();
    double errorT = t2 - t1;
    int startIndex = 0;
    int endIndex = 0;
    startIndex = (start / errorT) * dd->m_curveList.at(fftInfo.inputIndex)->sData()->keys.count();
    endIndex = (end / errorT) * dd->m_curveList.at(fftInfo.inputIndex)->sData()->keys.count();
    if ((endIndex - startIndex + 1) % 2 != 0) {
        endIndex = endIndex - 1;
    }
//    qDebug()<<"start "<<start<<" end"<<end;
//    qDebug()<<"t1 "<<t1<<"t2 "<<t2;
//    qDebug()<<"1-> "<<start/errorT<<"2-> "<<end/errorT;
//    qDebug()<<"startindex "<<startIndex<<" endindex"<<endIndex;
//    qDebug()<<"length "<<endIndex-startIndex+1;
    double samplingFreq;
    int length = endIndex - startIndex + 1;
    double *in = new double[length];
    double *out = new double[length];
    double *outp = new double[length];
    samplingFreq = (1.0 / dd->m_period) * 1000;
    for (int i = 0; i < length / 2; i++)
    {
        dd->m_fftResult.freq.append(samplingFreq * i / length);
    }

    QVector<double>::const_iterator valueIt = dd->m_curveList.at(fftInfo.inputIndex)->sData()->values.cbegin();

    for (int i = 0; i < length; i++)
    {
        in[i] = (*valueIt);
        valueIt++;
    }

    if(fftInfo.fftType == ANALYSIS_SIGNAL)
    {
        SignalAnalysisFFT(in, out, outp, length);
    }
    else
    {
        double *rout = new double[length];
        QVector<double>::const_iterator outIt = dd->m_curveList.at(fftInfo.outputIndex)->sData()->values.cbegin();
        for (int i = 0; i < length; i++) {
            rout[i] = (*outIt);
            outIt++;
        }
        RespAnalysisInFreDomain(in, rout, out, outp, length);
        delete []rout;
    }
    for(int i = 0; i < length / 2; i++)
    {
        dd->m_fftResult.amp.append(out[i]);
        dd->m_fftResult.phase.append(outp[i] * dd->m_piRadio);
    }
    delete []in;//释放数组
    delete []out;
    delete []outp;
}

void DFTDialog::onBtnAmpClicked(bool checked)
{
    if (checked) {
        ui->plot_amp->setVisible(true);
        ui->label_amp->setVisible(true);
    } else {
        ui->plot_amp->setVisible(false);
        ui->label_amp->setVisible(false);
    }
}

void DFTDialog::onBtnPhaClicked(bool checked)
{
    if (checked) {
        ui->plot_phase->setVisible(true);
        ui->label_phase->setVisible(true);
    } else {
        ui->plot_phase->setVisible(false);
        ui->label_phase->setVisible(false);
    }
}

void DFTDialog::onBtnSignalClicked()
{
    if (ui->radioBtn_signal->isChecked()) {
        dd->m_type = ANALYSIS_SIGNAL;
        ui->label_fft_output->setVisible(false);
        ui->comboBox_fft_output->setVisible(false);
    } else {
        dd->m_type = ANALYSIS_SYSTEM;
        ui->label_fft_output->setVisible(true);
        ui->comboBox_fft_output->setVisible(true);
    }
}

void DFTDialog::onBtnSettingClicked()
{
    dd->m_setDialog = new DFTSettingDialog(0);
    connect(dd->m_setDialog, SIGNAL(axisRangeChanged(QVector<double>)), this, SLOT(onAxisRangeChanged(QVector<double>)));
    dd->m_setDialog->show();
}

void DFTDialog::onAxisRangeChanged(QVector<double> limitList)
{
    ui->plot_amp->xAxis->setRange(limitList.at(1), limitList.at(0));
    ui->plot_amp->yAxis->setRange(limitList.at(3), limitList.at(2));
    ui->plot_phase->xAxis->setRange(limitList.at(5), limitList.at(4));
    ui->plot_phase->yAxis->setRange(limitList.at(7), limitList.at(6));

    ui->plot_amp->replot();
    ui->plot_phase->replot();
}

void DFTDialog::onBtnFitClicked()
{
    ui->plot_amp->rescaleAxes(true);
    double low = ui->plot_amp->yAxis->range().lower;
    double upper = ui->plot_amp->yAxis->range().upper;
    double size = ui->plot_amp->yAxis->range().size();
    double dsize = 0.1 * size;
    QCPRange plottableRange(low - dsize, upper + dsize);
    ui->plot_amp->yAxis->setRange(plottableRange);
    ui->plot_amp->replot();

    ui->plot_phase->rescaleAxes(true);
    low = ui->plot_phase->yAxis->range().lower;
    upper = ui->plot_phase->yAxis->range().upper;
    size = ui->plot_phase->yAxis->range().size();
    dsize = 0.1 * size;
    QCPRange plottableRange_phase(low - dsize, upper + dsize);
    ui->plot_phase->yAxis->setRange(plottableRange_phase);
    ui->plot_phase->replot();
}

void DFTDialog::onBtnOpenClicked()
{
    QString fileName;
    QString iniPath = GTUtils::data(GTUtils::customPath() + "option/opt.ini", "path", "curvepath", ".").toString();
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), iniPath, tr("FFT Curve File(*.dat)"));
    if (fileName.compare("") == 0) {
        return;
    }
    //qDebug()<<"fileName"<<fileName;

    QFile file;
    file.setFileName(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,tr("file error"),tr("can not open file :\n%1").arg(fileName));
        return;
    }

    enableAllUi(false);
    ui->progressBar->setVisible(true);
    ui->label_fft_processInfo->setVisible(true);
    QDataStream in(&file);
    int curveCount;
    quint16 streamVersion;
    in>>streamVersion>>curveCount;
    in.setVersion(streamVersion);

    QString name;
    QString fullName;
    QString note;
    QColor color;
    bool isShown;
    FFTResult result;

    for (int i = 0; i < curveCount; i++) {
        emit sendSaveMsg((i + 1) * 100 / curveCount, tr("Reading Curve %1").arg(i + 1));
        in>>name>>fullName>>note>>color>>isShown;
        in>>result.freq>>result.amp>>result.phase;
        addCurve(name, fullName, note, color, isShown, result);
    }
    emit sendSaveMsg(0, tr("Reading Finish!"));
    file.close();
    GTUtils::delayms(1000);
    ui->progressBar->setVisible(false);
    ui->label_fft_processInfo->setVisible(false);
    enableAllUi(true);
}

void DFTDialog::onBtnSaveClicked()
{
    QString iniPath = GTUtils::data(GTUtils::customPath() + "option/opt.ini", "path", "curvepath", ".").toString();
    iniPath = iniPath + "/fftData_" + QDate::currentDate().toString("yyyyMMdd") + "_" + QTime::currentTime().toString("hhmmss");
    QString filePath = QFileDialog::getSaveFileName(0, tr("Save FFT Curve"), iniPath , tr("FFT Curve File(*.dat)"));
    if (filePath.compare("") == 0) {
        return;
    }
    QFile fdata(filePath);
    int curveNum = ui->tableWidget->rowCount();
    if (fdata.open(QFile::WriteOnly | QFile::Truncate)) {
        enableAllUi(false);
        ui->progressBar->setVisible(true);
        ui->label_fft_processInfo->setVisible(true);
        QDataStream out(&fdata);
        out.setVersion(QDataStream::Qt_5_5);
        out<<quint16(out.version())<<curveNum;
        for (int i = 0; i < curveNum; i++) {
            emit sendSaveMsg((i + 1) * 100 / curveNum, tr("Saving Curve %1").arg(i + 1));
            out<<ui->tableWidget->item(i, COL_FFT_NAME)->text();
            out<<dd->m_curveFullName.at(i);
            out<<ui->tableWidget->item(i, COL_FFT_NOTE)->text();
            out<<ui->tableWidget->item(i, COL_FFT_NAME)->textColor();
            out<<dd->m_showVector.at(i);
            QVector<double> keyVector;
            QVector<double> ampValueVector;
            QVector<double> phaseValueVector;
            for (int j = 0; j < ui->plot_amp->graph(i)->dataCount(); j++) {
                keyVector.append(ui->plot_amp->graph(i)->dataMainKey(j));
                ampValueVector.append(ui->plot_amp->graph(i)->dataMainValue(j));
                phaseValueVector.append(ui->plot_phase->graph(i)->dataMainValue(j));
            }
            out<<keyVector;
            out<<ampValueVector;
            out<<phaseValueVector;
        }
        emit sendSaveMsg(0, tr("Saving Finish!"));
        fdata.close();
        GTUtils::delayms(1000);
        ui->progressBar->setVisible(false);
        ui->label_fft_processInfo->setVisible(false);
        enableAllUi(true);
    }
}

void DFTDialog::onBtnAllClicked()
{
    bool static flag=true;
    if (flag) {
    int row = 0;
        QMutableVectorIterator<bool> it(dd->m_showVector);
        while (it.hasNext()) {
            bool temp = it.next();
            if (!temp) {
                it.setValue(true);
                ui->tableWidget->item(row, COL_FFT_SHOW)->setText(tr("on"));
            }
            row++;
        }
    } else {
        int row = 0;
        QMutableVectorIterator<bool> it(dd->m_showVector);
        while (it.hasNext()) {
            bool temp = it.next();
            if (temp) {
                it.setValue(false);
                ui->tableWidget->item(row, COL_FFT_SHOW)->setText(tr("off"));
            }
            row++;
        }
    }
    flag = !flag;
    int row = 0;
    foreach (bool checked, dd->m_showVector) {
        if (checked) {
            ui->plot_amp->graph(row)->setVisible(true);
            ui->plot_phase->graph(row)->setVisible(true);
        } else {
            ui->plot_amp->graph(row)->setVisible(false);
            ui->plot_phase->graph(row)->setVisible(false);
        }
        row++;
    }
    ui->plot_amp->replot();
    ui->plot_phase->replot();
}

void DFTDialog::onBtnClearClicked()
{
    if (ui->tableWidget->rowCount() >= 0) {
        ui->plot_amp->clearGraphs();
        ui->plot_phase->clearGraphs();
        dd->m_showVector.clear();
        dd->m_curveFullName.clear();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->clearContents();
        ui->plot_amp->replot();
        ui->plot_phase->replot();
    }
}

void DFTDialog::onBtnRemoveClicked()
{
    int row = 0;
    row = ui->tableWidget->currentRow();
    if (row >= 0)
    {
        ui->plot_amp->removeGraph(row);
        ui->plot_phase->removeGraph(row);
        dd->m_showVector.removeAt(row);
        dd->m_curveFullName.removeAt(row);
        ui->tableWidget->removeRow(row);
        ui->plot_amp->replot();
        ui->plot_phase->replot();
    }
}

void DFTDialog::onBtnAddClicked()
{
    if (dd->m_curveList.length() == 0) {
        return;
    }
    double startT, endT, min, max;
    startT = ui->doubleSpinBox_fft_start->value();
    endT = ui->doubleSpinBox_fft_terminal->value();
    min = dd->m_curveList.at(0)->sData()->keys.first();
    max = dd->m_curveList.at(0)->sData()->keys.last();

    if (startT < min) {
        //qDebug()<<tr("min %1, max %2").arg(min).arg(max);
        QMessageBox::information(0, tr("Range Error"), tr("StartTime out range"));
        return;
    }
    if (endT > max) {
        //qDebug()<<tr("min %1, max %2").arg(min).arg(max);
        QMessageBox::information(0,tr("Range Error"),tr("EndTime out range"));
        return;
    }

    double start = ui->doubleSpinBox_fft_start->value();
    double end = ui->doubleSpinBox_fft_terminal->value();
    double temp = 0;
    if (end < start) {
        temp = start;
        start = end;
        end = temp;
        ui->doubleSpinBox_fft_start->setValue(start);
        ui->doubleSpinBox_fft_terminal->setValue(end);
    }

    QString name;
    QString fullName;
    QColor color;
    int length;
    int index;
    length = ui->comboBox_fft_input->currentText().length();
    index = ui->comboBox_fft_input->currentText().indexOf(")");

    int length2;
    int index2;

    switch (dd->m_type) {
    case ANALYSIS_SIGNAL:
        fullName = tr("%1 ( %2 - %3 s)").arg(ui->comboBox_fft_input->currentText())\
            .arg(QString::number(ui->doubleSpinBox_fft_start->value(), 'f', 2))\
            .arg(QString::number(ui->doubleSpinBox_fft_terminal->value(), 'f', 2));
        name = tr("%1").arg(ui->comboBox_fft_input->currentText().right(length - index - 1));
        dd->m_fftInfo.inputIndex = ui->comboBox_fft_input->currentIndex();
        dd->m_fftInfo.outputIndex = -1;
        break;
    case ANALYSIS_SYSTEM:
        fullName = tr(">>%1\n<<%2\n( %3 - %4 s)").arg(ui->comboBox_fft_input->currentText())\
            .arg(ui->comboBox_fft_output->currentText())\
            .arg(QString::number(ui->doubleSpinBox_fft_start->value(), 'f', 2))\
            .arg(QString::number(ui->doubleSpinBox_fft_terminal->value(), 'f', 2));
        length2 = ui->comboBox_fft_output->currentText().length();
        index2 = ui->comboBox_fft_output->currentText().lastIndexOf(")");
        name = tr(">>%1\n<<%2").arg(ui->comboBox_fft_input->currentText().right(length - index - 1))\
            .arg(ui->comboBox_fft_output->currentText().right(length2 - index2 - 1));
        dd->m_fftInfo.inputIndex = ui->comboBox_fft_input->currentIndex();
        dd->m_fftInfo.outputIndex = ui->comboBox_fft_output->currentIndex();
        break;
    default:
        break;
    }

    dd->m_fftInfo.fftType = dd->m_type;

    fftAnalysis(dd->m_fftInfo);
    color = dd->m_curveList.at(dd->m_fftInfo.inputIndex)->color();
    addCurve(name, fullName, tr(""), color, true, dd->m_fftResult);
}

void DFTDialog::onPlotAmplitudeMouseMoved(QMouseEvent *event)
{
    double dx = ui->plot_amp->xAxis->pixelToCoord(event->x());
    double dy = ui->plot_amp->yAxis->pixelToCoord(event->y());

    QCPRange cpX = ui->plot_amp->xAxis->range();
    QCPRange cpY = ui->plot_amp->yAxis->range();

    if (cpX.contains(dx) && cpY.contains(dy)) {
        ui->label_amp->setText(tr("Amplitude Information:  frequency: %1 HZ , AMP: %2 DB")\
                                       .arg(QString::number(dx, 'E', 4))\
                                       .arg(QString::number(dy, 'f', 2)));
    }
}

void DFTDialog::onPlotPhaseMouseMoved(QMouseEvent *event)
{
    double dx = ui->plot_phase->xAxis->pixelToCoord(event->x());
    double dy = ui->plot_phase->yAxis->pixelToCoord(event->y());

    QCPRange cpX = ui->plot_phase->xAxis->range();
    QCPRange cpY = ui->plot_phase->yAxis->range();

    if (cpX.contains(dx) && cpY.contains(dy)) {
        ui->label_phase->setText(tr("Phase Information:  frequency: %1 HZ , Phase: %2 deg")\
                                       .arg(QString::number(dx, 'E', 4))\
                                       .arg(QString::number(dy, 'f', 2)));
    }
}

void DFTDialog::onTableItemClicked(QTableWidgetItem *item)
{
    int row = item->row();
    if (item->column() == COL_FFT_SHOW) {
        QString str;
        if (dd->m_showVector.at(row)) {
            str=tr("off");
            dd->m_showVector[row] = false;
        } else {
            str = tr("on");
            dd->m_showVector[row]=true;
        }
        QFont font;
        font.setBold(true);
        font.setPixelSize(12);
        item->setText(str);
        item->setFont(font);
        ui->plot_amp->graph(row)->setVisible(dd->m_showVector.at(row));
        ui->plot_amp->replot();
        ui->plot_phase->graph(row)->setVisible(dd->m_showVector.at(row));
        ui->plot_phase->replot();
    }
}

void DFTDialog::onTableWidgetCellEnteredMoreDetail(int row, int col)
{
    if (col == COL_FFT_NAME) {
        ui->label_curveInfo->setText(tr("%1").arg(dd->m_curveFullName.at(row)));
    }
}

void DFTDialog::getSaveMsg(int value, const QString &msg)
{
    ui->progressBar->setValue(value);
    ui->label_fft_processInfo->setText(msg);
}

void DFTDialog::onComboBoxActivated(int index)
{
    double min, max;
    double tempMin, tempMax;
    if (dd->m_curveList.at(index)->sData()->keys.size() == 1) {
        return;
    }
    min = dd->m_curveList.at(index)->sData()->keys.first();
    tempMin = min;
    do {
        tempMin += dd->m_period / 1000;
        ui->doubleSpinBox_fft_start->setValue(tempMin);
    } while(ui->doubleSpinBox_fft_start->value() < min);

    max = dd->m_curveList.at(index)->sData()->keys.last();
    tempMax = max;
    do {
        tempMax -= dd->m_period / 1000;
        ui->doubleSpinBox_fft_terminal->setValue(tempMax);
    } while(ui->doubleSpinBox_fft_terminal->value() > max);
    setTimeRange(min, max);
}
