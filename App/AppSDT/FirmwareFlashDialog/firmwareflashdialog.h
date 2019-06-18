#ifndef FIRMWAREFLASHDIALOG_H
#define FIRMWAREFLASHDIALOG_H

#include <QDialog>

class SevDevice;
class QCheckBox;

namespace Ui {
class FirmwareFlashDialog;
}

class FirmwareFlashDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FirmwareFlashDialog(QList<SevDevice*> &devList, QWidget *parent = 0);
    ~FirmwareFlashDialog();

signals:
    void startDownload(bool en);
    void sendProcessMsg(int value, const QString &msg);
private:
    void uiInit();
    void createConnections();
    QStringList getFilesFromExt(const QString &fileExt, const QString &filePath, int num);
    QString getFileFromName(const QString &fileName, const QString &filePath);
    bool deleteDir(const QString &path);
    bool getParaFiles();
    bool downloadHexFile();
    bool downloadRpdFile();
    bool downloadXmlFiles();
    static void processCallBack(void *argv, short *value);
    static void updateProgressValueFPGA(void *arg, qint16 *value);
protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::FirmwareFlashDialog *ui;
    QString m_filePath;
    QString m_decompressPath;
    QString m_desPath;
    QList<SevDevice*> m_devList;
    QString m_dspVersion;
    QString m_fpgVersion;
    int m_barCount;
    QList<QCheckBox*> m_boxList;
    int m_boxCount;
    QString m_devStr;
    SevDevice* m_curDevice;
private slots:
    void onActnToolbtnClicked();
    void onActnFlashBtnClicked();
    void receiveProcessMsg(int value, QString msg);
    void onActnComboBoxIndexChanged(int index);
    void onAllBoxClicked(bool checked);
    void onSingleBoxClicked(bool checked);
};

#endif // FIRMWAREFLASHDIALOG_H
