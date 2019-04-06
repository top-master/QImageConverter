#include "mainview.h"
#include "ui_mainview.h"

#include <main-tool.h>
#include <service/apptool.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

MainView::MainView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainView)
    , m_thread(Q_NULLPTR)
{
    ui->setupUi(this);

    this->setWindowTitle(qApp->applicationName());

    connect(qApp->systemTray.restoreAction, &QAction::triggered
            , this, &MainView::showNormal);
    qApp->systemTray.icon->setVisible(true);

    QList<QByteArray> &formats = QImageReader::supportedImageFormats();

    foreach (const QByteArray &format, formats) {
        ui->formatCombo->addItem(format);
    }
    ui->formatCombo->setCurrentText(QLL("png"));
    // Drop-down menu
    QAction *a = new QAction(tr("&Stop Operation"), this);
    connect(a, &QAction::triggered, this, &MainView::onAbort);
    ui->convertButton->addAction(a);
    a = new QAction(tr("&List files"), this);
    connect(a, &QAction::triggered, this, &MainView::onListFiles);
    ui->convertButton->addAction(a);
    ui->convertButton->setContextMenuPolicy(Qt::ActionsContextMenu);
}

MainView::~MainView()
{
    delete ui;
}

int MainView::convertFile(const QFileInfo &in, const QFileInfo &out, const QString &format)
{
    QImage image;
    QImageReader reader;
    reader.setFileName(in.filePath());
    if ( ! reader.read( & image ) ) {
        qDebug(reader.errorString().toLocal8Bit().constData());
        return InputReadError;
    }
    // Ensures directory exists
    QDir().mkpath(out.absolutePath());
    QImageWriter writer;
    writer.setFileName(out.filePath());
    writer.setFormat(format.toLatin1());
    if ( ! writer.write(image) ) {
        qDebug(writer.errorString().toLocal8Bit().constData());
        return OutputWriteError;
    }
    return 0; // Success
}

QString MainView::getDefaultOutputPath() const
{
    QString &path = ui->inputEdit->text();
    if ( ! path.isEmpty()) {
        path += QDir::separator();
    }
    path += QLL("output");
    return QDir::cleanPath(path);
}

void MainView::log(const QString &msg, int id)
{
    Q_UNUSED(id)
    ui->logView->appendPlainText(msg);
}

void MainView::closeEvent(QCloseEvent *event)
{
    if (this->askAbort()) {
        // same as event->accept();
        super::closeEvent(event);
    } else {
        event->ignore();
    }
}

void MainView::startOperation(ConvertThread::OperationMode mode)
{
    if ( ! this->askAbort())
        return;
    // Validate arguments
    const QString &inputPath = ui->inputEdit->text();
    if ( ! validateDir(inputPath, tr("Input"))) {
        ui->inputEdit->setFocus();
        return;
    }
    QString &outputPath = ui->outputEdit->text();
    if (outputPath.isEmpty()) {
        outputPath = getDefaultOutputPath();
        ui->outputEdit->setText(outputPath);
    }
    const QString &outputFormat = ui->formatCombo->currentText();
    // Start operation
    m_thread = new ConvertThread();
    connect(m_thread, &ConvertThread::finished, this, &MainView::onThreadFinish);
    connect(m_thread, &ConvertThread::log, this, &MainView::log, Qt::QueuedConnection);
    m_thread->setMode(mode);
    m_thread->start(inputPath, outputPath, outputFormat);
}

bool MainView::askAbort()
{
    if (m_thread) {
        int r = QMessageBox::question(this
                , QString()
                , tr("Do you want to abort currently running convert operation?")
            );
        if (r != QMessageBox::Yes)
            return false;
        m_thread->stop();
        delete m_thread;
        m_thread = Q_NULLPTR;
    }
    return true;
}

bool MainView::validateDir(const QString &path, const QString &name)
{
    QString msg;
    if (path.isEmpty()) {
        msg = tr("%1 directory is not set").arg(name);
    }
    QDir dir( path );
    if ( ! dir.exists() ) {
        msg = tr("%1 directory does not exist: %2").arg(name, path);
    }
    if ( ! msg.isEmpty()) {
        qApp->toast( msg );
        qApp->beep();
        return false;
    }
    return true;
}

void MainView::onThreadFinish()
{
    delete m_thread;
    m_thread = Q_NULLPTR;
    qApp->beep();
}

void MainView::on_browseButton_clicked()
{
    const QString &path = QFileDialog::getExistingDirectory(this);
    if ( ! path.isEmpty() ) {
        QString &lastInputPath = ui->inputEdit->text();
        // Must be before input-path update
        QString &lastDefaultOutputPath = getDefaultOutputPath();
        // Update input and output paths
        ui->inputEdit->setText(path);
        const QString &outputPath = ui->outputEdit->text();
        if (outputPath.isEmpty()
                || outputPath == lastInputPath
                || outputPath == lastDefaultOutputPath)
        {
            ui->outputEdit->setText(getDefaultOutputPath());
        }
    }
}

void MainView::on_outputSelect_clicked()
{
    const QString &path = QFileDialog::getExistingDirectory(this);
    if ( ! path.isEmpty()) {
        ui->outputEdit->setText(path);
    }
}


