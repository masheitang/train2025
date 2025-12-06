#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "User.h" // 需要 User 结构体

namespace Ui {
    class RegisterDialog; // 对应 RegisterDialog.ui
}

class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget* parent = nullptr);
    ~RegisterDialog();

private slots:
    // 槽函数：处理“确认注册”按钮点击
    void on_registerConfirmButton_clicked();

    // 槽函数：处理“取消”按钮点击
    void on_cancelButton_clicked();

private:
    Ui::RegisterDialog* ui;

    // 辅助函数：校验用户名是否已存在
    bool isUsernameExists(const QString& username) const;
};

#endif // REGISTERDIALOG_H