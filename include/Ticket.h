#ifndef TICKET_H
#define TICKET_H

#include <QString>
#include <QDateTime>

class Ticket {
public:
    Ticket();
    Ticket(int ticketId, const QString& username, int scheduleId, 
           int row, int col, const QDateTime& purchaseTime);
    
    int getTicketId() const;
    QString getUsername() const;
    int getScheduleId() const;
    int getRow() const;
    int getCol() const;
    QDateTime getPurchaseTime() const;
    
private:
    int ticketId;
    QString username;
    int scheduleId;
    int row;
    int col;
    QDateTime purchaseTime;
};

#endif