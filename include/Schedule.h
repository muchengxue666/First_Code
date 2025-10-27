#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QDateTime>
#include <vector>
#include "Movie.h"
#include "CinemaHall.h"

class Schedule {
public:
    Schedule();
    Schedule(int scheduleId, int movieId, int hallId, const QDateTime& showTime);
    
    int getScheduleId() const;
    int getMovieId() const;
    int getHallId() const;
    QDateTime getShowTime() const;
    
    std::vector<std::vector<bool>>& getSeats();
    const std::vector<std::vector<bool>>& getSeats() const;
    int getAvailableSeats() const;
    double getBoxOffice() const;
    
    void setShowTime(const QDateTime& showTime);
    bool bookSeat(int row, int col);
    bool cancelSeat(int row, int col);
    void setSeatLayout(int rows, int cols);
    void addBoxOffice(double amount);
    
private:
    int scheduleId;
    int movieId;
    int hallId;
    QDateTime showTime;
    std::vector<std::vector<bool>> seats; // true = occupied, false = available
    double boxOffice;
};

#endif