# 物語アシスタント｜模組實作範例

本文件提供各種模組實作的完整範例，幫助開發者快速理解和實作自己的模組。

---

## 基礎模組範例

### 簡單的文字統計模組

```cpp
// TextStatsModule.h
#ifndef TEXTSTATSMODULE_H
#define TEXTSTATSMODULE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include "ModuleTemplate.h"

class TextStatsModule : public QWidget, public ModuleTemplate
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    explicit TextStatsModule(QWidget *parent = nullptr);

    // ModuleTemplate 介面實作
    QString name() const override { return "文字統計"; }
    QUuid moduleUuid() const override { return QUuid("a1b2c3d4-e5f6-7890-abcd-ef1234567890"); }
    int priority() const override { return 80; }
    QIcon icon() const override { return QIcon(":/icons/stats.png"); }
    QWidget* widget() override { return this; }
    
    void OpenFile(const QString& content) override;
    QString SaveFile() override;
    void setContentAccess(QObject* contentWidget) override;
    void setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel) override;

private slots:
    void onContentChanged();

private:
    void updateStats(const QString& text);
    
    QLabel* m_wordCountLabel;
    QLabel* m_charCountLabel;
    QLabel* m_lineCountLabel;
    QLabel* m_chapterCountLabel;
    QObject* m_contentWidget;
    QStringList m_chapterIdx;
    QStringList m_chapterLabel;
};

// TextStatsModule.cpp
#include "TextStatsModule.h"

TextStatsModule::TextStatsModule(QWidget *parent)
    : QWidget(parent), m_contentWidget(nullptr)
{
    setLayout(new QVBoxLayout);
    
    m_wordCountLabel = new QLabel("字數: 0");
    m_charCountLabel = new QLabel("字元數: 0");
    m_lineCountLabel = new QLabel("行數: 0");
    m_chapterCountLabel = new QLabel("章節數: 0");
    
    layout()->addWidget(m_wordCountLabel);
    layout()->addWidget(m_charCountLabel);
    layout()->addWidget(m_lineCountLabel);
    layout()->addWidget(m_chapterCountLabel);
}

void TextStatsModule::OpenFile(const QString& content) {
    // 分析檔案內容中的統計資訊
    if (content.contains("<TextStats>")) {
        // 從 XML 格式載入之前儲存的統計資料
        // 這裡可以解析並恢復模組狀態
    }
    // 如果沒有統計資料，等待 setContentAccess 後自動更新
}

QString TextStatsModule::SaveFile() {
    // 輸出統計資料（可選）
    QString result = "<TextStats>\n";
    result += "    <WordCount>" + QString::number(m_wordCountLabel->text().split(": ")[1].toInt()) + "</WordCount>\n";
    result += "    <CharCount>" + QString::number(m_charCountLabel->text().split(": ")[1].toInt()) + "</CharCount>\n";
    result += "    <LineCount>" + QString::number(m_lineCountLabel->text().split(": ")[1].toInt()) + "</LineCount>\n";
    result += "</TextStats>";
    return result;
}

void TextStatsModule::setContentAccess(QObject* contentWidget) {
    m_contentWidget = contentWidget;
    QPlainTextEdit* editor = qobject_cast<QPlainTextEdit*>(contentWidget);
    if (editor) {
        connect(editor, &QPlainTextEdit::textChanged, this, &TextStatsModule::onContentChanged);
        onContentChanged(); // 初始更新
    }
}

void TextStatsModule::setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel) {
    m_chapterIdx = chapterIdx;
    m_chapterLabel = chapterLabel;
    m_chapterCountLabel->setText("章節數: " + QString::number(chapterIdx.size()));
}

void TextStatsModule::onContentChanged() {
    QPlainTextEdit* editor = qobject_cast<QPlainTextEdit*>(m_contentWidget);
    if (editor) {
        updateStats(editor->toPlainText());
    }
}

void TextStatsModule::updateStats(const QString& text) {
    int wordCount = text.split(QRegExp("\\s+"), Qt::SkipEmptyParts).size();
    int charCount = text.length();
    int lineCount = text.split('\n').size();
    
    m_wordCountLabel->setText("字數: " + QString::number(wordCount));
    m_charCountLabel->setText("字元數: " + QString::number(charCount));
    m_lineCountLabel->setText("行數: " + QString::number(lineCount));
}

#include "TextStatsModule.moc"
```

---

## 進階模組範例

### 章節管理模組

```cpp
// ChapterManagerModule.h
#ifndef CHAPTERMANAGERMODULE_H
#define CHAPTERMANAGERMODULE_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include "ModuleTemplate.h"

class ChapterManagerModule : public QWidget, public ModuleTemplate
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    explicit ChapterManagerModule(QWidget *parent = nullptr);

    // ModuleTemplate 介面實作
    QString name() const override { return "章節管理"; }
    QUuid moduleUuid() const override { return QUuid("b2c3d4e5-f6g7-8901-bcde-f23456789012"); }
    int priority() const override { return 90; }
    QIcon icon() const override { return QIcon(":/icons/chapters.png"); }
    QWidget* widget() override { return this; }
    
    void OpenFile(const QString& content) override;
    QString SaveFile() override;
    void setContentAccess(QObject* contentWidget) override;
    void setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel) override;

private slots:
    void onAddChapter();
    void onDeleteChapter();
    void onChapterSelected();
    void onContentChanged();

private:
    void setupUI();
    void updateChapterList();
    void saveChapterData();
    
    QListWidget* m_chapterList;
    QPushButton* m_addButton;
    QPushButton* m_deleteButton;
    QLineEdit* m_chapterNameEdit;
    
    QObject* m_contentWidget;
    QStringList m_chapterIdx;
    QStringList m_chapterLabel;
    QMap<QString, QString> m_chapterContent; // 儲存每章節的內容
};

// ChapterManagerModule.cpp
#include "ChapterManagerModule.h"
#include <QMessageBox>

ChapterManagerModule::ChapterManagerModule(QWidget *parent)
    : QWidget(parent), m_contentWidget(nullptr)
{
    setupUI();
}

void ChapterManagerModule::setupUI() {
    setLayout(new QVBoxLayout);
    
    // 章節列表
    m_chapterList = new QListWidget;
    layout()->addWidget(m_chapterList);
    
    // 新增章節區域
    QHBoxLayout* addLayout = new QHBoxLayout;
    m_chapterNameEdit = new QLineEdit;
    m_chapterNameEdit->setPlaceholderText("章節名稱");
    m_addButton = new QPushButton("新增章節");
    addLayout->addWidget(m_chapterNameEdit);
    addLayout->addWidget(m_addButton);
    
    m_deleteButton = new QPushButton("刪除章節");
    
    layout()->addItem(addLayout);
    layout()->addWidget(m_deleteButton);
    
    // 連接訊號
    connect(m_addButton, &QPushButton::clicked, this, &ChapterManagerModule::onAddChapter);
    connect(m_deleteButton, &QPushButton::clicked, this, &ChapterManagerModule::onDeleteChapter);
    connect(m_chapterList, &QListWidget::currentRowChanged, this, &ChapterManagerModule::onChapterSelected);
}

void ChapterManagerModule::OpenFile(const QString& content) {
    // 解析章節資料
    if (content.contains("<ChapterManager>")) {
        // 從 XML 格式載入章節資料
        QRegExp rx("<Chapter><Index>(.*)</Index><Label>(.*)</Label></Chapter>");
        int pos = 0;
        m_chapterIdx.clear();
        m_chapterLabel.clear();
        
        while ((pos = rx.indexIn(content, pos)) != -1) {
            m_chapterIdx.append(rx.cap(1));
            m_chapterLabel.append(rx.cap(2));
            pos += rx.matchedLength();
        }
        updateChapterList();
    }
}

QString ChapterManagerModule::SaveFile() {
    QString result = "<ChapterManager>\n";
    for (int i = 0; i < m_chapterIdx.size() && i < m_chapterLabel.size(); ++i) {
        result += "    <Chapter>\n";
        result += "        <Index>" + m_chapterIdx[i] + "</Index>\n";
        result += "        <Label>" + m_chapterLabel[i] + "</Label>\n";
        result += "    </Chapter>\n";
    }
    result += "</ChapterManager>";
    return result;
}

void ChapterManagerModule::setContentAccess(QObject* contentWidget) {
    m_contentWidget = contentWidget;
    QPlainTextEdit* editor = qobject_cast<QPlainTextEdit*>(contentWidget);
    if (editor) {
        connect(editor, &QPlainTextEdit::textChanged, this, &ChapterManagerModule::onContentChanged);
    }
}

void ChapterManagerModule::setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel) {
    m_chapterIdx = chapterIdx;
    m_chapterLabel = chapterLabel;
    updateChapterList();
}

void ChapterManagerModule::onAddChapter() {
    QString chapterName = m_chapterNameEdit->text().trimmed();
    if (!chapterName.isEmpty()) {
        QString newIndex = QString::number(m_chapterIdx.size() + 1);
        m_chapterIdx.append(newIndex);
        m_chapterLabel.append(chapterName);
        m_chapterNameEdit->clear();
        updateChapterList();
        saveChapterData();
    }
}

void ChapterManagerModule::onDeleteChapter() {
    int currentRow = m_chapterList->currentRow();
    if (currentRow >= 0 && currentRow < m_chapterIdx.size()) {
        m_chapterIdx.removeAt(currentRow);
        m_chapterLabel.removeAt(currentRow);
        updateChapterList();
        saveChapterData();
    }
}

void ChapterManagerModule::onChapterSelected() {
    int currentRow = m_chapterList->currentRow();
    if (currentRow >= 0 && currentRow < m_chapterLabel.size()) {
        // 這裡可以實作跳轉到對應章節的功能
        // 例如：在主編輯器中定位到該章節
    }
}

void ChapterManagerModule::onContentChanged() {
    // 監聽內容變化，可以用來自動識別章節分隔符
    // 例如：自動偵測 "第一章"、"Chapter 1" 等模式
}

void ChapterManagerModule::updateChapterList() {
    m_chapterList->clear();
    for (int i = 0; i < m_chapterIdx.size() && i < m_chapterLabel.size(); ++i) {
        QString itemText = m_chapterIdx[i] + ". " + m_chapterLabel[i];
        m_chapterList->addItem(itemText);
    }
}

void ChapterManagerModule::saveChapterData() {
    // 通知主程式章節資料已更新
    // 這裡可以發出訊號或直接更新主程式的章節資訊
}

#include "ChapterManagerModule.moc"
```

---

## 共享資源使用範例

### 內容同步模組

```cpp
// ContentSyncModule.h - 展示如何在多個模組間同步內容
#ifndef CONTENTSYNCMODULE_H
#define CONTENTSYNCMODULE_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QTimer>
#include "ModuleTemplate.h"

class ContentSyncModule : public QWidget, public ModuleTemplate
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    explicit ContentSyncModule(QWidget *parent = nullptr);

    // ModuleTemplate 介面實作
    QString name() const override { return "內容同步"; }
    QUuid moduleUuid() const override { return QUuid("c3d4e5f6-g7h8-9012-cdef-345678901234"); }
    int priority() const override { return 60; }
    QIcon icon() const override { return QIcon(":/icons/sync.png"); }
    QWidget* widget() override { return this; }
    
    void OpenFile(const QString& content) override;
    QString SaveFile() override;
    void setContentAccess(QObject* contentWidget) override;
    void setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel) override;

private slots:
    void onMainContentChanged();
    void onLocalContentChanged();
    void onSyncToMain();
    void onSyncFromMain();
    void performAutoSync();

private:
    void setupUI();
    
    QTextEdit* m_localEditor;
    QPushButton* m_syncToMainButton;
    QPushButton* m_syncFromMainButton;
    QTimer* m_autoSyncTimer;
    
    QPlainTextEdit* m_mainEditor;
    bool m_isUpdating;
};

// ContentSyncModule.cpp
#include "ContentSyncModule.h"

ContentSyncModule::ContentSyncModule(QWidget *parent)
    : QWidget(parent), m_mainEditor(nullptr), m_isUpdating(false)
{
    setupUI();
    
    // 設定自動同步計時器
    m_autoSyncTimer = new QTimer(this);
    m_autoSyncTimer->setSingleShot(true);
    m_autoSyncTimer->setInterval(2000); // 2秒延遲
    connect(m_autoSyncTimer, &QTimer::timeout, this, &ContentSyncModule::performAutoSync);
}

void ContentSyncModule::setupUI() {
    setLayout(new QVBoxLayout);
    
    m_localEditor = new QTextEdit;
    m_localEditor->setPlaceholderText("本地編輯區...");
    
    m_syncToMainButton = new QPushButton("同步到主編輯器");
    m_syncFromMainButton = new QPushButton("從主編輯器同步");
    
    layout()->addWidget(m_localEditor);
    layout()->addWidget(m_syncToMainButton);
    layout()->addWidget(m_syncFromMainButton);
    
    connect(m_syncToMainButton, &QPushButton::clicked, this, &ContentSyncModule::onSyncToMain);
    connect(m_syncFromMainButton, &QPushButton::clicked, this, &ContentSyncModule::onSyncFromMain);
    connect(m_localEditor, &QTextEdit::textChanged, this, &ContentSyncModule::onLocalContentChanged);
}

void ContentSyncModule::setContentAccess(QObject* contentWidget) {
    m_mainEditor = qobject_cast<QPlainTextEdit*>(contentWidget);
    if (m_mainEditor) {
        connect(m_mainEditor, &QPlainTextEdit::textChanged, this, &ContentSyncModule::onMainContentChanged);
        // 初始同步
        onSyncFromMain();
    }
}

void ContentSyncModule::onMainContentChanged() {
    if (!m_isUpdating && m_mainEditor) {
        // 主編輯器內容變化時，啟動自動同步計時器
        m_autoSyncTimer->start();
    }
}

void ContentSyncModule::onLocalContentChanged() {
    if (!m_isUpdating) {
        // 本地內容變化時的處理
        // 可以在這裡加入自動儲存或其他邏輯
    }
}

void ContentSyncModule::onSyncToMain() {
    if (m_mainEditor) {
        m_isUpdating = true;
        m_mainEditor->setPlainText(m_localEditor->toPlainText());
        m_isUpdating = false;
    }
}

void ContentSyncModule::onSyncFromMain() {
    if (m_mainEditor) {
        m_isUpdating = true;
        m_localEditor->setPlainText(m_mainEditor->toPlainText());
        m_isUpdating = false;
    }
}

void ContentSyncModule::performAutoSync() {
    if (m_mainEditor && !m_isUpdating) {
        onSyncFromMain();
    }
}

// 其他必要方法的實作...
void ContentSyncModule::OpenFile(const QString& content) { Q_UNUSED(content); }
QString ContentSyncModule::SaveFile() { return QString(); }
void ContentSyncModule::setChapterAccess(const QStringList& chapterIdx, const QStringList& chapterLabel) { 
    Q_UNUSED(chapterIdx); Q_UNUSED(chapterLabel); 
}

#include "ContentSyncModule.moc"
```

---

## 檔案格式處理範例

### 多格式支援模組

```cpp
// MultiFormatModule.cpp - 展示如何處理多種檔案格式
void MultiFormatModule::OpenFile(const QString& content) {
    QString format = detectFormat(content);
    
    if (format == "XML") {
        loadXmlFormat(content);
    } else if (format == "JSON") {
        loadJsonFormat(content);
    } else if (format == "MARKDOWN") {
        loadMarkdownFormat(content);
    } else {
        loadPlainTextFormat(content);
    }
}

QString MultiFormatModule::detectFormat(const QString& content) {
    QString trimmed = content.trimmed();
    
    // XML 格式檢測
    if (trimmed.startsWith("<?xml") || trimmed.contains("<Type>")) {
        return "XML";
    }
    
    // JSON 格式檢測
    if (trimmed.startsWith("{") && trimmed.contains("\"Type\"")) {
        return "JSON";
    }
    
    // Markdown 格式檢測
    if (trimmed.contains("# ") || trimmed.contains("## ")) {
        return "MARKDOWN";
    }
    
    return "PLAINTEXT";
}

void MultiFormatModule::loadXmlFormat(const QString& content) {
    QDomDocument doc;
    if (doc.setContent(content)) {
        QDomElement root = doc.documentElement();
        if (root.tagName() == "Type") {
            // 處理標準 MGF 格式
            processStandardFormat(root);
        }
    }
}

void MultiFormatModule::loadJsonFormat(const QString& content) {
    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject root = doc.object();
        if (root.contains("Type")) {
            // 處理 JSON 格式
            processJsonFormat(root);
        }
    }
}

QString MultiFormatModule::SaveFile() {
    // 根據使用者偏好或模組設定決定輸出格式
    switch (m_preferredFormat) {
        case XmlFormat:
            return generateXmlOutput();
        case JsonFormat:
            return generateJsonOutput();
        case MarkdownFormat:
            return generateMarkdownOutput();
        default:
            return generatePlainTextOutput();
    }
}
```

---

## 優先級設定範例

```cpp
// 不同類型模組的優先級設定建議

// 核心功能模組 (100+)
class CoreEditorModule : public ModuleTemplate {
public:
    int priority() const override { return 150; } // 最高優先級
};

class BaseInfoModule : public ModuleTemplate {
public:
    int priority() const override { return 120; } // 高優先級
};

// 輔助功能模組 (50-99)
class TextStatsModule : public ModuleTemplate {
public:
    int priority() const override { return 80; } // 中高優先級
};

class SpellCheckModule : public ModuleTemplate {
public:
    int priority() const override { return 70; } // 中優先級
};

class FindReplaceModule : public ModuleTemplate {
public:
    int priority() const override { return 60; } // 中低優先級
};

// 擴充功能模組 (1-49)
class ExportToolModule : public ModuleTemplate {
public:
    int priority() const override { return 30; } // 低優先級
};

class ThemeCustomizerModule : public ModuleTemplate {
public:
    int priority() const override { return 20; } // 較低優先級
};

class DebugToolModule : public ModuleTemplate {
public:
    int priority() const override { return 10; } // 最低優先級
};
```

---

## 錯誤處理範例

```cpp
// 完善的錯誤處理示範
void RobustModule::setContentAccess(QObject* contentWidget) {
    if (!contentWidget) {
        qWarning() << "RobustModule: contentWidget is null";
        return;
    }
    
    QPlainTextEdit* editor = qobject_cast<QPlainTextEdit*>(contentWidget);
    if (!editor) {
        qWarning() << "RobustModule: contentWidget is not a QPlainTextEdit";
        qWarning() << "Actual type:" << contentWidget->metaObject()->className();
        return;
    }
    
    // 檢查是否已經連接過
    if (m_contentWidget == contentWidget) {
        return; // 避免重複連接
    }
    
    // 斷開舊連接
    if (m_contentWidget) {
        disconnect(m_contentWidget, nullptr, this, nullptr);
    }
    
    m_contentWidget = contentWidget;
    connect(editor, &QPlainTextEdit::textChanged, this, &RobustModule::onContentChanged);
    
    qDebug() << "RobustModule: Successfully connected to content widget";
}

void RobustModule::OpenFile(const QString& content) {
    if (content.isEmpty()) {
        qWarning() << "RobustModule: Attempting to open empty content";
        return;
    }
    
    try {
        QString format = detectFormat(content);
        qDebug() << "RobustModule: Detected format:" << format;
        
        if (format == "XML") {
            if (!loadXmlFormat(content)) {
                qWarning() << "RobustModule: Failed to load XML format, trying fallback";
                loadPlainTextFormat(content);
            }
        } else {
            loadPlainTextFormat(content);
        }
        
    } catch (const std::exception& e) {
        qCritical() << "RobustModule: Exception in OpenFile:" << e.what();
    } catch (...) {
        qCritical() << "RobustModule: Unknown exception in OpenFile";
    }
}
```

這些範例展示了如何實作各種類型的模組，包括基礎功能、進階功能、共享資源使用、多格式支援、優先級設定和錯誤處理。開發者可以根據這些範例快速建立自己的模組。