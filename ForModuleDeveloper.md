# 物語アシスタント｜模組開發文件

本文件說明如何撰寫一個可整合於物語アシスタント框架的模組（Module）。模組需遵循以下介面規範，確保其可以與主應用程式動態載入與互動。

---

## 目標

本開發介面旨在提供模組化擴充能力，使開發者可獨立構建功能性元件，並於執行期間整合至物語アシスタント的使用者介面中。新架構提供了更強大的內容共享機制和章節管理功能。

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

class QPlainTextEdit;
class MainWindow;

using ChapterIdx = QPair<int, int>;     // 章節編號 (章節編號, 子章節編號)
using ChapterLabel = QString;           // 章節標籤

class ModuleTemplate {
public:
    virtual ~ModuleTemplate() {}
    virtual QString name() const = 0;           // 顯示名稱
    virtual QUuid moduleUuid() const = 0;       // 唯一識別 UUID
    virtual int priority() const = 0;           // 優先級（越大越高）
    virtual QIcon icon() const = 0;             // 64x64 icon
    virtual QWidget* widget() = 0;              // 內容顯示用

    // 新增：內容存取方法
    virtual void setContentAccess(QPlainTextEdit* content) = 0;
    
    // 新增：章節存取方法
    virtual void setChapterAccess(MainWindow* mainWindow) = 0;

    // 檔案操作函式（已修改）
    virtual void OpenFile(const QString& content) = 0;
    virtual QString SaveFile() = 0;
};

#define ModuleInterface_iid "org.example.Monogatari.ModuleTemplate"
Q_DECLARE_INTERFACE(ModuleTemplate, ModuleInterface_iid)
```

---

## 成員說明

### 基本介面

* `QString name() const`：
  回傳模組在 UI 中顯示的名稱。

* `QUuid moduleUuid() const`：
  回傳模組的唯一識別UUID。建議使用 `QUuid::createUuid()` 生成，並在開發期間保持不變。

* `int priority() const`：
  回傳模組的優先級。數值越大，優先級越高。主程式會依據優先級排序模組在側邊欄的顯示順序。

* `QIcon icon() const`：
  回傳 64x64 的圖示，供模組圖示顯示使用。

* `QWidget* widget()`：
  提供模組的主要畫面元件（會嵌入於主視窗中）。

### 新增：內容存取方法

* `void setContentAccess(QPlainTextEdit* content)`：
  主程式會呼叫此方法，提供主視窗的內容編輯器存取權限。模組可儲存此指標以便：
  - 直接讀取和修改主視窗的文字內容
  - 監聽文字變更事件
  - 實現統一的文字編輯功能

* `void setChapterAccess(MainWindow* mainWindow)`：
  主程式會呼叫此方法，提供主視窗的存取權限。模組可透過此方法：
  - 存取章節資訊（ChapterIdx 和 ChapterLabel）
  - 使用訊號槽通知機制
  - 與其他模組進行通訊

### 檔案操作函式

* `void OpenFile(const QString& content)`：
  載入檔案內容。參數 `content` 包含完整的檔案內容字串。模組應自行解析所需的資料格式。

* `QString SaveFile()`：
  儲存檔案內容。回傳序列化後的內容字串，由主程式寫入檔案。

---

## 優先級系統說明

優先級系統決定模組在側邊欄的顯示順序：

* **高優先級（90-100）**：核心功能模組（如基本資訊、章節管理）
* **中優先級（50-89）**：常用功能模組（如角色管理、情節分析）
* **低優先級（10-49）**：輔助功能模組（如統計分析、匯出工具）
* **最低優先級（0-9）**：實驗性或專用模組

範例：
```cpp
int priority() const override {
    return 75;  // 中等優先級
}
```

---

## Content 和 Chapter 存取方法

### Content 存取範例

```cpp
class MyModule : public QWidget, public ModuleTemplate {
private:
    QPlainTextEdit* m_contentEditor = nullptr;

public:
    void setContentAccess(QPlainTextEdit* content) override {
        m_contentEditor = content;
        
        // 連接文字變更信號
        if (m_contentEditor) {
            connect(m_contentEditor, &QPlainTextEdit::textChanged,
                    this, &MyModule::onContentChanged);
        }
    }

private slots:
    void onContentChanged() {
        if (m_contentEditor) {
            QString currentText = m_contentEditor->toPlainText();
            // 處理文字變更...
        }
    }
};
```

### Chapter 存取範例

```cpp
class MyModule : public QWidget, public ModuleTemplate {
private:
    MainWindow* m_mainWindow = nullptr;

public:
    void setChapterAccess(MainWindow* mainWindow) override {
        m_mainWindow = mainWindow;
        
        // 連接章節相關信號
        if (m_mainWindow) {
            // 假設主視窗有章節變更信號
            connect(m_mainWindow, &MainWindow::chapterChanged,
                    this, &MyModule::onChapterChanged);
        }
    }

private slots:
    void onChapterChanged(const ChapterIdx& idx, const ChapterLabel& label) {
        // 處理章節變更...
        qDebug() << "Chapter changed to:" << idx.first << "," << idx.second 
                 << "Label:" << label;
    }
};
```

---

## 完整模組實作範例

```cpp
// MyModule.h
#pragma once
#include <QWidget>
#include <QPlainTextEdit>
#include "ModuleTemplate.h"

class MyModule : public QWidget, public ModuleTemplate {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    explicit MyModule(QWidget* parent = nullptr);
    ~MyModule() override;

    // ModuleTemplate 介面實作
    QString name() const override;
    QUuid moduleUuid() const override;
    int priority() const override;
    QIcon icon() const override;
    QWidget* widget() override;

    // 新增方法
    void setContentAccess(QPlainTextEdit* content) override;
    void setChapterAccess(MainWindow* mainWindow) override;

    // 檔案操作
    void OpenFile(const QString& content) override;
    QString SaveFile() override;

private slots:
    void onContentChanged();
    void onChapterChanged(const ChapterIdx& idx, const ChapterLabel& label);

private:
    QPlainTextEdit* m_contentEditor;
    MainWindow* m_mainWindow;
    QString m_moduleData;
};

// MyModule.cpp
#include "MyModule.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QUuid>

MyModule::MyModule(QWidget* parent)
    : QWidget(parent)
    , m_contentEditor(nullptr)
    , m_mainWindow(nullptr)
{
    // 設定 UI
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel("我的模組", this);
    layout->addWidget(label);
}

MyModule::~MyModule() = default;

QString MyModule::name() const {
    return "我的模組";
}

QUuid MyModule::moduleUuid() const {
    // 使用固定的 UUID，在開發期間保持不變
    return QUuid("{12345678-1234-1234-1234-123456789abc}");
}

int MyModule::priority() const {
    return 60;  // 中等優先級
}

QIcon MyModule::icon() const {
    return QIcon(":/icons/my_module.png");
}

QWidget* MyModule::widget() {
    return this;
}

void MyModule::setContentAccess(QPlainTextEdit* content) {
    m_contentEditor = content;
    if (m_contentEditor) {
        connect(m_contentEditor, &QPlainTextEdit::textChanged,
                this, &MyModule::onContentChanged);
    }
}

void MyModule::setChapterAccess(MainWindow* mainWindow) {
    m_mainWindow = mainWindow;
    if (m_mainWindow) {
        // 連接相關信號...
    }
}

void MyModule::OpenFile(const QString& content) {
    // 解析檔案內容
    m_moduleData = content;
    // 更新 UI...
}

QString MyModule::SaveFile() {
    // 序列化模組資料
    return m_moduleData;
}

void MyModule::onContentChanged() {
    if (m_contentEditor) {
        QString text = m_contentEditor->toPlainText();
        // 處理文字變更...
    }
}

void MyModule::onChapterChanged(const ChapterIdx& idx, const ChapterLabel& label) {
    // 處理章節變更...
}
```

---

## 實作提示

### 基本要求
* 請以 `QObject` 派生類別包裝您的模組，以支援 Qt Meta-Object System。
* 若使用 Qt Designer，建議將 `QWidget* widget()` 回傳為自訂的 UI 表單類別。
* 請確保您的模組使用唯一 `moduleUuid()`，避免與其他模組衝突。

### 新架構最佳實踐
* **內容同步**：使用 `setContentAccess()` 取得的內容編輯器進行統一的文字操作。
* **章節管理**：透過 `setChapterAccess()` 監聽章節變更，保持模組資料與當前章節同步。
* **資料持久化**：在 `SaveFile()` 中儲存模組的狀態資料，在 `OpenFile()` 中恢復。
* **錯誤處理**：對空指標和無效資料進行適當的檢查和處理。

### 效能考量
* 避免在 `onContentChanged()` 中進行重量級操作，考慮使用定時器延遲處理。
* 大型資料應考慮分段載入和快取機制。
* 適當使用 Qt 的訊號槽機制，避免輪詢檢查。

---

## 模組註冊

模組須透過 Qt 插件系統進行註冊。範例如下：

```cpp
// 在模組類別中添加：
Q_PLUGIN_METADATA(IID ModuleInterface_iid)
Q_INTERFACES(ModuleTemplate)
```

### 建置設定

確保您的模組專案包含以下設定：

```cmake
# CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_add_plugin(MyModule
    MyModule.h
    MyModule.cpp
)

target_link_libraries(MyModule Qt6::Core Qt6::Widgets)
```

或使用 qmake：

```pro
# MyModule.pro
QT += core widgets
CONFIG += plugin
TARGET = MyModule
TEMPLATE = lib

HEADERS += MyModule.h
SOURCES += MyModule.cpp
```

---

## 向後相容性

新架構在設計時考慮了向後相容性：

* 舊版本的 `OpenFile(content, type)` 和 `SaveFile(content, type)` 仍然支援，但建議遷移至新版本。
* 新的 `setContentAccess()` 和 `setChapterAccess()` 方法為必須實作，但可以提供空實作以保持相容性。
* 優先級系統為新功能，舊模組預設優先級為 50。

### 遷移指南

1. 更新 `ModuleTemplate.h` 引用
2. 實作新的 `setContentAccess()` 和 `setChapterAccess()` 方法
3. 修改 `OpenFile()` 和 `SaveFile()` 方法簽名
4. 設定適當的優先級
5. 測試模組在新架構下的功能

---
