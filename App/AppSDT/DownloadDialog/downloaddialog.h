#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>

class SevDevice;
class QCheckBox;

namespace Ui {
class DownloadDialog;
}

class DownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadDialog(QWidget *parent = 0);
    ~DownloadDialog();
    void uiInit(const QList<SevDevice *> &devList, const QString &downloadPath, QString &filePath, QList<int> &indexList);
signals:

private:
    Ui::DownloadDialog *ui;
private slots:
    void onToolButtonClicked();
    void onOkButtonClicked();
    void onAllBoxClicked(bool checked);
    void onSingleBoxClicked(bool checked);
private:
    QString* m_filePath;
    QString m_downloadPath;
    QList<int> *m_indexList;
    QList<QCheckBox *> m_boxList;
    int m_devNum;
};

#endif // DOWNLOADDIALOG_H
