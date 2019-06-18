#include "advuserstation.h"
#include "ui_advuserstation.h"
#include "sevdevice.h"
#include "iadvuser_p.h"

#include <QMessageBox>

#define    STATION_ADDR         28700
#define    AUTO_MODE_FLAG       0xFFEF
#define    MANUAL_MODE_FLAG     0x0010

class AdvUserStationPrivate : public IAdvUserPrivate
{
   Q_DECLARE_PUBLIC(AdvUserStation)

public:
    AdvUserStationPrivate();
    ~AdvUserStationPrivate();
    quint16 m_stationId;
    bool m_isAuto;
    QList<SevDevice *> m_devList;
    SevDevice* m_curDev;
};

AdvUserStationPrivate::AdvUserStationPrivate()
{

}

AdvUserStationPrivate::~AdvUserStationPrivate()
{

}

AdvUserStation::AdvUserStation(const QString &advUserName, QWidget *parent) :
    IAdvUser(advUserName, *new AdvUserStationPrivate, parent),
    ui(new Ui::AdvUserStation)
{
    ui->setupUi(this);
    connect(ui->spinBox_stationid, SIGNAL(valueChanged(int)), this, SLOT(onStationIdValueChanged(int)));
    connect(ui->radioBtn_auto, SIGNAL(toggled(bool)), this, SLOT(onBtnChecked(bool)));
}

AdvUserStation::~AdvUserStation()
{
    delete ui;
}

QString AdvUserStation::nickName()
{
    return tr("Device Station");
}

void AdvUserStation::uiInit()
{
    Q_D(AdvUserStation);
    d->m_curDev = NULL;
    disconnect(ui->comboBox_deviceList, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    ui->comboBox_deviceList->clear();
    if (d->m_devList.count() != 0) {
         d->m_curDev = d->m_devList.at(0);
         for (int i = 0; i < d->m_devList.length(); i++) {
//             bool hasNickName = d->m_devList.count() > 1;
             QString prefix;
//             prefix = hasNickName?tr("[%1] ").arg(d->m_devList.at(i)->aliasName()):"";
             prefix = tr("[%1] ").arg(d->m_devList.at(i)->aliasName());
             ui->comboBox_deviceList->addItem(prefix + d->m_devList.at(i)->modelName());
         }
     }
     connect(ui->comboBox_deviceList, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
     ui->comboBox_deviceList->setCurrentIndex(0);
     if (d->m_curDev != NULL) {
         quint16 configValue = 0;
         bool ok = d->m_curDev->readFLASH16ByAddr(0, STATION_ADDR, configValue);
         if (ok) {
             d->m_stationId = (configValue & 0xFF00) >> 8;
             ui->spinBox_stationid->setValue(d->m_stationId);
             int mode = (configValue & MANUAL_MODE_FLAG) >> 4;
             if (mode == 0) {
                 ui->radioBtn_manual->setChecked(true);
                 d->m_isAuto = false;
             } else {
                 ui->radioBtn_auto->setChecked(true);
                 d->m_isAuto = true;
             }
         }
     }
}

void AdvUserStation::setSevList(const QList<SevDevice *> &list)
{
    Q_D(AdvUserStation);
    d->m_devList = list;
}

bool AdvUserStation::advUserActive()
{
    Q_D(AdvUserStation);
    bool ok;
    quint16 configValue = 0;
    ok = d->m_curDev->readFLASH16ByAddr(0, STATION_ADDR, configValue);
    if (!ok) {
        return false;
    }
    if (d->m_isAuto) {
        configValue |= MANUAL_MODE_FLAG;
    } else {
        configValue &= AUTO_MODE_FLAG;
    }
    configValue = (configValue & 0x00FF) | (d->m_stationId << 8);
    ok = d->m_curDev->writeFLASH16ByAddr(0, STATION_ADDR, configValue);
    if (!ok) {
        return false;
    }
    return true;
}

bool AdvUserStation::readAdv()
{
    return true;
}

bool AdvUserStation::writeAdv()
{
    return true;
}

void AdvUserStation::respondErrorExecute()
{
    QMessageBox::warning(this, tr("Warning"), tr("Setting Error"), QMessageBox::Ok);
    return;
}

void AdvUserStation::onComboBoxIndexChanged(int index)
{
    Q_D(AdvUserStation);
    d->m_curDev = d->m_devList.at(index);
    if (d->m_curDev != NULL) {
        quint16 configValue = 0;
        bool ok = d->m_curDev->readFLASH16ByAddr(0, STATION_ADDR, configValue);
        if (ok) {
            d->m_stationId = (configValue & 0xFF00) >> 8;
            ui->spinBox_stationid->setValue(d->m_stationId);
            int mode = (configValue & MANUAL_MODE_FLAG) >> 4;
            if (mode == 0) {
                ui->radioBtn_manual->setChecked(true);
                d->m_isAuto = false;
            } else {
                ui->radioBtn_auto->setChecked(true);
                d->m_isAuto = true;
            }
        }
    }
}

void AdvUserStation::onStationIdValueChanged(int value)
{
    Q_D(AdvUserStation);
    d->m_stationId = value;
    d->m_isModify = true;
}

void AdvUserStation::onBtnChecked(bool checked)
{
    Q_D(AdvUserStation);
    d->m_isAuto = checked;
    d->m_isModify = true;
}


