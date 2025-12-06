#ifndef USER_H
#define USER_H

#include <QString>

struct User {
    int id;
    QString username;
    QString displayName;
    QString password;
    QString phone;
    QString role; // "admin" or "customer" 
    QString idCard;
};

#endif // USER_H