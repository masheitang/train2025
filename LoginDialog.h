#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "User.h" // 引入用户数据结构

// *** 重点：声明 UI 类，用于关联 LoginDialog.ui ***
namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();

    // 登录成功后获取当前用户
    User getLoggedInUser() const { return currentUser; }

private slots:
    void on_loginButton_clicked();//登录
    void on_registerButton_clicked();//注册

private:
    // *** 重点：UI 成员变量 ***
    Ui::LoginDialog* ui;
    User currentUser;
};

#endif // LOGINDIALOG_H