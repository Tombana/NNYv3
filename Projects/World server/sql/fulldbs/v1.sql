/*
MySQL Data Transfer
Source Host: localhost
Source Database: nnyv3_world_v1
Target Host: localhost
Target Database: nnyv3_world_v1
Date: 5/6/2010 7:14:07 PM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for accounts
-- ----------------------------
DROP TABLE IF EXISTS `accounts`;
CREATE TABLE `accounts` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `username` varchar(30) default NULL,
  `password` varchar(32) default NULL,
  `online` smallint(1) default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for config
-- ----------------------------
DROP TABLE IF EXISTS `config`;
CREATE TABLE `config` (
  `db_version` int(10) unsigned default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `accounts` VALUES ('1', 'nitrix', '098f6bcd4621d373cade4e832627b4f6', '0');
INSERT INTO `accounts` VALUES ('2', 'tombana', '098f6bcd4621d373cade4e832627b4f6', '0');
INSERT INTO `config` VALUES ('1');
