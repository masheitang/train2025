#ifndef ORDER_H
#define ORDER_H

#include <QString>

struct Order {
    int id;             // order_id
    int userId;
    int trainId;
    int seatId;
    int startStationId;
    int endStationId;
    QString travelDate; // 出行日期 (e.g., 2025-07-01)
    QString orderTime;  // 下单时间 (e.g., 2025-06-25 08:00)
    QString status;     // 状态 (e.g., booked, canceled)
};

#endif // ORDER_H