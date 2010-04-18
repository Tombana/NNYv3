SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for config
-- ----------------------------
DROP TABLE IF EXISTS `config`;
CREATE TABLE `config` (
  `db_version` int(10) unsigned default NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for updates
-- ----------------------------
DROP TABLE IF EXISTS `updates`;
CREATE TABLE `updates` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `revision` int(10) unsigned default NULL,
  `url` varchar(300) default NULL,
  `relative_path` varchar(300) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for worldservers
-- ----------------------------
DROP TABLE IF EXISTS `worldservers`;
CREATE TABLE `worldservers` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(100) default NULL,
  `ipv4` varchar(15) default NULL,
  `port` int(10) unsigned default NULL,
  `sync_key` varchar(10) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `config` VALUES ('3');
INSERT INTO `updates` VALUES ('1', '1', 'http://192.168.2.13/shared/nnyv3/test01.txt', '/test01.txt');
INSERT INTO `updates` VALUES ('2', '1', 'http://192.168.2.13/shared/nnyv3/test02.txt', '/test02.txt');
INSERT INTO `updates` VALUES ('3', '1', 'http://192.168.2.13/shared/nnyv3/folder1/test03.txt', '/folder1/test03.txt');
INSERT INTO `updates` VALUES ('4', '2', 'http://192.168.2.13/shared/nnyv3/testAA.txt', '/testAA.txt');
INSERT INTO `updates` VALUES ('5', '2', 'http://192.168.2.13/shared/nnyv3/testBB.txt', '/testBB.txt');
INSERT INTO `updates` VALUES ('6', '4', 'http://192.168.2.13/shared/nnyv3/kewl.txt', '/kewl.txt');
INSERT INTO `worldservers` VALUES ('1', 'Ceres', '127.0.0.1', '6132', 'SDFXqjs7nX');
INSERT INTO `worldservers` VALUES ('2', 'Pluto', '127.0.0.1', '6133', 'D9tAhDhLAc');
