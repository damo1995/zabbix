alter table actions add        esc_period              integer         DEFAULT '0'     NOT NULL;
alter table actions add        def_shortdata           varchar(255)            DEFAULT ''      NOT NULL;
alter table actions add        def_longdata            blob            DEFAULT ''      NOT NULL;
alter table actions add        recovery_msg            integer         DEFAULT '0'     NOT NULL;
alter table actions add        r_shortdata             varchar(255)            DEFAULT ''      NOT NULL;
alter table actions add        r_longdata              blob            DEFAULT ''      NOT NULL;
