#ifndef CONVERTTHREAD_H
#define CONVERTTHREAD_H

#include <QThread>
#include <QImageReader>
#include <QImageWriter>

class ConvertThread : public QThread
{
    Q_OBJECT
    typedef QThread super;
public:
    ConvertThread();

    void start(const QString &inputPath
            , const QString &outputPath
            , const QString &outputFormat);

    void stop() {
        this->abort();
        this->wait();
    }

    void abort() { m_abort = true; }

signals:
    void log(const QString &msg, int id = -1);

protected:
    // QThread interface
    void run() Q_DECL_OVERRIDE;

    bool convertFile(const QString &inputFile
            , const QString &outputFile
            , int msecs = 30000);

private:
    volatile bool m_abort;

    QString m_inputPath;
    QString m_inputFilter;
    QString m_outputPath;
    QString m_outputFormat;
};

#endif // CONVERTTHREAD_H
