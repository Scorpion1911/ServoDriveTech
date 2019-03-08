#include "curvecusaccactivator.h"
#include "icurve.h"
#include "curvecusacc.h"

#include <QDebug>

CurveCusAccActivator::CurveCusAccActivator()
{

}

void CurveCusAccActivator::start(ctkPluginContext *context)
{
    qDebug()<<"activator start   new CurveCusAcc";
    m_iCurve = new CurveCusAcc(context);
    m_iCurve->prepare();
}

void CurveCusAccActivator::stop(ctkPluginContext *context)
{
    Q_UNUSED(context);
    if (m_iCurve != NULL)
    {
        delete m_iCurve;
        m_iCurve = NULL;
    }
    qDebug()<<"activator stop delete CurveCusAcc";
}

