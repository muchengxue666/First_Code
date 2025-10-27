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

    // ID生成方法
    int getNextMovieId();
    int getNextScheduleId();
    int getNextTicketId();
    
    // 当前用户管理
    User* getCurrentUser() const;
    void setCurrentUser(User* user);

    // User management
    bool registerUser(const QString& username, const QString& password, UserRole role);
    User* loginUser(const QString& username, const QString& password);
    User* findUser(const QString& username);
    
    // Movie management
    void addMovie(const Movie& movie);
    void updateMovie(const Movie& movie);
    void deleteMovie(int movieId);
    Movie* findMovie(int movieId);
    QVector<Movie> getAllMovies() const;
    
    // Schedule management
    void addSchedule(const Schedule& schedule);
    void updateSchedule(const Schedule& schedule);
    void deleteSchedule(int scheduleId);
    Schedule* findSchedule(int scheduleId);
    std::vector<Schedule> getSchedulesByDate(const QDate& date) const;
    QVector<Schedule> getAllSchedules() const;
    
    // Hall management
    void addHall(const CinemaHall& hall);
    CinemaHall* findHall(int hallId);
    QVector<CinemaHall> getAllHalls() const;
    
    // Ticket management
    void addTicket(const Ticket& ticket);
    void removeTicket(int ticketId);
    std::vector<Ticket> getTicketsByUser(const QString& username) const;
    std::vector<Ticket> getTicketsBySchedule(int scheduleId) const;
    
    // Data persistence
    bool loadAllData();
    bool saveAllData();
    
private:
    DataManager() = default;
    
    QVector<User> users;
    QVector<Movie> movies;
    QVector<CinemaHall> halls;
    QVector<Schedule> schedules;
    QVector<Ticket> tickets;
    
    User* currentUser = nullptr;
    
    int nextMovieId = 1;
    int nextScheduleId = 1;
    int nextTicketId = 1;
};

// Serialization operators for QDataStream
QDataStream &operator<<(QDataStream &out, const User &user);
QDataStream &operator>>(QDataStream &in, User &user);

QDataStream &operator<<(QDataStream &out, const Movie &movie);
QDataStream &operator>>(QDataStream &in, Movie &movie);

QDataStream &operator<<(QDataStream &out, const CinemaHall &hall);
QDataStream &operator>>(QDataStream &in, CinemaHall &hall);

QDataStream &operator<<(QDataStream &out, const Schedule &schedule);
QDataStream &operator>>(QDataStream &in, Schedule &schedule);

QDataStream &operator<<(QDataStream &out, const Ticket &ticket);
QDataStream &operator>>(QDataStream &in, Ticket &ticket);

#endif