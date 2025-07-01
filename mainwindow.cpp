#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 儲存、搜尋按鈕預留（不連接功能）
    connect(ui->UndoBtn, &QToolButton::clicked, this, &MainWindow::onUndoClicked);
    connect(ui->RedoBtn, &QToolButton::clicked, this, &MainWindow::onRedoClicked);
    connect(ui->AllSelectBtn, &QToolButton::clicked, this, &MainWindow::onAllSelectClicked);
    connect(ui->CopyBtn, &QToolButton::clicked, this, &MainWindow::onCopyClicked);
    connect(ui->CutBtn, &QToolButton::clicked, this, &MainWindow::onCutClicked);
    connect(ui->PasteBtn, &QToolButton::clicked, this, &MainWindow::onPasteClicked);

    // 預留 SaveBtn, FindBtn
    // connect(ui->SaveBtn, &QToolButton::clicked, ...);
    // connect(ui->FindBtn, &QToolButton::clicked, ...);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onUndoClicked()
{
    if (ui->Content)
        ui->Content->undo();
}

void MainWindow::onRedoClicked()
{
    if (ui->Content)
        ui->Content->redo();
}

void MainWindow::onAllSelectClicked()
{
    if (ui->Content)
        ui->Content->selectAll();
}

void MainWindow::onCopyClicked()
{
    if (ui->Content)
        ui->Content->copy();
}

void MainWindow::onCutClicked()
{
    if (ui->Content)
        ui->Content->cut();
}

void MainWindow::onPasteClicked()
{
    if (ui->Content)
        ui->Content->paste();
}
