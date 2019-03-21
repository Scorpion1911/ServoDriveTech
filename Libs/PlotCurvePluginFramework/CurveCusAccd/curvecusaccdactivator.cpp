#include "curvecusaccdactivator.h"
#include "icurve.h"
#include "curvecusaccd.h"

#include <QDebug>

CurveCusAccdActivator::CurveCusAccdActivator()
{

}

void CurveCusAccdActivator::start(ctkPluginContext *context)
{
    qDebug()<<"activator start   new CurveCusAccd";
    m_iCurve = new CurveCusAccd(context);
    m_iCurve->prepare();
}

void CurveCusAccdActivator::stop(ctkPluginContext *context)
{
    Q_UNUSED(context);
    if (m_iCurve != NULL)
    {
        delete m_iCurve;
        m_iCurve = NULL;
    }
    qDebug()<<"activator stop delete CurveCusAccd";
}

