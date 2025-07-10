# 物語アシスタント｜模組開發文件

本文件說明如何撰寫一個可整合於物語アシスタント框架的模組（Module）。模組需遵循以下介面規範，確保其可以與主應用程式動態載入與互動。

---

## 目標

本開發介面旨在提供模組化擴充能力，使開發者可獨立構建功能性元件，並於執行期間整合至物語アシスタント的使用者介面中。所有模組都能存取主視窗的 Content 編輯器和 Chapter 資訊。

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

// 前向聲明
class MainWindow;

class ModuleTemplate {
public:
    virtual ~ModuleTemplate() {}
    virtual QString name() const = 0;           // 顯示名稱
    virtual QUuid moduleUuid() const = 0;       // 唯一識別 UUID
    virtual int priority() const = 0;           // 優先級（越大越高）
    virtual QIcon icon() const = 0;             // 64x64 icon
    virtual QWidget* widget() = 0;              // 內容顯示用

    // 存取權限設定
    virtual void setContentAccess(QPlainTextEdit* content) = 0;
    virtual void setChapterAccess(MainWindow* mainWindow) = 0;

    // 檔案操作函式
    virtual void OpenFile(const QString& content) = 0;
    virtual QString SaveFile() = 0;
};

#define ModuleInterface_iid "org.example.Monogatari.ModuleTemplate"
Q_DECLARE_INTERFACE(ModuleTemplate, ModuleInterface_iid)
```

---

## 成員說明

### **基本資訊**
* `QString name() const`：
  回傳模組在 UI 中顯示的名稱。

* `QUuid moduleUuid() const`：
  回傳模組的唯一識別UUID。每個模組必須有唯一的UUID，避免衝突。

* `int priority() const`：
  回傳模組的優先級（數字越大優先級越高）。影響模組在側邊欄的排序順序。

* `QIcon icon() const`：
  回傳 64x64 的圖示，供模組圖示顯示使用。

* `QWidget* widget()`：
  提供模組的主要畫面元件（會嵌入於主視窗的側邊欄中）。

### **存取權限設定**
* `void setContentAccess(QPlainTextEdit* content)`：
  主程式會呼叫此方法，提供對主視窗 Content 編輯器的存取權限。

* `void setChapterAccess(MainWindow* mainWindow)`：
  主程式會呼叫此方法，提供對主視窗 Chapter 資訊的存取權限。

### **檔案操作**
* `void OpenFile(const QString& content)`：
  由主程式呼叫以提供載入的檔案內容。模組需自行判斷內容格式。

* `QString SaveFile()`：
  由主程式呼叫以取得儲存後的內容資料。模組需自行序列化資料。

---

## 存取權限系統

### **Content 存取**
模組可以透過 `setContentAccess()` 獲得的 `QPlainTextEdit*` 指標來：
- 讀取主視窗的文字內容
- 修改主視窗的文字內容
- 執行編輯操作（復製、貼上、撤銷等）

### **Chapter 存取**
模組可以透過 `setChapterAccess()` 獲得的 `MainWindow*` 指標來：
- 讀取目前章節資訊：`getChapterIdx()` 和 `getChapterLabel()`
- 修改章節資訊：`setChapterIdx()` 和 `setChapterLabel()`
- 監聽章節變化：連接相關訊號

#### **Chapter 資料格式**
- `ChapterIdx`: `QPair<int, int>` 格式（章節編號, 子章節編號）
- `ChapterLabel`: `QString` 格式（章節標籤）

---

## 優先級系統

模組的載入順序和顯示順序由 `priority()` 方法決定：

1. **排序規則**：
   - 優先級越高（數字越大）的模組排在越前面
   - 相同優先級的模組按名稱字母順序排列

2. **建議優先級範圍**：
   - 系統核心模組：900-999
   - 重要功能模組：700-899
   - 一般功能模組：500-699
   - 輔助工具模組：300-499
   - 測試/實驗模組：100-299

---

## 完整實作範例

```cpp
// MyModule.h
#pragma once
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include "ModuleTemplate.h"

class MainWindow;

class MyModule : public QObject, public ModuleTemplate
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    MyModule(QObject *parent = nullptr);
    
    // ModuleTemplate 介面實作
    QString name() const override { return "我的模組"; }
    QUuid moduleUuid() const override { return QUuid::fromString("{12345678-1234-1234-1234-123456789012}"); }
    int priority() const override { return 600; }
    QIcon icon() const override { return QIcon(":/icons/my_module.png"); }
    QWidget* widget() override { return m_widget; }
    
    // 存取權限設定
    void setContentAccess(QPlainTextEdit* content) override;
    void setChapterAccess(MainWindow* mainWindow) override;
    
    // 檔案操作
    void OpenFile(const QString& content) override;
    QString SaveFile() override;

private slots:
    void onButtonClicked();
    void onChapterChanged(const QPair<int, int>& idx, const QString& label);

private:
    QWidget* m_widget;
    QLabel* m_chapterLabel;
    QPushButton* m_button;
    QPlainTextEdit* m_contentAccess;
    MainWindow* m_mainWindow;
};

// MyModule.cpp
#include "MyModule.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

MyModule::MyModule(QObject *parent)
    : QObject(parent)
    , m_widget(new QWidget())
    , m_chapterLabel(new QLabel("章節: 未載入"))
    , m_button(new QPushButton("取得內容"))
    , m_contentAccess(nullptr)
    , m_mainWindow(nullptr)
{
    // 建立模組 UI
    QVBoxLayout* layout = new QVBoxLayout(m_widget);
    layout->addWidget(m_chapterLabel);
    layout->addWidget(m_button);
    
    // 連接按鈕事件
    connect(m_button, &QPushButton::clicked, this, &MyModule::onButtonClicked);
}

void MyModule::setContentAccess(QPlainTextEdit* content)
{
    m_contentAccess = content;
    qDebug() << "Content access granted to module:" << name();
}

void MyModule::setChapterAccess(MainWindow* mainWindow)
{
    m_mainWindow = mainWindow;
    
    // 監聽章節變化
    connect(mainWindow, &MainWindow::chapterChanged, 
            this, &MyModule::onChapterChanged);
    
    // 更新初始章節資訊
    QPair<int, int> idx = mainWindow->getChapterIdx();
    QString label = mainWindow->getChapterLabel();
    onChapterChanged(idx, label);
    
    qDebug() << "Chapter access granted to module:" << name();
}

void MyModule::onButtonClicked()
{
    if (m_contentAccess) {
        QString content = m_contentAccess->toPlainText();
        QMessageBox::information(m_widget, "內容資訊", 
                               QString("目前內容長度: %1 字元").arg(content.length()));
    }
}

void MyModule::onChapterChanged(const QPair<int, int>& idx, const QString& label)
{
    m_chapterLabel->setText(QString("章節: %1.%2 - %3")
                           .arg(idx.first)
                           .arg(idx.second)
                           .arg(label));
}

void MyModule::OpenFile(const QString& content)
{
    // 解析檔案內容
    qDebug() << "Loading file content:" << content.left(100) << "...";
    
    // 自行判斷內容格式並處理
    if (content.startsWith("<MyModuleData>")) {
        // 處理 XML 格式
    } else if (content.startsWith("{")) {
        // 處理 JSON 格式
    }
}

QString MyModule::SaveFile()
{
    // 序列化模組資料
    QString data = "<MyModuleData>\n";
    data += "<Settings>...</Settings>\n";
    data += "</MyModuleData>\n";
    
    return data;
}

#include "MyModule.moc"
```

---

## 模組註冊

模組須透過 Qt 插件系統進行註冊：

```cpp
Q_PLUGIN_METADATA(IID ModuleInterface_iid)
Q_INTERFACES(ModuleTemplate)
```

---

## 編譯配置

在您的 `.pro` 檔案中加入：

```pro
CONFIG += plugin
TARGET = MyModule
TEMPLATE = lib

# 模組將被複製到 Modules 目錄
target.path = $$OUT_PWD/../Modules
INSTALLS += target
```

---

## 注意事項

1. **UUID 唯一性**：每個模組必須有唯一的 UUID，建議使用線上 UUID 產生器
2. **執行緒安全**：所有 UI 操作都必須在主執行緒中進行
3. **記憶體管理**：模組的 widget 會被主程式管理，無需手動刪除
4. **錯誤處理**：請在模組中適當處理例外情況
5. **相容性**：保持與舊版本模組的相容性

---