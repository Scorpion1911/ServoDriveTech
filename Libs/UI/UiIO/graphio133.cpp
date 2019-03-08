#include "graphio133.h"
#include "ui_graphio133.h"
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

#define GEN_CMD_IO_REVERSE "gSevDrv.sev_obj.cur.pwr.act_res_ob_flag"

class GraphIO133Private:public IGraphIOPrivate {
    Q_DECLARE_PUBLIC(GraphIO133)
public:
    GraphIO133Private(){}
    ~GraphIO133Private(){}
public:
    QList<QCheckBox*> m_boxList;
    QList<QLabel*> m_labelList;
};

GraphIO133::GraphIO133(QWidget *parent) :
    IGraphIO(*(new GraphIO133Private), parent),
    ui(new Ui::GraphIO133)
{
    ui->setupUi(this);
}

GraphIO133::~GraphIO133()
{
    delete ui;
}

void GraphIO133::syncTreeDataToUiFace()
{

}

void GraphIO133::setCustomVisitActive(IUiWidget *uiWidget)
{

}

void GraphIO133::setUiVersionName()
{
    Q_D(GraphIO133);
    d->m_versionName = "V133";
    setObjectName("GraphIO133");
}

void GraphIO133::setupDataMappings()
{

}

void GraphIO133::onCheckBoxStateChanged(int axisIndex)
{
    Q_D(GraphIO133);
    qDebug()<<"index clicked"<<axisIndex;
    bool checked = d->m_boxList.at(axisIndex)->isChecked();
    if (!d->m_dev->isConnecting()) {
        d->m_boxList.at(axisIndex)->setChecked(!checked);
        return;
    }
    if (d->m_dev->axisServoIsOn(axisIndex)) {
        QMessageBox::warning(0, tr("Warning"), tr("Axis%1 is on Servo!").arg(axisIndex));
        d->m_boxList.at(axisIndex)->setChecked(!checked);
        return;
    }
    if (checked) {
        QMessageBox::StandardButton rb = QMessageBox::question(0, tr("Warning"), tr("Are you sure to loose the brake?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (!(rb == QMessageBox::Yes)) {
            d->m_boxList.at(axisIndex)->setChecked(!checked);
            return;
        }
        d->m_dev->genCmdWrite(GEN_CMD_IO_REVERSE, 1, axisIndex);
        d->m_dev->setBrakeMarkOn(axisIndex, false);
        d->m_labelList.at(axisIndex)->setText(tr("State: ") + tr("Unlocked"));
    } else {
        d->m_dev->genCmdWrite(GEN_CMD_IO_REVERSE, 0, axisIndex);
        d->m_dev->setBrakeMarkOn(axisIndex, true);
        d->m_labelList.at(axisIndex)->setText(tr("State: ") + tr("Locked"));
    }
}

void GraphIO133::setBoxStyleSheet(QCheckBox *box)
{
    QString style = "QCheckBox {spacing: 1px;}QCheckBox::indicator {width: 64px;height: 64px;}QCheckBox::indicator:unchecked {image: url(" + GTUtils::iconPath() + "checkbox_unchecked_io.png" + ");}QCheckBox::indicator:unchecked:hover {image: url(" + GTUtils::iconPath() + "checkbox_unchecked_hover_io.png" + ");}"
                    "QCheckBox::indicator:checked:hover {image: url(" + GTUtils::iconPath() + "checkbox_checked_hover_io.png" + ");}QCheckBox::indicator:checked {image: url("  + GTUtils::iconPath() + "checkbox_checked_io.png" + ");}";
    box->setStyleSheet(style);
}

void GraphIO133::initUi()
{
    Q_D(GraphIO133);
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
        bool isOk;
        if (d->m_dev->isConnecting()) {
            qint16 ret = d->m_dev->genCmdRead(GEN_CMD_IO_REVERSE, i, isOk);
            if (ret == 1) {
                ioBox->setChecked(true);
                stateLabel->setText(tr("State: ") + tr("Unlocked"));
                d->m_dev->setBrakeMarkOn(i, false);
            } else if (ret == 0) {
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
        ui->ioLayout133->addLayout(vBox);
    }
    connect(pMapper, SIGNAL(mapped(int)), this, SLOT(onCheckBoxStateChanged(int)));
}

void GraphIO133::visit(IUiWidget *uiWidget)
{
    initPrivateData(uiWidget);
    setUiVersionName();
}

void GraphIO133::initPrivateData(IUiWidget *uiWidget)
{
    Q_D(GraphIO133);
    d->m_dev = uiWidget->device();
    d->m_uiWidget = uiWidget;
//    int page = uiWidget->uiIndexs().pageInx;
//    d->m_treeWidget = d->m_dev->globalTreeSource(page);
}
