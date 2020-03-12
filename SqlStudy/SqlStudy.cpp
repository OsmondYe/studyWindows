// SqlStudy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "sqlite3/sqlite3.h"


using namespace std;

template<typename T>
inline void println(T t,bool newline=true) {
    if (newline) {
        cout << t << endl;
    }
    else {
        cout << t << ":                ";
    }
}

#define pln(x) println((#x),false);println(x);



sqlite3* db = NULL;


void create_db_table_here();

int main()
{
    sqlite3_initialize();
    sqlite3_os_init();

    pln(SQLITE_VERSION);
    pln(SQLITE_VERSION_NUMBER);
    pln(SQLITE_SOURCE_ID);
    //
    pln(sqlite3_libversion());
    pln(sqlite3_sourceid());
    pln(sqlite3_libversion_number());
    pln(sqlite3_threadsafe());


    
    pln(sqlite3_open("C:\\OyeOutput\\myDb.db", &db));  // will call sqlite3_initialize internal

    create_db_table_here();

    pln(sqlite3_close(db));

}


void create_db_table_here() {

    const char* sql_create = R"rs(
        create table if not exist user(
            id  integer not null primary key autoincrement unique,
            name  text,
            school  text,
            );
    )rs";
    char* emsg=NULL;
    if (sqlite3_exec(db, sql_create, NULL, NULL, &emsg)) {
        println(emsg);
        sqlite3_free(emsg);
    }

}