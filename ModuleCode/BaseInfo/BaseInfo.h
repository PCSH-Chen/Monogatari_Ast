#ifndef BASEINFOMODULE_H
#define BASEINFOMODULE_H

#include <QWidget>
#include <QSet>
#include <QStringListModel>
#include <QPlainTextEdit>
#include <QUuid>
#include "../../ModuleTemplate.h"

class MainWindow; // 前向宣告

QT_BEGIN_NAMESPACE
namespace Ui { class BaseInfo; }
QT_END_NAMESPACE

class BaseInfoModule : public QWidget, public ModuleTemplate
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid) // 移除 FILE 參數
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
    void OpenFile(const QString& content) override;
    QString SaveFile() override;
    void setContentAccess(QPlainTextEdit* content) override;
    void setChapterAccess(MainWindow* mainWindow) override;

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
    
    // 存取權限（新增）
    QPlainTextEdit* m_contentEditor;
    MainWindow* m_mainWindow;
};

#endif // BASEINFOMODULE_H
