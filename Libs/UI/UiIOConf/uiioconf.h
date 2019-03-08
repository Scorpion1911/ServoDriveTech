#ifndef UIIOCONF_H
#define UIIOCONF_H

#include "uiioconf_global.h"
#include "iuiwidget.h"

namespace Ui {
class UiIOConf;
}
class QWidget;
class UiIOConfPrivate;

class UIIOCONFSHARED_EXPORT UiIOConf : public IUiWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(UiIOConf)

public:
    explicit UiIOConf(QWidget *parent = 0);
    ~UiIOConf();

    void accept(QWidget*w) Q_DECL_OVERRIDE;
    void setUiActive(bool actived)Q_DECL_OVERRIDE;
    bool writePageFLASH()Q_DECL_OVERRIDE;
    bool writePageFlashToOtherAxis(int srcAxisInx, int desAxisInx, QTreeWidget *tree)Q_DECL_OVERRIDE;
    bool hasConfigFunc()Q_DECL_OVERRIDE;
    bool hasSaveFunc()Q_DECL_OVERRIDE;
protected slots:

protected:
    void setContextAction() Q_DECL_OVERRIDE;
    QStackedWidget *getUiStackedWidget(void)Q_DECL_OVERRIDE;
    QVBoxLayout *getVBoxLayout(void)Q_DECL_OVERRIDE;
    void setDefaultUi()Q_DECL_OVERRIDE;

private:
    Ui::UiIOConf *ui;
};

#endif // UIIOCONF_H
