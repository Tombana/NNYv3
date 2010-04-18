/*
MySQL Data Transfer
Source Host: localhost
Source Database: nnyv3
Target Host: localhost
Target Database: nnyv3
Date: 1/2/2010 1:33:08 PM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for config
-- ----------------------------
DROP TABLE IF EXISTS `config`;
CREATE TABLE `config` (
  `db_version` int(10) unsigned default NULL,
  `last_revision` int(10) unsigned default NULL,
  `worldserver_id` int(10) unsigned default NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

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
INSERT INTO `config` VALUES ('2', '4', '1');
INSERT INTO `worldservers` VALUES ('1', 'Ceres', '127.0.0.1', '6132', 'SDFXqjs7nX');
INSERT INTO `worldservers` VALUES ('2', 'Pluto', '127.0.0.1', '6133', 'D9tAhDhLAc');
