#ifndef SEAT_H
#define SEAT_H

#include <QString>

struct Seat {
    int id;           // seat_id
    int trainId;      // train_id
    QString seatNo;   // seat_no (e.g., 1A)
    QString seatType; // seat_type (e.g., 商务, 一等, 二等)
};

#endif // SEAT_H