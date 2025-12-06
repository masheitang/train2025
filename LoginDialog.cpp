#include "logindialog.h"
#include "ui_logindialog.h"
#include "datamanager.h"
#include <QMessageBox>
#include "RegisterDialog.h"

LoginDialog::LoginDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 隐藏密码输入
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginButton_clicked() {
    QString inputUsername = ui->usernameLineEdit->text();
    QString inputPassword = ui->passwordLineEdit->text();

    bool found = false;
    // 遍历内存中的用户列表进行查找
    for (const User& u : DataManager::instance().userList) {
        if (u.username == inputUsername && u.password == inputPassword) {
            currentUser = u;
            found = true;
            break;
        }
    }

    if (found) {
        // 登录成功
        QMessageBox::information(this, "成功", QString("欢迎您，%1！").arg(currentUser.displayName));
        accept(); // 关闭对话框，并返回 Accepted 结果
    }
    else {
        // 登录失败
        QMessageBox::warning(this, "错误", "用户名或密码错误。");
        ui->passwordLineEdit->clear(); // 清空密码框
    }
}

void LoginDialog::on_registerButton_clicked() {
    // 实例化并显示注册对话框
    RegisterDialog registerDialog(this);
    // 使用 exec() 以模态方式显示，用户必须先关闭注册窗口才能操作登录窗口
    registerDialog.exec();

    // 注册成功后，可以清空登录框以便新用户输入
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
}