#include "DataManager.h"
#include <QFile>
#include <QDataStream>
#include <QDir>

DataManager& DataManager::getInstance() {
    static DataManager instance;
    return instance;
}

// User management implementations
bool DataManager::registerUser(const QString& username, const QString& password, UserRole role) {
    if (findUser(username)) {
        return false; // User already exists
    }
    users.emplace_back(username, password, role);
    saveAllData();
    return true;
}

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


// Movie management implementations
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

void DataManager::deleteMovie(int movieId) {
    movies.erase(
        std::remove_if(movies.begin(), movies.end(),
                      [movieId](const Movie& m) { return m.getId() == movieId; }),
        movies.end()
    );
    saveAllData();
}

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

// Schedule management implementations
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
    // First remove all tickets for this schedule
    tickets.erase(
        std::remove_if(tickets.begin(), tickets.end(),
                      [scheduleId](const Ticket& t) { return t.getScheduleId() == scheduleId; }),
        tickets.end()
    );
    
    // Then remove the schedule
    schedules.erase(
        std::remove_if(schedules.begin(), schedules.end(),
                      [scheduleId](const Schedule& s) { return s.getScheduleId() == scheduleId; }),
        schedules.end()
    );
    saveAllData();
}

Schedule* DataManager::findSchedule(int scheduleId) {
    for (auto& schedule : schedules) {
        if (schedule.getScheduleId() == scheduleId) {
            return &schedule;
        }
    }
    return nullptr;
}

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

// Hall management implementations
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

// Ticket management implementations
void DataManager::addTicket(const Ticket& ticket) {
    tickets.push_back(ticket);
    if (ticket.getTicketId() >= nextTicketId) {
        nextTicketId = ticket.getTicketId() + 1;
    }
    
    // Update schedule box office AND mark seat as occupied
    Schedule* schedule = findSchedule(ticket.getScheduleId());
    if (schedule) {
        Movie* movie = findMovie(schedule->getMovieId());
        if (movie) {
            schedule->addBoxOffice(movie->getPrice());
        }
        // 关键修复：标记座位为已售
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
        // Adjust box office
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

// Data persistence implementations
bool DataManager::loadAllData() {
    // Create data directory if it doesn't exist
    QDir dir;
    if (!dir.exists("data")) {
        dir.mkdir("data");
    }
    
    bool success = true;
    
    // Load users
    QFile userFile("data/users.dat");
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
            //saveAllData();  // 立即保存
        }
    } else {
        qDebug() << "No user file found, creating default admin";
        // 创建默认管理员用户如果文件不存在
        users.emplace_back("muchengxue", "666", UserRole::ADMIN);
        //saveAllData();  // 立即保存
        success = false;
    }
    
    // Load movies
    QFile movieFile("data/movies.dat");
    if (movieFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&movieFile);
        in >> movies;
        movieFile.close();
        
        // Find the next movie ID
        for (const auto& movie : movies) {
            if (movie.getId() >= nextMovieId) {
                nextMovieId = movie.getId() + 1;
            }
        }
    } else {
        // Create some sample movies
        movies.emplace_back(nextMovieId++, "流浪地球", "郭帆", QTime(2, 5, 0), "科幻", 45.0);
        movies.emplace_back(nextMovieId++, "热辣滚烫", "贾玲", QTime(2, 10, 0), "喜剧", 40.0);
        movies.emplace_back(nextMovieId++, "第二十条", "张艺谋", QTime(2, 15, 0), "剧情", 42.0);
        success = false;
    }
    
    // Load halls
    QFile hallFile("data/halls.dat");
    if (hallFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&hallFile);
        in >> halls;
        hallFile.close();
    } else {
        // Create some sample halls
        halls.emplace_back(1, "1号厅", 8, 10);
        halls.emplace_back(2, "2号厅", 6, 8);
        halls.emplace_back(3, "VIP厅", 4, 6);
        success = false;
    }
    
    // Load schedules
    QFile scheduleFile("data/schedules.dat");
    if (scheduleFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&scheduleFile);
        in >> schedules;
        scheduleFile.close();
        
        // Find the next schedule ID and set up seat layouts
        for (auto& schedule : schedules) {
            if (schedule.getScheduleId() >= nextScheduleId) {
                nextScheduleId = schedule.getScheduleId() + 1;
            }
            
            // Ensure seat layout is set up
            CinemaHall* hall = findHall(schedule.getHallId());
            if (hall && schedule.getSeats().empty()) {
                schedule.setSeatLayout(hall->getRows(), hall->getCols());
            }
        }
    } else {
        // Create some sample schedules for tomorrow
        QDate tomorrow = QDate::currentDate().addDays(1);
        schedules.emplace_back(nextScheduleId++, 1, 1, QDateTime(tomorrow, QTime(10, 0, 0)));
        schedules.emplace_back(nextScheduleId++, 2, 2, QDateTime(tomorrow, QTime(13, 30, 0)));
        schedules.emplace_back(nextScheduleId++, 3, 3, QDateTime(tomorrow, QTime(16, 0, 0)));
        schedules.emplace_back(nextScheduleId++, 1, 1, QDateTime(tomorrow, QTime(19, 0, 0)));
        
        // Set up seat layouts for new schedules
        for (auto& schedule : schedules) {
            CinemaHall* hall = findHall(schedule.getHallId());
            if (hall) {
                schedule.setSeatLayout(hall->getRows(), hall->getCols());
            }
        }
        success = false;
    }
    
    // Load tickets
    QFile ticketFile("data/tickets.dat");
    if (ticketFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&ticketFile);
        in >> tickets;
        ticketFile.close();
        
        // Find the next ticket ID
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

bool DataManager::saveAllData() {
    // Create data directory if it doesn't exist
    QDir dir;
    if (!dir.exists("data")) {
        qDebug() << "Creating data directory for saving";
        dir.mkdir("data");
    }
    
    bool success = true;
    
    // Save users
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
    
    // Save movies
    QFile movieFile("data/movies.dat");
    if (movieFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&movieFile);
        out << movies;
        movieFile.close();
    } else {
        success = false;
    }
    
    // Save halls
    QFile hallFile("data/halls.dat");
    if (hallFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&hallFile);
        out << halls;
        hallFile.close();
    } else {
        success = false;
    }
    
    // Save schedules
    QFile scheduleFile("data/schedules.dat");
    if (scheduleFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&scheduleFile);
        out << schedules;
        scheduleFile.close();
    } else {
        success = false;
    }
    
    // Save tickets
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

// Helper function to get box office statistics
std::vector<std::pair<QString, double>> DataManager::getBoxOfficeStats(const QDate& date) const {
    std::vector<std::pair<QString, double>> stats;
    
    // Map to accumulate box office by movie title
    std::map<QString, double> movieBoxOffice;
    
    for (const auto& schedule : schedules) {
        if (schedule.getShowTime().date() == date && schedule.getBoxOffice() > 0) {
            Movie* movie = const_cast<DataManager*>(this)->findMovie(schedule.getMovieId());
            if (movie) {
                movieBoxOffice[movie->getTitle()] += schedule.getBoxOffice();
            }
        }
    }
    
    // Convert to vector and sort by box office (descending)
    for (const auto& [title, revenue] : movieBoxOffice) {
        stats.emplace_back(title, revenue);
    }
    
    std::sort(stats.begin(), stats.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    return stats;
}

// Get next available IDs
int DataManager::getNextMovieId() { return nextMovieId++; }
int DataManager::getNextScheduleId() { return nextScheduleId++; }
int DataManager::getNextTicketId() { return nextTicketId++; }

// Get current user
User* DataManager::getCurrentUser() const { return currentUser; }
void DataManager::setCurrentUser(User* user) { currentUser = user; }

// Serialization implementations
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

QDataStream &operator<<(QDataStream &out, const Schedule &schedule) {
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
    
    // Use a const_cast to set the seats (not ideal but works for this case)
    const_cast<std::vector<std::vector<bool>>&>(schedule.getSeats()) = seats;
    
    // Set box office
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