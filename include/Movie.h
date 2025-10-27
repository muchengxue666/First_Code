#ifndef MOVIE_H
#define MOVIE_H

#include <QString>
#include <QTime>

class Movie {
public:
    Movie();
    Movie(int id, const QString& title, const QString& director, 
          const QTime& duration, const QString& genre, double price);
    
    int getId() const;
    QString getTitle() const;
    QString getDirector() const;
    QTime getDuration() const;
    QString getGenre() const;
    double getPrice() const;
    
    void setTitle(const QString& title);
    void setPrice(double price);
    
private:
    int id;
    QString title;
    QString director;
    QTime duration;
    QString genre;
    double price;
};

#endif