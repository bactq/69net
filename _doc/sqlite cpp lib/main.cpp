#include "Sqlite.h"
#include <iostream>

int main()
{
    Sqlite::Connection conn( "C:\\erere.db" );
    conn.open();
    conn.execute( "DROP TABLE IF EXISTS 'ttt'" );
    conn.execute( R"--(CREATE TABLE "ttt" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
"name" TEXT NOT NULL, 
"double" REAL
);)--" );

    conn.beginTranslation();
    auto& q = *conn.newQuery( "INSERT INTO `ttt` ( `name`, `double` ) VALUES ( ?, ? )" );
    q << "eeeeeee" << 123.4;
    q.execute();
    q << "qwer" << 11.0;
    q.execute();
    q << "zxcv" << ( char const* )nullptr;
    q.execute();
    conn.commit();

    q.assign( "SELECT `id`,`name`,`double` FROM `ttt`" );
    q.execute( []( Sqlite::DataReader& dr )
    {
        std::cout << dr.readInt32() << ", " << dr.readString() << ", " << dr.readDouble() << std::endl;
    } );

    conn.close();


    return 0;
}
