#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <vector>
#include "User.h"
#include "Movie.h"
#include "Schedule.h"
#include "CinemaHall.h"
#include "Ticket.h"

class DataManager {
public:
    static DataManager& getInstance();
    
    std::vector<std::pair<QString, double>> getBoxOfficeStats(const QDate& date) const;
    std::vector<std::pair<QString, double>> getTotalBoxOfficeStats() const;

    // ID生成方法
    int getNextMovieId();
    int getNextScheduleId();
    int getNextTicketId();
    
    // 当前用户管理
    User* getCurrentUser() const;
    void setCurrentUser(User* user);

    // 用户管理
    bool registerUser(const QString& username, const QString& password, UserRole role);
    User* loginUser(const QString& username, const QString& password);
    User* findUser(const QString& username);
    
    // 影片管理
    void addMovie(const Movie& movie);
    void updateMovie(const Movie& movie);
    void deleteMovie(int movieId);
    Movie* findMovie(int movieId);
    QVector<Movie> getAllMovies() const;
    
    // 排片管理
    void addSchedule(const Schedule& schedule);
    void updateSchedule(const Schedule& schedule);
    void deleteSchedule(int scheduleId);
    Schedule* findSchedule(int scheduleId);
    std::vector<Schedule> getSchedulesByDate(const QDate& date) const;
    QVector<Schedule> getAllSchedules() const;
    
    // 影厅管理
    void addHall(const CinemaHall& hall);
    CinemaHall* findHall(int hallId);
    QVector<CinemaHall> getAllHalls() const;
    
    // 售票管理
    void addTicket(const Ticket& ticket);
    void removeTicket(int ticketId);
    std::vector<Ticket> getTicketsByUser(const QString& username) const;
    std::vector<Ticket> getTicketsBySchedule(int scheduleId) const;
    
    // 数据持久性
    bool loadAllData();
    bool saveAllData();
    
private:
    DataManager() = default;
    
    QVector<User> users;    //用户的容器
    QVector<Movie> movies;  //影片的容器
    QVector<CinemaHall> halls;  //影厅的容器
    QVector<Schedule> schedules;    //排片的容器
    QVector<Ticket> tickets;    //票的容器
    
    User* currentUser = nullptr;
    
    int nextMovieId = 1;
    int nextScheduleId = 1;
    int nextTicketId = 1;
};

//序列化重载
QDataStream &operator<<(QDataStream &out, const User &user);  //序列化
QDataStream &operator>>(QDataStream &in, User &user);   //反序列化

QDataStream &operator<<(QDataStream &out, const Movie &movie);
QDataStream &operator>>(QDataStream &in, Movie &movie);

QDataStream &operator<<(QDataStream &out, const CinemaHall &hall);
QDataStream &operator>>(QDataStream &in, CinemaHall &hall);

QDataStream &operator<<(QDataStream &out, const Schedule &schedule);
QDataStream &operator>>(QDataStream &in, Schedule &schedule);

QDataStream &operator<<(QDataStream &out, const Ticket &ticket);
QDataStream &operator>>(QDataStream &in, Ticket &ticket);

#endif