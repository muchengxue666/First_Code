#include "DataManager.h"
#include <QFile>
#include <QDataStream>
#include <QDir>

DataManager& DataManager::getInstance() {
    static DataManager instance;
    return instance;
}

// 用户注册方法
bool DataManager::registerUser(const QString& username, const QString& password, UserRole role) {
    if (findUser(username)) {
        return false; // 用户已经存在
    }
    users.emplace_back(username, password, role);
    saveAllData();
    return true;
}

//用户登录函数，返回用户实例，失败返回空
User* DataManager::loginUser(const QString& username, const QString& password) {
    // 强制重新加载数据以确保数据是最新的
    // loadAllData();
    
    for (auto& user : users) {
        if (user.getUsername() == username && user.validatePassword(password)) {
            currentUser = &user;
            qDebug() << "Login successful for user:" << username;
            return &user;
        }
    }
    qDebug() << "Login failed for user:" << username;
    return nullptr;
}

User* DataManager::findUser(const QString& username) {
    for (auto& user : users) {
        if (user.getUsername() == username) {
            return &user;
        }
    }
    return nullptr;
}


// 影片添加
void DataManager::addMovie(const Movie& movie) {
    movies.push_back(movie);
    if (movie.getId() >= nextMovieId) {
        nextMovieId = movie.getId() + 1;
    }
    saveAllData();
}

void DataManager::updateMovie(const Movie& movie) {
    for (auto& m : movies) {
        if (m.getId() == movie.getId()) {
            m = movie;
            saveAllData();
            return;
        }
    }
}
//删除影片
void DataManager::deleteMovie(int movieId) {
    movies.erase(
        std::remove_if(movies.begin(), movies.end(),
                      [movieId](const Movie& m) { return m.getId() == movieId; }),
        movies.end()
    );
    saveAllData();
}
//查找影片
Movie* DataManager::findMovie(int movieId) {
    for (auto& movie : movies) {
        if (movie.getId() == movieId) {
            return &movie;
        }
    }
    return nullptr;
}

QVector<Movie> DataManager::getAllMovies() const {
    return movies;
}

// 排片添加
void DataManager::addSchedule(const Schedule& schedule) {
    schedules.push_back(schedule);
    if (schedule.getScheduleId() >= nextScheduleId) {
        nextScheduleId = schedule.getScheduleId() + 1;
    }
    saveAllData();
}

void DataManager::updateSchedule(const Schedule& schedule) {
    for (auto& s : schedules) {
        if (s.getScheduleId() == schedule.getScheduleId()) {
            s = schedule;
            saveAllData();
            return;
        }
    }
}

void DataManager::deleteSchedule(int scheduleId) {
    // 首先删除票的信息
    tickets.erase(
        std::remove_if(tickets.begin(), tickets.end(),
                      [scheduleId](const Ticket& t) { return t.getScheduleId() == scheduleId; }),
        tickets.end()
    );
    
    // 然后删除排片
    schedules.erase(
        std::remove_if(schedules.begin(), schedules.end(),
                      [scheduleId](const Schedule& s) { return s.getScheduleId() == scheduleId; }),
        schedules.end()
    );
    saveAllData();
}

//根据id获取排片
Schedule* DataManager::findSchedule(int scheduleId) {
    for (auto& schedule : schedules) {
        if (schedule.getScheduleId() == scheduleId) {
            return &schedule;
        }
    }
    return nullptr;
}

//根据日期获取排片
std::vector<Schedule> DataManager::getSchedulesByDate(const QDate& date) const {
    std::vector<Schedule> result;
    for (const auto& schedule : schedules) {
        if (schedule.getShowTime().date() == date) {
            result.push_back(schedule);
        }
    }
    return result;
}

QVector<Schedule> DataManager::getAllSchedules() const {
    return schedules;
}

// 添加影厅
void DataManager::addHall(const CinemaHall& hall) {
    halls.push_back(hall);
    saveAllData();
}

CinemaHall* DataManager::findHall(int hallId) {
    for (auto& hall : halls) {
        if (hall.getHallId() == hallId) {
            return &hall;
        }
    }
    return nullptr;
}

QVector<CinemaHall> DataManager::getAllHalls() const {
    return halls;
}

// 添加购票信息
void DataManager::addTicket(const Ticket& ticket) {
    tickets.push_back(ticket);
    if (ticket.getTicketId() >= nextTicketId) {
        nextTicketId = ticket.getTicketId() + 1;
    }
    
    // 更新票房并且标记座位为已购票
    Schedule* schedule = findSchedule(ticket.getScheduleId());
    if (schedule) {
        Movie* movie = findMovie(schedule->getMovieId());
        if (movie) {
            schedule->addBoxOffice(movie->getPrice());
        }
        // 标记座位为已售
        schedule->bookSeat(ticket.getRow(), ticket.getCol());
    }
    saveAllData();
}

void DataManager::removeTicket(int ticketId) {
    // Find the ticket first to get schedule info for box office adjustment
    Ticket* ticketToRemove = nullptr;
    for (const auto& ticket : tickets) {
        if (ticket.getTicketId() == ticketId) {
            ticketToRemove = const_cast<Ticket*>(&ticket);
            break;
        }
    }
    
    if (ticketToRemove) {
        // 修改票房信息
        Schedule* schedule = findSchedule(ticketToRemove->getScheduleId());
        if (schedule) {
            Movie* movie = findMovie(schedule->getMovieId());
            if (movie) {
                schedule->addBoxOffice(-movie->getPrice());
            }
            // Free up the seat
            schedule->cancelSeat(ticketToRemove->getRow(), ticketToRemove->getCol());
        }
    }
    
    // Remove the ticket
    tickets.erase(
        std::remove_if(tickets.begin(), tickets.end(),
                      [ticketId](const Ticket& t) { return t.getTicketId() == ticketId; }),
        tickets.end()
    );
    saveAllData();
}

std::vector<Ticket> DataManager::getTicketsByUser(const QString& username) const {
    std::vector<Ticket> result;
    for (const auto& ticket : tickets) {
        if (ticket.getUsername() == username) {
            result.push_back(ticket);
        }
    }
    return result;
}

std::vector<Ticket> DataManager::getTicketsBySchedule(int scheduleId) const {
    std::vector<Ticket> result;
    for (const auto& ticket : tickets) {
        if (ticket.getScheduleId() == scheduleId) {
            result.push_back(ticket);
        }
    }
    return result;
}

//加载数据文件的方法实现
bool DataManager::loadAllData() {
    // 如果没有就创建data文件夹
    QDir dir;
    if (!dir.exists("data")) {
        dir.mkdir("data");
    }
    
    bool success = true;
    
    //加载User
    QFile userFile("data/users.dat");   //创建文件实例
    if (userFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Loading users from file";
        QDataStream in(&userFile);
        int userCount;
        in >> userCount;
        users.clear();
        for (int i = 0; i < userCount; ++i) {
            User user;
            in >> user;
            users.push_back(user);
            qDebug() << "Loaded user:" << user.getUsername();
        }
        userFile.close();
        
        // 检查是否加载了管理员账户
        bool hasAdmin = false;
        for (const auto& user : users) {
            if (user.getUsername() == "muchengxue" && user.getRole() == UserRole::ADMIN) {
                hasAdmin = true;
                break;
            }
        }
        
        // 如果没有管理员账户，创建默认管理员
        if (!hasAdmin) {
            qDebug() << "No admin user found, creating default admin";
            users.emplace_back("muchengxue", "666", UserRole::ADMIN);
            //saveAllData();  
        }
    } else {
        qDebug() << "No user file found, creating default admin";
        // 创建默认管理员用户如果文件不存在
        users.emplace_back("muchengxue", "666", UserRole::ADMIN);
        //saveAllData(); 
        success = false;
    }
    
    // 加载电影数据
    QFile movieFile("data/movies.dat");
    if (movieFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&movieFile);
        in >> movies;
        movieFile.close();
        
        // 寻找下一个Id
        for (const auto& movie : movies) {
            if (movie.getId() >= nextMovieId) {
                nextMovieId = movie.getId() + 1;
            }
        }
    } else {
        //创建默认电影
        movies.emplace_back(nextMovieId++, "流浪地球", "郭帆", QTime(2, 5, 0), "科幻", 45.0);
        movies.emplace_back(nextMovieId++, "热辣滚烫", "贾玲", QTime(2, 10, 0), "喜剧", 40.0);
        movies.emplace_back(nextMovieId++, "第二十条", "张艺谋", QTime(2, 15, 0), "剧情", 42.0);
        success = false;
    }
    
    // 加载影厅数据
    QFile hallFile("data/halls.dat");
    if (hallFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&hallFile);
        in >> halls;
        hallFile.close();
    } else {
        // 创建默认影厅数据
        halls.emplace_back(1, "1号厅", 8, 10);
        halls.emplace_back(2, "2号厅", 6, 8);
        halls.emplace_back(3, "3号厅", 10, 12);  // 添加3号厅
        halls.emplace_back(4, "4号厅", 8, 8);    // 添加4号厅
        halls.emplace_back(5, "5号厅", 6, 10);   // 添加5号厅
        success = false;
    }
    
    // 加载排片信息
    QFile scheduleFile("data/schedules.dat");
    if (scheduleFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&scheduleFile);
        in >> schedules;
        scheduleFile.close();
        
        // 查找下一个排片信息并设置座位布局
        for (auto& schedule : schedules) {
            if (schedule.getScheduleId() >= nextScheduleId) {
                nextScheduleId = schedule.getScheduleId() + 1;  //自增ID生成
            }
            
            // 确保座位布局被设置
            CinemaHall* hall = findHall(schedule.getHallId());
            if (hall && schedule.getSeats().empty()) {
                schedule.setSeatLayout(hall->getRows(), hall->getCols());
            }
        }
    } else {
        // 创建默认影片
        QDate tomorrow = QDate::currentDate().addDays(1);
        schedules.emplace_back(nextScheduleId++, 1, 1, QDateTime(tomorrow, QTime(10, 0, 0)));
        schedules.emplace_back(nextScheduleId++, 2, 2, QDateTime(tomorrow, QTime(13, 30, 0)));
        schedules.emplace_back(nextScheduleId++, 3, 3, QDateTime(tomorrow, QTime(16, 0, 0)));  // 改为3号厅
        
        // 给新加的排片增加布局
        for (auto& schedule : schedules) {
            CinemaHall* hall = findHall(schedule.getHallId());
            if (hall) {
                schedule.setSeatLayout(hall->getRows(), hall->getCols());
            }
        }
        success = false;
    }
    
    // 加载票的信息
    QFile ticketFile("data/tickets.dat");
    if (ticketFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&ticketFile);
        in >> tickets;
        ticketFile.close();
        
        // 查找下一个ID
        for (const auto& ticket : tickets) {
            if (ticket.getTicketId() >= nextTicketId) {
                nextTicketId = ticket.getTicketId() + 1;
            }
        }
    }
    qDebug() << "Total users loaded:" << users.size();
    if (!success) {
    // 如果有任何数据文件不存在（即创建了示例数据），则保存所有数据
    saveAllData();}
    return success;
    
}

//保存所有数据
bool DataManager::saveAllData() {
    //创建文件夹
    QDir dir;
    if (!dir.exists("data")) {
        qDebug() << "Creating data directory for saving";
        dir.mkdir("data");
    }
    
    bool success = true;
    
    // 保存Users
    QFile userFile("data/users.dat");
    if (userFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Saving users to file, count:" << users.size();
        QDataStream out(&userFile);
        out << static_cast<int>(users.size());
        for (const auto& user : users) {
            out << user;
            qDebug() << "Saved user:" << user.getUsername();
        }
        userFile.close();
    } else {
        qDebug() << "Failed to open user file for writing:" << userFile.errorString();
        success = false;
    }
    
    // 保存影片
    QFile movieFile("data/movies.dat");
    if (movieFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&movieFile);
        out << movies;
        movieFile.close();
    } else {
        success = false;
    }
    
    // 保存影厅
    QFile hallFile("data/halls.dat");
    if (hallFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&hallFile);
        out << halls;
        hallFile.close();
    } else {
        success = false;
    }
    
    // 保存排片
    QFile scheduleFile("data/schedules.dat");
    if (scheduleFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&scheduleFile);
        out << schedules;
        scheduleFile.close();
    } else {
        success = false;
    }
    
    // 保存票的信息
    QFile ticketFile("data/tickets.dat");
    if (ticketFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&ticketFile);
        out << tickets;
        ticketFile.close();
    } else {
        success = false;
    }
    
    return success;
}

// 获取票房信息(已失用)
std::vector<std::pair<QString, double>> DataManager::getBoxOfficeStats(const QDate& date) const {
    std::vector<std::pair<QString, double>> stats;
    
    std::map<QString, double> movieBoxOffice;
    
    for (const auto& schedule : schedules) {
        if (schedule.getShowTime().date() == date && schedule.getBoxOffice() > 0) {
            Movie* movie = const_cast<DataManager*>(this)->findMovie(schedule.getMovieId());
            if (movie) {
                movieBoxOffice[movie->getTitle()] += schedule.getBoxOffice();
            }
        }
    }
    
    for (const auto& [title, revenue] : movieBoxOffice) {
        stats.emplace_back(title, revenue);
    }
    
    std::sort(stats.begin(), stats.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    return stats;
}

// 获取电影总票房统计
std::vector<std::pair<QString, double>> DataManager::getTotalBoxOfficeStats() const {
    std::vector<std::pair<QString, double>> stats;
    
    // map容器计算影片名字下累计票房
    std::map<QString, double> movieBoxOffice;
    
    // 遍历所有排片，累加每部电影的总票房
    for (const auto& schedule : schedules) {
        if (schedule.getBoxOffice() > 0) {
            Movie* movie = const_cast<DataManager*>(this)->findMovie(schedule.getMovieId());    //类型转换
            if (movie) {
                movieBoxOffice[movie->getTitle()] += schedule.getBoxOffice();
            }
        }
    }
    
    // 转换为向量并按票房降序排序
    for (const auto& [title, revenue] : movieBoxOffice) {
        stats.emplace_back(title, revenue);
    }
    
    std::sort(stats.begin(), stats.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    return stats;
}

// 获取自增id值
int DataManager::getNextMovieId() { return nextMovieId++; }
int DataManager::getNextScheduleId() { return nextScheduleId++; }
int DataManager::getNextTicketId() { return nextTicketId++; }

// 获取当前用户
User* DataManager::getCurrentUser() const { return currentUser; }
void DataManager::setCurrentUser(User* user) { currentUser = user; }

// 序列化重载，以下
QDataStream &operator<<(QDataStream &out, const User &user) {
    out << user.getUsername() << user.getPassword() << static_cast<int>(user.getRole());
    return out;
}

QDataStream &operator>>(QDataStream &in, User &user) {
    QString username, password;
    int role;
    in >> username >> password >> role;
    user = User(username, password, static_cast<UserRole>(role));
    return in;
}

QDataStream &operator<<(QDataStream &out, const Movie &movie) {
    out << movie.getId() << movie.getTitle() << movie.getDirector() 
        << movie.getDuration() << movie.getGenre() << movie.getPrice();
    return out;
}

QDataStream &operator>>(QDataStream &in, Movie &movie) {
    int id;
    QString title, director, genre;
    QTime duration;
    double price;
    in >> id >> title >> director >> duration >> genre >> price;
    movie = Movie(id, title, director, duration, genre, price);
    return in;
}

QDataStream &operator<<(QDataStream &out, const CinemaHall &hall) {
    out << hall.getHallId() << hall.getName() << hall.getRows() << hall.getCols();
    return out;
}

QDataStream &operator>>(QDataStream &in, CinemaHall &hall) {
    int hallId, rows, cols;
    QString name;
    in >> hallId >> name >> rows >> cols;
    hall = CinemaHall(hallId, name, rows, cols);
    return in;
}

QDataStream <DataManager*>(this)<<(QDataStream &out, const Schedule &schedule) {
    out << schedule.getScheduleId() << schedule.getMovieId() << schedule.getHallId() 
        << schedule.getShowTime() << schedule.getBoxOffice();
    
    // Serialize seats
    const auto& seats = schedule.getSeats();
    out << static_cast<int>(seats.size());
    for (const auto& row : seats) {
        out << static_cast<int>(row.size());
        for (bool occupied : row) {
            out << occupied;
        }
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, Schedule &schedule) {
    int scheduleId, movieId, hallId;
    QDateTime showTime;
    double boxOffice;
    in >> scheduleId >> movieId >> hallId >> showTime >> boxOffice;
    
    schedule = Schedule(scheduleId, movieId, hallId, showTime);
    
    // Deserialize seats
    int rows, cols;
    in >> rows;
    std::vector<std::vector<bool>> seats(rows);
    for (int i = 0; i < rows; ++i) {
        in >> cols;
        seats[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            bool occupied;
            in >> occupied;
            seats[i][j] = occupied;
        }
    }
    
    //使用const_cast修改seats，合法
    const_cast<std::vector<std::vector<bool>>&>(schedule.getSeats()) = seats;
    
    // 修改票房
    const_cast<Schedule&>(schedule).addBoxOffice(boxOffice);
    
    return in;
}

QDataStream &operator<<(QDataStream &out, const Ticket &ticket) {
    out << ticket.getTicketId() << ticket.getUsername() << ticket.getScheduleId() 
        << ticket.getRow() << ticket.getCol() << ticket.getPurchaseTime();
    return out;
}

QDataStream &operator>>(QDataStream &in, Ticket &ticket) {
    int ticketId, scheduleId, row, col;
    QString username;
    QDateTime purchaseTime;
    in >> ticketId >> username >> scheduleId >> row >> col >> purchaseTime;
    ticket = Ticket(ticketId, username, scheduleId, row, col, purchaseTime);
    return in;
}