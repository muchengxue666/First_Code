#include "MainWindow.h"
#include "DataManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QMessageBox>
#include <QGroupBox>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), currentUser(nullptr) {
    setupUI();
    showRoleSelection();
}

void MainWindow::setupUI() {
    setWindowTitle("电影院售票系统");
    setMinimumSize(800, 600);
    
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    
    createRoleSelectionPage();
    createLoginPage(true);  // Admin login
    createLoginPage(false); // Customer login
    createRegisterPage();
    createAdminDashboard();
    createCustomerDashboard();
    createScheduleManagementPage();
    createBoxOfficePage();
    createMovieListPage();
    createSeatSelectionPage();
}

void MainWindow::createRoleSelectionPage() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    QLabel *titleLabel = new QLabel("电影院售票系统");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleLabel->setFont(titleFont);
    
    QPushButton *adminBtn = new QPushButton("管理员登录");
    QPushButton *customerBtn = new QPushButton("观众登录/注册");
    
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(50);
    layout->addWidget(adminBtn);
    layout->addWidget(customerBtn);
    layout->addStretch();
    
    connect(adminBtn, &QPushButton::clicked, this, &MainWindow::showAdminLogin);
    connect(customerBtn, &QPushButton::clicked, this, &MainWindow::showCustomerLogin);
    
    stackedWidget->addWidget(page);
}

void MainWindow::showRoleSelection() {
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showAdminLogin() {
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showCustomerLogin() {
    stackedWidget->setCurrentIndex(2);
}

// ... 其他MainWindow方法的实现（由于篇幅限制，这里省略详细实现）
// 实际实现中需要完成所有界面创建和业务逻辑