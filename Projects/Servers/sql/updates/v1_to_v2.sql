CREATE TABLE `w_characters` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `accountID` int(10) unsigned default NULL,
  `username` varchar(50) default NULL,
  `level` int(3) unsigned default '1',
  `class` int(2) unsigned default NULL,
  `gender` int(1) unsigned default '0',
  `online` int(1) unsigned default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

CREATE TABLE `r_servers` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `ipv4` varchar(255) NOT NULL default '0.0.0.0',
  `port` int(10) unsigned NOT NULL default '0',
  `name` varchar(20) NOT NULL default '',
  `sync_key` varchar(10) NOT NULL,
  `flag` char(1) NOT NULL default 'N',
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

ALTER TABLE `version` 
ADD COLUMN `version_r` int(10) unsigned  NULL DEFAULT NULL ,
ADD COLUMN `version_w` int(10) unsigned  NULL DEFAULT NULL ;

CREATE TABLE `w_accounts` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `username` varchar(50) default NULL,
  `password` varchar(50) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

CREATE TABLE `r_updates` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `version` int(10) unsigned default '0',
  `url` varchar(200) default NULL,
  `path` varchar(100) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

DROP TABLE `updates`;

DROP TABLE `servers`;

-- ----------------------------
-- Records of r_servers
-- ----------------------------
INSERT INTO `r_servers` VALUES ('1', '127.0.0.1', '6132', 'TestLocalMachine', '7jpWng9Wjc', 'N');

-- ----------------------------
-- Records of r_updates
-- ----------------------------
INSERT INTO `r_updates` VALUES ('1', '1', 'http://google.com', '/tests/google.html');
INSERT INTO `r_updates` VALUES ('2', '1', 'http://deviantart.com', '/tests/deviantart.jpg');
INSERT INTO `r_updates` VALUES ('3', '3', 'http://youtube.com', '/tests/youtube.flv');
INSERT INTO `r_updates` VALUES ('4', '5', 'http://example.com', '/tests/example.exe');

-- ----------------------------
-- Records of version
-- ----------------------------
DELETE FROM `version`;
INSERT INTO `version` VALUES ('2', '1', '1');

-- ----------------------------
-- Records of w_accounts
-- ----------------------------
INSERT INTO `w_accounts` VALUES ('1', 'test', '098f6bcd4621d373cade4e832627b4f6');

-- ----------------------------
-- Records of w_characters
-- ----------------------------
INSERT INTO `w_characters` VALUES ('1', '1', 'ExempleChar', '1', '0', '0', '0');