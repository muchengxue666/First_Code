#include "Schedule.h"

Schedule::Schedule() : scheduleId(0), movieId(0), hallId(0), boxOffice(0.0) {}

Schedule::Schedule(int scheduleId, int movieId, int hallId, const QDateTime& showTime)
    : scheduleId(scheduleId), movieId(movieId), hallId(hallId), 
      showTime(showTime), boxOffice(0.0) {}

int Schedule::getScheduleId() const { return scheduleId; }
int Schedule::getMovieId() const { return movieId; }
int Schedule::getHallId() const { return hallId; }
QDateTime Schedule::getShowTime() const { return showTime; }

std::vector<std::vector<bool>>& Schedule::getSeats() { return seats; }
const std::vector<std::vector<bool>>& Schedule::getSeats() const { return seats; }

int Schedule::getAvailableSeats() const {
    int available = 0;
    for (const auto& row : seats) {
        for (bool occupied : row) {
            if (!occupied) available++;
        }
    }
    return available;
}

double Schedule::getBoxOffice() const { return boxOffice; }

void Schedule::setMovieId(int movieId) {
    this->movieId = movieId;
}

void Schedule::setHallId(int hallId) {
    this->hallId = hallId;
}

void Schedule::setShowTime(const QDateTime& showTime) {
    this->showTime = showTime;
}

bool Schedule::bookSeat(int row, int col) {
    if (row < 0 || row >= seats.size() || col < 0 || col >= seats[0].size()) {
        return false;
    }
    if (seats[row][col]) {
        return false; // Seat already occupied
    }
    seats[row][col] = true;
    return true;
}

bool Schedule::cancelSeat(int row, int col) {
    if (row < 0 || row >= seats.size() || col < 0 || col >= seats[0].size()) {
        return false;
    }
    if (!seats[row][col]) {
        return false; // Seat not occupied
    }
    seats[row][col] = false;
    return true;
}

void Schedule::setSeatLayout(int rows, int cols) {
    seats.resize(rows, std::vector<bool>(cols, false));
}

void Schedule::addBoxOffice(double amount) {
    boxOffice += amount;
}