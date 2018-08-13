#include "graphio126.h"
#include "ui_graphio126.h"
#include "igraphio_p.h"
#include "iuiwidget.h"
#include "uiio.h"
#include "sevdevice.h"
#include "boxitemmapping.h"
#include "gtutils.h"

#include <QDebug>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QMessageBox>

#define GEN_CMD_IO_REVERSE "gSevDrv.no.prm.mot_brk_out_polarity"

class GraphIO126Private:public IGraphIOPrivate {
    Q_DECLARE_PUBLIC(GraphIO126)
public:
    GraphIO126Private(){}
    ~GraphIO126Private(){}
public:
    QList<QCheckBox*> m_boxList;
    QList<QLabel*> m_labelList;
};

GraphIO126::GraphIO126(QWidget *parent) :
    IGraphIO(*(new GraphIO126Private), parent),
    ui(new Ui::GraphIO126)
{
    ui->setupUi(this);
}

GraphIO126::~GraphIO126()
{
    delete ui;
}

void GraphIO126::syncTreeDataToUiFace()
{

}

void GraphIO126::setCustomVisitActive(IUiWidget *uiWidget)
{

}

void GraphIO126::setUiVersionName()
{
    Q_D(GraphIO126);
    d->m_versionName = "V126";
    setObjectName("GraphIO126");
}

void GraphIO126::setupDataMappings()
{

}

void GraphIO126::onCheckBoxStateChanged(int axisIndex)
{
    Q_D(GraphIO126);
    qDebug()<<"index clicked"<<axisIndex;
    bool checked = d->m_boxList.at(axisIndex)->isChecked();
    if (!d->m_dev->isConnecting()) {
        d->m_boxList.at(axisIndex)->setChecked(!checked);
        return;
    }
    if (d->m_dev->axisServoIsOn(axisIndex)) {
        QMessageBox::warning(0, tr("Warning"), tr("Axis%1 is on Servo!").arg(axisIndex));
        return;
    }
    if (checked) {
        QMessageBox::StandardButton rb = QMessageBox::question(0, tr("Warning"), tr("Are you sure to loose the brake?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (!(rb == QMessageBox::Yes)) {
            return;
        }
    }
    QTreeWidgetItem *item = d->m_treeWidget->topLevelItem(0)->child(0)->child(axisIndex)->child(1);
    //QString memberType=item->text(TREE_IO_MEMTYPE_COL);
    qint16 address = item->text(TREE_IO_ADDR_COL).toShort(0,16);
    quint16 bitstart = item->text(TREE_IO_BITSTART_COL).toUShort();
    quint16 bitwidth = item->text(TREE_IO_BITWIDTH_COL).toUShort();
    qint16 base = item->text(TREE_IO_BASEADDR_COL).toShort();
    //QString controlType=item->text(TREE_IO_CONTROLTYPE_COL);
    quint16 value = 0;
    ComDriver::int16_t value16 = 0;
    quint32 operation = operationValue(bitstart, bitwidth);

    int count = 0;
    quint16 err = 2;
    do {
        err = d->m_dev->socketCom()->readFPGAReg16(base, address, value16, 0);
        count++;
    } while (count < 2 && err != 0);
    value = value16;

    if (checked) {
        value16 = (quint16)(operation|value);
    } else {
        value16 = (quint16)((~operation)&value);
    }

    count = 0;
    err = -1;
    do {
        err = d->m_dev->socketCom()->writeFPGAReg16(base, address, value16, 0);
        count++;
    } while (count < 2 && err != 0);

    count = 0;
    err = -1;
    do {
        err = d->m_dev->socketCom()->readFPGAReg16(base, address, value16, 0);
        count++;
    } while (count < 2 && err != 0);
    value = value16;

    bool ret = (bool)(operation&value);
    if (ret) {
        item->setText(TREE_IO_VALUE_COL, "1");
        d->m_dev->setBrakeMarkOn(axisIndex, false);
        d->m_labelList.at(axisIndex)->setText(tr("State: ") + tr("Unlocked"));
    } else {
        item->setText(TREE_IO_VALUE_COL, "0");
        d->m_dev->setBrakeMarkOn(axisIndex, true);
        d->m_labelList.at(axisIndex)->setText(tr("State: ") + tr("Locked"));
    }
}

quint32 GraphIO126::operationValue(quint32 bitstart, quint32 bitwidth)
{
  quint32 temp = 0;
  for (quint32 i = 0; i < bitwidth; i++)
  {
    temp <<= 1;
    temp += 1;
  }
  temp <<= bitstart;
  return temp;
}

void GraphIO126::setBoxStyleSheet(QCheckBox *box)
{
    QString style = "QCheckBox {spacing: 1px;}QCheckBox::indicator {width: 64px;height: 64px;}QCheckBox::indicator:unchecked {image: url(" + GTUtils::iconPath() + "checkbox_unchecked_io.png" + ");}QCheckBox::indicator:unchecked:hover {image: url(" + GTUtils::iconPath() + "checkbox_unchecked_hover_io.png" + ");}"
                    "QCheckBox::indicator:checked:hover {image: url(" + GTUtils::iconPath() + "checkbox_checked_hover_io.png" + ");}QCheckBox::indicator:checked {image: url("  + GTUtils::iconPath() + "checkbox_checked_io.png" + ");}";
    box->setStyleSheet(style);
}

void GraphIO126::initUi()
{
    Q_D(GraphIO126);
    if (d->m_dev == NULL) {
        return;
    }
    QSignalMapper *pMapper = new QSignalMapper();
    for (int i = 0; i < d->m_dev->axisNum(); i++) {
        QVBoxLayout *vBox = new QVBoxLayout;
        QCheckBox *ioBox = new QCheckBox;
        QLabel *axisLabel = new QLabel;
        QLabel *stateLabel = new QLabel;
        setBoxStyleSheet(ioBox);
        axisLabel->setText(tr("Motor Brake Polarity\n\nAxis%1").arg(i + 1));
        axisLabel->setAlignment(Qt::AlignCenter);
        if (d->m_dev->isConnecting()) {
            QTreeWidgetItem *item = d->m_treeWidget->topLevelItem(0)->child(0)->child(i)->child(1);
            qint16 address = item->text(TREE_IO_ADDR_COL).toShort(0,16);
            qint16 base = item->text(TREE_IO_BASEADDR_COL).toShort();
            ComDriver::int16_t value = 2;
            quint16 err;
            int count = 0;
            do {
                err = d->m_dev->socketCom()->readFPGAReg16(base, address, value, 0);
                count++;
            } while (count < 2 && err != 0);

            if (value == 1) {
                ioBox->setChecked(true);
                stateLabel->setText(tr("State: ") + tr("Unlocked"));
                d->m_dev->setBrakeMarkOn(i, false);
            } else if (value == 0) {
                ioBox->setChecked(false);
                stateLabel->setText(tr("State: ") + tr("Locked"));
                d->m_dev->setBrakeMarkOn(i, true);
            }
        } else {
            ioBox->setChecked(false);
            stateLabel->setText(tr("State: ") + tr("Locked"));
        }
        stateLabel->setAlignment(Qt::AlignCenter);
        QHBoxLayout *hBox = new QHBoxLayout;
        QSpacerItem *spacer=new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QSpacerItem *spacer2=new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        connect(ioBox, SIGNAL(clicked()), pMapper, SLOT(map()));
        pMapper->setMapping(ioBox, i);
        hBox->addItem(spacer);
        hBox->addWidget(ioBox);
        hBox->addItem(spacer2);
        vBox->addWidget(axisLabel);
        vBox->addLayout(hBox);
        vBox->addWidget(stateLabel);
        d->m_boxList.append(ioBox);
        d->m_labelList.append(stateLabel);
        ui->ioLayout126->addLayout(vBox);
    }
    connect(pMapper, SIGNAL(mapped(int)), this, SLOT(onCheckBoxStateChanged(int)));
}

void GraphIO126::visit(IUiWidget *uiWidget)
{
    initPrivateData(uiWidget);
    setUiVersionName();
}

void GraphIO126::initPrivateData(IUiWidget *uiWidget)
{
    Q_D(GraphIO126);
    d->m_dev = uiWidget->device();
    d->m_uiWidget = uiWidget;
    int page = uiWidget->uiIndexs().pageInx;
    d->m_treeWidget = d->m_dev->globalTreeSource(page);
}
