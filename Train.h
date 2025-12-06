#ifndef TRAIN_H
#define TRAIN_H

#include <QString>

struct Train {
    int id;               // train_id
    QString code;         // train_code (e.g., G1)
    QString type;         // type (e.g., G)
    int startStationId;   // 始发站ID
    int endStationId;     // 终点站ID
};

#endif // TRAIN_H