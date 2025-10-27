#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "User.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
class QListWidget;
class QTableWidget;
class QLabel;
class QComboBox;
class QDateEdit;
class QGridLayout;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void showRoleSelection();
    void showAdminLogin();
    void showCustomerLogin();
    void showCustomerRegister();
    void handleAdminLogin();
    void handleCustomerLogin();
    // void handleCustomerRegister();
    void showAdminDashboard();
    void showCustomerDashboard();
    void logout();
    
    // Admin functions
    void showScheduleManagement();
    void showBoxOfficeStats();
    void showMovieManagement();  // 添加电影管理显示
    void addNewSchedule();
    void editSchedule();
    void deleteSchedule();
    void refreshScheduleList();
    void addNewMovie();          // 添加新增电影
    void editMovie();            // 添加编辑电影
    void deleteMovie();          // 添加删除电影
    void refreshMovieList();     // 添加刷新电影列表
    
    // Customer functions
    void showMovieList();
    void showSeatSelection(int scheduleId);
    void purchaseTicket();
    void cancelTicket();
    void refreshCustomerMovieList();
    void onSeatSelected(bool checked);
    void showMyTickets();  // 添加我的票务显示

private:
    void setupUI();
    void createRoleSelectionPage();
    void createLoginPage(bool isAdmin);
    void createRegisterPage();
    void createAdminDashboard();
    void createCustomerDashboard();
    void createMovieManagementPage();  // 添加电影管理页面创建
    void createScheduleManagementPage();
    void createBoxOfficePage();
    void createMovieListPage();
    void createSeatSelectionPage();
    void createMyTicketsPage();  // 添加我的票务页面创建
    void refreshMyTickets();     // 添加刷新我的票务
    
    QStackedWidget *stackedWidget;
    User* currentUser;
    
    // Common widgets
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QListWidget *scheduleList;
    QListWidget *movieList;      // 添加电影列表
    QTableWidget *movieTable;
    QTableWidget *myTicketsTable;  // 添加我的票务表格
    QComboBox *hallComboBox;
    QComboBox *movieComboBox;
    QDateEdit *dateEdit;
    QComboBox *timeComboBox;

    int currentScheduleId = -1;
    QGridLayout* seatLayout = nullptr;
    QLabel* seatInfoLabel = nullptr;
    QVector<QPushButton*> seatButtons;
};

#endif