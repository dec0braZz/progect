#include "bazza.h"
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlQuery"

Bazza::Bazza()
{
bazza= QSqlDatabase::addDatabase("SQLLT");
bazza.setDatabaseName("DATABAZZA.Cheburashka_v_tapke");
bazza.setUserName(Username);
bazza.setPassword(Password);
}
