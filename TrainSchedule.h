#ifndef TRAINSCHEDULE_H
#define TRAINSCHEDULE_H

#include <QString>

struct TrainSchedule {
    int scheduleId;    // schedule_id 
    int trainId;       // train_id 
    int stationId;     // station_id 
    int stopOrder;     // stop_order (停靠顺序) 
    QString arriveTime;  // arrive_time 
    QString departTime;  // depart_time 
};

#endif // TRAINSCHEDULE_H