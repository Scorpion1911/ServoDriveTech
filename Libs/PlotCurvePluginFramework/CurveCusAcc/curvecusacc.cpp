#include "curvecusacc.h"
#include "mathtoolbox.h"
#include <QVector>
#include <ctkPluginContext.h>
#include <QDebug>

#define VAR_KEYNAME_POS_FFD_SPDR    "gSevDrv.sev_obj.pos.ffd.spdr"
#define CON_KEYNAME_MOT_NOS         "gSevDrv.sev_obj.cur.mot.Nos_1"
#define FILTER_WIDTH                15

CurveCusAcc::CurveCusAcc()
{
}

CurveCusAcc::CurveCusAcc(ctkPluginContext *context)
{
    ctkDictionary dic;
    dic.insert("name","CurveCusAcc");
    context->registerService<ICurve>(this,dic);
}

ICurve *CurveCusAcc::clone()
{
    ICurve *c = new CurveCusAcc();
    c->prepare();
    c->setUnit(dd.m_unitName);
    c->setAxisInx(dd.m_axisInx);
    c->setAxisCount(dd.m_axisCount);
    c->setDevInx(dd.m_devInx);
    c->setColor(dd.m_color);
    c->setIsDraw(dd.m_isDraw);

    for (int i = 0; i < c->constInputKeys().size(); i++)
    {
        c->fillConstInputsPrm(i,dd.m_constInputs.at(i).prm);
    }

    for(int i = 0;i<c->varInputsKeys().size();i++)
    {
        c->fillVarInputsPrm(i,dd.m_varInputs.at(i).prm);
    }
    return c;
}

void CurveCusAcc::init()
{
    addUnit("r/m^2",1);
    addVarInputByName(VAR_KEYNAME_POS_FFD_SPDR); //对应输入通道 dd.m_varInputs.at(0).datas
    addConstInputByName(CON_KEYNAME_MOT_NOS);//对应输入值 dd.m_constInputs.at(0).constK
}

void CurveCusAcc::calculate()
{
    double result = 0;
      double nos = dd.m_constInputs.at(0).constK;
    //  nos = 5000;
      dd.m_cData.values.clear();
      QVector<double> velValues;
      QVector<double> accValues;
      for(int i = 0;i<dd.m_varInputs.at(0).datas.size();i++)
      {
        result = (dd.m_varInputs.at(0).datas.at(i)/16777216.0)*nos;
        velValues.append(result);
    //    qDebug()<<QString("vcmd %1 cal: in = %2,out = %3").arg(dd.m_axisInx).arg(dd.m_varInputs.at(0).datas.at(i)).arg(result);
      }

      MathToolbox::differential(velValues, accValues, dd.m_samplInterval / 60, 0, 2);
      MathToolbox::smoothingFilter(accValues, dd.m_cData.values, FILTER_WIDTH);
      //MathToolbox::differential(velValues, accValues, dd.m_samplInterval / 60, 1);
      //MathToolbox::smoothingFilter(accValues, dd.m_cData.values, FILTER_WIDTH);
}

QString CurveCusAcc::name()
{
    return QString("Acc");
}

QString CurveCusAcc::note()
{
    return tr("Acceleration");
}

QString CurveCusAcc::pluginName()
{
    return "CurveCusAcc";
}
