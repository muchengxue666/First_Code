#include "User.h"

User::User() : username(""), password(""), role(UserRole::CUSTOMER) {}

User::User(const QString& username, const QString& password, UserRole role)
    : username(username), password(password), role(role) {}

//属性的获取
QString User::getUsername() const { return username; }
QString User::getPassword() const { return password; }
UserRole User::getRole() const { return role; }

bool User::validatePassword(const QString& password) const {
    return this->password == password;
}

void User::setPassword(const QString& newPassword) {
    password = newPassword;
}