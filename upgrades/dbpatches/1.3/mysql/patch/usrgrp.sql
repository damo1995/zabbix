CREATE TABLE usrgrp_tmp (
	usrgrpid		bigint unsigned		DEFAULT '0'	NOT NULL,
	name		varchar(64)		DEFAULT ''	NOT NULL,
	PRIMARY KEY (usrgrpid)
) ENGINE=InnoDB;
CREATE INDEX usrgrp_1 on usrgrp (name);

insert into usrgrp_tmp select * from usrgrp;
drop table usrgrp;
alter table usrgrp_tmp rename usrgrp;
