import  sqlite3


def all():
    cnx =sqlite3.connect('database/test')
    cur=cnx.cursor()
    cur.execute('create table user(login vchar(8),uid integer )')



