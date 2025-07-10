#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "storewindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDatabase();
    updatePointsLabel();
}

MainWindow::~MainWindow() { delete ui; }

/* ---------- helpers ---------- */
void MainWindow::initDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("wallet.db");
    if (!db.open()) {
        QMessageBox::critical(this, "DB Error", db.lastError().text());
        return;
    }
    QSqlQuery q;
    q.exec("CREATE TABLE IF NOT EXISTS wallet (user TEXT PRIMARY KEY, points REAL)");
    q.prepare("INSERT OR IGNORE INTO wallet(user, points) VALUES (:u, 0)");
    q.bindValue(":u", user_id_);
    q.exec();
}

void MainWindow::updatePointsLabel()
{
    QSqlQuery q;
    q.prepare("SELECT points FROM wallet WHERE user = :u");
    q.bindValue(":u", user_id_);
    q.exec();
    double pts = 0;
    if (q.next()) pts = q.value(0).toDouble();
    ui->pointsLabel->setText(QString("Points: %1").arg(pts));
}

void MainWindow::awardPoints(double amount)
{
    double ptsAdd = amount * 0.0001;     // 0.01 %
    QSqlQuery q;
    q.prepare("UPDATE wallet SET points = points + :p WHERE user = :u");
    q.bindValue(":p", ptsAdd);
    q.bindValue(":u", user_id_);
    q.exec();
    updatePointsLabel();
}

/* ---------- slots ---------- */
void MainWindow::on_purchaseButton_clicked()
{
    bool ok = false;
    double amt = ui->amountEdit->text().toDouble(&ok);
    if (!ok || amt <= 0) {
        QMessageBox::warning(this, "Invalid", "Enter a valid purchase amount");
        return;
    }

    std::string token = generate_token(amt, user_id_.toStdString());
    if (!store_token(user_id_.toStdString(), token)) {
        QMessageBox::critical(this, "Error", "Failed to store token");
        return;
    }

    awardPoints(amt);

    QString file = QFileDialog::getSaveFileName(this, "Save QR", {}, "PNG (*.png)");
    if (!file.isEmpty()) {
        QImage img(token.size()*8, token.size()*8, QImage::Format_Mono);
        img.fill(Qt::white);
        img.save(file);
    }
    QMessageBox::information(this, "Success", "Token generated & points awarded");
}

void MainWindow::on_redeemButton_clicked()
{
    bool ok = false;
    double req = ui->amountEdit->text().toDouble(&ok);
    if (!ok || req <= 0) {
        QMessageBox::warning(this, "Invalid", "Enter amount");
        return;
    }

    try {
        double leftover = redeem_token(user_id_.toStdString(), req);
        QMessageBox::information(this, "Redeemed",
                                 QString("Redeemed %1 pts. Leftover: %2 pts.")
                                 .arg(req).arg(leftover));
        updatePointsLabel();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Redeem failed", e.what());
    }
}

void MainWindow::on_openStoreButton_clicked()
{
    if (!storeWindow_) storeWindow_ = new StoreWindow(this);
    storeWindow_->show();
}
