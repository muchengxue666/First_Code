#include <QCoreApplication>
#include <QDebug>
#include "DataManager.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== 测试登录系统修复 ===";
    
    // 加载数据
    DataManager::getInstance().loadAllData();
    
    // 测试管理员登录
    qDebug() << "测试管理员登录...";
    User* admin = DataManager::getInstance().loginUser("admin", "admin123");
    if (admin && admin->getRole() == UserRole::ADMIN) {
        qDebug() << "✓ 管理员登录成功";
        qDebug() << "用户名:" << admin->getUsername();
        qDebug() << "角色:" << (admin->getRole() == UserRole::ADMIN ? "管理员" : "观众");
    } else {
        qDebug() << "✗ 管理员登录失败";
    }
    
    // 测试用户注册和登录
    qDebug() << "测试用户注册...";
    bool regResult = DataManager::getInstance().registerUser("testuser", "testpass", UserRole::CUSTOMER);
    if (regResult) {
        qDebug() << "✓ 用户注册成功";
    } else {
        qDebug() << "✗ 用户注册失败";
    }
    
    // 测试重复注册
    qDebug() << "测试重复注册...";
    bool dupResult = DataManager::getInstance().registerUser("testuser", "testpass", UserRole::CUSTOMER);
    if (!dupResult) {
        qDebug() << "✓ 重复注册检测正常";
    } else {
        qDebug() << "✗ 重复注册检测失败";
    }
    
    // 测试用户登录
    qDebug() << "测试用户登录...";
    User* user = DataManager::getInstance().loginUser("testuser", "testpass");
    if (user && user->getRole() == UserRole::CUSTOMER) {
        qDebug() << "✓ 用户登录成功";
        qDebug() << "用户名:" << user->getUsername();
        qDebug() << "角色:" << (user->getRole() == UserRole::ADMIN ? "管理员" : "观众");
    } else {
        qDebug() << "✗ 用户登录失败";
    }
    
    qDebug() << "=== 测试完成 ===";
    
    return 0;
}
