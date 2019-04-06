#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <feature/convert-thread.h>

#include <QWidget>
#include <QFileInfo>


namespace Ui {
class MainView;
}

class MainView : public QWidget
{
    Q_OBJECT
    typedef QWidget super;
public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

    static int convertFile(const QFileInfo &in, const QFileInfo &out
            , const QString &format = QString());

    QString getDefaultOutputPath() const;

public slots:
    void log(const QString &msg, int id = -1);

protected:
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

private:
    void startOperation(ConvertThread::OperationMode mode);
    // returns true when there was no running operation
    //  or when the user decided to abort it
    bool askAbort();
    bool validateDir(const QString &, const QString &name);

private slots:
    void onThreadFinish();
    void onAbort() { this->askAbort(); }
    void onListFiles() { startOperation(ConvertThread::OpLogOnly); }

    void on_browseButton_clicked();
    void on_outputSelect_clicked();
    void on_convertButton_clicked() { startOperation(ConvertThread::OpConvert); }

private:
    Ui::MainView *ui;
    ConvertThread *m_thread;
};

#endif // MAINVIEW_H
