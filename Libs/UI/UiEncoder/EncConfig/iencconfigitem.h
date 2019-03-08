#ifndef IENCCONFIGITEM_H
#define IENCCONFIGITEM_H

#include <QObject>

class QWidget;
class IEncConfigItem : public QObject
{
  Q_OBJECT
public:
  enum EncType{
    ENC_TYPE_UNKNOW = 0,
    ENC_TYPE_ABSOLUTE = 1,
    ENC_TYPE_INCREASE = 2,
    ENC_TYPE_NONE = 3,
    ENC_TYPE_SIN = 0,
    ENC_TYPE_ROTA = 2,
    ENC_TYPE_ABZ = 3
  };
  explicit IEncConfigItem(QObject *parent = 0);
  virtual ~IEncConfigItem();

  virtual QStringList errorStrings(quint16 errorCode);
  virtual bool hasWarning(quint16 errorCode);
  virtual bool hasLostError(quint16 errorCode);
  virtual void createAttributeUi();
  virtual void updateAttributeUi(){}
  virtual bool execute();

  virtual void setAbsPos(quint32 value);

  EncType encType() const;
  void setEncType(const EncType encType);

  quint16 encConfigData() const;
  void setEncConfigData(const quint16 encConfigData);

  quint32 lineNumber() const;
  void setLineNumber(const quint32 lineNumber);

  QWidget *attributeUi() const;

  void setWarningsString(const QStringList &warnings);//定义每一位报警位信息

  quint16 lostOper() const;
  void setLostOper(const quint16 lostOper);

  quint16 alarmOper() const;
  void setAlarmOper(const quint16 alarmOper);

  quint16 crcErrOper() const;
  void setCrcErrOper(const quint16 crcErrOper);

  quint32 absLineNum() const;
  void setAbsLineNum(const quint32 absLineNum);

  quint16 adGain() const;
  void setAdGain(const quint16 adGain);

  quint16 sinCfg() const;
  void setSinCfg(const quint16 sinCfg);

  quint16 comboType() const;
  void setComboType(const quint16 type);

  quint32 pulseZCount();
  void setPulseZCount(quint32 value);

  quint16 shiftNum();
  void setShiftNum(quint16 value);

  quint16 pulseABModel();
  void setPulseABModel(quint16 value);

  quint16 aufCfg();
  void setAufCfg(quint16 value);

  bool isNeedTimer();


  bool isLineNumEditable() const;


signals:
  void lineNumChanged(int num);

public slots:
protected:
  EncType m_encType;//绝对值  还是增量式
  quint16 m_encConfigData;//(12-15 对应波特率 0-3对应是哪一家
  quint32 m_lineNumber;
  QStringList m_warnings;//报警信息

  quint16 m_lostOper;
  quint16 m_alarmOper;
  quint16 m_crcErrOper;
  QWidget *m_attributeUi;

  quint16 m_comboType;
  quint32 m_absLineNum;
  quint16 m_adGain;
  quint16 m_sinConf;
  bool m_isLineNumEditable;

  quint32 m_pulseZCount;
  quint16 m_shiftNum;
  quint16 m_pulseABModel;
  quint16 m_auxCfg;

  bool m_needTimer;
};

#endif // IENCCONFIGITEM_H
