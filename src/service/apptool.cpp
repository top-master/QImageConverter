#include "apptool.h"


AppTool::AppTool(int &argc, char **argv)
    : super(argc, argv)
{
}

AppTool::~AppTool()
{
}

void AppTool::prepare()
{
    if(systemTray.icon == Q_NULLPTR) {
        systemTray.menu = new QMenu();

        //the "MainView" will connect to "systemTray.restoreAction"
        systemTray.restoreAction = new QAction(tr("&Show/Restore"), this);
        systemTray.menu->addAction(systemTray.restoreAction);

        systemTray.menu->addSeparator();

        systemTray.quitAction = new QAction(tr("&Quit"), this);
        connect(systemTray.quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
        systemTray.menu->addAction(systemTray.quitAction);

        systemTray.icon = new QSystemTrayIcon(this);
        systemTray.icon->setIcon(this->windowIcon());
        systemTray.icon->setContextMenu(systemTray.menu);
        connect(systemTray.icon, &QSystemTrayIcon::activated, this, &AppTool::onSystemTray);
    }
}

void AppTool::toast(const QString &msg, int miliSeconds)
{
    if(systemTray.icon) {
        systemTray.icon->showMessage(this->applicationName(), msg, QSystemTrayIcon::Information, miliSeconds);
    }
}

void AppTool::onSystemTray(QSystemTrayIcon::ActivationReason r)
{
    if(r != QSystemTrayIcon::Context) {
        systemTray.restoreAction->trigger();
    }
}
