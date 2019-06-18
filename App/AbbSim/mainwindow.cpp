#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QDebug>
//#include <QTranslator>

//#include "icom.h"
//#include "qttreemanager.h"
//#include "sdtglobaldef.h"
//#include "gtutils.h"
//#include "IUiWidget/iuiwidget.h"

//#include "globalconfig.h"
//#include "gconfigreadwriter.h"
//#include "idevreadwriter.h"
//#include "devtextrwriter.h"
//#include "deviceconfig.h"
//#include "sevuicontroler.h"
//#include "sdassembly.h"
//#include "devcomrwriter.h"
//#include "iuicontroler.h"
//#include "globaluicontroler.h"
//#include "sevdevice.h"

//#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //staticUiInit();

}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::staticUiInit()
//{

//}

//void MainWindow::closeEvent(QCloseEvent *e)
//{

//}

//void MainWindow::onActnConnectClicked(bool checked)
//{

//}

//void MainWindow::onActnDisConnectClicked(bool checked)
//{

//}

//void MainWindow::onActnSaveClicked()
//{

//}

//void MainWindow::onActnConfigClicked()
//{

//}

//void MainWindow::onActnDownloadClicked()
//{

//}

//void MainWindow::onActnUploadClicked()
//{

//}
