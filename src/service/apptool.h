#ifndef APPTOOL_H
#define APPTOOL_H


#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>

#if defined(qApp)
#  undef qApp
#endif
#define qApp (static_cast<AppTool *>(QCoreApplication::instance()))

class AppTool : public QApplication
{
    Q_OBJECT
    typedef QApplication super;
public:
    AppTool(int &argc, char **argv);
    ~AppTool() Q_DEL_OVERRIDE;

    void prepare();

public slots:
    void toast(const QString &, int miliSeconds = 3000);

protected slots:
    void onSystemTray(QSystemTrayIcon::ActivationReason);

public:
    struct SystemTray {
        inline SystemTray()
            : restoreAction(0)
            , quitAction(0)
            , menu(0), icon(0)
        {}
        inline ~SystemTray() {
            if(icon)
                icon->setVisible(false);
            delete menu;
        }

        QAction *restoreAction;
        QAction *quitAction;
        QMenu *menu;
        QSystemTrayIcon *icon;
    } systemTray;
};

#endif // APPTOOL_H
