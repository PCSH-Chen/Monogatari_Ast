#include "TestModule.h"
#include <QDebug>

TestModule::TestModule(QWidget *parent)
    : QWidget(parent), m_contentAccess(nullptr), m_contentLength(0)
{
    setupUI();
}

TestModule::~TestModule()
{
    qDebug() << "TestModule destructor called";
}

void TestModule::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    m_statusLabel = new QLabel("測試模組 - 已載入", this);
    m_statusLabel->setStyleSheet("font-weight: bold; color: green;");
    
    m_contentInfoLabel = new QLabel("內容長度: 0 字", this);
    
    m_testButton = new QPushButton("測試按鈕", this);
    connect(m_testButton, &QPushButton::clicked, this, &TestModule::onTestButtonClicked);
    
    layout->addWidget(m_statusLabel);
    layout->addWidget(m_contentInfoLabel);
    layout->addWidget(m_testButton);
    layout->addStretch();
    
    setLayout(layout);
}

// ModuleTemplate interface implementation
QString TestModule::name() const
{
    return "測試模組";
}

QUuid TestModule::moduleUuid() const
{
    return QUuid("{12345678-1234-5678-9abc-123456789def}");
}

int TestModule::priority() const
{
    return 10;  // Lower priority than BaseInfo
}

QIcon TestModule::icon() const
{
    return QIcon();  // Empty icon for now
}

QWidget* TestModule::widget()
{
    return this;
}

void TestModule::OpenFile(const QString& content, const QString& type)
{
    qDebug() << "TestModule::OpenFile called with type:" << type 
             << "content length:" << content.length();
}

QString TestModule::SaveFile(const QString& content, const QString& type)
{
    qDebug() << "TestModule::SaveFile called with type:" << type 
             << "content length:" << content.length();
    return content; // Return content as-is
}

void TestModule::setContentAccess(QObject* contentWidget)
{
    qDebug() << "TestModule::setContentAccess called";
    
    m_contentAccess = qobject_cast<QPlainTextEdit*>(contentWidget);
    if (m_contentAccess) {
        connect(m_contentAccess, &QPlainTextEdit::textChanged,
                this, &TestModule::onContentChanged);
        // Initial update
        onContentChanged();
        qDebug() << "TestModule: Successfully connected to Content widget";
    } else {
        qWarning() << "TestModule: Failed to cast contentWidget to QPlainTextEdit";
    }
}

bool TestModule::initialize()
{
    qDebug() << "TestModule::initialize called";
    m_statusLabel->setText("測試模組 - 已初始化");
    return true;
}

bool TestModule::activate()
{
    qDebug() << "TestModule::activate called";
    m_statusLabel->setText("測試模組 - 已啟用");
    m_statusLabel->setStyleSheet("font-weight: bold; color: blue;");
    return true;
}

bool TestModule::deactivate()
{
    qDebug() << "TestModule::deactivate called";
    m_statusLabel->setText("測試模組 - 已載入");
    m_statusLabel->setStyleSheet("font-weight: bold; color: green;");
    return true;
}

void TestModule::cleanup()
{
    qDebug() << "TestModule::cleanup called";
    m_statusLabel->setText("測試模組 - 清理中");
}

void TestModule::onContentChanged()
{
    if (m_contentAccess) {
        m_contentLength = m_contentAccess->toPlainText().length();
        m_contentInfoLabel->setText(QString("內容長度: %1 字").arg(m_contentLength));
        qDebug() << "TestModule: Content changed, new length:" << m_contentLength;
    }
}

void TestModule::onTestButtonClicked()
{
    qDebug() << "TestModule: Test button clicked";
    if (m_contentAccess) {
        QString currentText = m_contentAccess->toPlainText();
        m_contentAccess->appendPlainText("\n[測試模組] 按鈕被點擊了！");
    }
}