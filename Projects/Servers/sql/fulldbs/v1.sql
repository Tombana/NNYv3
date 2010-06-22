/*
Navicat MySQL Data Transfer

Source Server         : Localhost as root
Source Server Version : 50148
Source Host           : localhost:3306
Source Database       : nnyv3

Target Server Type    : MYSQL
Target Server Version : 50148
File Encoding         : 65001

Date: 2010-06-21 19:26:01
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `servers`
-- ----------------------------
DROP TABLE IF EXISTS `servers`;
CREATE TABLE `servers` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ipv4` varchar(255) DEFAULT '0.0.0.0',
  `port` int(10) unsigned DEFAULT '0',
  `name` varchar(20) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of servers
-- ----------------------------
INSERT INTO `servers` VALUES ('1', '127.0.0.1', '6132', 'TestLocalMachine');

-- ----------------------------
-- Table structure for `updates`
-- ----------------------------
DROP TABLE IF EXISTS `updates`;
CREATE TABLE `updates` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `version` int(10) unsigned DEFAULT '0',
  `url` varchar(200) DEFAULT NULL,
  `path` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of updates
-- ----------------------------
INSERT INTO `updates` VALUES ('1', '1', 'http://google.com', '/tests/google.html');
INSERT INTO `updates` VALUES ('2', '1', 'http://deviantart.com', '/tests/deviantart.jpg');
INSERT INTO `updates` VALUES ('3', '3', 'http://youtube.com', '/tests/youtube.flv');
INSERT INTO `updates` VALUES ('4', '5', 'http://example.com', '/tests/example.exe');

-- ----------------------------
-- Table structure for `version`
-- ----------------------------
DROP TABLE IF EXISTS `version`;
CREATE TABLE `version` (
  `version_nb` int(10) unsigned DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of version
-- ----------------------------
INSERT INTO `version` VALUES ('1');
