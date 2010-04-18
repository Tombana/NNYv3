/*
MySQL Data Transfer
Source Host: localhost
Source Database: nnyv3
Target Host: localhost
Target Database: nnyv3
Date: 1/1/2010 11:33:08 PM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for config
-- ----------------------------
DROP TABLE IF EXISTS `config`;
CREATE TABLE `config` (
  `db_version` int(10) unsigned default NULL,
  `last_revision` int(10) unsigned default NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `config` VALUES ('1', '4');
