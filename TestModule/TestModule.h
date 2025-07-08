#ifndef TESTMODULE_H
#define TESTMODULE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include "../ModuleTemplate.h"

class TestModule : public QWidget, public ModuleTemplate
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid)
    Q_INTERFACES(ModuleTemplate)

public:
    explicit TestModule(QWidget *parent = nullptr);
    ~TestModule() override;

    // ModuleTemplate interface implementation
    QString name() const override;
    QUuid moduleUuid() const override;
    int priority() const override;
    QIcon icon() const override;
    QWidget* widget() override;
    void OpenFile(const QString& content, const QString& type) override;
    QString SaveFile(const QString& content, const QString& type) override;
    
    // New interface methods
    void setContentAccess(QObject* contentWidget) override;
    bool initialize() override;
    bool activate() override;
    bool deactivate() override;
    void cleanup() override;

private slots:
    void onContentChanged();
    void onTestButtonClicked();

private:
    void setupUI();
    
    QLabel* m_statusLabel;
    QLabel* m_contentInfoLabel;
    QPushButton* m_testButton;
    QPlainTextEdit* m_contentAccess;
    int m_contentLength;
};

#endif // TESTMODULE_H