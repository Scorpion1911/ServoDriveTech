﻿#include "uimotionPTSL.h"
#include "ui_uimotionPTSL.h"
#include "motionPTSL.h"
#include "sevdevice.h"
#include "Option"
#include "gtutils.h"

#include <QListWidget>
#include <QKeyEvent>
#include <QDebug>

#define PIC_NAME_VPOINT          "plot_vPoint.png"
#define PIC_NAME_VRECI           "plot_vReci.png"

UiMotionPTSL::UiMotionPTSL(MotionPTSL *mp, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiMotionPTSL),
    q_ptr(mp)
    //m_timer(new QTimer(this))
{
    ui->setupUi(this);
    ui->stackedWidget_Point->setCurrentIndex(0);
    m_axisCount = mp->sevDevice()->axisNum();

    ui->comboBox_pointAcc->addItem("ms");
    ui->comboBox_pointDec->addItem("ms");
    ui->comboBox_pointMaxVel->addItem("rpm");
    ui->comboBox_pointTimes->addItem("");
    ui->comboBox_pointCons->addItem("ms");

    ui->comboBox_reciAcc->addItem("ms");
    ui->comboBox_reciDec->addItem("ms");
    ui->comboBox_reciCons->addItem("ms");
    ui->comboBox_reciMaxVel->addItem("rpm");
    ui->comboBox_reciTimes->addItem("");
    ui->comboBox_reciInterval->addItem("ms");
    //ui->comboBox_reciTimes->addItem("");

    for (int i = 0; i < m_axisCount; i++) {
        UiPTSLMotionData* data = new UiPTSLMotionData;
        m_uiDataList.append(data);
    }

    ui->doubleSpinBox_PointAcc->setValue(2000);
    ui->doubleSpinBox_PointDec->setValue(2000);
    ui->doubleSpinBox_pointCons->setValue(3000);
    ui->doubleSpinBox_PointMaxVel->setValue(1000);
    ui->spinBox_PointTimes->setValue(2);

    ui->doubleSpinBox_ReciAcc->setValue(2000);
    ui->doubleSpinBox_ReciDec->setValue(2000);
    ui->doubleSpinBox_ReciCons->setValue(3000);
    ui->doubleSpinBox_ReciInterval->setValue(1000);
    ui->doubleSpinBox_ReciMaxVel->setValue(1000);
    ui->spinBox_ReciTimes->setValue(2);


    ui->doubleSpinBox_PointAcc->installEventFilter(this);
    ui->doubleSpinBox_PointDec->installEventFilter(this);
    ui->doubleSpinBox_pointCons->installEventFilter(this);
    ui->doubleSpinBox_PointMaxVel->installEventFilter(this);
    ui->spinBox_PointTimes->installEventFilter(this);

    ui->doubleSpinBox_ReciAcc->installEventFilter(this);
    ui->doubleSpinBox_ReciDec->installEventFilter(this);
    ui->doubleSpinBox_ReciCons->installEventFilter(this);
    ui->doubleSpinBox_ReciInterval->installEventFilter(this);
    ui->doubleSpinBox_ReciMaxVel->installEventFilter(this);
    ui->spinBox_ReciTimes->installEventFilter(this);

    OptFace *face = dynamic_cast<OptFace *>(OptContainer::instance()->optItem("optface"));
    connect(face, SIGNAL(faceCssChanged(QString)), this, SLOT(onCssChanged(QString)));
    setupIcons(face->css());
    connect(ui->checkBox_PTSLMotion, SIGNAL(clicked(bool)), this, SLOT(onCheckBoxReciClicked(bool)));
    connect(ui->doubleSpinBox_PointAcc,SIGNAL(valueChanged(double)),this,SLOT(onDoubleSpinBoxValueChanged(double)));
    connect(ui->doubleSpinBox_PointDec,SIGNAL(valueChanged(double)),this,SLOT(onDoubleSpinBoxValueChanged(double)));
    connect(ui->doubleSpinBox_PointMaxVel,SIGNAL(valueChanged(double)),this,SLOT(onDoubleSpinBoxValueChanged(double)));
    //connect(ui->doubleSpinBox_PointPulse,SIGNAL(valueChanged(double)),this,SLOT(onDoubleSpinBoxValueChanged(double)));
    connect(ui->doubleSpinBox_ReciAcc,SIGNAL(valueChanged(double)),this,SLOT(onDoubleSpinBoxValueChanged(double)));
    connect(ui->doubleSpinBox_ReciDec,SIGNAL(valueChanged(double)),this,SLOT(onDoubleSpinBoxValueChanged(double)));
    connect(ui->doubleSpinBox_ReciInterval,SIGNAL(valueChanged(double)),this,SLOT(onDoubleSpinBoxValueChanged(double)));
    connect(ui->doubleSpinBox_ReciMaxVel,SIGNAL(valueChanged(double)),this,SLOT(onDoubleSpinBoxValueChanged(double)));
    //connect(ui->doubleSpinBox_ReciPulse,SIGNAL(valueChanged(double)),this,SLOT(onDoubleSpinBoxValueChanged(double)));
    connect(ui->spinBox_ReciTimes,SIGNAL(valueChanged(int)),this,SLOT(onSpinBoxValueChanged(int)));
    //connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeOut()));
}

UiMotionPTSL::~UiMotionPTSL()
{
    delete ui;
}

void UiMotionPTSL::updataUi(int axisInx)
{
    UiPTSLMotionData* data = m_uiDataList.at(axisInx);
    if (data->m_isReci) {
        ui->checkBox_PTSLMotion->setChecked(true);
        ui->stackedWidget_Point->setCurrentIndex(1);
        ui->doubleSpinBox_ReciAcc->setValue(data->m_reciAcc);
        ui->doubleSpinBox_ReciDec->setValue(data->m_reciDec);
        ui->doubleSpinBox_ReciInterval->setValue(data->m_reciInterval);
        ui->doubleSpinBox_ReciMaxVel->setValue(data->m_reciMaxVel);
        //ui->doubleSpinBox_ReciPulse->setValue(data->m_reciPulse / 65536.0);
        ui->spinBox_ReciTimes->setValue(data->m_reciTimes);

        ui->doubleSpinBox_ReciAcc->setStyleSheet("color:black");
        ui->doubleSpinBox_ReciDec->setStyleSheet("color:black");
        ui->doubleSpinBox_ReciInterval->setStyleSheet("color:black");
        ui->doubleSpinBox_ReciMaxVel->setStyleSheet("color:black");
        //ui->doubleSpinBox_ReciPulse->setStyleSheet("color:black");
        ui->spinBox_ReciTimes->setStyleSheet("color:black");
    } else {
        ui->checkBox_PTSLMotion->setChecked(false);
        ui->stackedWidget_Point->setCurrentIndex(0);
        ui->doubleSpinBox_PointAcc->setValue(data->m_pointAcc);
        ui->doubleSpinBox_PointDec->setValue(data->m_pointDec);
        ui->doubleSpinBox_PointMaxVel->setValue(data->m_pointMaxVel);
        //ui->doubleSpinBox_PointPulse->setValue(data->m_pointPulse / 65536.0);

        ui->doubleSpinBox_PointAcc->setStyleSheet("color:black");
        ui->doubleSpinBox_PointDec->setStyleSheet("color:black");
        ui->doubleSpinBox_PointMaxVel->setStyleSheet("color:black");
        //ui->doubleSpinBox_PointPulse->setStyleSheet("color:black");
    }
    if (data->m_motionSta == UiPTSLMotionData::RUN_STA_RUNNING) {
        setEnabled(false);
    } else {
        setEnabled(true);
    }
}

QList<UiPTSLMotionData *> UiMotionPTSL::uiDataList()
{
    return m_uiDataList;
}

void UiMotionPTSL::onSpinBoxValueChanged(int value)
{
    Q_UNUSED(value);
    QSpinBox *box = qobject_cast<QSpinBox *>(sender());
    box->setStyleSheet("color:red");
}

void UiMotionPTSL::onDoubleSpinBoxValueChanged(double value)
{
    Q_UNUSED(value);
    QDoubleSpinBox *box = qobject_cast<QDoubleSpinBox *>(sender());
    box->setStyleSheet("color:red");
}

//void UiMotionPTSL::onTimerTimeOut()
//{

//}

bool UiMotionPTSL::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug()<<"key"<<keyEvent->key();
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            //QSpinBox *sbox = dynamic_cast<QSpinBox *>(obj);
            //qDebug()<<"value "<<sbox->value();
            if (ui->stackedWidget_Point->currentIndex() == 0) {
                ui->doubleSpinBox_PointAcc->setStyleSheet("color:black");
                ui->doubleSpinBox_PointDec->setStyleSheet("color:black");
                ui->doubleSpinBox_PointMaxVel->setStyleSheet("color:black");
                //ui->doubleSpinBox_PointPulse->setStyleSheet("color:black");

                for (int i = 0; i < q_ptr->axisListWidget()->count(); i++) {
                    if (q_ptr->axisListWidget()->item(i)->isSelected()) {
                        m_uiDataList.at(i)->m_pointAcc = ui->doubleSpinBox_PointAcc->value();
                        m_uiDataList.at(i)->m_pointDec = ui->doubleSpinBox_PointDec->value();
                        m_uiDataList.at(i)->m_pointMaxVel = ui->doubleSpinBox_PointMaxVel->value();
                        //m_uiDataList.at(i)->m_pointPulse = ui->doubleSpinBox_PointPulse->value();
                    }
                }
            } else if (ui->stackedWidget_Point->currentIndex() == 1) {
                ui->doubleSpinBox_ReciAcc->setStyleSheet("color:black");
                ui->doubleSpinBox_ReciDec->setStyleSheet("color:black");
                ui->doubleSpinBox_ReciInterval->setStyleSheet("color:black");
                ui->doubleSpinBox_ReciMaxVel->setStyleSheet("color:black");
                //ui->doubleSpinBox_ReciPulse->setStyleSheet("color:black");
                ui->spinBox_ReciTimes->setStyleSheet("color:black");

                for (int i = 0; i < q_ptr->axisListWidget()->count(); i++) {
                    if (q_ptr->axisListWidget()->item(i)->isSelected()) {
                        m_uiDataList.at(i)->m_reciAcc = ui->doubleSpinBox_ReciAcc->value();
                        m_uiDataList.at(i)->m_reciDec = ui->doubleSpinBox_ReciDec->value();
                        m_uiDataList.at(i)->m_reciMaxVel = ui->doubleSpinBox_ReciMaxVel->value();
                        m_uiDataList.at(i)->m_reciInterval = ui->doubleSpinBox_ReciInterval->value();
                       //m_uiDataList.at(i)->m_reciPulse = ui->doubleSpinBox_ReciPulse->value();
                        m_uiDataList.at(i)->m_reciTimes = ui->spinBox_ReciTimes->value();
                    }
                }
            }
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void UiMotionPTSL::onCheckBoxReciClicked(bool checked)
{
    if(checked) {
        ui->stackedWidget_Point->setCurrentIndex(1);
    } else {
        ui->stackedWidget_Point->setCurrentIndex(0);
    }
    qDebug()<<q_ptr->axisListWidget()->count();
    for (int i = 0; i < q_ptr->axisListWidget()->count(); i++) {
        if (q_ptr->axisListWidget()->item(i)->isSelected()) {
            m_uiDataList.at(i)->m_isReci = checked ;
        }
    }
}

void UiMotionPTSL::onCssChanged(const QString &css)
{
    setupIcons(css);
}

void UiMotionPTSL::setupIcons(const QString &css)
{
    QString iconPath = GTUtils::customPath() + "option/style/" + css + "/icon/";

    ui->label_PointMotion->setScaledContents(true);
    ui->label_ReciMotion->setScaledContents(true);
    ui->label_PointMotion->setPixmap(QPixmap(iconPath + PIC_NAME_VPOINT));
    ui->label_ReciMotion->setPixmap(QPixmap(iconPath + PIC_NAME_VRECI));
}