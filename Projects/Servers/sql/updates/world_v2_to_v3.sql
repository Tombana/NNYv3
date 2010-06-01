ALTER TABLE `accounts` 
ADD COLUMN `nbr_online` smallint(5) unsigned  NOT NULL DEFAULT '0' ,
DROP COLUMN `online`,
DROP COLUMN `online_serverid`;

ALTER TABLE `characters` 
ADD COLUMN `online` smallint(1) unsigned  NOT NULL DEFAULT '0' ;