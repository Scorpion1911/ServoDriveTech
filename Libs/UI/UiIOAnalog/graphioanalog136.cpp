#include "graphioanalog136.h"
#include "ui_graphioanalog136.h"
#include "igraphioanalog_p.h"
#include "iuiwidget.h"
#include "uiioanalog.h"
#include "sevdevice.h"

#include <QDebug>
#include <QTreeWidget>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QTimer>

#define OFST_POS_X -50
#define OFST_POS_Y -50

#define SCALE_INDEX 0
#define OFST_INDEX 1

#define CMD_READ_OUTPUT_VOLTAGE "FPGA.ANALOGIO.VOLTAGE.OUTPUT"

class GraphIOAnalog136Private:public IGraphIOAnalogPrivate
{
  Q_DECLARE_PUBLIC(GraphIOAnalog136)
public:
  GraphIOAnalog136Private(){}
  ~GraphIOAnalog136Private(){}
};

GraphIOAnalog136::GraphIOAnalog136(QWidget *parent) :
    IGraphIOAnalog(*(new GraphIOAnalog136Private),parent),
    ui(new Ui::GraphIOAnalog136)
{
    Q_D(GraphIOAnalog136);
    ui->setupUi(this);
    d->m_scene = new QGraphicsScene;
    createInputItems();
    createOutputItems();

    d->m_timer = new QTimer;
    d->m_timer->setInterval(1000);
    connect(d->m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

GraphIOAnalog136::~GraphIOAnalog136()
{
    Q_D(GraphIOAnalog136);
    if (d->m_timer->isActive())
    {
        d->m_timer->stop();
        delete d->m_timer;
    }
    delete ui;
}

void GraphIOAnalog136::syncTreeDataToUiFace()
{
    Q_D(GraphIOAnalog136);
    d->m_mapping->syncAllItem2BoxText();
}

void GraphIOAnalog136::setCustomVisitActive(IUiWidget *uiWidget)
{
    Q_UNUSED(uiWidget);
}

void GraphIOAnalog136::setUiVersionName()
{
    Q_D(GraphIOAnalog136);
    d->m_versionName = "V136";
    setObjectName("GraphIOAnalog136");
}

void GraphIOAnalog136::setupDataMappings()
{
    Q_D(GraphIOAnalog136);
    d->m_mapping->insertBox2Item(d->m_ofstEdit, d->m_treeWidget->topLevelItem(OFST_INDEX));
    d->m_mapping->insertBox2Item(d->m_scaleEdit, d->m_treeWidget->topLevelItem(SCALE_INDEX));

    d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(OFST_INDEX), d->m_ofstEdit);
    d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(SCALE_INDEX), d->m_scaleEdit);
}

QPointF GraphIOAnalog136::ofstInitPos()
{
    return QPointF(OFST_POS_X, OFST_POS_Y);
}

void GraphIOAnalog136::setParaScale()
{
    Q_D(GraphIOAnalog136);
    bool ok;
    quint64 nos;
    nos = d->m_dev->genCmdReadNos(d->m_uiWidget->uiIndexs().axisInx, ok);
    if (nos == 0) {
        return;
    }
    double scale = 10 * 4096 / (double)nos;
    d->m_treeWidget->topLevelItem(SCALE_INDEX)->setText(GT::COL_PAGE_TREE_SCALE, QString::number(scale));
}

void GraphIOAnalog136::setTimerActive(bool en)
{
    Q_D(GraphIOAnalog136);
    if (en) {
        d->m_timer->start();
    } else {
        d->m_timer->stop();
    }
}

void GraphIOAnalog136::createInputItems()
{
    Q_D(GraphIOAnalog136);
    ui->graphicsView_input->setScene(d->m_scene);
    createStartText();
    createOffsetWidget();
    createVoltageScale();
    createVoltageCommand();
    createStartEndItem();
    createEndText();

    createArrowItems();
    createAnchorItemHelper();
    setUpItemPosAnchors();
    adjustPosition();
}

void GraphIOAnalog136::createOutputItems()
{

}

void GraphIOAnalog136::onTimeOut()
{
    Q_D(GraphIOAnalog136);
    bool ok;
    qint16 value = d->m_dev->genCmdRead(CMD_READ_OUTPUT_VOLTAGE, d->m_uiWidget->uiIndexs().axisInx, ok);
    int ofst = d->m_treeWidget->topLevelItem(OFST_INDEX)->text(GT::COL_PAGE_TREE_VALUE).toInt();
    int voltageValue = value - ofst;
    if (voltageValue > 32767) {
        voltageValue = 32767;
    } else if (voltageValue < -32767) {
        voltageValue = -32767;
    }
    double voltage = (double)voltageValue / 32767 * 10;
    d->m_commandEdit->setValue(voltage);
}
