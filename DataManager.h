#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QList>
#include "User.h"
#include "Station.h"
#include "Train.h"      // NEW
#include "Seat.h"       // NEW
#include "Order.h"      // NEW
#include "TrainSchedule.h"// ... 引入其他结构体头文件

class DataManager {
public:
    static DataManager& instance() {
        static DataManager instance;
        return instance;
    }

    QList<User> userList;
    QList<Station> stationList;
    // ... 其他列表

    void loadData(); // 负责加载所有文件
    QList<Train> trainList;
    QList<Seat> seatList;
    QList<Order> orderList;
    QList<TrainSchedule> scheduleList; // 时刻表列表

    // **重要辅助函数：用于在界面显示订单详情时查找关联信息**
    QString getStationName(int id) const;
    QString getTrainCode(int id) const;
    QString getSeatInfo(int id) const; // 返回 座位号(类型)，如 1A(商务)

    // **订单保存函数（用于退票或购票成功后写回文件）**
    void saveOrders();

    void saveUsers(); // Add this declaration

private:
    // 通用加载函数模板
    QList<User> loadUsers(const QString& fileName);
    QList<Station> loadStations(const QString& fileName);

    DataManager() {} // 私有构造函数，实现单例
    ~DataManager() {}
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    QList<Train> loadTrains(const QString& fileName);       // 新增
    QList<Seat> loadSeats(const QString& fileName);         // 新增
    QList<Order> loadOrders(const QString& fileName);       // 新增
    QList<TrainSchedule> loadTrainSchedule(const QString& fileName); // 新增
};

#endif // DATAMANAGER_H