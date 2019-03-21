#ifndef GRAPHENCODER126_H
#define GRAPHENCODER126_H

#include <QWidget>
#include "igraphencoder.h"
#include "uiencoder_global.h"

namespace Ui {
class GraphEncoder126;
}
class GraphEncoder126Private;
class QwtDial;

class UIENCODERSHARED_EXPORT GraphEncoder126 : public IGraphEncoder
{
  Q_OBJECT

  Q_DECLARE_PRIVATE(GraphEncoder126)
public:
  explicit GraphEncoder126(QWidget *parent = 0);
  ~GraphEncoder126();

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

  void onEncConfigListWidgetRowChanged(int curRow);

  void onEncActive();

  void onBtnClearEcnAlarmClicked();


private:
  void initDial(QwtDial *dial);
  void setEncConfigUiEnable(bool en);
  void setEncErrorUiEnable(bool en);

  void updateEncConfigUiByCurrentConfigItem();

  void showEncoderError(quint16 lost,quint16 encinfo);
  void initCurEncConfigItem();

private:
  Ui::GraphEncoder126 *ui;

};

#endif // GRAPHENCODER126_H
