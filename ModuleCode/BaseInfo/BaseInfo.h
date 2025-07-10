#pragma once
#include <QObject>
#include <QString>
#include <QWidget>
#include <QSet>
#include <QVector>
#include <QIcon>
#include "ModuleTemplate.h"
#include "ui_BaseInfo.h"

class BaseInfoModule : public QObject, public ModuleTemplate {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    BaseInfoModule();
    ~BaseInfoModule();

    // ModuleTemplate API
    QString name() const override { return tr("基本資訊"); }
    QString moduleUuid() const override { return QStringLiteral("a3d63b30-c9cd-4c7c-ac47-8dae778dc933"); }
    QIcon icon() const override { return QIcon(":/icon/BaseInfo.svg"); }
    QWidget* widget() override { return m_widget; }
    void OpenFile(const QString& content) override;
    QString SaveFile() override;

    // 外部匯入常用字（詞頻、統計值），主程式呼叫
    void ImportWords(const QVector<QString>& words, int total, double redundancyPercent, double filterPercent);

private:
    void loadJsonToSet(const QString& filename, QSet<QString>& targetSet);
    void updateRecentWordsTable(const QVector<QString>& words);

    QWidget* m_widget;
    Ui::Form ui;

    QSet<QString> m_words;          // 常用字集合
    QSet<QString> m_redundancySet;  // 贅字集合
    QSet<QString> m_filterSet;      // 修飾詞集合
};
