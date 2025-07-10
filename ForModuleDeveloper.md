# 物語アシスタント｜模組開發文件

本文件說明如何撰寫一個可整合於物語アシスタント框架的模組（Module）。模組需遵循以下介面規範，確保其可以與主應用程式動態載入與互動。

---

## 目標

本開發介面旨在提供模組化擴充能力，使開發者可獨立構建功能性元件，並於執行期間整合至物語アシスタント的使用者介面中。透過新的架構更新，模組可以更好地與主應用程式的內容編輯器和章節管理系統整合。

---

## 模組介面說明

所有模組皆需實作以下類別：

```cpp
// ModuleTemplate.h
#pragma once
#include <QString>
#include <QIcon>
#include <QWidget>
#include <QUuid>
#include <QPair>
#include <QPlainTextEdit>

class MainWindow; // 前向宣告

class ModuleTemplate {
public:
    virtual ~ModuleTemplate() = default;
    virtual QString name() const = 0;           // 顯示名稱
    virtual QUuid moduleUuid() const = 0;       // 唯一識別 UUID
    virtual int priority() const = 0;           // 優先級（越大越高）
    virtual QIcon icon() const = 0;             // 64x64 圖示
    virtual QWidget* widget() = 0;              // 提供內容顯示的 QWidget

    // 檔案操作函式（更新的簽名）
    virtual void OpenFile(const QString& content) = 0;  // 移除 type 參數
    virtual QString SaveFile() = 0;                     // 移除所有參數

    // 存取權限設定（新增）
    virtual void setContentAccess(QPlainTextEdit* content) = 0;     // Content 存取
    virtual void setChapterAccess(MainWindow* mainWindow) = 0;      // Chapter 存取
};

#define ModuleInterface_iid "org.example.Monogatari.ModuleTemplate"
Q_DECLARE_INTERFACE(ModuleTemplate, ModuleInterface_iid)
```

---

## 介面更新說明

### 1. 章節資料結構更新
- **ChapterIdx**: 改為 `QPair<int, int>` 格式，提供更精確的章節索引
- **ChapterLabel**: 改為 `QString` 類型，支援多語言章節標籤

### 2. 檔案操作函式更新
- **OpenFile**: 簽名更新為 `void OpenFile(const QString& content)`，移除 `type` 參數
- **SaveFile**: 簽名更新為 `QString SaveFile()`，移除所有參數

### 3. 新增存取權限系統
提供模組對主應用程式核心元件的存取能力：
- **setContentAccess**: 提供對內容編輯器的存取權限
- **setChapterAccess**: 提供對章節管理系統的存取權限

---

## 成員說明

### 基本介面
* `QString name() const`：
  回傳模組在 UI 中顯示的名稱。

* `QUuid moduleUuid() const`：
  回傳模組的唯一識別UUID，用於防止模組衝突。

* `int priority() const`：
  回傳模組的優先級數值，數值越大優先級越高。用於控制模組在側邊欄中的排序順序。

* `QIcon icon() const`：
  回傳 64x64 的圖示，供模組圖示顯示使用。

* `QWidget* widget()`：
  提供模組的主要畫面元件（會嵌入於主視窗中）。

### 檔案操作介面
* `void OpenFile(const QString& content)`：
  當使用者開啟檔案時由主程式呼叫。模組應解析並處理提供的內容。

* `QString SaveFile()`：
  當使用者儲存檔案時由主程式呼叫。模組應回傳序列化後的內容資料。

### 存取權限介面
* `void setContentAccess(QPlainTextEdit* content)`：
  設定模組對主應用程式內容編輯器的存取權限。透過此介面可以：
  - 讀取目前編輯的內容
  - 修改編輯器中的文字
  - 監聽內容變更事件

* `void setChapterAccess(MainWindow* mainWindow)`：
  設定模組對主應用程式章節管理系統的存取權限。透過此介面可以：
  - 存取章節列表
  - 切換章節
  - 管理章節標籤

---

## 優先級系統

模組優先級系統決定模組在側邊欄中的顯示順序：

### 優先級規則
1. **優先級數值越大，排序越前面**
2. **相同優先級時，按模組名稱字母順序排序**
3. **建議優先級範圍：0-100**

### 優先級建議
- **系統核心模組**: 90-100
- **常用功能模組**: 70-89
- **專業工具模組**: 50-69
- **實驗性模組**: 10-49
- **測試模組**: 0-9

```cpp
int priority() const override {
    return 80; // 常用功能模組
}
```

---

## 完整實作範例

以下是一個完整的模組實作範例，示範如何使用所有介面功能：

```cpp
// ExampleModule.h
#pragma once
#include <QWidget>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "ModuleTemplate.h"
#include "mainwindow.h"

class ExampleModule : public QWidget, public ModuleTemplate
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    explicit ExampleModule(QWidget *parent = nullptr);
    ~ExampleModule() override;

    // ModuleTemplate 介面實作
    QString name() const override { return "範例模組"; }
    QUuid moduleUuid() const override { return QUuid("12345678-1234-1234-1234-123456789abc"); }
    int priority() const override { return 75; }
    QIcon icon() const override { return QIcon(":/icons/example.png"); }
    QWidget* widget() override { return this; }

    // 檔案操作
    void OpenFile(const QString& content) override;
    QString SaveFile() override;

    // 存取權限
    void setContentAccess(QPlainTextEdit* content) override;
    void setChapterAccess(MainWindow* mainWindow) override;

private slots:
    void onContentChanged();
    void onProcessContent();
    void onSwitchChapter();

private:
    void setupUI();
    void updateWordCount();

    // UI 元件
    QVBoxLayout* m_layout;
    QLabel* m_statusLabel;
    QPushButton* m_processButton;
    QPushButton* m_chapterButton;

    // 存取權限
    QPlainTextEdit* m_contentEditor;
    MainWindow* m_mainWindow;

    // 模組資料
    QString m_moduleData;
    int m_wordCount;
};

// ExampleModule.cpp
#include "ExampleModule.h"
#include <QDebug>

ExampleModule::ExampleModule(QWidget *parent)
    : QWidget(parent)
    , m_layout(nullptr)
    , m_statusLabel(nullptr)
    , m_processButton(nullptr)
    , m_chapterButton(nullptr)
    , m_contentEditor(nullptr)
    , m_mainWindow(nullptr)
    , m_wordCount(0)
{
    setupUI();
}

ExampleModule::~ExampleModule()
{
    // 清理資源
}

void ExampleModule::setupUI()
{
    m_layout = new QVBoxLayout(this);
    
    m_statusLabel = new QLabel("模組狀態：未連接", this);
    m_layout->addWidget(m_statusLabel);
    
    m_processButton = new QPushButton("處理內容", this);
    m_layout->addWidget(m_processButton);
    
    m_chapterButton = new QPushButton("切換章節", this);
    m_layout->addWidget(m_chapterButton);
    
    // 連接訊號槽
    connect(m_processButton, &QPushButton::clicked, this, &ExampleModule::onProcessContent);
    connect(m_chapterButton, &QPushButton::clicked, this, &ExampleModule::onSwitchChapter);
    
    setLayout(m_layout);
}

void ExampleModule::OpenFile(const QString& content)
{
    qDebug() << "模組載入檔案內容，長度：" << content.length();
    m_moduleData = content;
    updateWordCount();
    m_statusLabel->setText(QString("檔案已載入，字數：%1").arg(m_wordCount));
}

QString ExampleModule::SaveFile()
{
    qDebug() << "模組儲存檔案內容";
    return m_moduleData;
}

void ExampleModule::setContentAccess(QPlainTextEdit* content)
{
    qDebug() << "模組獲得內容編輯器存取權限";
    m_contentEditor = content;
    
    if (m_contentEditor) {
        // 連接內容變更訊號
        connect(m_contentEditor, &QPlainTextEdit::textChanged, 
                this, &ExampleModule::onContentChanged);
        m_statusLabel->setText("內容編輯器已連接");
    }
}

void ExampleModule::setChapterAccess(MainWindow* mainWindow)
{
    qDebug() << "模組獲得章節管理存取權限";
    m_mainWindow = mainWindow;
    
    if (m_mainWindow) {
        m_statusLabel->setText("章節管理已連接");
        m_chapterButton->setEnabled(true);
    }
}

void ExampleModule::onContentChanged()
{
    if (m_contentEditor) {
        QString currentContent = m_contentEditor->toPlainText();
        m_wordCount = currentContent.length();
        m_statusLabel->setText(QString("內容已更新，字數：%1").arg(m_wordCount));
    }
}

void ExampleModule::onProcessContent()
{
    if (!m_contentEditor) {
        qWarning() << "內容編輯器未連接";
        return;
    }
    
    QString content = m_contentEditor->toPlainText();
    // 執行內容處理邏輯
    QString processedContent = content.toUpper(); // 範例：轉換為大寫
    m_contentEditor->setPlainText(processedContent);
    
    qDebug() << "內容處理完成";
}

void ExampleModule::onSwitchChapter()
{
    if (!m_mainWindow) {
        qWarning() << "章節管理未連接";
        return;
    }
    
    // 透過主視窗存取章節功能
    // 注意：這需要根據實際的 MainWindow 介面來實作
    qDebug() << "切換章節功能";
}

void ExampleModule::updateWordCount()
{
    m_wordCount = m_moduleData.length();
}
```

---

## 訊號槽連接範例

模組可以透過 Qt 的訊號槽機制與主應用程式和其他元件通訊：

```cpp
// 監聽內容變更
connect(m_contentEditor, &QPlainTextEdit::textChanged, 
        this, &YourModule::onContentChanged);

// 監聽游標位置變更
connect(m_contentEditor, &QPlainTextEdit::cursorPositionChanged, 
        this, &YourModule::onCursorPositionChanged);

// 自訂訊號發射
void YourModule::onAnalysisComplete()
{
    emit analysisFinished(m_analysisResult);
}
```

---

## 實作提示

### 開發建議
* 請以 `QObject` 派生類別包裝您的模組，以支援 Qt Meta-Object System
* 若使用 Qt Designer，建議將 `QWidget* widget()` 回傳為自訂的 UI 表單類別
* 請確保您的模組使用唯一 UUID，避免與其他模組衝突
* 妥善處理存取權限為 nullptr 的情況

### 錯誤處理
* 在存取 `m_contentEditor` 和 `m_mainWindow` 前，請先檢查指標是否為 nullptr
* 使用 `qDebug()` 記錄重要操作，便於除錯
* 適當處理異常情況，避免程式崩潰

### 效能考量
* 避免在內容變更訊號中執行耗時操作
* 使用計時器延遲處理高頻率更新
* 適當快取計算結果，減少重複計算

---

## 模組註冊

模組須透過 Qt 插件系統進行註冊：

```cpp
Q_PLUGIN_METADATA(IID ModuleInterface_iid FILE "YourModule.json")
Q_INTERFACES(ModuleTemplate)
```

可選擇性地使用 JSON 檔案來描述插件元數據：

```json
{
    "name": "你的模組名稱",
    "version": "1.0.0",
    "description": "模組功能描述",
    "author": "作者名稱"
}
```

---

## API 參考

### 必須實作的方法
- `QString name() const` - 模組顯示名稱
- `QUuid moduleUuid() const` - 唯一識別符
- `int priority() const` - 優先級數值
- `QIcon icon() const` - 模組圖示
- `QWidget* widget()` - 主介面元件
- `void OpenFile(const QString& content)` - 載入檔案
- `QString SaveFile()` - 儲存檔案
- `void setContentAccess(QPlainTextEdit* content)` - 設定內容存取
- `void setChapterAccess(MainWindow* mainWindow)` - 設定章節存取

### 資料類型
- `ChapterIdx`: `QPair<int, int>` - 章節索引
- `ChapterLabel`: `QString` - 章節標籤

---
