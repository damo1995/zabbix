## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	src \
	create \
	misc \
	upgrades

EXTRA_DIST = \
	bin \
	build \
	docs \
	FAQ \
	frontends \
	include \
	test

## "dist-hook" run after the distribution directory is filled, but before the actual tar (or shar) file is created.
dist-hook:
	rm -rf `find $(top_distdir) -name ".svn"`
	rm -f $(top_distdir)/frontends/php/conf/zabbix.conf.php
	rm -f $(top_distdir)/do
	rm -f $(top_distdir)/go
	rm -f $(top_distdir)/splint
	rm -rf $(top_distdir)/test
	rm -f $(top_distdir)/TODO
##TODO  makdir $(top_distdir)/docs
##TODO  cp ?????/ZABBIX Manual v1.4.pdf $(top_distdir)/docs/

dbschema:
	create/schema/gen.pl mysql > create/schema/mysql.sql
	create/schema/gen.pl postgresql > create/schema/postgresql.sql
	create/schema/gen.pl sqlite > create/schema/sqlite.sql
	create/schema/gen.pl oracle > create/schema/oracle.sql
	create/schema/gen.pl c >src/libs/zbxdbhigh/dbschema.c
	echo -e "\n#ifdef HAVE_MYSQL\nconst char *db_schema= {\"\\" >>src/libs/zbxdbhigh/dbschema.c
	create/schema/gen.pl mysql|sed -e 's/$$/\\/' >>src/libs/zbxdbhigh/dbschema.c
	echo -e "\"};\n#elif HAVE_POSTGRESQL\nconst char *db_schema = {\"\\" >>src/libs/zbxdbhigh/dbschema.c
	create/schema/gen.pl postgresql|sed -e 's/\t\t*/ /g' -e 's/$$/\\/' >>src/libs/zbxdbhigh/dbschema.c
	echo -e "\"};\n#elif HAVE_ORACLE\nconst char *db_schema = {\"\\" >>src/libs/zbxdbhigh/dbschema.c
	create/schema/gen.pl oracle|sed -e 's/\t\t*/ /g' -e 's/$$/\\/' >>src/libs/zbxdbhigh/dbschema.c
	echo -e "\"};\n#elif HAVE_SQLITE3\nconst char *db_schema = {\"\\"  >>src/libs/zbxdbhigh/dbschema.c
	create/schema/gen.pl sqlite|sed -e 's/\t\t*/ /g' -e 's/$$/\\/' >>src/libs/zbxdbhigh/dbschema.c
	echo -e "\"};\n#endif /* HAVE_SQLITE3 */\n" >>src/libs/zbxdbhigh/dbschema.c
