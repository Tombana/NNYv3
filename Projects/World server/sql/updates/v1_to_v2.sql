ALTER TABLE `accounts` 
MODIFY COLUMN `online` smallint(1)  NOT NULL DEFAULT '0' ,
ADD COLUMN `online_serverid` int(10) unsigned  NOT NULL DEFAULT '0',
ADD COLUMN `banned` smallint(1) unsigned  NOT NULL DEFAULT '0' ;

CREATE TABLE `characters` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `account_id` int(10) unsigned NOT NULL DEFAULT '0',
  `slot` int(11) NOT NULL DEFAULT '0',
  `name` varchar(30) DEFAULT NULL,
  `level` int(10) unsigned NOT NULL DEFAULT '0',
  `gender` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `x` int(11) NOT NULL DEFAULT '0',
  `y` int(11) NOT NULL DEFAULT '0',
  `z` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `characters` VALUES ('1', '1', '0', 'b4ckb0n3', '1', '0', '123', '456', '789');
INSERT INTO `characters` VALUES ('2', '0', '0', 'Dummy', '26', '0', '0', '0', '0');
UPDATE `config` SET db_version=2;