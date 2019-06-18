#ifndef GRAPHENCODER136_H
#define GRAPHENCODER136_H

#include <QWidget>
#include "igraphencoder.h"
#include "uiencoder_global.h"

namespace Ui {
class GraphEncoder136;
}
class GraphEncoder136Private;
class QwtDial;

class UIENCODERSHARED_EXPORT GraphEncoder136 : public IGraphEncoder
{
  Q_OBJECT

  Q_DECLARE_PRIVATE(GraphEncoder136)
public:
  explicit GraphEncoder136(QWidget *parent = 0);
  ~GraphEncoder136();

  void syncTreeDataToUiFace()Q_DECL_OVERRIDE;

protected:
  void setCustomVisitActive(IUiWidget *uiWidget)Q_DECL_OVERRIDE;
  void setUiVersionName()Q_DECL_OVERRIDE;
  void setupDataMappings() Q_DECL_OVERRIDE;



  quint32 getLineNumber()Q_DECL_OVERRIDE;
  void createSupportEncoderItem() Q_DECL_OVERRIDE;

protected slots:
  void onUpdateTimeOut() Q_DECL_OVERRIDE;
public slots:
  void onBtnEncConfigSaveClicked();//保存编码器配置

private slots:
  void onBtnEncConfigClicked(bool checked);//打开编码器配置界面 
  void onRadioBtnClicked();
  void onBtnSearchPhaseClicked();
  void onBtnSavePhaseClicked();
  void onBtnSaveHomePosClicked();

  void onEncConfigListWidgetRowChanged(int curRow);

  void onEncActive();

  void onBtnClearEcnAlarmClicked();

  void onCheckBoxGearAssociationClicked(bool checked);
  void onEncTypeChanged(int index);
  void receiveLineNum(int num);
  void onLineNumChanged(int num);
  void onSearchTypeChanged(int index);

private:
  void initDial(QwtDial *dial);
  void setEncConfigUiEnable(bool en);
  void setEncErrorUiEnable(bool en);

  void updateEncConfigUiByCurrentConfigItem();

  void showEncoderError(quint16 lost,quint16 encinfo);
  void initCurEncConfigItem();

  void setGearAssociateUiEnable(bool en);

  quint32 gdc(qint32 a,qint32 b);
  void readGearPrm();

private:
  Ui::GraphEncoder136 *ui;

};

#endif // GRAPHENCODER136_H
