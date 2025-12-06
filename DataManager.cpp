#include "datamanager.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

void DataManager::loadData() {
    // 假设数据文件在项目根目录下的 data/ 文件夹中
    userList = loadUsers("data/users.txt");
    stationList = loadStations("data/stations.txt");

    // --- 新增加载项 ---
    trainList = loadTrains("data/trains.txt");
    seatList = loadSeats("data/seats.txt");
    orderList = loadOrders("data/orders.txt");
    scheduleList = loadTrainSchedule("data/train_schedule.txt");
    // -------------------

    qDebug() << "Data loaded. Total Orders:" << orderList.size();
}

QList<User> DataManager::loadUsers(const QString& fileName) {
    QList<User> list;
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.readLine(); // 跳过第一行标题 

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            QStringList parts = line.split(",");

            if (parts.size() >= 7) {
                User u;

                u.id = parts[0].trimmed().toInt();  // user_id
                u.username = parts[2].trimmed();          // username
                u.displayName = parts[1].trimmed();          // display_name
                u.password = parts[3].trimmed();          // password
                u.phone = parts[4].trimmed();          // phone
                u.role = parts[5].trimmed();          // role
                u.idCard = parts[6].trimmed();          // id_card

                list.append(u);
            }
        }
        file.close();
    }
    else {
        qDebug() << "Error: Could not open file for reading:" << fileName;
    }

    return list;
}



QList<Station> DataManager::loadStations(const QString& fileName) {
    QList<Station> list;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.readLine(); // 跳过第一行标题 [cite: 3]
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            QStringList parts = line.split(",");
            if (parts.size() >= 2) { // 2个字段 [cite: 3]
                Station s;
                s.id = parts[0].toInt();
                s.name = parts[1];
                list.append(s);
            }
        }
        file.close();
    }
    return list;
}

QList<QStringList> readFileLines(const QString& fileName) {
    QList<QStringList> records;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.readLine(); // 跳过第一行标题 [cite: 49, 50, 51, 52, 53, 54]
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            records.append(line.split(","));
        }
        file.close();
    }
    else {
        qDebug() << "Error: Could not open file for reading:" << fileName;
    }
    return records;
}

QList<Train> DataManager::loadTrains(const QString& fileName) {
    QList<Train> list;
    for (const QStringList& parts : readFileLines(fileName)) {
        if (parts.size() >= 5) { // train_id,train_code,type,start_station_id,end_station_id 
            Train t;
            t.id = parts[0].toInt();
            t.code = parts[1];
            t.type = parts[2];
            t.startStationId = parts[3].toInt();
            t.endStationId = parts[4].toInt();
            list.append(t);
        }
    }
    return list;
}

QList<Seat> DataManager::loadSeats(const QString& fileName) {
    QList<Seat> list;
    for (const QStringList& parts : readFileLines(fileName)) {
        if (parts.size() >= 4) { // seat_id,train_id,seat_no,seat_type 
            Seat s;
            s.id = parts[0].toInt();
            s.trainId = parts[1].toInt();
            s.seatNo = parts[2];
            s.seatType = parts[3];
            list.append(s);
        }
    }
    return list;
}

QList<Order> DataManager::loadOrders(const QString& fileName) {
    QList<Order> list;
    for (const QStringList& parts : readFileLines(fileName)) {
        if (parts.size() >= 9) { // order_id,user_id,train_id,seat_id,start_station_id,end_station_id,travel_date,order_time,status 
            Order o;
            o.id = parts[0].toInt();
            o.userId = parts[1].toInt();
            o.trainId = parts[2].toInt();
            o.seatId = parts[3].toInt();
            o.startStationId = parts[4].toInt();
            o.endStationId = parts[5].toInt();
            o.travelDate = parts[6];
            o.orderTime = parts[7];
            o.status = parts[8];
            list.append(o);
        }
    }
    return list;
}

// --- 辅助函数实现 (用于关联查询) ---

QString DataManager::getStationName(int id) const {
    for (const Station& s : stationList) {
        if (s.id == id) {
            return s.name;
        }
    }
    return "未知站";
}

QString DataManager::getTrainCode(int id) const {
    for (const Train& t : trainList) {
        if (t.id == id) {
            return t.code;
        }
    }
    return "未知车次";
}

QString DataManager::getSeatInfo(int id) const {
    for (const Seat& s : seatList) {
        if (s.id == id) {
            return QString("%1 (%2)").arg(s.seatNo, s.seatType);
        }
    }
    return "未知座位";
}

// --- 保存函数实现 ---

void DataManager::saveOrders() {
    QFile file("data/orders.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // 写入头文件 (必须确保与读取格式一致)
        out << "order_id,user_id,train_id,seat_id,start_station_id,end_station_id,travel_date,order_time,status\n";

        for (const Order& o : orderList) {
            // 写入订单记录
            out << o.id << "," << o.userId << "," << o.trainId << ","
                << o.seatId << "," << o.startStationId << "," << o.endStationId << ","
                << o.travelDate << "," << o.orderTime << "," << o.status << "\n";
        }
        file.close();
        qDebug() << "Orders saved successfully.";
    }
    else {
        qDebug() << "Error: Could not open orders.txt for writing.";
    }
}

void DataManager::saveUsers() {
    QFile file("data/users.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "错误: 无法打开 users.txt 进行写入。";
        return;
    }

    QTextStream out(&file);

    // 写入 CSV 头行 (必须与 loadUsers 函数读取的格式一致)
    out << "user_id,username,display_name,password,phone,role,id_card\n"; //

    // 遍历 userList 并写入数据
    for (const User& user : userList) {
        // 格式: id,username,display_name,password,phone,role,id_card
        out << user.id << ","
            << user.username << ","
            << user.displayName << ","
            << user.password << ","
            << user.phone << ","
            << user.role << ","
            << user.idCard << "\n"; // 假设 user.idCard 存在
    }

    file.close();
    qDebug() << "用户数据已成功保存到 users.txt。";
}

QList<TrainSchedule> DataManager::loadTrainSchedule(const QString& fileName) {
    QList<TrainSchedule> list;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "错误: 无法打开文件" << fileName;
        return list;
    }

    QTextStream in(&file);
    // 跳过第一行（标题行）
    if (!in.atEnd()) {
        in.readLine();
    }

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // 假设数据格式为: schedule_id,train_id,station_id,stop_order,arrive_time,depart_time
        QStringList fields = line.split(',');
        if (fields.size() < 6) { // 至少需要 6 个字段
            qDebug() << "错误: 时刻表数据格式不正确，跳过行:" << line;
            continue;
        }

        TrainSchedule ts;
        // 1. schedule_id
        ts.scheduleId = fields.at(0).toInt();
        // 2. train_id
        ts.trainId = fields.at(1).toInt();
        // 3. station_id
        ts.stationId = fields.at(2).toInt();
        // 4. stop_order
        ts.stopOrder = fields.at(3).toInt();
        // 5. arrive_time (可能有空值)
        ts.arriveTime = fields.at(4).trimmed();
        // 6. depart_time (可能有空值)
        ts.departTime = fields.at(5).trimmed();

        list.append(ts);
    }

    file.close();
    qDebug() << QString("成功加载 %1 条时刻表记录。").arg(list.size());
    return list;
}