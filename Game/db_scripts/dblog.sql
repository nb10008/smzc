CREATE TABLE `log_money` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',
	`AccountID` int(10) unsigned NOT NULL COMMENT '账号ID',
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
	`CmdID` int(10) unsigned NOT NULL COMMENT '操作',
	`ConType` tinyint(3) NOT NULL COMMENT '容器类型',
	`Num` bigint(10) NOT NULL COMMENT '数量(+：表示存入；-：表示取出)',
	`TotalNum` bigint(20) NOT NULL COMMENT '指定类型容器中总量',
	`RoleIDRel` int(10) unsigned NOT NULL default '4294967295' COMMENT '相关联角色ID',
	`Time` datetime NOT NULL COMMENT '时间',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '背包、仓库金钱变化(单位：银)';
	
	
CREATE TABLE `log_yuanbao` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',
	`AccountID` int(10) unsigned NOT NULL COMMENT '账号ID',
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
	`CmdID` int(10) unsigned NOT NULL COMMENT '操作',
	`ConType` tinyint(3) NOT NULL COMMENT '容器类型',
	`Num` int(10) NOT NULL COMMENT '数量(+：表示存入；-：表示取出)',
	`TotalNum` int(20) NOT NULL COMMENT '指定类型容器中总量',
	`Time` datetime NOT NULL COMMENT '时间',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '背包、百宝袋元宝变化';


CREATE TABLE `log_exvolume` (                                       
                `ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',   
                `AccountID` int(10) unsigned NOT NULL COMMENT '账号ID',           
                `RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',              
                `CmdID` int(10) unsigned NOT NULL COMMENT '操作',                 
                `Num` int(10) NOT NULL COMMENT '数量(+：表示存入；-：表示取出)',  
                `TotalNum` int(20) NOT NULL COMMENT '总量',         
                `Time` datetime NOT NULL COMMENT '时间',                          
                PRIMARY KEY (`ID`)                                                
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='赠点变化';
	

CREATE TABLE `log_time_stat` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',
	`AccountID` int(10) unsigned NOT NULL COMMENT '账号ID',
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
	`BagSilver` bigint(10) NOT NULL COMMENT '背包银子数量',
	`WareSilver` bigint(20) NOT NULL COMMENT '角色仓库银子总量',
	`BagYB` int(10) NOT NULL COMMENT '背包元宝数量',
	`BaiBaoYB` int(10) NOT NULL COMMENT '角色仓库元宝总量',
	`ExVolume` int(10) NOT NULL COMMENT '玩家角色赠点数量',
	`Time` datetime NOT NULL COMMENT '时间',
	`VIPPoint` int(10) NOT NULL DEFAULT '0' COMMENT '会员积分',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '在线玩家定时金钱元宝记录';
	
	
CREATE TABLE `log_item` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',
	
	`RoleID` int(10) unsigned NOT NULL COMMENT 'role id',
	`CmdID` int(10) unsigned NOT NULL COMMENT '操作',
	`TypeID` int(10) unsigned NOT NULL COMMENT 'type id',	
	`OptNum` smallint(5) NOT NULL COMMENT '变化个数(+：表示获得；-：表示失去)',

	`Serial1` bigint(20) NOT NULL COMMENT '物品64位序号',
	`ConType1` tinyint(1) NOT NULL COMMENT '容器编号(0表示没有在容器中)',
	`ResNum1` smallint(5) NOT NULL COMMENT '最终个数',
	
	`Serial2` bigint(20) NOT NULL COMMENT '物品64位序号(0表示没有)',
	`ConType2` tinyint(1) NOT NULL COMMENT '容器编号(0表示没有在容器中)',
	`ResNum2` smallint(5) NOT NULL COMMENT '最终个数',
	
	`RoleIDRel` int(10) unsigned NOT NULL default '4294967295' COMMENT '相关联角色ID',

	`Time` datetime NOT NULL COMMENT '时间',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '物品记录';
	
	
CREATE TABLE `log_itemtimes` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',
		
	`RoleID` int(10) unsigned NOT NULL COMMENT 'role id',
	`CmdID` int(10) unsigned NOT NULL COMMENT '操作',

	`Serial` bigint(20) NOT NULL COMMENT '物品64位序号',
	`TypeID` int(10) unsigned NOT NULL COMMENT 'type id',
	`ConType` tinyint(1) NOT NULL COMMENT '容器编号(0表示没有在容器中)',
	`UsedTimes` int(10) NOT NULL COMMENT '已使用次数',
	`MaxUseTimes` int(10) NOT NULL COMMENT '最多可使用次数',

	`Time` datetime NOT NULL COMMENT '时间',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '物品记录';
	
	
CREATE TABLE `log_mallsell` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',
	
	`CmdID` int(10) unsigned NOT NULL COMMENT '操作',
	`Serial` bigint(20) NOT NULL COMMENT '64为序号',
	`TypeID` int(10) unsigned NOT NULL COMMENT '物品id',
	`RoleID` int(10) unsigned NOT NULL COMMENT '购买者id',
	`Num` smallint(5) NOT NULL COMMENT '个数',
	`FstGainTime` char(20) NOT NULL default '2000-00-00 00:00:00' COMMENT '第一个玩家获得该道具的时间',
	`ExistTime` int(10) unsigned NOT NULL COMMENT '时限(单位：秒)',
	`MaxUseTimes` int(10) NOT NULL COMMENT '最多可使用次数',
	`CostYB` int(10) NOT NULL COMMENT '购买价格(总价：元宝)',
	`CostExVolume` int(10) NOT NULL COMMENT '兑换花费的赠点数量',
        `CostVIPPoint` int(10) NOT NULL COMMENT '花费积分数',
	`Time` datetime NOT NULL COMMENT '时间',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '商城物品出售记录';
	
CREATE TABLE `log_fund` (                                      
            `guild_id` int(10) unsigned NOT NULL COMMENT '帮派ID',     
            `account_id` int(10) unsigned NOT NULL COMMENT '账号ID',   
            `role_id` int(10) unsigned NOT NULL COMMENT '角色ID',      
            `cmd_id` int(10) NOT NULL COMMENT '操作',                  
            `fund` int(10) NOT NULL COMMENT '帮派资金变化',        
            `total_fund` int(10) NOT NULL COMMENT '帮派当前资金',  
            `time` time NOT NULL COMMENT 'log时间'                     
          ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE `log_material` (                                      
                `guild_id` int(10) unsigned NOT NULL COMMENT '帮派ID',         
                `account_id` int(10) unsigned NOT NULL COMMENT '账号ID',       
                `role_id` int(10) unsigned NOT NULL COMMENT '角色ID',          
                `cmd_id` int(10) NOT NULL COMMENT '操作',                      
                `material` int(10) NOT NULL COMMENT '帮派资材变化',        
                `total_material` int(10) NOT NULL COMMENT '帮派当前资材',  
                `time` time NOT NULL COMMENT 'log时间'                         
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE `log_reputation` (                                      
                  `guild_id` int(10) unsigned NOT NULL COMMENT '帮派ID',           
                  `account_id` int(10) unsigned NOT NULL COMMENT '账号ID',         
                  `role_id` int(10) unsigned NOT NULL COMMENT '角色ID',            
                  `cmd_id` int(10) NOT NULL COMMENT '操作',                        
                  `reputation` int(10) NOT NULL COMMENT '帮派声望变化',        
                  `total_reputation` int(10) NOT NULL COMMENT '帮派当前声望',  
                  `time` time NOT NULL COMMENT 'log时间'                           
                ) ENGINE=MyISAM DEFAULT CHARSET=utf8;	


CREATE TABLE `log_mallsell_pack` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',

	`PackID` int(10) unsigned NOT NULL COMMENT '礼品包id',
	`BuyID` int(10) unsigned NOT NULL COMMENT '购买者role id',
	`ToID` int(10) unsigned NOT NULL COMMENT '获得者role id',
	`CostYB` int(10) NOT NULL COMMENT '购买价格(总价：元宝)',

	`Time` datetime NOT NULL COMMENT '时间',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '商城出售的礼品包记录';

CREATE TABLE `log_GMCmd` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',

	`role_id` int(10) unsigned NOT NULL COMMENT '角色id',
	`gm_cmd` varchar(256) NOT NULL COMMENT 'Gm命令',
	`error_code` int(10) unsigned NOT NULL COMMENT '执行命令的返回码',
	`time` datetime NOT NULL COMMENT '时间',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '角色使用GM命令的log';


CREATE TABLE `log_Pet` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',
	`master_id` int(10) unsigned NOT NULL COMMENT '角色id',
	`pet_id` int(10) unsigned NOT NULL COMMENT '宠物id',
	`cmd_id` int(10) unsigned NOT NULL COMMENT '操作',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '角色对宠物操作的log';

############################## 1.0.0 start #########################################
CREATE TABLE `log_vippoint` (                                                                 
                `ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',                       
                `AccountID` int(10) unsigned NOT NULL COMMENT '帐号',                                 
                `RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',                                    
                `CmdID` int(10) unsigned NOT NULL COMMENT '命令id',                                       
                `Num` int(10) NOT NULL COMMENT '变化个数',  
                `TotalNum` int(20) NOT NULL COMMENT '总个数',                                             
                `Time` datetime NOT NULL COMMENT '时间',                                                
                PRIMARY KEY (`ID`)                                                                          
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='积分的log'   ;
############################## 1.0.0 end ###########################################

############################## 1.3.0 start #########################################

#神之竞标
CREATE TABLE `log_godbid` (                                                                 
                `ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',                       
                `AccountID` int(10) unsigned NOT NULL COMMENT '帐号',
                `RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
                `ItemID` int(10) unsigned NOT NULL COMMENT '道具ID',
                `MoneyType` tinyint(1) unsigned NOT NULL COMMENT '金钱类型，0：金币； 1：圣币',
                `Money` int(10) unsigned NOT NULL COMMENT '金钱数',
                `Time` datetime NOT NULL COMMENT '获得时间',
                PRIMARY KEY (`ID`)                                                                          
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='神之竞标的log'  ;

#战场占领记录
CREATE TABLE `log_battlefield` (                                                                 
                `ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',                       
                `GuildID` int(10) unsigned NOT NULL COMMENT '公会ID',
                `GuildName` varchar(64) NOT NULL DEFAULT 'N/A' COMMENT '公会名字',
                `Battlefield` tinyint(1) unsigned NOT NULL COMMENT '战场枚举，0--天神冢 1--七花之海 2--奔雷裂谷',
                `Time` datetime NOT NULL COMMENT '记录时间',
                PRIMARY KEY (`ID`)                                                                          
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='战场占领记录'  ;



############################## 1.3.0 end #########################################

############################## 1.3.8 start #########################################
#妖精繁育日志
CREATE TABLE `log_FairyBirth` (
                `ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',
				`AccountID` int(10) unsigned NOT NULL COMMENT '账号ID',
				`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
				`YaoJingSerial` bigint(20) NOT NULL COMMENT '妖精的64为序号',
				`IsNormalChange` tinyint(1) unsigned NOT NULL default '1' COMMENT '是否是正常的变化',
                `BeforeChangeBirthState` tinyint(1) unsigned NOT NULL default '0' COMMENT '变化前生育状态，0--正常状态 1--等待生育确认 2--孕育 3--生育 4--休息',
                `AfterChangeBirthState` tinyint(1) unsigned NOT NULL default '0' COMMENT '变化后生育状态，0--正常状态 1--等待生育确认 2--孕育 3--生育 4--休息',
                `Time` datetime NOT NULL COMMENT '记录时间',
                PRIMARY KEY (`ID`)                                                                          
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='妖精繁育日志'  ;
############################## 1.3.8 end #########################################

############################## 1.4.8 start #########################################
#by ZR
create table log_role_online
(
	`RoleID` int(10) unsigned not null,
	`Date` DATE not null,
	`OnlineTime` int(5) default 0,
	`Mac` varchar(20) default null
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

# by ZR
#drop table if exists log_world_data;
create table log_world_data
(
`Date` DATE not null,
data1	int(10) unsigned, 
data2	int(10) unsigned, 
data3	int(10) unsigned, 
data4	int(10) unsigned, 
data5	int(10) unsigned, 
data6	int(10) unsigned, 
data7	int(10) unsigned, 
data8	int(10) unsigned, 
data9	int(10) unsigned, 
data10	int(10) unsigned, 
data11	int(10) unsigned, 
data12	int(10) unsigned, 
data13	int(10) unsigned, 
data14	int(10) unsigned, 
data15	int(10) unsigned, 
data16	int(10) unsigned, 
data17	int(10) unsigned, 
data18	int(10) unsigned, 
data19	int(10) unsigned, 
data20	int(10) unsigned, 
data21	int(10) unsigned, 
data22	int(10) unsigned, 
data23	int(10) unsigned, 
data24	int(10) unsigned, 
data25	int(10) unsigned, 
data26	int(10) unsigned, 
data27	int(10) unsigned, 
data28	int(10) unsigned, 
data29	int(10) unsigned, 
data30	int(10) unsigned, 
data31	int(10) unsigned, 
data32	int(10) unsigned, 
data33	int(10) unsigned, 
data34	int(10) unsigned, 
data35	int(10) unsigned, 
data36	int(10) unsigned, 
data37	int(10) unsigned, 
data38	int(10) unsigned, 
data39	int(10) unsigned, 
data40	int(10) unsigned, 
data41	int(10) unsigned, 
data42	int(10) unsigned, 
data43	int(10) unsigned, 
data44	int(10) unsigned, 
data45	int(10) unsigned, 
data46	int(10) unsigned, 
data47	int(10) unsigned, 
data48	int(10) unsigned, 
data49	int(10) unsigned, 
data50	int(10) unsigned, 
data51	int(10) unsigned, 
data52	int(10) unsigned, 
data53	int(10) unsigned, 
data54	int(10) unsigned, 
data55	int(10) unsigned, 
data56	int(10) unsigned, 
data57	int(10) unsigned, 
data58	int(10) unsigned, 
data59	int(10) unsigned, 
data60	int(10) unsigned, 
data61	int(10) unsigned, 
data62	int(10) unsigned, 
data63	int(10) unsigned, 
data64	int(10) unsigned, 
data65	int(10) unsigned, 
data66	int(10) unsigned, 
data67	int(10) unsigned, 
data68	int(10) unsigned, 
data69	int(10) unsigned, 
data70	int(10) unsigned, 
data71	int(10) unsigned, 
data72	int(10) unsigned, 
data73	int(10) unsigned, 
data74	int(10) unsigned, 
data75	int(10) unsigned, 
data76	int(10) unsigned, 
data77	int(10) unsigned, 
data78	int(10) unsigned, 
data79	int(10) unsigned, 
data80	int(10) unsigned, 
data81	int(10) unsigned, 
data82	int(10) unsigned, 
data83	int(10) unsigned, 
data84	int(10) unsigned, 
data85	int(10) unsigned, 
data86	int(10) unsigned, 
data87	int(10) unsigned, 
data88	int(10) unsigned, 
data89	int(10) unsigned, 
data90	int(10) unsigned, 
data91	int(10) unsigned, 
data92	int(10) unsigned, 
data93	int(10) unsigned, 
data94	int(10) unsigned, 
data95	int(10) unsigned, 
data96	int(10) unsigned, 
data97	int(10) unsigned, 
data98	int(10) unsigned, 
data99	int(10) unsigned, 
data100	int(10) unsigned
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
insert into log_world_data (`Date`) values(CURDATE());

drop table if exists log_world_data;

create table log_role_yuanbaocost
(
	`AccountID` int(10) not null,
	`yuanbaocost` int(10) default 0,
	`exvolumecost` int(10) default 0,
	`vippointcost` int(10) default 0,
primary key(AccountID)
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

create table log_account_online
(
	`AccountID` int(10) not null,
	`OnlineTime` int(10) not null default 0
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

############################## F-Plan 1.6.0 start ##########################################
## 元神装备强化log
CREATE TABLE `log_holyequipconsolidate` (                                
                      `Id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',  
		      `ConsolidateType` int(10) unsigned NOT NULL COMMENT '元神装备强化类型', 
                      `HolyEquipID` bigint(20) unsigned NOT NULL COMMENT '元神装备ID',       
                      `bCostMoney` tinyint(2) NOT NULL COMMENT '扣除金钱是否成功',       
                      `CostMoney` int(10) unsigned NOT NULL COMMENT '扣除钱数',                
                      `bCostItem` tinyint(2) NOT NULL COMMENT '扣除物品是否成功',  
		      `CostItem` int(10) unsigned NOT NULL COMMENT '扣除物品数',
		      `bCostItemOther` tinyint(2) NOT NULL default '0' COMMENT '扣除老君仙露是否成功',  
  		      `CostItemOther` int(10) unsigned NOT NULL default '0' COMMENT '扣除老君仙露个数', 
		      `bConsolidateRes` tinyint(2) NOT NULL COMMENT '强化是否成功',
		      `CurrentLevel` tinyint(2) NOT NULL COMMENT '元神装备当前等级',
		      `RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
                      `Time` datetime NOT NULL COMMENT '强化时间',                     
                      PRIMARY KEY (`Id`)                                               
                    ) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;

############################## F-Plan 1.6.0 end ##########################################

############################## F-Plan 1.6.1 start ##########################################

## 角色PK被杀Log
CREATE TABLE `log_killrole` (                                
                      `Id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '流水号',  
		      `SrcRoleID` int(10) unsigned NOT NULL COMMENT '杀人角色ID',
                      `TargetRoleID` int(10) unsigned NOT NULL COMMENT '被杀角色ID',
                      `Time` datetime NOT NULL COMMENT '被杀时间',                     
                      PRIMARY KEY (`Id`)                                               
                    ) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;

############################## F-Plan 1.6.1 end ##########################################

