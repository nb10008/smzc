/* 帐号库 */
DROP TABLE IF EXISTS `account`;

CREATE TABLE `account` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '帐号ID',
  `name` char(36) NOT NULL COMMENT '帐号名',
  `psd` char(50) NOT NULL COMMENT '密码，一般为MD5',
  `privilege` tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT 'GM权限',
  `login_status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '登录状态',
  `worldname_crc` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '登陆的游戏世界',
  `forbid_mask` tinyint(4) UNSIGNED NOT NULL DEFAULT 0 comment '对应目前各种封停类型，掩码',
  `guard` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否防沉迷',
  `mibao` char(240) DEFAULT NULL COMMENT '密保',
  `SecondPsd` char(50) NOT NULL COMMENT '(korea)二次密码',
  `ip` varchar(20) DEFAULT NULL COMMENT '上次登录ip',
  `time` datetime DEFAULT NULL COMMENT '上次登录time',
  `savetelnum` tinyint(4) NOT NULL DEFAULT '1' COMMENT '是否需要玩家填写手机号',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/* 反外挂日志 */

DROP TABLE IF EXISTS `game_guarder_log`;

CREATE TABLE `game_guarder_log` (                         
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,          
  `account_id` int(11) unsigned NOT NULL COMMENT '帐号ID',
  `name` varchar(32) NOT NULL,                            
  `kick_time` char(20) NOT NULL COMMENT '踢掉的时间',
  `seal` tinyint(1) NOT NULL COMMENT '封号',
  `error_code` int(11) unsigned NOT NULL COMMENT '原因',
  PRIMARY KEY (`id`)                                      
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/* 登陆日志 */

DROP TABLE IF EXISTS `login_log`;

CREATE TABLE `login_log` (                                   
 `accountID` int(11) unsigned NOT NULL COMMENT '帐号ID',  
 `accountName` varchar(32) NOT NULL,                        
 `ip` varchar(20) NOT NULL COMMENT '登陆的IP',           
 `action` varchar(6) NOT NULL COMMENT '动作',             
 `time` datetime DEFAULT NULL COMMENT '时间',
 `mac` varchar(64) DEFAULT NULL COMMENT 'MAC地址',
 `worldid` int(10) unsigned COMMENT '登陆的游戏世界'          
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/* 游戏世界状态 */
DROP TABLE IF EXISTS `world_state`;
CREATE TABLE `world_state` (
  `worldid` bigint(11) NOT NULL DEFAULT '0' COMMENT '世界ID',
  `rolenum` int(11) NOT NULL DEFAULT '0' COMMENT '当前在线人数',
  `worldstate` tinyint(3) NOT NULL DEFAULT '0' COMMENT '(当前人数/最高负载)*100',
  `time` char(20) DEFAULT NULL COMMENT '更新时间',
  PRIMARY KEY (`worldid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='世界状态 不停的update';

DROP TABLE IF EXISTS `world_state_log`;
CREATE TABLE `world_state_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `worldid` bigint(11) NOT NULL DEFAULT '0' COMMENT '世界ID',
  `rolenum` int(11) NOT NULL DEFAULT '0' COMMENT '当前在线人数',
  `queuesize` smallint(10) NOT NULL default 0 COMMENT '当前排队人数',
  `worldstate` tinyint(3) NOT NULL DEFAULT '0' COMMENT '(当前人数/最高负载)*100',
  `time` char(20) DEFAULT NULL COMMENT '更新时间',
  PRIMARY KEY (`id`)  
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='世界状态 不停的insert';

/* gm封停游戏玩家指定游戏世界 记录表 */
DROP TABLE IF EXISTS `world_forbid`;
CREATE TABLE `world_forbid` (
  `accountid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '账号id',
  `worldname_crc` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '游戏世界名的crc',
  PRIMARY KEY (`accountid`,`worldname_crc`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='记录gm工具封停账号';

##2009-09-07
#增加ip封停黑名单
DROP TABLE IF EXISTS `black_list`;
CREATE TABLE `black_list` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(20) DEFAULT NULL COMMENT '被封的ip',
  PRIMARY KEY (`id`),
  UNIQUE KEY `ip` (`ip`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='黑名单';

##2009-10-10
#添加防沉迷时间表
CREATE TABLE `fatigue_time` (                                           
                `accountname_crc` int(11) UNSIGNED NOT NULL COMMENT '帐号名crc',            
                `acc_online_time` int(11) DEFAULT '0' COMMENT '累计在线时间',   
                `acc_offline_time` int(11) DEFAULT '0' COMMENT '累计离线时间',  
                PRIMARY KEY (`accountname_crc`)                                       
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
              
##2010-5-11
#添加ActiveX控件登入的验证表
CREATE TABLE `zinaccount` (                                
              `idxno` int(11) NOT NULL AUTO_INCREMENT COMMENT '备用',  
              `name` char(36) NOT NULL COMMENT 'UserName',             
              `sitecode` char(10) NOT NULL,                            
              `secrandkey` char(32) NOT NULL,                          
              PRIMARY KEY (`name`),                                    
              UNIQUE KEY `idxno` (`idxno`)                             
            ) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ;  

##2010-6-17
#全区角色名字和帮派名字统一
CREATE TABLE  `role_name` (                                                                                          
              `name` varchar(32) NOT NULL  COMMENT '角色名字',   
               PRIMARY KEY (`name`)                                      
            ) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;  

CREATE TABLE  `guild_name` (                                                                                          
              `name` varchar(32) NOT NULL  COMMENT '角色名字',   
               PRIMARY KEY (`name`)                                      
            ) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;  

##2010-11-18
#账号表建立索引
create index idx_crc_status on account(worldname_crc,login_status);

# Jason 2010-11-24 1.2.2 台湾金牌网吧表
CREATE TABLE `inetcafes` (                                                        
             `InetCafesID` int(11) NOT NULL AUTO_INCREMENT,                                  
             `InetCafesName` varchar(32) DEFAULT NULL COMMENT 'internet cafes name',                                       
             `BuffID` int(10) unsigned DEFAULT '4294967295' COMMENT 'CB buff id',                                 
             `InstMapBuffID` int(10) unsigned DEFAULT '4294967295' COMMENT '副本地图要加的增益状态',                          
             `IPRangeMin1` varchar(20) DEFAULT NULL COMMENT 'dotted ip address',             
             `IpRangeMax1` varchar(20) DEFAULT NULL COMMENT 'dotted ip address',             
             `IpRangeMin2` varchar(20) DEFAULT NULL COMMENT 'dotted ip address',             
             `IpRangeMax2` varchar(20) DEFAULT NULL COMMENT 'dotted ip address',             
             `IpRangeMin3` varchar(20) DEFAULT NULL COMMENT 'dotted ip address',             
             `IpRangeMax3` varchar(20) DEFAULT NULL COMMENT 'dotted ip address',             
             `IpRangeMin4` varchar(20) DEFAULT NULL COMMENT 'dotted ip address',             
             `IpRangeMax4` varchar(20) DEFAULT NULL COMMENT 'dotted ip address',             
             `IpRangeMin5` varchar(20) DEFAULT NULL COMMENT 'dotted ip address',             
             `IpRangeMax5` varchar(20) DEFAULT NULL COMMENT 'dotted ip address',             
             PRIMARY KEY (`InetCafesID`)                                                     
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='台湾金牌网吧';

#全区家族名字统一
CREATE TABLE  `family_name` (                                                                                          
              `name` varchar(32) NOT NULL  COMMENT '家族名字',   
               PRIMARY KEY (`name`)                                      
            ) ENGINE=MyISAM DEFAULT CHARSET=utf8 ; 


#################################MAC黑名单 begin ##########################################
DROP TABLE IF EXISTS `black_mac`;
    CREATE TABLE `black_mac` (
      `id` int(11) NOT NULL AUTO_INCREMENT,
      `mac` varchar(40) DEFAULT NULL COMMENT '被封的mac',
      PRIMARY KEY (`id`),
      UNIQUE KEY `mac` (`mac`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='黑名单';
#################################MAC黑名单 end ##########################################

#增加保存玩家手机号
CREATE TABLE `account_tel` (
  `accountid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '账号id',
  `account_name` char(36) NOT NULL COMMENT '帐号名',
  `tel_num` bigint(20) NOT NULL DEFAULT '0' COMMENT '手机号',
  `time` datetime DEFAULT NULL COMMENT '存手机号时间',
  PRIMARY KEY (`accountid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='记录玩家手机号';

