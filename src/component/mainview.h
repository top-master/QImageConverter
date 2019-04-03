#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>
#include <QFileInfo>

class ConvertThread;

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

public slots:
    void log(const QString &msg, int id = -1);

protected:
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

private:
    // returns true when there was no running operation
    //  or when the user decided to abort it
    bool askAbort();
    bool isDirValid(const QString &);

private slots:
    void onThreadFinish();
    void onAbort() { this->askAbort(); }

    void on_browseButton_clicked();
    void on_outputSelect_clicked();
    void on_convertButton_clicked();

private:
    Ui::MainView *ui;
    ConvertThread *m_thread;
};

#endif // MAINVIEW_H
