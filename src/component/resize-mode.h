#ifndef RESIZEOPTIONS_H
#define RESIZEOPTIONS_H

#include <QDialog>

namespace Ui {
class ResizeOptions;
}

class ResizeMode {
public:
    ResizeMode()
        : enabled(false)
        , type(KeepAspectRatio)
    {}

    enum Type {
        IgnoreAspectRatio = Qt::IgnoreAspectRatio,
        KeepAspectRatio = Qt::KeepAspectRatio,
        KeepAspectRatioByExpanding = Qt::KeepAspectRatioByExpanding,
        ResizeCanvas
    };

    QString toString() const;
    static ResizeMode fromString(const QString &);

    void validate();

public:
    bool enabled;
    QSize size;
    Type type;
};

class ResizeModeView : public QDialog
{
    Q_OBJECT
public:
    explicit ResizeModeView(QWidget *parent = 0);
    ~ResizeModeView();

    void set(const ResizeMode &v);
    const ResizeMode &get() { return m_data; }

private slots:
    void ofToggle(bool checked) { m_data.enabled = checked; }
    void ofWidthChange(int v) { m_data.size.setWidth(v); }
    void ofHeightChange(int v) { m_data.size.setHeight(v); }
    void ofTypeChange(int v) { m_data.type = ResizeMode::Type(v); }

private:
    Ui::ResizeOptions *ui;
    ResizeMode m_data;
};

#endif // RESIZEOPTIONS_H
