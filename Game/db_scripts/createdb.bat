mysql -uroot -p123456 < rebuilddb.sql
mysql -uroot -p123456 ll_base < db.sql --default-character-set=utf8
mysql -uroot -p123456 ll_login < login.sql --default-character-set=utf8
mysql -uroot -p123456 ll_gmdb < gmdb.sql --default-character-set=utf8
mysql -uroot -p123456 ll_log < dblog.sql --default-character-set=utf8

@rem 创建测试账号
mysql -uroot -p123456 ll_login < logindata.sql --default-character-set=utf8