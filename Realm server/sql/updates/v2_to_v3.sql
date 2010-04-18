SET FOREIGN_KEY_CHECKS = 0;

CREATE TABLE updates (
    id int(10) unsigned NOT NULL DEFAULT 0 COMMENT '' auto_increment,
    revision int(10) unsigned NULL DEFAULT NULL COMMENT '',
    url varchar(300) NULL DEFAULT NULL COMMENT '' COLLATE latin1_swedish_ci,
    relative_path varchar(300) NULL DEFAULT NULL COMMENT '' COLLATE latin1_swedish_ci,
    PRIMARY KEY (id)
) DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci COMMENT='InnoDB free: 7168 kB';

ALTER TABLE config
    DROP last_revision,
    DROP worldserver_id;

INSERT INTO updates (id,revision,url,relative_path) VALUES ('1','1','http://192.168.2.13/shared/nnyv3/test01.txt','/test01.txt');
INSERT INTO updates (id,revision,url,relative_path) VALUES ('2','1','http://192.168.2.13/shared/nnyv3/test02.txt','/test02.txt');
INSERT INTO updates (id,revision,url,relative_path) VALUES ('3','1','http://192.168.2.13/shared/nnyv3/folder1/test03.txt','/folder1/test03.txt');
INSERT INTO updates (id,revision,url,relative_path) VALUES ('4','2','http://192.168.2.13/shared/nnyv3/testAA.txt','/testAA.txt');
INSERT INTO updates (id,revision,url,relative_path) VALUES ('5','2','http://192.168.2.13/shared/nnyv3/testBB.txt','/testBB.txt');
INSERT INTO updates (id,revision,url,relative_path) VALUES ('6','4','http://192.168.2.13/shared/nnyv3/kewl.txt','/kewl.txt');

REPLACE INTO config (db_version) VALUES ('3');

SET FOREIGN_KEY_CHECKS = 1;