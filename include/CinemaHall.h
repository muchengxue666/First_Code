#ifndef CINEMAHALL_H
#define CINEMAHALL_H

#include <QString>
#include <vector>

class CinemaHall {
public:
    CinemaHall();
    CinemaHall(int hallId, const QString& name, int rows, int cols);
    
    int getHallId() const;
    QString getName() const;
    int getRows() const;
    int getCols() const;
    int getTotalSeats() const;
    
    void setName(const QString& name);
    
private:
    int hallId;
    QString name;
    int rows;
    int cols;
};

#endif