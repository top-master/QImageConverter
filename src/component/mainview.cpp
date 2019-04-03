#include "mainview.h"
#include "ui_mainview.h"

#include <main-tool.h>
#include <feature/convert-thread.h>
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

    connect(qApp->systemTray.restoreAction, &QAction::triggered
            , this, &MainView::showNormal);
    qApp->systemTray.icon->setVisible(true);

    QList<QByteArray> &formats = QImageReader::supportedImageFormats();

    foreach (const QByteArray &format, formats) {
        ui->formatCombo->addItem(format);
    }
    ui->formatCombo->setCurrentText(QLL("jpg"));

    QAction *a = new QAction(tr("&Stop Operation"), this);
    connect(a, &QAction::triggered, this, &MainView::onAbort);
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

bool MainView::isDirValid(const QString &path)
{
    QDir dir(path);
    return dir.exists();
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
        const QString &outputPath = ui->outputEdit->text();
        if (outputPath.isEmpty() || outputPath == ui->inputEdit->text()) {
            ui->outputEdit->setText(path);
        }
        ui->inputEdit->setText(path);
    }
}

void MainView::on_outputSelect_clicked()
{
    const QString &path = QFileDialog::getExistingDirectory(this);
    if ( ! path.isEmpty()) {
        ui->outputEdit->setText(path);
    }
}

void MainView::on_convertButton_clicked()
{
    if ( ! this->askAbort())
        return;
    // Validate arguments
    const QString &pathErrorMessage = tr("Folder does not exist: %1");
    const QString &inputPath = ui->inputEdit->text();
    if ( ! isDirValid(inputPath)) {
        qApp->toast(pathErrorMessage.arg(inputPath));
        ui->inputEdit->setFocus();
        return;
    }
    const QString &outputPath = ui->outputEdit->text();
    if ( ! isDirValid(outputPath)) {
        qApp->toast(pathErrorMessage.arg(outputPath));
        ui->outputEdit->setFocus();
        return;
    }
    const QString &outputFormat = ui->formatCombo->currentText();
    // Start operation
    m_thread = new ConvertThread();
    connect(m_thread, &ConvertThread::finished, this, &MainView::onThreadFinish);
    connect(m_thread, &ConvertThread::log, this, &MainView::log, Qt::QueuedConnection);
    m_thread->start(inputPath, outputPath, outputFormat);
}
