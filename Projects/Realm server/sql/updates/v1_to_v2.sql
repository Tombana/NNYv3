SET FOREIGN_KEY_CHECKS = 0;

CREATE TABLE worldservers (
    id int(10) unsigned NOT NULL DEFAULT 0 COMMENT '' auto_increment,
    name varchar(100) NULL DEFAULT NULL COMMENT '' COLLATE latin1_swedish_ci,
    ipv4 varchar(15) NULL DEFAULT NULL COMMENT '' COLLATE latin1_swedish_ci,
    port int(10) unsigned NULL DEFAULT NULL COMMENT '',
    sync_key varchar(10) NULL DEFAULT NULL COMMENT '' COLLATE latin1_swedish_ci,
    PRIMARY KEY (id)
) DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci COMMENT='InnoDB free: 4096 kB';

ALTER TABLE config
    ADD worldserver_id int(10) unsigned NULL DEFAULT NULL COMMENT '' AFTER last_revision;

REPLACE INTO config (db_version,last_revision,worldserver_id) VALUES ('2','4','1');
INSERT INTO worldservers (id,name,ipv4,port,sync_key) VALUES ('1','Ceres','127.0.0.1','6132','SDFXqjs7nX');
INSERT INTO worldservers (id,name,ipv4,port,sync_key) VALUES ('2','Pluto','127.0.0.1','6133','D9tAhDhLAc');

SET FOREIGN_KEY_CHECKS = 1;
