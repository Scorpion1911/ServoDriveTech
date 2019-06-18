#include "graphioconf138.h"
#include "ui_graphioconf138.h"
#include "igraphioconf_p.h"
#include "singleiowidget.h"
#include "bititemhelper.h"
#include "sevdevice.h"
#include "iuiwidget.h"

#include <QDebug>
#include <QTreeWidget>
#include <QMessageBox>
#include <QTimer>

#define INPUT_NUM 11
#define OUTPUT_NUM 7

#define INPUT_FUNC_INDEX 9
#define OUTPUT_FUNC_INDEX 19
#define INPUT_REVERSE_INDEX 20
#define OUTPUT_REVERSE_INDEX 21

#define CMD_READ_INPUT_EN "gSevDrv.sev_obj.pos.apio.io_in_reg"
#define CMD_READ_OUTPUT_EN "gSevDrv.sev_obj.pos.apio.io_out_reg"

class GraphIOConf138Private:public IGraphIOConfPrivate
{
  Q_DECLARE_PUBLIC(GraphIOConf138)
public:
  GraphIOConf138Private(){}
  ~GraphIOConf138Private(){}
  QList<SingleIOWidget*> m_inputList;
  QList<SingleIOWidget*> m_outputList;
  QStringList m_inputNoteList;
  QStringList m_outputNoteList;
  QStringList m_inputFuncList;
  QStringList m_outputFuncList;
  QStringList m_inputPinList;
  QStringList m_outputPinList;
  QList<bool> m_inputEnableList;
  QList<bool> m_outputEnableList;
};

GraphIOConf138::GraphIOConf138(QWidget *parent) :
    IGraphIOConf(*(new GraphIOConf138Private),parent),
    ui(new Ui::GraphIOConf138)
{
    Q_D(GraphIOConf138);
    ui->setupUi(this);
    d->m_inputNoteList<<tr("")
                <<tr("")
               <<tr("")
              <<tr("")
             <<tr("")
            <<tr("")
           <<tr("")
          <<tr("")
         <<tr("")
        <<tr("")
       <<tr("")
      <<tr("")
     <<tr("")
    <<tr("")
    <<tr("")
    <<tr("");

    d->m_outputNoteList<<tr("")
                <<tr("")
               <<tr("")
              <<tr("")
             <<tr("")
            <<tr("")
           <<tr("")
          <<tr("")
         <<tr("")
        <<tr("")
       <<tr("")
      <<tr("")
     <<tr("")
    <<tr("")
    <<tr("")
    <<tr("");

    d->m_inputFuncList<<tr("0 Empty")
                    <<tr("1 Servo On")
                   <<tr("2 Emergency Stop")
                  <<tr("3 Clear Alarm")
                 <<tr("4 Mode Switch")
                <<tr("5 Accurate Stop")
               <<tr("6 Return Orientation")
              <<tr("7 Positive Limit Switch")
             <<tr("8 Negative Limit Switch")
            <<tr("9 Home Switch");
    d->m_outputFuncList<<tr("0 Empty")
                     <<tr("1 Is Alarm")
                    <<tr("2 Is Moving")
                   <<tr("3 Is Stop Accurate")
                  <<tr("4 Is Velocity Achieved")
                 <<tr("5 Is Return Successful")
                <<tr("6 Is Servo Ready")
               <<tr("7 Is Homing Finished")
              <<tr("8 Is Phase Searched")
             <<tr("9 Is Current out  of Range")
            <<tr("10 Is Brake on");

    d->m_inputPinList<<tr("C_3")
                <<tr("C_20")
               <<tr("C_31")
              <<tr("C_14")
             <<tr("C_32_F")
            <<tr("C_15_F")
           <<tr("M_5")
          <<tr("M_15")
         <<tr("M_6")
        <<tr("M_16")
       <<tr("M_7_F")
      <<tr("General IO")
     <<tr("General IO")
    <<tr("General IO")
    <<tr("General IO")
    <<tr("General IO");

    d->m_outputPinList<<tr("C_2")
                <<tr("C_33")
               <<tr("C_16_F")
              <<tr("M_17")
             <<tr("M_8")
            <<tr("M_18_F")
           <<tr("M_10_20")
          <<tr("General IO")
         <<tr("General IO")
        <<tr("General IO")
       <<tr("General IO")
      <<tr("General IO")
     <<tr("General IO")
    <<tr("General IO")
    <<tr("General IO")
    <<tr("General IO");

    d->m_inputEnableList<<true
                       <<true
                      <<true
                     <<true
                    <<true
                   <<true
                  <<true
                 <<true
                <<true
               <<true
              <<true
             <<true
            <<true
           <<true
          <<true
         <<true;

    d->m_outputEnableList<<true
                       <<true
                      <<true
                     <<true
                    <<true
                   <<true
                  <<true
                 <<true
                <<true
               <<true
              <<true
             <<true
            <<true
           <<true
          <<true
         <<true;

    d->m_timer=new QTimer;
    d->m_timer->setInterval(1000);
    connect(d->m_timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
}

GraphIOConf138::~GraphIOConf138()
{
    Q_D(GraphIOConf138);
    if (d->m_timer->isActive())
    {
        d->m_timer->stop();
        delete d->m_timer;
    }
    delete ui;
}

void GraphIOConf138::syncTreeDataToUiFace()
{
    Q_D(GraphIOConf138);
    qDebug()<<"input count"<<d->m_inputList.count();
    for (int i = 0; i < d->m_inputList.count(); i++) {
        d->m_inputList.at(i)->setFuncIndex(0);
    }
    qDebug()<<"output count"<<d->m_inputList.count();
    for (int i = 0; i < d->m_outputList.count(); i++) {
        d->m_outputList.at(i)->setFuncIndex(0);
    }
    for (int i = 0; i < d->m_treeWidget->topLevelItemCount(); i++) {
        qDebug()<<"i"<<i;
        QString str = d->m_treeWidget->topLevelItem(i)->text(GT::COL_PAGE_TREE_VALUE);
        double value = str.toDouble();
        qDebug()<<"value"<<value;
        if (i < INPUT_FUNC_INDEX) {
            if (value == 0) {
                continue;
            }
            int index = (int)log2(value);
            qDebug()<<"index"<<index;
            if (index >= INPUT_NUM) {
                continue;
            }
            d->m_inputList.at(index)->setFuncIndex(i + 1);
        } else if (i < OUTPUT_FUNC_INDEX) {
            if (value == 0) {
                continue;
            }
            int index = (int)log2(value);
            qDebug()<<"index"<<index;
            if (index >= OUTPUT_NUM) {
                continue;
            }
            d->m_outputList.at(index)->setFuncIndex(i - INPUT_FUNC_INDEX + 1);
        } else if (i < INPUT_REVERSE_INDEX) {
            QString binStr = BitItemHelper::decToBin16(str);
            int len = binStr.length();
            for (int j = 0; j < len; j++) {
                if (j >= d->m_inputList.count()) {
                    break;
                }
                int mode = binStr.at(len - 1 - j).digitValue();
                if (mode == 1) {
                    d->m_inputList.at(j)->setReverseStatus(true);
                } else if (mode == 0) {
                    d->m_inputList.at(j)->setReverseStatus(false);
                }
            }
        } else if (i < OUTPUT_REVERSE_INDEX) {
            QString binStr = BitItemHelper::decToBin16(str);
            int len = binStr.length();
            for (int j = 0; j < len; j++) {
                if (j >= d->m_outputList.count()) {
                    break;
                }
                int mode = binStr.at(len - 1 - j).digitValue();
                if (mode == 1) {
                    d->m_outputList.at(j)->setReverseStatus(true);
                } else if (mode == 0) {
                    d->m_outputList.at(j)->setReverseStatus(false);
                }
            }
        }
    }
}

void GraphIOConf138::syncUiToTree()
{
    Q_D(GraphIOConf138);
    bool ok = checkFuncSetting();
    QString inputReverseStr = "0000000000000000";
    QString outputReverseStr = "0000000000000000";
    if (!ok) {
        return;
    }
    for (int i = 0; i < OUTPUT_FUNC_INDEX; i++) {
        d->m_treeWidget->topLevelItem(i)->setText(GT::COL_PAGE_TREE_VALUE, "0");
    }
    for (int i = 0; i < INPUT_NUM; i++) {
        bool reverse = d->m_inputList.at(i)->getReverseStatus();
        if (reverse) {
            inputReverseStr.replace(inputReverseStr.length() - 1 - i, 1, "1");
        }
        int index = d->m_inputList.at(i)->getFuncIndex();
        if (index == 0) {
            continue;
        }
        d->m_treeWidget->topLevelItem(index - 1)->setText(GT::COL_PAGE_TREE_VALUE, QString::number(pow(2, i)));
    }
    for (int i = 0; i < OUTPUT_NUM; i++) {
        bool reverse = d->m_outputList.at(i)->getReverseStatus();
        if (reverse) {
            outputReverseStr.replace(outputReverseStr.length() - 1 - i, 1, "1");
        }
        int index = d->m_outputList.at(i)->getFuncIndex();
        if (index == 0) {
            continue;
        }
        d->m_treeWidget->topLevelItem(INPUT_FUNC_INDEX + index - 1)->setText(GT::COL_PAGE_TREE_VALUE, QString::number(pow(2, i)));
    }
    QString inputReverseValue = BitItemHelper::binToDec(inputReverseStr);
    d->m_treeWidget->topLevelItem(INPUT_REVERSE_INDEX - 1)->setText(GT::COL_PAGE_TREE_VALUE, inputReverseValue);
    QString outputReverseValue = BitItemHelper::binToDec(outputReverseStr);
    d->m_treeWidget->topLevelItem(OUTPUT_REVERSE_INDEX - 1)->setText(GT::COL_PAGE_TREE_VALUE, outputReverseValue);
}

void GraphIOConf138::setCustomVisitActive(IUiWidget *uiWidget)
{
    Q_UNUSED(uiWidget);
}

void GraphIOConf138::setUiVersionName()
{
    Q_D(GraphIOConf138);
    d->m_versionName = "V138";
}

void GraphIOConf138::initUi()
{
    Q_D(GraphIOConf138);
    QVBoxLayout *inBox = new QVBoxLayout;
    QVBoxLayout *outBox = new QVBoxLayout;
    for (int i = 0; i < INPUT_NUM; i++) {
        SingleIOWidget* inW = new SingleIOWidget;
        inW->setCheckable(false);
        inW->setIOStatus(false);
        inW->setIOName(tr("Input_%1").arg(i));
        inW->setIOFuncList(d->m_inputFuncList);
        inW->setReverseStatus(false);
        inW->setFuncListEnable(d->m_inputEnableList.at(i));
        inW->setPinName(d->m_inputPinList.at(i));
        inW->setIONote(d->m_inputNoteList.at(i));
        inW->setBoxIndex(i);
        d->m_inputList.append(inW);
        inBox->addWidget(inW);
    }
    ui->scrollAreaWidget_input->setLayout(inBox);
    for (int i = 0; i < OUTPUT_NUM; i++) {
        SingleIOWidget* outW = new SingleIOWidget;
        outW->setCheckable(true);
        outW->setIOStatus(false);
        outW->setIOName(tr("Output_%1").arg(i));
        outW->setIOFuncList(d->m_outputFuncList);
        outW->setReverseStatus(false);
        outW->setPinName(d->m_outputPinList.at(i));
        outW->setIONote(d->m_outputNoteList.at(i));
        outW->setFuncListEnable(d->m_outputEnableList.at(i));
        outW->setBoxIndex(i);
        d->m_outputList.append(outW);
        connect(outW, SIGNAL(enStatusChanged(int,bool)), this, SLOT(onEnStatusChanged(int,bool)));
        outBox->addWidget(outW);
    }
    ui->scrollAreaWidget_output->setLayout(outBox);
}

void GraphIOConf138::visit(IUiWidget *uiWidget)
{
    initPrivateData(uiWidget);
    setUiVersionName();
}

void GraphIOConf138::setTimerActive(bool en)
{
    Q_D(GraphIOConf138);
    if (en) {
        d->m_timer->start();
    } else {
        d->m_timer->stop();
    }
}

void GraphIOConf138::setupDataMappings() {

}

bool GraphIOConf138::checkFuncSetting()
{
    Q_D(GraphIOConf138);
    QVector<int> indexList;
    for (int i = 0; i < INPUT_NUM; i++) {
        int index = d->m_inputList.at(i)->getFuncIndex();
        if (index == 0) {
            continue;
        }
        if (indexList.contains(index)) {
            QMessageBox::warning(0, tr("Warning"), tr("Cannot set a same function for different inputs!"));
            return false;
        }
        indexList.append(index);
    }
    indexList.clear();
    for (int i = 0; i < OUTPUT_NUM; i++) {
        int index = d->m_outputList.at(i)->getFuncIndex();
        if (index == 0) {
            continue;
        }
        if (indexList.contains(index)) {
            QMessageBox::warning(0, tr("Warning"), tr("Cannot set a same function for different outputs!"));
            return false;
        }
        indexList.append(index);
    }
    return true;
}

void GraphIOConf138::onTimeOut()
{
    Q_D(GraphIOConf138);
    bool ok;
    quint64 inputValue = d->m_dev->genCmdRead(CMD_READ_INPUT_EN, d->m_uiWidget->uiIndexs().axisInx, ok);
    if (!ok) {
        return;
    }
    quint64 outputValue = d->m_dev->genCmdRead(CMD_READ_OUTPUT_EN, d->m_uiWidget->uiIndexs().axisInx, ok);
    if (!ok) {
        return;
    }
    QString inputStr = BitItemHelper::decToBin16(QString::number(inputValue));
    QString outputStr = BitItemHelper::decToBin16(QString::number(outputValue));

    int inputLen = inputStr.length();
    for (int i = 0; i < inputLen; i++) {
        if (i >= d->m_inputList.count()) {
            break;
        }
        int mode = inputStr.at(inputLen - 1 - i).digitValue();
        if (mode == 1) {
            d->m_inputList.at(i)->setIOStatus(true);
        } else if (mode == 0) {
            d->m_inputList.at(i)->setIOStatus(false);
        }
    }

    int outputLen = outputStr.length();
    for (int i = 0; i < outputLen; i++) {
        if (i >= d->m_outputList.count()) {
            break;
        }
        int mode = outputStr.at(outputLen - 1 - i).digitValue();
        if (mode == 1) {
            d->m_outputList.at(i)->setIOStatus(true);
        } else if (mode == 0) {
            d->m_outputList.at(i)->setIOStatus(false);
        }
    }
}

void GraphIOConf138::onEnStatusChanged(int index, bool en)
{
    Q_D(GraphIOConf138);
    if (!d->m_dev->isConnecting()) {
        d->m_outputList.at(index)->setIOStatus(false);
        return;
    }
    bool ok;
    quint64 outputValue = d->m_dev->genCmdRead(CMD_READ_OUTPUT_EN, d->m_uiWidget->uiIndexs().axisInx, ok);
    if (!ok) {
        return;
    }
    QString outputStr = BitItemHelper::decToBin16(QString::number(outputValue));
    int outputLen = outputStr.length();
    if (en) {
        outputStr.replace(outputLen - 1 - index, 1, "1");
    } else {
        outputStr.replace(outputLen - 1 - index, 1, "0");
    }
    QString decStr = BitItemHelper::binToDec(outputStr);
    quint64 value = decStr.toLongLong();
    d->m_dev->genCmdWrite(CMD_READ_OUTPUT_EN, value, d->m_uiWidget->uiIndexs().axisInx);
}
