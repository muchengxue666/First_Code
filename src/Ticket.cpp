#include "Ticket.h"

Ticket::Ticket() : ticketId(0), scheduleId(0), row(0), col(0) {}

Ticket::Ticket(int ticketId, const QString& username, int scheduleId, 
               int row, int col, const QDateTime& purchaseTime)
    : ticketId(ticketId), username(username), scheduleId(scheduleId),
      row(row), col(col), purchaseTime(purchaseTime) {}

//属性的获取
int Ticket::getTicketId() const { return ticketId; }
QString Ticket::getUsername() const { return username; }
int Ticket::getScheduleId() const { return scheduleId; }
int Ticket::getRow() const { return row; }
int Ticket::getCol() const { return col; }
QDateTime Ticket::getPurchaseTime() const { return purchaseTime; }