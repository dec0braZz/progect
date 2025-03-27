#ifndef BAZZA_H
#define BAZZA_H
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlQuery"

class Bazza
{
public:
    Bazza();
private: //вход в баззу данных
    QString Username;
    QString Password;

private:
    QSqlDatabase bazza;//база
    QSqlQuery *query;//библиотека
};

#endif // BAZZA_H
