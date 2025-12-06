#include "CustomerMainWindow.h"
#include "ui_CustomerMainWindow.h" // 一定要包含这个自动生成的头文件
#include "DataManager.h"
#include <QMessageBox>
#include <QDate>
#include <QDebug>
#include <QPushButton>
#include <LoginDialog.h>

// ====================================================================
// 构造函数与初始化
// ====================================================================

CustomerMainWindow::CustomerMainWindow(const User& user, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CustomerMainWindow),
    currentUser(user)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle(QString("高铁订票系统 - 欢迎您, %1").arg(currentUser.displayName));

    // 初始化三个标签页的数据
    setupQueryTab();    // 1. 购票查询页初始化
    setupOrderTab();    // 2. 订单页初始化
    setupProfileTab();  // 3. 个人中心初始化
}

CustomerMainWindow::~CustomerMainWindow()
{
    delete ui;
}

// ====================================================================
// TAB 1: 购票查询功能 (Ticket Query)
// ====================================================================

void CustomerMainWindow::setupQueryTab() {
    // 1. 初始化表格列头
    QStringList headers = { "车次", "出发站", "到达站", "出发时间", "到达时间", "二等座", "一等座", "操作" };
    ui->resultTableWidget->setColumnCount(headers.size());
    ui->resultTableWidget->setHorizontalHeaderLabels(headers);
    ui->resultTableWidget->horizontalHeader()->setStretchLastSection(true); // 让最后一列填充

    // 2. 加载车站到下拉框 (ComboBox)
    ui->startStationCombo->clear();
    ui->endStationCombo->clear();

    // 遍历 DataManager 中的车站列表
    for (const Station& s : DataManager::instance().stationList) {
        // addItem(显示的文字, 隐藏的数据ID)
        ui->startStationCombo->addItem(s.name, s.id);
        ui->endStationCombo->addItem(s.name, s.id);
    }

    // 设置默认日期为今天
    ui->travelDateEdit->setDate(QDate::currentDate());
}

// 点击“查询”按钮
void CustomerMainWindow::on_queryButton_clicked() {
    // 1. 获取用户输入
    int startStationId = ui->startStationCombo->currentData().toInt();
    int endStationId = ui->endStationCombo->currentData().toInt();
    QDate travelDate = ui->travelDateEdit->date();

    // 简单校验
    if (startStationId == endStationId) {
        QMessageBox::warning(this, "提示", "出发站和到达站不能相同！");
        return;
    }
    if (travelDate < QDate::currentDate()) {
        QMessageBox::warning(this, "提示", "不能选择过去的日期！");
        return;
    }

    // 2. 清空表格
    ui->resultTableWidget->setRowCount(0);

    // 3. 核心算法：查找符合条件的车次
    // 遍历所有列车
    for (const Train& train : DataManager::instance().trainList) {

        // 临时变量，用于记录该车次在出发站和到达站的信息
        QString departTime = "";
        QString arriveTime = "";
        int startSeq = -1; // 出发站的停靠顺序
        int endSeq = -1;   // 到达站的停靠顺序

        // 遍历时刻表，查看该车次是否经过这两个站
        for (const TrainSchedule& ts : DataManager::instance().scheduleList) {
            if (ts.trainId == train.id) {
                if (ts.stationId == startStationId) {
                    startSeq = ts.stopOrder;
                    departTime = ts.departTime; // 记录出发时间
                }
                else if (ts.stationId == endStationId) {
                    endSeq = ts.stopOrder;
                    arriveTime = ts.arriveTime; // 记录到达时间
                }
            }
        }

        // 4. 判断逻辑：
        // (1) 必须都找到了 (Seq != -1)
        // (2) 出发站顺序必须在到达站之前 (startSeq < endSeq)
        if (startSeq != -1 && endSeq != -1 && startSeq < endSeq) {

            // 5. 将结果添加到表格中
            int row = ui->resultTableWidget->rowCount();
            ui->resultTableWidget->insertRow(row);

            // 填充基本信息
            ui->resultTableWidget->setItem(row, 0, new QTableWidgetItem(train.code));
            ui->resultTableWidget->setItem(row, 1, new QTableWidgetItem(ui->startStationCombo->currentText()));
            ui->resultTableWidget->setItem(row, 2, new QTableWidgetItem(ui->endStationCombo->currentText()));
            ui->resultTableWidget->setItem(row, 3, new QTableWidgetItem(departTime));
            ui->resultTableWidget->setItem(row, 4, new QTableWidgetItem(arriveTime));

            // 显示余票 (简化逻辑：显示"有票"或者根据 seats 表统计总数)
            // 这里为了演示，我们先假设都有票
            ui->resultTableWidget->setItem(row, 5, new QTableWidgetItem("有票")); // 二等座
            ui->resultTableWidget->setItem(row, 6, new QTableWidgetItem("有票")); // 一等座

            // 6. 添加“预订”按钮
            QPushButton* bookBtn = new QPushButton("预订");
            bookBtn->setProperty("trainId", train.id); // 绑定车次ID
            ui->resultTableWidget->setCellWidget(row, 7, bookBtn);

            // 连接预订按钮 (这里暂未实现具体选座逻辑，可留空或简单弹窗)
            connect(bookBtn, &QPushButton::clicked, [=]() {
                QMessageBox::information(this, "预订", QString("您选择了 %1 次列车，功能开发中...").arg(train.code));
                // TODO: 下一步可以在这里弹出选座对话框，或者直接生成订单
                });
        }
    }

    if (ui->resultTableWidget->rowCount() == 0) {
        QMessageBox::information(this, "提示", "没有查到符合条件的车次。");
    }
}

// ====================================================================
// TAB 2: 订单管理功能 (Order History)
// ====================================================================

void CustomerMainWindow::setupOrderTab() {
    // 设置表格头
    QStringList headers = { "订单号", "车次", "行程", "座位", "日期", "状态", "操作" };
    ui->orderTableWidget->setColumnCount(headers.size());
    ui->orderTableWidget->setHorizontalHeaderLabels(headers);
    ui->orderTableWidget->horizontalHeader()->setStretchLastSection(true);

    // 绑定刷新按钮
    connect(ui->refreshOrdersButton, &QPushButton::clicked, this, &CustomerMainWindow::on_refreshOrdersButton_clicked);

    // 初始加载一次
    on_refreshOrdersButton_clicked();
}

void CustomerMainWindow::on_refreshOrdersButton_clicked() {
    ui->orderTableWidget->setRowCount(0); // 清空

    // 遍历所有订单，筛选出当前用户的
    for (const Order& o : DataManager::instance().orderList) {
        if (o.userId != currentUser.id) continue;

        int row = ui->orderTableWidget->rowCount();
        ui->orderTableWidget->insertRow(row);

        // 获取关联信息 (需要 DataManager 提供辅助函数)
        QString trainCode = DataManager::instance().getTrainCode(o.trainId);
        QString startStation = DataManager::instance().getStationName(o.startStationId);
        QString endStation = DataManager::instance().getStationName(o.endStationId);
        QString seatInfo = DataManager::instance().getSeatInfo(o.seatId);

        ui->orderTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(o.id)));
        ui->orderTableWidget->setItem(row, 1, new QTableWidgetItem(trainCode));
        ui->orderTableWidget->setItem(row, 2, new QTableWidgetItem(startStation + "->" + endStation));
        ui->orderTableWidget->setItem(row, 3, new QTableWidgetItem(seatInfo));
        ui->orderTableWidget->setItem(row, 4, new QTableWidgetItem(o.travelDate));

        // 状态处理 (booked: 已支付, canceled: 已退票)
        QString statusStr = (o.status == "booked") ? "已支付" : "已退票";
        ui->orderTableWidget->setItem(row, 5, new QTableWidgetItem(statusStr));

        // 如果状态是"booked"，显示退票按钮
        if (o.status == "booked") {
            QPushButton* cancelBtn = new QPushButton("退票");
            cancelBtn->setProperty("orderId", o.id); // 绑定订单ID
            ui->orderTableWidget->setCellWidget(row, 6, cancelBtn);

            // 连接退票槽函数
            connect(cancelBtn, &QPushButton::clicked, this, &CustomerMainWindow::onCancelOrderClicked);
        }
        else {
            ui->orderTableWidget->setItem(row, 6, new QTableWidgetItem("-"));
        }
    }
}

// 退票逻辑
void CustomerMainWindow::onCancelOrderClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int orderId = btn->property("orderId").toInt();

    // 确认对话框
    auto reply = QMessageBox::question(this, "退票确认", "您确定要退掉这张票吗？",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;

    // 1. 在内存列表中查找并修改状态
    bool found = false;
    // 注意：这里必须使用引用 &，才能修改原数据
    for (Order& o : DataManager::instance().orderList) {
        if (o.id == orderId) {
            o.status = "canceled";
            found = true;
            break;
        }
    }

    if (found) {
        // 2. 保存到文件
        DataManager::instance().saveOrders();

        // 3. 刷新界面
        QMessageBox::information(this, "成功", "退票成功！");
        on_refreshOrdersButton_clicked();
    }
    else {
        QMessageBox::critical(this, "错误", "未找到订单信息！");
    }
}

// ====================================================================
// TAB 3: 个人中心功能 (Profile)
// ====================================================================

void CustomerMainWindow::setupProfileTab() {
    // 将当前用户信息填入输入框
    ui->usernameLineEdit->setText(currentUser.username);
    ui->displayNameLineEdit->setText(currentUser.displayName);
    ui->phoneLineEdit->setText(currentUser.phone);
    // 用户名通常不可修改，设为只读
    ui->usernameLineEdit->setReadOnly(true);
}

void CustomerMainWindow::on_saveProfileButton_clicked() {
    QString newDisplayName = ui->displayNameLineEdit->text();
    QString newPhone = ui->phoneLineEdit->text();

    if (newDisplayName.isEmpty() || newPhone.isEmpty()) {
        QMessageBox::warning(this, "警告", "姓名和手机号不能为空！");
        return;
    }

    // 1. 更新当前内存中的对象
    currentUser.displayName = newDisplayName;
    currentUser.phone = newPhone;

    // 2. 更新 DataManager 全局列表中的数据
    bool found = false;
    for (User& u : DataManager::instance().userList) {
        if (u.id == currentUser.id) {
            u.displayName = newDisplayName;
            u.phone = newPhone;
            found = true;
            break;
        }
    }

    // 3. 保存回文件
    if (found) {
        DataManager::instance().saveUsers();
        QMessageBox::information(this, "成功", "个人信息已更新！");

        // 更新窗口标题
        setWindowTitle(QString("高铁订票系统 - 欢迎您, %1").arg(currentUser.displayName));
    }
}

// 登出按钮
void CustomerMainWindow::on_logoutButton_clicked() {
    this->close(); // 关闭当前主窗口，main函数会结束或者返回到登录逻辑
}