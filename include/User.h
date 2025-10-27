#ifndef USER_H
#define USER_H

#include <QString>
#include <QDate>

enum class UserRole {
    ADMIN,
    CUSTOMER
};

class User {
public:
    User();
    User(const QString& username, const QString& password, UserRole role);
    
    QString getUsername() const;
    QString getPassword() const;
    UserRole getRole() const;
    bool validatePassword(const QString& password) const;
    
    void setPassword(const QString& newPassword);
    
private:
    QString username;
    QString password;
    UserRole role;
};

#endif