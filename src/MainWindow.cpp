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
#include <QGridLayout>
#include <QScrollArea>
#include <QButtonGroup>
#include <QRadioButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTableWidgetItem>
#include <QDateTime>

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
    createMyTicketsPage();  // 添加我的票务页面
}

// 角色选择页面
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
    
    adminBtn->setMinimumHeight(50);
    customerBtn->setMinimumHeight(50);
    
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

// 登录页面
void MainWindow::createLoginPage(bool isAdmin) {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    QLabel *titleLabel = new QLabel(isAdmin ? "管理员登录" : "观众登录");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleLabel->setFont(titleFont);
    
    QFormLayout *formLayout = new QFormLayout;
    QLineEdit *loginUsernameEdit = new QLineEdit;  // 使用不同的变量名
    QLineEdit *loginPasswordEdit = new QLineEdit;
    loginPasswordEdit->setEchoMode(QLineEdit::Password);
    
    formLayout->addRow("用户名:", loginUsernameEdit);
    formLayout->addRow("密码:", loginPasswordEdit);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *loginBtn = new QPushButton("登录");
    QPushButton *backBtn = new QPushButton("返回");
    
    buttonLayout->addWidget(backBtn);
    buttonLayout->addWidget(loginBtn);
    
    if (!isAdmin) {
        QPushButton *registerBtn = new QPushButton("注册账号");
        buttonLayout->addWidget(registerBtn);
        connect(registerBtn, &QPushButton::clicked, this, &MainWindow::showCustomerRegister);
    }
    
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addLayout(formLayout);
    layout->addSpacing(20);
    layout->addLayout(buttonLayout);
    layout->addStretch();
    
    if (isAdmin) {
        connect(loginBtn, &QPushButton::clicked, [this, loginUsernameEdit, loginPasswordEdit]() {
            QString username = loginUsernameEdit->text();
            QString password = loginPasswordEdit->text();
            
            User* user = DataManager::getInstance().loginUser(username, password);
            if (user && user->getRole() == UserRole::ADMIN) {
                currentUser = user;
                showAdminDashboard();
            } else {
                QMessageBox::warning(this, "登录失败", "用户名或密码错误");
            }
        });
    } else {
        connect(loginBtn, &QPushButton::clicked, [this, loginUsernameEdit, loginPasswordEdit]() {
            QString username = loginUsernameEdit->text();
            QString password = loginPasswordEdit->text();
            
            User* user = DataManager::getInstance().loginUser(username, password);
            if (user && user->getRole() == UserRole::CUSTOMER) {
                currentUser = user;
                showCustomerDashboard();
            } else {
                QMessageBox::warning(this, "登录失败", "用户名或密码错误");
            }
        });
    }
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showRoleSelection);
    
    stackedWidget->addWidget(page);
}

// 注册页面
void MainWindow::createRegisterPage() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    QLabel *titleLabel = new QLabel("观众注册");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleLabel->setFont(titleFont);
    
    QFormLayout *formLayout = new QFormLayout;
    QLineEdit *regUsernameEdit = new QLineEdit;  // 使用不同的变量名
    QLineEdit *regPasswordEdit = new QLineEdit;
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    QLineEdit *confirmPasswordEdit = new QLineEdit;
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    
    formLayout->addRow("用户名:", regUsernameEdit);
    formLayout->addRow("密码:", regPasswordEdit);
    formLayout->addRow("确认密码:", confirmPasswordEdit);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *registerBtn = new QPushButton("注册");
    QPushButton *backBtn = new QPushButton("返回登录");
    
    buttonLayout->addWidget(backBtn);
    buttonLayout->addWidget(registerBtn);
    
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addLayout(formLayout);
    layout->addSpacing(20);
    layout->addLayout(buttonLayout);
    layout->addStretch();
    
    connect(registerBtn, &QPushButton::clicked, [this, regUsernameEdit, regPasswordEdit, confirmPasswordEdit]() {
        QString username = regUsernameEdit->text();
        QString password = regPasswordEdit->text();
        QString confirmPassword = confirmPasswordEdit->text();
        
        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "注册失败", "用户名和密码不能为空");
            return;
        }
        
        if (password != confirmPassword) {
            QMessageBox::warning(this, "注册失败", "两次输入的密码不一致");
            return;
        }
        
        if (DataManager::getInstance().registerUser(username, password, UserRole::CUSTOMER)) {
            QMessageBox::information(this, "注册成功", "注册成功，请登录");
            // 清空输入框
            regUsernameEdit->clear();
            regPasswordEdit->clear();
            confirmPasswordEdit->clear();
            showCustomerLogin();
        } else {
            QMessageBox::warning(this, "注册失败", "用户名已存在");
        }
    });
    
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showCustomerLogin);
    
    stackedWidget->addWidget(page);
}

// 管理员仪表板
void MainWindow::createAdminDashboard() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    QLabel *titleLabel = new QLabel("管理员控制台");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleLabel->setFont(titleFont);
    
    QPushButton *scheduleBtn = new QPushButton("排片管理");
    QPushButton *boxOfficeBtn = new QPushButton("票房统计");
    QPushButton *logoutBtn = new QPushButton("退出登录");
    
    scheduleBtn->setMinimumHeight(50);
    boxOfficeBtn->setMinimumHeight(50);
    logoutBtn->setMinimumHeight(40);
    
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(30);
    layout->addWidget(scheduleBtn);
    layout->addWidget(boxOfficeBtn);
    layout->addStretch();
    layout->addWidget(logoutBtn);
    
    connect(scheduleBtn, &QPushButton::clicked, this, &MainWindow::showScheduleManagement);
    connect(boxOfficeBtn, &QPushButton::clicked, this, &MainWindow::showBoxOfficeStats);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::logout);
    
    stackedWidget->addWidget(page);
}

// 观众仪表板
void MainWindow::createCustomerDashboard() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    QLabel *titleLabel = new QLabel("观众中心");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleLabel->setFont(titleFont);
    
    QPushButton *movieListBtn = new QPushButton("浏览电影");
    QPushButton *myTicketsBtn = new QPushButton("我的票务");
    QPushButton *logoutBtn = new QPushButton("退出登录");
    
    movieListBtn->setMinimumHeight(50);
    myTicketsBtn->setMinimumHeight(50);
    logoutBtn->setMinimumHeight(40);
    
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(30);
    layout->addWidget(movieListBtn);
    layout->addWidget(myTicketsBtn);
    layout->addStretch();
    layout->addWidget(logoutBtn);
    
    connect(movieListBtn, &QPushButton::clicked, this, &MainWindow::showMovieList);
    connect(myTicketsBtn, &QPushButton::clicked, this, &MainWindow::showMyTickets);  // 连接我的票务
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::logout);
    
    stackedWidget->addWidget(page);
}

void MainWindow::createMyTicketsPage() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    QLabel *titleLabel = new QLabel("我的票务");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleLabel->setFont(titleFont);
    
    // 票务表格
    myTicketsTable = new QTableWidget;
    myTicketsTable->setColumnCount(6);
    myTicketsTable->setHorizontalHeaderLabels({"电影名称", "放映厅", "放映时间", "座位", "购票时间", "操作"});
    
    QPushButton *backBtn = new QPushButton("返回");
    
    layout->addWidget(titleLabel);
    layout->addWidget(myTicketsTable);
    layout->addWidget(backBtn);
    
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showCustomerDashboard);
    
    stackedWidget->addWidget(page);
}

// 添加显示我的票务方法
void MainWindow::showMyTickets() {
    refreshMyTickets();
    stackedWidget->setCurrentIndex(10);  // 假设这是第10个页面
}

// 添加刷新我的票务方法
void MainWindow::refreshMyTickets() {
    if (!currentUser) {
        QMessageBox::warning(this, "错误", "请先登录");
        return;
    }
    
    auto userTickets = DataManager::getInstance().getTicketsByUser(currentUser->getUsername());
    myTicketsTable->setRowCount(userTickets.size());
    
    for (int i = 0; i < userTickets.size(); ++i) {
        const auto& ticket = userTickets[i];
        Schedule* schedule = DataManager::getInstance().findSchedule(ticket.getScheduleId());
        
        if (schedule) {
            Movie* movie = DataManager::getInstance().findMovie(schedule->getMovieId());
            CinemaHall* hall = DataManager::getInstance().findHall(schedule->getHallId());
            
            if (movie && hall) {
                myTicketsTable->setItem(i, 0, new QTableWidgetItem(movie->getTitle()));
                myTicketsTable->setItem(i, 1, new QTableWidgetItem(hall->getName()));
                myTicketsTable->setItem(i, 2, new QTableWidgetItem(schedule->getShowTime().toString("yyyy-MM-dd hh:mm")));
                myTicketsTable->setItem(i, 3, new QTableWidgetItem(QString("第%1排 第%2座").arg(ticket.getRow() + 1).arg(ticket.getCol() + 1)));
                myTicketsTable->setItem(i, 4, new QTableWidgetItem(ticket.getPurchaseTime().toString("yyyy-MM-dd hh:mm")));
                
                QPushButton *cancelBtn = new QPushButton("退票");
                cancelBtn->setProperty("ticketId", ticket.getTicketId());
                connect(cancelBtn, &QPushButton::clicked, this, &MainWindow::cancelTicket);
                myTicketsTable->setCellWidget(i, 5, cancelBtn);
            }
        }
    }
}

// 完善退票方法
void MainWindow::cancelTicket() {
    QPushButton* cancelBtn = qobject_cast<QPushButton*>(sender());
    if (!cancelBtn) return;
    
    int ticketId = cancelBtn->property("ticketId").toInt();
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认退票", "确定要退票吗？",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        DataManager::getInstance().removeTicket(ticketId);
        refreshMyTickets();
        QMessageBox::information(this, "退票成功", "退票成功！");
    }
}

// 排片管理页面
void MainWindow::createScheduleManagementPage() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    QLabel *titleLabel = new QLabel("排片管理");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleLabel->setFont(titleFont);
    
    // 排片列表
    scheduleList = new QListWidget;
    
    // 操作按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *addBtn = new QPushButton("新增排片");
    QPushButton *editBtn = new QPushButton("编辑排片");
    QPushButton *deleteBtn = new QPushButton("删除排片");
    QPushButton *backBtn = new QPushButton("返回");
    
    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(editBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(backBtn);
    
    layout->addWidget(titleLabel);
    layout->addWidget(scheduleList);
    layout->addLayout(buttonLayout);
    
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addNewSchedule);
    connect(editBtn, &QPushButton::clicked, this, &MainWindow::editSchedule);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteSchedule);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showAdminDashboard);
    
    stackedWidget->addWidget(page);
}

// 票房统计页面
void MainWindow::createBoxOfficePage() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    QLabel *titleLabel = new QLabel("票房统计");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleLabel->setFont(titleFont);
    
    // 日期选择
    QHBoxLayout *dateLayout = new QHBoxLayout;
    QLabel *dateLabel = new QLabel("选择日期:");
    dateEdit = new QDateEdit;
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    QPushButton *refreshBtn = new QPushButton("刷新");
    
    dateLayout->addWidget(dateLabel);
    dateLayout->addWidget(dateEdit);
    dateLayout->addWidget(refreshBtn);
    dateLayout->addStretch();
    
    // 票房表格
    QTableWidget *boxOfficeTable = new QTableWidget;
    boxOfficeTable->setColumnCount(3);
    boxOfficeTable->setHorizontalHeaderLabels({"电影名称", "票房收入(元)", "排名"});
    
    QPushButton *backBtn = new QPushButton("返回");
    
    layout->addWidget(titleLabel);
    layout->addLayout(dateLayout);
    layout->addWidget(boxOfficeTable);
    layout->addWidget(backBtn);
    
    connect(refreshBtn, &QPushButton::clicked, [this, boxOfficeTable]() {
        QDate selectedDate = dateEdit->date();
        auto stats = DataManager::getInstance().getBoxOfficeStats(selectedDate);
        
        boxOfficeTable->setRowCount(stats.size());
        for (int i = 0; i < stats.size(); ++i) {
            boxOfficeTable->setItem(i, 0, new QTableWidgetItem(stats[i].first));
            boxOfficeTable->setItem(i, 1, new QTableWidgetItem(QString::number(stats[i].second, 'f', 2)));
            boxOfficeTable->setItem(i, 2, new QTableWidgetItem(QString::number(i + 1)));
        }
    });
    
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showAdminDashboard);
    
    stackedWidget->addWidget(page);
}

// 电影列表页面
void MainWindow::createMovieListPage() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    QLabel *titleLabel = new QLabel("电影排片");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleLabel->setFont(titleFont);
    
    // 日期选择
    QHBoxLayout *dateLayout = new QHBoxLayout;
    QLabel *dateLabel = new QLabel("选择日期:");
    dateEdit = new QDateEdit;
    dateEdit->setDate(QDate::currentDate().addDays(1)); // 默认显示明天
    dateEdit->setCalendarPopup(true);
    QPushButton *refreshBtn = new QPushButton("刷新");
    
    dateLayout->addWidget(dateLabel);
    dateLayout->addWidget(dateEdit);
    dateLayout->addWidget(refreshBtn);
    dateLayout->addStretch();
    
    // 电影表格
    movieTable = new QTableWidget;
    movieTable->setColumnCount(5);
    movieTable->setHorizontalHeaderLabels({"电影名称", "放映厅", "放映时间", "剩余座位", "操作"});
    
    QPushButton *backBtn = new QPushButton("返回");
    
    layout->addWidget(titleLabel);
    layout->addLayout(dateLayout);
    layout->addWidget(movieTable);
    layout->addWidget(backBtn);
    
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::refreshCustomerMovieList);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showCustomerDashboard);
    
    stackedWidget->addWidget(page);
}

// 座位选择页面
void MainWindow::createSeatSelectionPage() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    QLabel *titleLabel = new QLabel("选择座位");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleLabel->setFont(titleFont);
    
    // 座位图容器
    QWidget *seatMapWidget = new QWidget;
    seatLayout = new QGridLayout(seatMapWidget);
    seatLayout->setSpacing(5);
    
    // 信息显示
    seatInfoLabel = new QLabel("请选择座位");
    seatInfoLabel->setAlignment(Qt::AlignCenter);
    
    // 操作按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *confirmBtn = new QPushButton("确认购票");
    QPushButton *cancelBtn = new QPushButton("取消");
    
    buttonLayout->addWidget(confirmBtn);
    buttonLayout->addWidget(cancelBtn);
    
    layout->addWidget(titleLabel);
    layout->addWidget(seatMapWidget);
    layout->addWidget(seatInfoLabel);
    layout->addLayout(buttonLayout);
    
    connect(confirmBtn, &QPushButton::clicked, this, &MainWindow::purchaseTicket);
    connect(cancelBtn, &QPushButton::clicked, this, &MainWindow::showMovieList);
    
    stackedWidget->addWidget(page);
}

// 添加座位选择处理
void MainWindow::onSeatSelected(bool checked) {
    QPushButton* seatBtn = qobject_cast<QPushButton*>(sender());
    if (!seatBtn) return;
    
    int row = seatBtn->property("row").toInt();
    int col = seatBtn->property("col").toInt();
    
    // 统计选中的座位数量
    int selectedCount = 0;
    for (QPushButton* btn : seatButtons) {
        if (btn->isChecked()) {
            selectedCount++;
        }
    }
    
    if (selectedCount > 0) {
        seatInfoLabel->setText(QString("已选择 %1 个座位").arg(selectedCount));
    } else {
        seatInfoLabel->setText("请选择座位");
    }
}

// 页面切换方法
void MainWindow::showRoleSelection() {
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showAdminLogin() {
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showCustomerLogin() {
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::showCustomerRegister() {
    stackedWidget->setCurrentIndex(3);
}

void MainWindow::showAdminDashboard() {
    stackedWidget->setCurrentIndex(4);
}

void MainWindow::showCustomerDashboard() {
    stackedWidget->setCurrentIndex(5);
}

void MainWindow::showScheduleManagement() {
    refreshScheduleList();
    stackedWidget->setCurrentIndex(6);
}

void MainWindow::showBoxOfficeStats() {
    stackedWidget->setCurrentIndex(7);
}

void MainWindow::showMovieList() {
    refreshCustomerMovieList();
    stackedWidget->setCurrentIndex(8);
}

void MainWindow::showSeatSelection(int scheduleId) {
    currentScheduleId = scheduleId;
    Schedule* schedule = DataManager::getInstance().findSchedule(scheduleId);
    if (!schedule) {
        QMessageBox::warning(this, "错误", "排片信息不存在");
        return;
    }
    
    // 清空之前的座位按钮和布局
    qDeleteAll(seatButtons);
    seatButtons.clear();
    
    // 清空座位布局中的所有子控件
    QLayoutItem* item;
    while ((item = seatLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // 获取排片和放映厅信息
    Movie* movie = DataManager::getInstance().findMovie(schedule->getMovieId());
    CinemaHall* hall = DataManager::getInstance().findHall(schedule->getHallId());
    
    if (!movie || !hall) {
        QMessageBox::warning(this, "错误", "电影或放映厅信息不存在");
        return;
    }
    
    // 更新标题
    QWidget* seatPage = stackedWidget->widget(9);
    QLabel* titleLabel = seatPage->findChild<QLabel*>();
    if (titleLabel) {
        titleLabel->setText(QString("选择座位 - %1 - %2").arg(movie->getTitle()).arg(hall->getName()));
    }
    
    // 创建座位图
    const auto& seats = schedule->getSeats();
    int rows = seats.size();
    int cols = rows > 0 ? seats[0].size() : 0;
    
    // 添加屏幕标识（放在最上面）
    QLabel* screenLabel = new QLabel("银幕");
    screenLabel->setAlignment(Qt::AlignCenter);
    screenLabel->setStyleSheet("background-color: #3498db; color: white; padding: 10px; border-radius: 5px;");
    seatLayout->addWidget(screenLabel, 0, 0, 1, cols + 2, Qt::AlignCenter);  // 占据所有列
    
    // 创建座位按钮
    for (int row = 0; row < rows; ++row) {
        // 添加排号标签（左边）
        QLabel* rowLabel = new QLabel(QString("第%1排").arg(row + 1));
        rowLabel->setAlignment(Qt::AlignCenter);
        seatLayout->addWidget(rowLabel, row + 1, 0);
        
        for (int col = 0; col < cols; ++col) {
            QPushButton* seatBtn = new QPushButton(QString::number(col + 1));
            seatBtn->setFixedSize(40, 40);
            seatBtn->setProperty("row", row);
            seatBtn->setProperty("col", col);
            seatBtn->setCheckable(true);
            
            // 设置座位状态
            if (seats[row][col]) {
                seatBtn->setText("已售");
                seatBtn->setEnabled(false);
                seatBtn->setStyleSheet("background-color: #e74c3c; color: white;");
            } else {
                seatBtn->setStyleSheet(
                    "QPushButton { background-color: #2ecc71; color: white; border: 1px solid #27ae60; }"
                    "QPushButton:checked { background-color: #f39c12; border: 2px solid #e67e22; }"
                    "QPushButton:hover { background-color: #27ae60; }"
                );
            }
            
            connect(seatBtn, &QPushButton::toggled, this, &MainWindow::onSeatSelected);
            seatLayout->addWidget(seatBtn, row + 1, col + 1);  // 从第1列开始
            seatButtons.append(seatBtn);
        }
    }
    
    seatInfoLabel->setText(QString("票价: %1元 - 请选择座位").arg(movie->getPrice()));
    stackedWidget->setCurrentIndex(9);
}

// 登录处理
void MainWindow::handleAdminLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    
    User* user = DataManager::getInstance().loginUser(username, password);
    if (user && user->getRole() == UserRole::ADMIN) {
        currentUser = user;
        showAdminDashboard();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
}

void MainWindow::handleCustomerLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    
    User* user = DataManager::getInstance().loginUser(username, password);
    if (user && user->getRole() == UserRole::CUSTOMER) {
        currentUser = user;
        showCustomerDashboard();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
}

void MainWindow::logout() {
    currentUser = nullptr;
    DataManager::getInstance().setCurrentUser(nullptr);
    showRoleSelection();
}

// 排片管理方法
void MainWindow::refreshScheduleList() {
    scheduleList->clear();
    
    auto schedules = DataManager::getInstance().getAllSchedules();
    for (const auto& schedule : schedules) {
        Movie* movie = DataManager::getInstance().findMovie(schedule.getMovieId());
        CinemaHall* hall = DataManager::getInstance().findHall(schedule.getHallId());
        
        if (movie && hall) {
            QString itemText = QString("%1 - %2 - %3 - 剩余座位: %4")
                                  .arg(movie->getTitle())
                                  .arg(hall->getName())
                                  .arg(schedule.getShowTime().toString("yyyy-MM-dd hh:mm"))
                                  .arg(schedule.getAvailableSeats());
            
            QListWidgetItem* item = new QListWidgetItem(itemText);
            item->setData(Qt::UserRole, schedule.getScheduleId());
            scheduleList->addItem(item);
        }
    }
}

void MainWindow::addNewSchedule() {
    // 创建添加排片对话框
    QDialog dialog(this);
    dialog.setWindowTitle("新增排片");
    
    QFormLayout *formLayout = new QFormLayout(&dialog);
    
    QComboBox *movieComboBox = new QComboBox;
    QComboBox *hallComboBox = new QComboBox;
    QDateTimeEdit *timeEdit = new QDateTimeEdit;
    timeEdit->setDateTime(QDateTime::currentDateTime().addDays(1));
    timeEdit->setCalendarPopup(true);
    
    // 填充电影列表
    auto movies = DataManager::getInstance().getAllMovies();
    for (const auto& movie : movies) {
        movieComboBox->addItem(movie.getTitle(), movie.getId());
    }
    
    // 填充放映厅列表
    auto halls = DataManager::getInstance().getAllHalls();
    for (const auto& hall : halls) {
        hallComboBox->addItem(hall.getName(), hall.getHallId());
    }
    
    formLayout->addRow("电影:", movieComboBox);
    formLayout->addRow("放映厅:", hallComboBox);
    formLayout->addRow("放映时间:", timeEdit);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    formLayout->addRow(buttonBox);
    
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        int movieId = movieComboBox->currentData().toInt();
        int hallId = hallComboBox->currentData().toInt();
        QDateTime showTime = timeEdit->dateTime();
        
        Schedule newSchedule(DataManager::getInstance().getNextScheduleId(), 
                           movieId, hallId, showTime);
        
        // 设置座位布局
        CinemaHall* hall = DataManager::getInstance().findHall(hallId);
        if (hall) {
            newSchedule.setSeatLayout(hall->getRows(), hall->getCols());
        }
        
        DataManager::getInstance().addSchedule(newSchedule);
        refreshScheduleList();
    }
}

void MainWindow::editSchedule() {
    QList<QListWidgetItem*> selectedItems = scheduleList->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "编辑排片", "请选择要编辑的排片");
        return;
    }
    
    int scheduleId = selectedItems.first()->data(Qt::UserRole).toInt();
    Schedule* schedule = DataManager::getInstance().findSchedule(scheduleId);
    if (!schedule) return;
    
    // 编辑对话框实现类似新增
    // 这里省略详细实现
}

void MainWindow::deleteSchedule() {
    QList<QListWidgetItem*> selectedItems = scheduleList->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "删除排片", "请选择要删除的排片");
        return;
    }
    
    int scheduleId = selectedItems.first()->data(Qt::UserRole).toInt();
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除", "确定要删除这个排片吗？",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        DataManager::getInstance().deleteSchedule(scheduleId);
        refreshScheduleList();
    }
}

// 观众功能方法
// 修改电影列表中的选座按钮连接
void MainWindow::refreshCustomerMovieList() {
    QDate selectedDate = dateEdit->date();
    auto schedules = DataManager::getInstance().getSchedulesByDate(selectedDate);
    
    movieTable->setRowCount(schedules.size());
    
    for (int i = 0; i < schedules.size(); ++i) {
        const auto& schedule = schedules[i];
        Movie* movie = DataManager::getInstance().findMovie(schedule.getMovieId());
        CinemaHall* hall = DataManager::getInstance().findHall(schedule.getHallId());
        
        if (movie && hall) {
            movieTable->setItem(i, 0, new QTableWidgetItem(movie->getTitle()));
            movieTable->setItem(i, 1, new QTableWidgetItem(hall->getName()));
            movieTable->setItem(i, 2, new QTableWidgetItem(schedule.getShowTime().toString("hh:mm")));
            movieTable->setItem(i, 3, new QTableWidgetItem(QString::number(schedule.getAvailableSeats())));
            
            QPushButton *bookBtn = new QPushButton("选座购票");
            bookBtn->setProperty("scheduleId", schedule.getScheduleId());
            // 修改连接，传递排片ID
            connect(bookBtn, &QPushButton::clicked, [this, scheduleId = schedule.getScheduleId()]() {
                this->showSeatSelection(scheduleId);
            });
            movieTable->setCellWidget(i, 4, bookBtn);
        }
    }
}

void MainWindow::purchaseTicket() {
    if (currentScheduleId == -1) {
        QMessageBox::warning(this, "购票失败", "请先选择排片");
        return;
    }
    
    // 查找所有选中的座位
    QList<QPair<int, int>> selectedSeats;
    for (QPushButton* seatBtn : seatButtons) {
        if (seatBtn->isChecked()) {
            int row = seatBtn->property("row").toInt();
            int col = seatBtn->property("col").toInt();
            selectedSeats.append(qMakePair(row, col));
        }
    }
    
    if (selectedSeats.isEmpty()) {
        QMessageBox::warning(this, "购票失败", "请先选择座位");
        return;
    }
    
    // 检查所有座位是否可用
    Schedule* schedule = DataManager::getInstance().findSchedule(currentScheduleId);
    if (!schedule) {
        QMessageBox::warning(this, "购票失败", "排片信息不存在");
        return;
    }
    
    const auto& seats = schedule->getSeats();
    for (const auto& seat : selectedSeats) {
        int row = seat.first;
        int col = seat.second;
        if (row >= seats.size() || col >= seats[row].size() || seats[row][col]) {
            QMessageBox::warning(this, "购票失败", 
                QString("座位第%1排第%2座已被购买").arg(row + 1).arg(col + 1));
            return;
        }
    }
    
    // 创建票务
    QString username = currentUser ? currentUser->getUsername() : "";
    if (username.isEmpty()) {
        QMessageBox::warning(this, "购票失败", "请先登录");
        return;
    }
    
    // 为每个选中的座位创建票务
    for (const auto& seat : selectedSeats) {
        int row = seat.first;
        int col = seat.second;
        
        Ticket ticket(DataManager::getInstance().getNextTicketId(), 
                     username, currentScheduleId, row, col, QDateTime::currentDateTime());
        
        DataManager::getInstance().addTicket(ticket);
    }
    
    // 计算总价
    Movie* movie = DataManager::getInstance().findMovie(schedule->getMovieId());
    double totalPrice = 0.0;
    if (movie) {
        totalPrice = movie->getPrice() * selectedSeats.size();
    }
    
    // 构建座位信息字符串
    QString seatInfo;
    for (const auto& seat : selectedSeats) {
        if (!seatInfo.isEmpty()) seatInfo += ", ";
        seatInfo += QString("第%1排第%2座").arg(seat.first + 1).arg(seat.second + 1);
    }
    
    QMessageBox::information(this, "购票成功", 
        QString("购票成功！\n座位: %1\n总价: %2元\n请按时观影").arg(seatInfo).arg(totalPrice));
    
    showMovieList();
}
