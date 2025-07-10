#pragma once
#include <QDialog>
#include <QSqlDatabase>

namespace Ui { class StoreWindow; }

class StoreWindow : public QDialog
{
    Q_OBJECT
public:
    explicit StoreWindow(QWidget *parent = nullptr);
    ~StoreWindow() override;

private slots:
    void on_scanQrButton_clicked();

private:
    Ui::StoreWindow *ui;
    QSqlDatabase     db;
};
