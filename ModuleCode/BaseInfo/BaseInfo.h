#ifndef BASEINFOMODULE_H
#define BASEINFOMODULE_H

#include <QWidget>
#include <QSet>
#include <QStringListModel>
#include "../../ModuleTemplate.h"
#include "ui_BaseInfo.h" // 由 uic 自動產生

QT_BEGIN_NAMESPACE
namespace Ui { class BaseInfo; }
QT_END_NAMESPACE

class BaseInfoModule : public QWidget, public ModuleTemplate
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid) // 模組元數據
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
    void OpenFile(const QString& content, const QString& type) override;
    QString SaveFile(const QString& content, const QString& type) override;
    void connectToMainContent(QObject* mainContentWidget) override;

    // --- 額外方法 (非介面必需) ---
    QString moduleName() const;

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
};

#endif // BASEINFOMODULE_H
