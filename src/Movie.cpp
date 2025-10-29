#include "Movie.h"

Movie::Movie() : id(0), title(""), director(""), genre(""), price(0.0) {}

Movie::Movie(int id, const QString& title, const QString& director, 
             const QTime& duration, const QString& genre, double price)
    : id(id), title(title), director(director), duration(duration), 
      genre(genre), price(price) {}

//属性的获取
int Movie::getId() const { return id; }
QString Movie::getTitle() const { return title; }
QString Movie::getDirector() const { return director; }
QTime Movie::getDuration() const { return duration; }
QString Movie::getGenre() const { return genre; }
double Movie::getPrice() const { return price; }

void Movie::setTitle(const QString& title) { this->title = title; }
void Movie::setPrice(double price) { this->price = price; }