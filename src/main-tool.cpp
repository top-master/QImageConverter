#include "main-tool.h"

#include <component/mainview.h>

#include <service/apptool.h>

#include <qcommandlineparser.h>
#include <QFileInfo>
#include <extras/crash-report.h>

int main(int argc, char *argv[])
{
    CrashReporter crashReporter;
    AppTool app( argc, argv ); {
        app.setOrganizationName("Top-Master");
        app.setApplicationName(QLL("Image Converter"));
        app.setApplicationVersion(QLL("v1.3"));
        app.setWindowIcon(QIcon(QLL(":/images/icon.png")));
        crashReporter.prepare();

        //process command-line-interface (CLI)
        QCommandLineParser parser;
        // Accept force-mode
        const QString &forceArg = QLL("force");
        QCommandLineOption forceOption( QStringList() << forceArg.at(0) << forceArg
                , QLL("force output overwrite")
            );
        parser.addOption(forceOption);
        // Accept input-path
        const QString &inputArg = QLL("input");
        const QString &valueName = QLL("value");
        QCommandLineOption inputOption( QStringList() << inputArg.at(0) << inputArg
                , QLL("input file-path")
                , valueName, QString()
            );
        parser.addOption(inputOption);
        // Accept output-path
        const QString &outputArg = QLL("output");
        QCommandLineOption outputOption( QStringList() << outputArg.at(0) << outputArg
                , QLL("output file-path")
                , valueName, QString()
            );
        parser.addOption(outputOption);
        // Accept output-format
        const QString &formatArg = QLL("format");
        QCommandLineOption formatOption( formatArg
                , QLL("output file-format")
                , valueName, QString()
            );
        parser.addOption(formatOption);
        parser.process(app);

        if(parser.isSet(inputOption) || parser.isSet(outputArg)) {
            QFileInfo &inputInfo = QFileInfo( parser.value(inputOption) );
            QFileInfo &outputInfo = QFileInfo( parser.value(outputOption) );
            if ( ! inputInfo.exists()) {
                qDebug("Input file does not exist.");
                return InputExpected;
            } else if ( outputInfo.exists() ) {
                if (parser.isSet(forceOption)) {
                    if ( ! QFile::remove(outputInfo.filePath()) ) {
                        qDebug("Failed to remove existing output file.");
                        return OutputRemoveFailed;
                    }
                } else {
                    qDebug("Output file already exists.");
                    return OutputExists;
                }
            }
            return MainView::convertFile(inputInfo, outputInfo, parser.value(formatOption));
        }
        // Prepare services
        app.prepare();
        // At last show the main-view
        MainView *w = new MainView();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
    }
    return app.exec();
}
