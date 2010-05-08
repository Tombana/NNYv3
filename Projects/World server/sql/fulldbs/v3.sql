/*
MySQL Data Transfer
Source Host: localhost
Source Database: world_v3
Target Host: localhost
Target Database: world_v3
Date: 5/8/2010 2:19:30 PM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for accounts
-- ----------------------------
DROP TABLE IF EXISTS `accounts`;
CREATE TABLE `accounts` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(30) DEFAULT NULL,
  `password` varchar(32) DEFAULT NULL,
  `nbr_online` smallint(5) unsigned NOT NULL DEFAULT '0',
  `banned` smallint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for characters
-- ----------------------------
DROP TABLE IF EXISTS `characters`;
CREATE TABLE `characters` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `account_id` int(10) unsigned NOT NULL DEFAULT '0',
  `slot` int(11) NOT NULL DEFAULT '0',
  `online` smallint(1) unsigned NOT NULL DEFAULT '0',
  `name` varchar(30) DEFAULT NULL,
  `level` int(10) unsigned NOT NULL DEFAULT '0',
  `gender` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `x` int(11) NOT NULL DEFAULT '0',
  `y` int(11) NOT NULL DEFAULT '0',
  `z` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for config
-- ----------------------------
DROP TABLE IF EXISTS `config`;
CREATE TABLE `config` (
  `db_version` int(10) unsigned DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `accounts` VALUES ('1', 'nitrix', '098f6bcd4621d373cade4e832627b4f6', '0', '0');
INSERT INTO `accounts` VALUES ('2', 'tombana', '098f6bcd4621d373cade4e832627b4f6', '0', '0');
INSERT INTO `characters` VALUES ('1', '1', '0', '0', 'b4ckb0n3', '1', '0', '123', '456', '789');
INSERT INTO `characters` VALUES ('2', '0', '0', '0', 'Dummy', '26', '0', '0', '0', '0');
INSERT INTO `config` VALUES ('3');
