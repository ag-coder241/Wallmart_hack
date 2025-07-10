#include "storewindow.h"
#include "ui_storewindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>

StoreWindow::StoreWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::StoreWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::database();   // reuse main connection
}

StoreWindow::~StoreWindow() { delete ui; }

void StoreWindow::on_scanQrButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Open QR", {}, "PNG (*.png)");
    if (file.isEmpty()) return;

    // Simplified: treat filename (without .png) as token
    std::string token = QFileInfo(file).baseName().toStdString();

    // TODO: decode token, validate, redeem, etc.
    QMessageBox::information(this, "Store", "Token accepted (stub)");
}
