CREATE TABLE autoreg_tmp (
	id		bigint	DEFAULT '0'	NOT NULL,
	priority	integer		DEFAULT '0'	NOT NULL,
	pattern		varchar(255)		DEFAULT ''	NOT NULL,
	hostid		bigint	DEFAULT '0'	NOT NULL,
	PRIMARY KEY (id)
);

insert into autoreg_tmp select * from autoreg;
drop table autoreg;
alter table autoreg_tmp rename to autoreg;