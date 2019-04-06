#include "convert-thread.h"

#include <QDirIterator>
#include <QProcess>
#include <QApplication>

ConvertThread::ConvertThread()
    : m_abort(false)
    , m_inputFilter(QLatin1Char('*'))
{
}

void ConvertThread::start(const QString &inputPath, const QString &outputPath, const QString &outputFormat)
{
    this->stop();
    m_inputPath = inputPath;
    m_outputPath = QDir(outputPath).absolutePath();
    if ( ! m_outputPath.endsWith(QLatin1Char('/')))
        m_outputPath += QLatin1Char('/');
    m_outputPath.squeeze();
    m_outputFormat = outputFormat;
    m_abort = false;
    super::start();
}

void ConvertThread::run()
{
    QString outputPath;
    outputPath.reserve(m_outputPath.size() + 1024);
    outputPath += m_outputPath;
    const int outputRootSize = outputPath.size();
    QDirIterator it(m_inputPath, QStringList() << m_inputFilter, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext() && m_abort == false) {
        const QString &currentPath = it.next();
        outputPath.resize(outputRootSize);
        outputPath += currentPath.midRef(m_inputPath.size());
        // Change output format
        const QFileInfo &outputInfo = QFileInfo(outputPath);
        const int suffixSize = outputInfo.suffix().size();
        if (suffixSize) {
            // Note that this will leave the trailing dot
            outputPath.chop(suffixSize);
        } else {
            outputPath += QLatin1Char('.');
        }
        outputPath += m_outputFormat;
        // At last convert input file to the required format
        this->convertFile(currentPath, outputPath);
    }
}

bool ConvertThread::convertFile(const QString &inputFile
        , const QString &outputFile, int msecs)
{
    // Prepare
    QProcess p;
    p.setProgram( qApp->applicationFilePath() );
    p.setArguments(
            QStringList() << QLL("--input") << inputFile
            << QLL("--output") << outputFile
            << QLL("--format") << m_outputFormat
        );
    // Launch and wait for result
    p.start();
    bool r = p.waitForFinished( msecs );
    if ( ! r ) {
        p.terminate();
        if ( ! p.waitForFinished(700) ) {
            p.kill();
        }
    }
    // Log result
    const int exitCode = p.exitCode();
    if ( ! r || exitCode != 0 ) {
        QString msg = tr("Failed to convert: ");
        msg += inputFile;
        const QByteArray &errors = p.readAllStandardError();
        if ( ! errors.isEmpty() ) {
            msg += QLatin1Char('\n');
            msg += QString::fromLocal8Bit(errors);
        }
        const QByteArray &outputs = p.readAllStandardOutput();
        if ( ! outputs.isEmpty() ) {
            msg += QLatin1Char('\n');
            msg += QString::fromLocal8Bit(outputs);
        }
        emit this->log( msg );
        return false;
    }
    return true;
}
