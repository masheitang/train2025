#ifndef CUSTOMERMAINWINDOW_H
#define CUSTOMERMAINWINDOW_H

#include <QMainWindow>
#include "User.h" // 引入用户数据结构

// *** 重点：声明 UI 类，用于关联 CustomerMainWindow.ui ***
namespace Ui {
    class CustomerMainWindow;
}

class CustomerMainWindow : public QMainWindow {
    Q_OBJECT

public:
    // 构造函数接收当前登录用户
    explicit CustomerMainWindow(const User& user, QWidget* parent = nullptr);
    ~CustomerMainWindow();

private slots:
    // --- Tab 1: 购票查询 ---
    void on_queryButton_clicked(); // 余票查询算法的入口

    // --- Tab 2: 订单管理 ---
    void on_refreshOrdersButton_clicked(); // 刷新订单列表
    void onCancelOrderClicked(); // 退票按钮的槽函数

    // --- Tab 3: 个人中心 ---
    void on_saveProfileButton_clicked(); // 保存个人信息

    void on_logoutButton_clicked(); // 登出按钮

private:
    // *** 重点：UI 成员变量 ***
    Ui::CustomerMainWindow* ui;
    User currentUser;

    // 私有初始化/加载函数
    void loadStationCombos(); // 加载车站到下拉框
    void loadOrderHistory();  // 加载并显示订单列表
    void setupProfileTab();   // 初始填充个人信息
    void setupQueryTab();
    void setupOrderTab();
};

#endif // CUSTOMERMAINWINDOW_H