﻿#include "curveusrvel.h"
#include <QVector>
#include <ctkPluginContext.h>
#include <QDebug>

#define VAR_KEYNAME_CUR_RSV_SPD      "gSevDrv.sev_obj.cur.rsv.spd"
#define CON_KEYNAME_MOT_NOS          "gSevDrv.sev_obj.cur.mot.Nos_1"

CurveUsrVel::CurveUsrVel()
{

}

CurveUsrVel::CurveUsrVel(ctkPluginContext *context)
{
  qDebug()<<"CurveUsrVel construct";
  ctkDictionary dic;
  dic.insert("name","CurveUsrVel");
  context->registerService<ICurve>(this,dic);
}

ICurve *CurveUsrVel::clone()
{
  ICurve *c=new CurveUsrVel();
  c->prepare();
  return c;
}

void CurveUsrVel::init()
{
  //设置单位
  //增加变量曲线，常量曲线读取 keyName
  addUnit("rpm",1);
  addUnit("rps",0.0167);
  addUnit("rad/s",0.1047);
  addVarInputByName(VAR_KEYNAME_CUR_RSV_SPD); //对应输入通道 dd.m_varInputs.at(0).datas
  addConstInputByName(CON_KEYNAME_MOT_NOS);//对应输入值 dd.m_constInputs.at(0).constK
}

void CurveUsrVel::calculate()
{
  double result = 0;
  double nos = dd.m_constInputs.at(0).constK;
//  nos = 5000;
  dd.m_cData.values.clear();
  for(int i = 0;i<dd.m_varInputs.at(0).datas.size();i++)
  {
    result = (dd.m_varInputs.at(0).datas.at(i)/16384.0)*nos;
    dd.m_cData.values.append(result);
//    qDebug()<<QString("vcmd %1 cal: in = %2,out = %3").arg(dd.m_axisInx).arg(dd.m_varInputs.at(0).datas.at(i)).arg(result);
  }

}

QString CurveUsrVel::name()
{
  return QString("Vel");
}

QString CurveUsrVel::note()
{
  return tr("Measure Velocity");
}

QString CurveUsrVel::pluginName()
{
  return "CurveUsrVel";
}
