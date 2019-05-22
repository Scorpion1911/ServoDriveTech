#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

//class QToolButton;
//class SdAssembly;
//class QTreeWidgetItem;
//class IUiControler;


//class QTreeWidget;
//class GlobalUiControler;
//class IUiWidget;
//class PlotUnit;
//class DeviceConfig;
//class SevDevice;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    bool init();
//private:
//    void staticUiInit();
//    void createActions();
//    void setAppIcon();
//    void createConnections();
//    void clearStackedWidget();
//    void closeEvent(QCloseEvent *e) override;

//    bool deviceInit();
//    void navigationTreeInit();
//    void clearNavigationTree();
//    void globalUiPageInit();
//    void stackedWidgetInit();
//    void removeAllStackedWidget();

//private slots:
//    void onActnConnectClicked(bool checked);
//    void onActnDisConnectClicked(bool checked);
//    void onActnSaveClicked();
//    void onActnConfigClicked();
//    void onActnDownloadClicked();
//    void onActnUploadClicked();
private:
    Ui::MainWindow *ui;
// action variable define
//    QAction                 *m_actnDisNet;
//    QAction                 *m_actnConnect;
//    QAction                 *m_actnDownload;
//    QAction                 *m_actnUpload;
//    QAction                 *m_actnConfig;
//    QAction                 *m_actnSave;
//// servo device variable define
//    QList<SdAssembly*>      m_sdAssemblyList;
//    GlobalUiControler       *m_gUiControl;
//    SdAssembly              *m_currentSdAssembly;
};

#endif // MAINWINDOW_H
