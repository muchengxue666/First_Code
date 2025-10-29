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
    // void handleAdminLogin();
    // void handleCustomerLogin();
    // void handleCustomerRegister();
    void showAdminDashboard();
    void showCustomerDashboard();
    void logout();
    
    // 管理员方法 
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
    
    // 用户方法
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
    
    //以下是css格式的表，用来加载不同风格
    QString buttonStyle = 
        "QPushButton {"
    "    background-color: #2c3e50;"
    "    color: white;"
    "    border: 2px solid #34495e;"
    "    border-radius: 8px;"
    "    font-size: 14px;"
    "    font-weight: bold;"
    "    padding: 8px 16px;"
    "    min-width: 80px;"
    "    min-height: 32px;"
    "}"
    "QPushButton:hover {"
    "    background-color: #34495e;"
    "    border-color: #4a69bd;"
    "}"
    "QPushButton:pressed {"
    "    background-color: #1a252f;"
    "    border-color: #4a69bd;"
    "}"
    "QPushButton:disabled {"
    "    background-color: #95a5a6;"
    "    border-color: #7f8c8d;"
    "    color: #bdc3c7;"
    "}";

    // 添加列表控件的统一样式
    QString listWidgetStyle = 
    "QListWidget {"
    "    background-color: #2c3e50;"           // 深蓝色背景
    "    border: 2px solid #34495e;"
    "    border-radius: 8px;"
    "    padding: 5px;"
    "    font-size: 14px;"
    "    outline: none;"
    "    color: #ecf0f1;"                      // 浅灰色文字
    "}"
    "QListWidget::item {"
    "    background-color: #34495e;"           // 稍亮的蓝色背景
    "    border: 1px solid #4a69bd;"           // 蓝色边框
    "    border-radius: 6px;"
    "    padding: 12px 15px;"
    "    margin: 3px 0px;"
    "    color: #ecf0f1;"
    "}"
    "QListWidget::item:selected {"
    "    background-color: #e74c3c;"           // 选中项红色背景
    "    color: white;"
    "    border: 1px solid #c0392b;"
    "    font-weight: bold;"
    "}"
    "QListWidget::item:hover {"
    "    background-color: #4a69bd;"           // 悬停时蓝色背景
    "    border: 1px solid #3498db;"
    "    color: white;"
    "}"
    "QListWidget::item:alternate {"
    "    background-color: #2c3e50;"           // 交替行颜色
    "}"
    "QScrollBar:vertical {"
    "    background-color: #2c3e50;"
    "    width: 15px;"
    "    border-radius: 7px;"
    "}"
    "QScrollBar::handle:vertical {"
    "    background-color: #3498db;"
    "    border-radius: 7px;"
    "    min-height: 20px;"
    "}"
    "QScrollBar::handle:vertical:hover {"
    "    background-color: #2980b9;"
    "}"
    "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
    "    border: none;"
    "    background: none;"
    "}";

    QString tableWidgetStyle = 
    "QTableWidget {"
    "    background-color: #2c3e50;"           // 深色背景
    "    border: 2px solid #2c3e50;"
    "    border-radius: 10px;"
    "    gridline-color: #34495e;"
    "    font-size: 14px;"
    "    outline: none;"
    "    color: #ecf0f1;"
    "    selection-background-color: #d7796fc4;" // 选中背景色
    "    selection-color: white;"
    "}"
    "QTableWidget::item {"
    "    padding: 12px 15px;"
    "    border-bottom: 1px solid #2c3e50;"
    "    background-color: #2c3e50;"
    "    color: #ecf0f1;"
    "}"
    "QTableWidget::item:selected {"
    "    background-color: #e74c3c;"
    "    color: white;"
    "    font-weight: bold;"
    "}"
    "QTableWidget::item:alternate {"
    "    background-color: #34495e;"           // 交替行颜色
    "}"
    "QHeaderView::section {"
    "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
    "                                stop:0 #13388dff, stop:1 #c0392b);"  // 红色渐变表头
    "    color: white;"
    "    padding: 15px 20px;"
    "    border: none;"
    "    border-right: 1px solid #2c3e50;"
    "    border-bottom: 2px solid #2c3e50;"
    "    font-weight: bold;"
    "    font-size: 15px;"
    "    text-align: center;"
    "}"
    "QHeaderView::section:first {"
    "    border-top-left-radius: 8px;"
    "}"
    "QHeaderView::section:last {"
    "    border-top-right-radius: 8px;"
    "}"
    "QTableCornerButton::section {"
    "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
    "                                stop:0 #e74c3c, stop:1 #c0392b);"
    "    border: none;"
    "    border-bottom: 2px solid #2c3e50;"
    "    border-top-left-radius: 8px;"
    "}"
    "QScrollBar:vertical {"
    "    background-color: #2c3e50;"
    "    width: 15px;"
    "    border-radius: 7px;"
    "}"
    "QScrollBar::handle:vertical {"
    "    background-color: #3498db;"
    "    border-radius: 7px;"
    "    min-height: 20px;"
    "}"
    "QScrollBar::handle:vertical:hover {"
    "    background-color: #2980b9;"
    "}"
    "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
    "    border: none;"
    "    background: none;"
    "}";

    // 添加标题样式
    QString titleLabelStyle = 
    "QLabel {"
    "    color: #f1c40f;"                    // 亮金色
    "    font-size: 20px;"
    "    font-weight: bold;"
    "    margin: 15px 0px;"
    "    padding: 12px 25px;"
    "    background-color: #1a1a1a;"         // 纯黑色背景模拟胶片
    "    border-radius: 0px;"                // 直角边框模拟胶片
    "    border: 3px solid #f1c40f;"
    "    text-align: center;"
    "    text-shadow: 0px 0px 8px rgba(241, 196, 15, 0.7);"  // 发光效果
    "    letter-spacing: 2px;"               // 更大的字间距
    "    font-family: 'Arial Black', 'Microsoft YaHei', sans-serif;"
    "}";
};
#endif