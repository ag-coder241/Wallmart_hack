#pragma once
#include <QMainWindow>
#include <QSqlDatabase>
#include "wallet_backend.h"  // bridge‑header

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class StoreWindow;   // fwd

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // customer‑side
    void on_purchaseButton_clicked();
    void on_redeemButton_clicked();

    // open store view
    void on_openStoreButton_clicked();

private:
    void initDatabase();
    void updatePointsLabel();
    void awardPoints(double amount);   // 0.01 % rule

    Ui::MainWindow *ui;
    QSqlDatabase   db;
    QString        user_id_ = "demo_user";  // TODO: replace with actual login

    StoreWindow   *storeWindow_ = nullptr;
};