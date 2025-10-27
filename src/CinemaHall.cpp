#include "CinemaHall.h"

CinemaHall::CinemaHall() : hallId(0), name(""), rows(0), cols(0) {}

CinemaHall::CinemaHall(int hallId, const QString& name, int rows, int cols)
    : hallId(hallId), name(name), rows(rows), cols(cols) {}

int CinemaHall::getHallId() const { return hallId; }
QString CinemaHall::getName() const { return name; }
int CinemaHall::getRows() const { return rows; }
int CinemaHall::getCols() const { return cols; }
int CinemaHall::getTotalSeats() const { return rows * cols; }

void CinemaHall::setName(const QString& name) { this->name = name; }