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
    void addNewSchedule();
    void editSchedule();
    void deleteSchedule();
    void refreshScheduleList();
    
    // Customer functions
    void showMovieList();
    void showSeatSelection();
    void purchaseTicket();
    void cancelTicket();
    void refreshCustomerMovieList();

private:
    void setupUI();
    void createRoleSelectionPage();
    void createLoginPage(bool isAdmin);
    void createRegisterPage();
    void createAdminDashboard();
    void createCustomerDashboard();
    void createScheduleManagementPage();
    void createBoxOfficePage();
    void createMovieListPage();
    void createSeatSelectionPage();
    
    QStackedWidget *stackedWidget;
    User* currentUser;
    
    // Common widgets
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QListWidget *scheduleList;
    QTableWidget *movieTable;
    QComboBox *hallComboBox;
    QComboBox *movieComboBox;
    QDateEdit *dateEdit;
    QComboBox *timeComboBox;
};

#endif