#ifndef BASEINFOMODULE_H
#define BASEINFOMODULE_H

#include <QWidget>
#include <QSet>
#include <QStringListModel>
#include <QPlainTextEdit>
#include "ModuleTemplate.h"
#include "ui_BaseInfo.h" // 由 uic 自動產生

class MainWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class BaseInfo; }
QT_END_NAMESPACE

class BaseInfoModule : public QWidget, public ModuleTemplate
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    explicit BaseInfoModule(QWidget *parent = nullptr);
    ~BaseInfoModule() override;

    // --- ModuleTemplate 介面實作 ---
    QString name() const override;
    QUuid moduleUuid() const override;
    int priority() const override;
    QIcon icon() const override;
    QWidget* widget() override;
    
    // 新增：內容和章節存取方法
    void setContentAccess(QPlainTextEdit* content) override;
    void setChapterAccess(MainWindow* mainWindow) override;
    
    // 檔案操作（已修改）
    void OpenFile(const QString& content) override;
    QString SaveFile() override;

    // --- 對外擴充函數 ---
    void WordsListImport(const QVector<QString>& lists);

private slots:
    // --- UI 事件 ---
    void onTagAdd();
    void onTagDel();
    void onTagSelectionChanged();

    // --- 核心邏輯 ---
    void onContentChanged();

private:
    void loadStopwords();
    void updateWordCount(const QString& text);
    void updateRecentWords(const QString& text);
    void updateRedundancy(const QString& text);
    void setupUI();

    Ui::BaseInfo *ui;
    QSet<QString> m_tags;
    QStringListModel *m_tagModel;

    QSet<QString> m_vocabulary;
    QSet<QString> m_stopwords;
    
    // 新增：存取權限
    QPlainTextEdit* m_contentEditor;
    MainWindow* m_mainWindow;
};

#endif // BASEINFOMODULE_H
