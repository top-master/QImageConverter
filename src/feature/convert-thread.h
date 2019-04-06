#ifndef CONVERTTHREAD_H
#define CONVERTTHREAD_H

#include <QThread>
#include <QImageReader>
#include <QImageWriter>

class ConvertThread : public QThread
{
    Q_OBJECT
    typedef QThread super;
    typedef ConvertThread self;
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

    enum OperationMode {
        OpConvert,
        OpLogOnly
    };

    OperationMode mode() const { return m_mode; }
    void setMode(OperationMode v) {
        m_mode = v;
        switch (m_mode) {
        case OpConvert:
            m_handler = &self::handleConvert;
            break;
        case OpLogOnly:
            // Fall through
        default:
            m_handler = &self::handleLog;
            break;
        }
    }

signals:
    void log(const QString &msg, int id = -1);

protected:
    // QThread interface
    void run() Q_DECL_OVERRIDE;

    struct Context {
        QString input;
        QString output;
    };
    typedef bool (self::*HandlerCallback)(const Context &);

    bool handleConvert(const Context &c)
            { return convertFile(c.input, c.output); }
    bool handleLog(const Context &c) {
        emit this->log(c.output.midRef(m_outputPath.size()));
        return true;
    }

    bool convertFile(const QString &inputFile
            , const QString &outputFile
            , int msecs = 30000);

private:
    volatile bool m_abort;
    OperationMode m_mode;
    HandlerCallback m_handler;

    QString m_inputPath;
    QString m_inputFilter;
    QString m_outputPath;
    QString m_outputFormat;
};

#endif // CONVERTTHREAD_H
