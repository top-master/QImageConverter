#include "resize-mode.h"
#include "ui_resize-mode.h"

ResizeModeView::ResizeModeView(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResizeOptions)
    , m_data()
{
    ui->setupUi(this);

    connect(ui->groupBox, &QGroupBox::toggled, this, &ResizeModeView::ofToggle);
    connect(ui->widthEdit, &QSpinBox::valueChanged, this, &ResizeModeView::ofWidthChange);
    connect(ui->heightEdit, &QSpinBox::valueChanged, this, &ResizeModeView::ofHeightChange);
    connect(ui->typeSelect, &QComboBox::currentIndexChanged, this, &ResizeModeView::ofTypeChange);
}

ResizeModeView::~ResizeModeView()
{
    delete ui;
}

void ResizeModeView::set(const ResizeMode &v)
{
    ui->groupBox->setChecked(v.enabled);
    ui->widthEdit->setValue(v.size.width());
    ui->heightEdit->setValue(v.size.height());
    ui->typeSelect->setCurrentIndex(v.type);
    m_data = v;
}

static const char STR_STRETCH[] = "Stretch";
static const char STR_KEEP_ASPECT[] = "KeepAspect";
static const char STR_KEEP_ASPECT_EXPANDING[] = "Fill";
static const char STR_RESIZE_CANVAS[] = "ResizeCanvas";


QString ResizeMode::toString() const
{
    QString s;
    s.reserve(1024);
    s += QString::number(this->size.width());
    s += QLatin1Char('x');
    s += QString::number(this->size.height());
    s += QLatin1Char(':');
    switch (this->type) {
    case ResizeMode::IgnoreAspectRatio:
        s += QLL(STR_STRETCH);
        break;
    case ResizeMode::KeepAspectRatio:
        s += QLL(STR_KEEP_ASPECT);
        break;
    case ResizeMode::KeepAspectRatioByExpanding:
        s += QLL(STR_KEEP_ASPECT_EXPANDING);
        break;
    case ResizeMode::ResizeCanvas:
        s += QLL(STR_RESIZE_CANVAS);
        break;
    }
    return s;
}

ResizeMode ResizeMode::fromString(const QString &s)
{
    ResizeMode r;
    if( s.isEmpty() )
        return r;
    r.enabled = true;
    int i;
    QChar c;
    for (i = 0; i < s.length(); ++i) {
        c = s.at(i);
        if ( ! c.isDigit() ) {
            break;
        }
    }
    r.size.setWidth( s.midRef(0, i).toInt() );
    // Parse Height
    if ( c == QLatin1Char('x') ) {
        int start = i + 1;
        for (i = start; i < s.length(); ++i) {
            c = s.at(i);
            if ( ! c.isDigit() ) {
                break;
            }
        }
        r.size.setHeight( s.midRef(start, i - start).toInt() );
    }
    // Parse Type
    const QStringRef &part = s.midRef( i + (c.isLetter() ? 0 : 1) );
    if (part.startsWith(QLL(STR_STRETCH), Qt::CaseInsensitive)) {
        r.type = ResizeMode::IgnoreAspectRatio;
    } else if (part.startsWith(QLL(STR_KEEP_ASPECT_EXPANDING), Qt::CaseInsensitive)) {
        r.type = ResizeMode::KeepAspectRatioByExpanding;
    } else if (part.startsWith(QLL(STR_KEEP_ASPECT), Qt::CaseInsensitive)) {
        r.type = ResizeMode::KeepAspectRatio;
    } else if (part.startsWith(QLL(STR_RESIZE_CANVAS), Qt::CaseInsensitive)) {
        r.type = ResizeMode::ResizeCanvas;
    }
    return r;
}

void ResizeMode::validate()
{
    if ( this->size.isEmpty() ) {
        this->enabled = false;
    } else if ( this->size.width() <= 0 ) {
        this->size.setWidth(this->size.height());
    } else if ( this->size.height() <= 0 ) {
        this->size.setHeight(this->size.width());
    }
}
