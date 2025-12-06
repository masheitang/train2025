#include <QApplication>
#include "LoginDialog.h"
#include "DataManager.h"// 数据管理单例
#include "CustomerMainWindow.h" // 【新增】引入顾客主窗口头文件
#include <QMessageBox> // 添加此行以包含 QMessageBox 的定义
// #include "AdminMainWindow.h" // 如果 AdminMainWindow 已经创建，也需要引入

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    // 核心步骤：加载所有 TXT 数据到内存
    // 确保 DataManager::loadData() 包含所有 loadXXX 函数的调用，如 loadUsers, loadStations, etc.
    DataManager::instance().loadData();

    LoginDialog loginDialog;

    // 启动登录对话框，如果用户点击了登录按钮且验证通过，exec() 返回 QDialog::Accepted
    if (loginDialog.exec() == QDialog::Accepted) {
        User loggedUser = loginDialog.getLoggedInUser();

        // 登录成功后，根据用户角色跳转到相应的主窗口
        if (loggedUser.role == "admin") {
            // TODO: 实例化和显示 AdminMainWindow
            // 任务书中定义的管理员角色
            // AdminMainWindow adminWindow(loggedUser);
            // adminWindow.show();
            // return a.exec();

            // 暂时使用提示信息作为占位符
            QMessageBox::information(nullptr, "提示", "管理员功能开发中，将继续运行主事件循环。");

        }
        else if (loggedUser.role == "customer") {
            // 任务书中定义的普通旅客角色
            // 【关键步骤】实例化并显示顾客主窗口
            CustomerMainWindow customerWindow(loggedUser);
            customerWindow.show();

            // 进入主事件循环，等待窗口操作，直到窗口关闭
            return a.exec();
        }
        else {
            // 角色不明确，退出程序
            return 0;
        }
    }

    // 如果 loginDialog.exec() 返回 QDialog::Rejected (用户点击取消或关闭窗口)
    return 0; // 退出程序
}