#include "RegisterDialog.h"
#include "ui_RegisterDialog.h" // 确保这个文件已生成
#include "DataManager.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    setWindowTitle("新用户注册");
    // 连接按钮信号 (确保 UI 中的按钮 objectName 是 registerConfirmButton 和 cancelButton)
    //connect(ui->registerConfirmButton, &QPushButton::clicked, this, &RegisterDialog::on_registerConfirmButton_clicked);
    //connect(ui->cancelButton, &QPushButton::clicked, this, &RegisterDialog::on_cancelButton_clicked);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

bool RegisterDialog::isUsernameExists(const QString& username) const {
    for (const User& user : DataManager::instance().userList) {
        if (user.username.compare(username, Qt::CaseInsensitive) == 0) {
            return true;
        }
    }
    return false;
}

void RegisterDialog::on_registerConfirmButton_clicked() {
    // 1. 获取输入
    QString password = ui->passwordLineEdit->text();
    QString passwordConfirm = ui->passwordConfirmLineEdit->text(); // 假设有确认密码框
    QString displayName = ui->displayNameLineEdit->text().trimmed();
    QString username = ui->usernameLineEdit->text().trimmed();
    QString phone = ui->phoneLineEdit->text().trimmed();

    // 2. 校验
    if (username.isEmpty() || password.isEmpty() || displayName.isEmpty() || phone.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "所有字段都不能为空！");
        return;
    }
    if (password != passwordConfirm) {
        QMessageBox::warning(this, "注册失败", "两次输入的密码不一致！");
        return;
    }
    if (isUsernameExists(username)) {
        QMessageBox::warning(this, "注册失败", "该用户名已被注册，请更换。");
        return;
    }

    // 3. 生成新用户 ID
    int newUserId = 0;
    for (const User& user : DataManager::instance().userList) {
        if (user.id > newUserId) {
            newUserId = user.id;
        }
    }
    newUserId++;

    // 4. 创建并保存用户
    User newUser;
    newUser.id = newUserId;
    newUser.username = username;
    newUser.displayName = displayName;
    newUser.password = password;
    newUser.phone = phone;
    newUser.role = "customer"; // 默认注册为普通顾客
    newUser.idCard = ""; // 身份证暂不要求

    DataManager::instance().userList.append(newUser);
    DataManager::instance().saveUsers(); // 保存到 users.txt

    QMessageBox::information(this, "注册成功", "账号已创建成功，请返回登录！");
    this->accept(); // 关闭对话框，返回 QDialog::Accepted 状态
}

void RegisterDialog::on_cancelButton_clicked() {
    this->reject(); // 关闭对话框，返回 QDialog::Rejected 状态
}