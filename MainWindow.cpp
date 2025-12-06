#include <QApplication>
#include "logindialog.h"
#include "datamanager.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    // 核心步骤：加载所有 TXT 数据到内存
    DataManager::instance().loadData();

    LoginDialog loginDialog;
    if (loginDialog.exec() == QDialog::Accepted) {
        User loggedUser = loginDialog.getLoggedInUser();
        // 登录成功后，根据角色跳转到主窗口
        if (loggedUser.role == "admin") {
            // TODO: 显示 AdminMainWindow
        }
        else {
            // TODO: 显示 CustomerMainWindow
        }
        // ... 
    }

    return a.exec();
}