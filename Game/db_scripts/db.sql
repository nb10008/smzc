CREATE TABLE `account_common` (
	`AccountID` int(10) unsigned NOT NULL COMMENT '账号ID',
        `ChannelID` int(10) unsigned  DEFAULT '0' NOT NULL  COMMENT '运营商ID',
	`AccountName` char(36) NOT NULL COMMENT '账号名',
	`SafeCodeCrc` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '安全码Crc32',
	`ResetTime` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '重置安全码',
	`BagPsdCrc` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '行囊密码CRC值',
	`BaiBaoYuanBao` int(10) NOT NULL DEFAULT '0' COMMENT '百宝袋中元宝',
	`WareSize` smallint(6) NOT NULL DEFAULT '40' COMMENT '仓库大小,默认值为40',
	`WareSilver` bigint(20) NOT NULL DEFAULT '0' COMMENT '仓库中金钱数',
	`PersonalSet` blob COMMENT '游戏设置 游戏选项',
	# Jason 2010-1-27 v1.3.2 记录账号登入/登出时间等信息，玩家回归/离线奖励使用
    `LastLoginTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '该账号上次登入时间',                        
    `LastLogoutTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '该账号上次登出时间',                       
    `LastReceiveDailyOfflineRewardTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次领取离线奖励时间',  
    `LastReceiveRegressionTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次领取回归奖励时间',
    `LastUseRoleID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '该用户上一次选择的角色id',
	`EMacc` tinyint(2) default 0,        
	PRIMARY KEY (`AccountID`),
	KEY `AccountName` (`AccountName`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='游戏世界内账号相关属性';

CREATE TABLE `roledata` (
	`AccountID` int(10) unsigned NOT NULL,
	
	`RoleID` int(10) unsigned NOT NULL,
	`RoleName` varchar(32) NOT NULL,
	`RoleNameCrc` int(10) unsigned NOT NULL,

	`Sex` tinyint(3) NOT NULL COMMENT '性别(0.女;1.男)',
	`SpeakOff` tinyint(3) NOT NULL default '0' COMMENT '玩家是否被禁言 1为是',

	`HairModelID` tinyint(3) unsigned NOT NULL COMMENT '发型',
	`HairColorID` tinyint(3) unsigned NOT NULL COMMENT '发色',
	`FaceModelID` tinyint(3) unsigned NOT NULL COMMENT '面部五官',
	`FaceDetailID` tinyint(3) unsigned NOT NULL COMMENT '面部细节',
	`DressModelID` tinyint(3) unsigned NOT NULL COMMENT '服装样式',
	
	`AvatarEquip` tinyblob COMMENT '装备外观',
	`DisplaySet` tinyint(3) NOT NULL default '0' COMMENT '外观显示设置(默认显示装备模式)',

	`MapID` int(10) unsigned NOT NULL COMMENT '角色所在地图ID',
	`X` float(10) NOT NULL COMMENT '角色所在地图中的X坐标',
	`Y` float(10) NOT NULL COMMENT '角色所在地图中的Y坐标',
	`Z` float(10) NOT NULL COMMENT '角色所在地图中的Z坐标',
	
	`FaceX` float(10) NOT NULL COMMENT '角色朝向的X坐标',
	`FaceY` float(10) NOT NULL COMMENT '角色朝向的Y坐标',
	`FaceZ` float(10) NOT NULL COMMENT '角色朝向的Z坐标',
	
	`RebornMapID` int(10) unsigned NOT NULL default '4294967295' COMMENT '角色复活地图ID',
	
	`Class` tinyint(3) NOT NULL default '1' COMMENT '专精职业',
	`ClassEx` tinyint(3) NOT NULL default '0' COMMENT '英雄职业',
	`LastUpgrade` int(10) unsigned NOT NULL default '0' COMMENT '最近一次升级的时间，DWORD格式',
	`Level` smallint(3) NOT NULL default '1' COMMENT '当前等级',
	`WorkedLevel` bigint(20) NOT NULL default '1' COMMENT '加成过的等级值，用于等级排名',
	`LevelPm` smallint(3) unsigned NOT NULL default '0' COMMENT '等级排名',
	`ExpCurLevel` bigint(20) unsigned NOT NULL default '0' COMMENT '当前级别的升级经验',
	
	`HP` mediumint(8) NOT NULL default '-1' COMMENT '血量',
	`MP` mediumint(8) NOT NULL default '-1' COMMENT '真气量',
	`Rage` smallint(6) NOT NULL default '0' COMMENT '怒气',
	`Endurance` smallint(6) NOT NULL default '-1' COMMENT '持久力',
	`Vitality` smallint(5) NOT NULL default '-1' COMMENT '活力(0～999)',
	`Injury` mediumint(8) NOT NULL default '0' COMMENT '内伤',
	`Knowledge` mediumint(8) NOT NULL default '0' COMMENT '阅历',
	`Morale` smallint(4) NOT NULL default '100' COMMENT '士气',
	`Morality` mediumint(8) NOT NULL default '0' COMMENT '道德',
	`Culture` mediumint(8) NOT NULL default '0' COMMENT '武学修为',
	`Credit` mediumint(8) NOT NULL default '0' COMMENT '信用度',
	`Identity` tinyint(3) NOT NULL default '0' COMMENT '身份',
	`VIPPoint` mediumint(8) NOT NULL default '0' COMMENT '会员积分',
	`Spirit` mediumint(8) NOT NULL default '300' COMMENT '精力',
	`FixSpirit` bigint(20) NOT NULL default '0' COMMENT '精力冷却时间',
	`WuXun` int(10) NOT NULL default '0' COMMENT '武勋',
	`WuJi` int(10) NOT NULL default '0' COMMENT '武技',
	
	`Weary` int(10) NOT NULL default '0' COMMENT '疲劳值',
	
	`AttPtAvail` smallint(6) NOT NULL default '0' COMMENT '可投属性点',
	`TalentPtAvail` smallint(6) NOT NULL default '0' COMMENT '可投天资点数',
	
	`PhysiqueAdded` smallint(6) NOT NULL default '0' COMMENT '筋骨',
	`StrengthAdded` smallint(6) NOT NULL default '0' COMMENT '劲力',
	`PneumaAdded` smallint(6) NOT NULL default '0' COMMENT '元气',
	`InnerforceAdded` smallint(6) NOT NULL default '0' COMMENT '内力',
	`TechniqueAdded` smallint(6) NOT NULL default '0' COMMENT '技力',
	`AgilityAdded` smallint(6) NOT NULL default '0' COMMENT '身法',

	`Strength` mediumint(8) DEFAULT '0' NOT NULL COMMENT '实力值',
	`StrengthNum` smallint(3) DEFAULT '0' NOT NULL COMMENT '实力排名',
	`EquipValue` mediumint(8) UNSIGNED DEFAULT '0' NOT NULL COMMENT '装备实力值',
	`EquipPm` smallint(3) NOT NULL default '0' COMMENT '装备实力排名',
	`AdvanceStrength` mediumint(8) UNSIGNED DEFAULT '0' NOT NULL COMMENT '角色进阶实力值',
	`ConsumptiveStrength` mediumint(8) UNSIGNED DEFAULT '0' NOT NULL COMMENT '角色可消耗实力值',
	`KillCount` mediumint(8) UNSIGNED DEFAULT '0' NOT NULL COMMENT '角色击杀的非白名玩家数',
	
	`TalentType1` tinyint(2) NOT NULL default '-1' COMMENT '天资类型',
	`TalentType2` tinyint(2) NOT NULL default '-1' COMMENT '天资类型',
	`TalentType3` tinyint(2) NOT NULL default '-1' COMMENT '天资类型',
	`TalentType4` tinyint(2) NOT NULL default '-1' COMMENT '天资类型',
	
	`TalentVal1` smallint(4) NOT NULL default '0' COMMENT '天资投放值',
	`TalentVal2` smallint(4) NOT NULL default '0' COMMENT '天资投放值',
	`TalentVal3` smallint(4) NOT NULL default '0' COMMENT '天资投放值',
	`TalentVal4` smallint(4) NOT NULL default '0' COMMENT '天资投放值',
	
	`SGFlag` tinyint(1) NOT NULL default '1' COMMENT '是否被PK保护',
	`CloseSGTime` char(20) NOT NULL default '2000-01-01 00:00:00' COMMENT '上次关闭PK保护的时间',
	
	`BagSize` smallint(6) NOT NULL default '40' COMMENT '背包大小,默认为40',
	`BagGold` int(10) NOT NULL default '0' COMMENT '背包中金子数',
	`BagSilver` int(10) NOT NULL default '0' COMMENT '背包中蝇子数',
	`BagYuanBao` int(10) NOT NULL default '0' COMMENT '背包中元宝数',
	`ExchangeVolume` int(10) NOT NULL default '0' COMMENT '玩家角色赠点的数量',
	
	`GuildID` int(10) unsigned NOT NULL default '4294967295' COMMENT '玩家所在帮派id',
	`TotalTax` int(10) NOT NULL default '0' COMMENT '缴税总额',
	
	`RemoteOpenSet` int(10) unsigned NOT NULL default '4294967295' COMMENT '是否对远端玩家公开相关信息',
	`CurTitleID` smallint(6) unsigned NOT NULL default '65535' COMMENT '当前称号',
	
	`GetMallFreeTime` char(20) NOT NULL default '2000-00-00 00:00:00' COMMENT '上次从商城领取免费物品时间',
	
	`CreateTime` char(20) NOT NULL COMMENT '创建时间',
	`LoginTime` char(20) NOT NULL default '2000-00-00 00:00:00' COMMENT '上线时间',
	`LogoutTime` char(20) NOT NULL default '2000-00-00 00:00:00' COMMENT '下线时间',
	`OnlineTime` int(10) NOT NULL default '-1' COMMENT '总在线时间(单位:秒)',
	`CurOnlineTime` int(10) NOT NULL default '0' COMMENT '当前在线时间(单位:秒)',
	
	`RemoveFlag` tinyint(1) NOT NULL default '0' COMMENT '是否处于删除状态(该状态玩家不会被读入游戏世界)',
	`RemoveTime` int(10) unsigned NOT NULL default '4294967295' COMMENT '删除时间',
	`ScriptData` blob COMMENT '脚本数据',
	`TreasureSum` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '角色开启宝箱数',

        `StallLevel` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '摊位等级',                                                                                                                                                                                                    
        `StallDailyExp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '摊位当天经验',                                                                                                                                                                                              
        `StallCurExp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '摊位当前等级经验', 
	`StallLastTime` int(10) unsigned NOT NULL DEFAULT '4325376' COMMENT '摊位最近更新时间(默认2000年1月1日)',
	`Hostility` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '戾气值',
	`DeadUnSetSafeGuardCountDown` smallint(5) signed NOT NULL DEFAULT '-100' COMMENT '关闭玩家戾气为0时回城复活所加上的pk保护倒计tick',
	`ItemTransportMapID` int(10) unsigned NOT NULL DEFAULT '3017298127' COMMENT '记录式传送符mapid',
	`ItemTransportX` float unsigned NOT NULL DEFAULT '110000' COMMENT '记录式传送符X坐标 2200*50',
	`ItemTransportZ` float unsigned NOT NULL DEFAULT '110000' COMMENT '记录式传送符Z坐标 2200*50',
	`ItemTransportY` float unsigned NOT NULL DEFAULT '332300' COMMENT '记录式传送符Y坐标 6646*50',
	
	`LoverID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '爱人对应的roleid没结婚就是4294967295',
	`HaveWedding` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '是否举行过婚礼',
	`LastLessingTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '上次领取神龙赐福的时间',
	`LastLessingLevel` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '领取到了多少级', 
	`NeedPrisonRevive` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '是否需要牢狱复活',
	# Jason 2010-3-15 v1.5.0神龙赐福扩展
	`LastLessingLevelEx` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '领取到了多少级', 
	`LastLessingLoongDate` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '最后一次神龙赐福时间',                                                        
	`CurrentDayOnlineTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当日在线时间',                                                                
	`OneDayFirstLoginedTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当日首次登陆时间',                                                          
	`OnlineRewardPerDayRecTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '当日领取在线奖励时间',
	`OfflineExperienceRewardFlag` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否离线挂机',                                                   

	`MasterID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '对应的师傅roleid 默认4294967295',   
	
	`JingWuPoint` int(10) unsigned NOT NULL default '0' COMMENT '精武点数',
	`QinWuPoint` int(10) unsigned NOT NULL default '0' COMMENT '勤武点数',
	`MARelation` int(10) NOT NULL DEFAULT '0' COMMENT '师徒总数量',
	
        `FollowPetPocketValue` smallint(6) unsigned NOT NULL DEFAULT '5' COMMENT '跟随宠物的栏位上限', 
        `RidingPetPocketValue` smallint(6) unsigned NOT NULL DEFAULT '5' COMMENT '坐骑的栏位上限', 
                                                                   
	`MiraclePrcNum` smallint(6) unsigned NOT NULL DEFAULT '100' COMMENT '仙界技能修炼点儿',                                          
	`MiracleResetTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '仙界技能修炼点儿重置时间',

	# 龙之试炼系统
	`TrainStateTable` blob COMMENT '试炼状态表',
	# Jason 2010-3-29 v1.5.1 神迹积分
	`GodMiraclePoints` int(10) NOT NULL DEFAULT '0' COMMENT '神迹争夺积分',

	# Jason 2010-5-21 v2.1.2 飞升属性
	`SoaringValue` tinyint(3) unsigned NOT NULL DEFAULT '255' COMMENT '飞升属性', 
	`SoaringSkillLearnTimes` tinyint(3) unsigned NOT NULL DEFAULT '100' COMMENT '飞升技能当天学习次数',
	# Jason 2010-5-27 v2.2.0 韧性
	`Toughness` smallint(4) NOT NULL DEFAULT '0' COMMENT '韧性',

	`CompleteRefreshTime` int(10) NOT NULL default 0 COMMENT '完成任务刷新任务板的次数',
	`IMRefreshTime` int(10) NOT NULL default 0 COMMENT '使用道具刷新任务板的次数',
	# Jason 2010-6-12 
	`ConsolidateTimes` int(10) NOT NULL default '0' COMMENT '角色强化操作总次数',	
	
	`TrainStateTransfered` tinyint(2) unsigned NOT NULL default '1' COMMENT '试炼状态表做过数据转移(0:没有转移过;1:已经转移过),圣魔之血中初始值为1',
	# 2010-7-14 Jason 1.0.0
	`RoleState` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色状态，如pk状态等',
	`VipLevel` tinyint(1) unsigned NOT NULL Default '0' COMMENT '角色VIP等级,0是无等级，1银，2金，3白金',
	`VipStartTime` int(10) unsigned NOT NULL Default '0' COMMENT 'Vip道具使用时间，没有使用过或者已经过期为0',
	`FreeWorldTalkTimes` smallint(5) unsigned NOT NULL Default '0' COMMENT 'Vip角色已经使用的免费世界喊话的次数',
	`VipMaxDays` int(10) unsigned NOT NULL Default '0' COMMENT 'Vip道具时限(单位为天)',
	`CanRankFlag` tinyint(1) unsigned NOT NULL Default '1' COMMENT '角色是否能进入排行,0不能，1能',
	`YaoJingValue` int(10) unsigned NOT NULL Default '0' COMMENT '角色妖精实力值',
	`Buy50LvlItemFlag` tinyint(1) unsigned NOT NULL Default '0' COMMENT '角色是否已经购买了一元计划活动中50级的装备,0未购买，1已经购买',
	`Buy60LvlItemFlag` tinyint(1) unsigned NOT NULL Default '0' COMMENT '角色是否已经购买了一元计划活动中60级的装备,0未购买，1已经购买',
	`Buy70LvlItemFlag` tinyint(1) unsigned NOT NULL Default '0' COMMENT '角色是否已经购买了一元计划活动中70级的装备,0未购买，1已经购买',
	`Buy80LvlItemFlag` tinyint(1) unsigned NOT NULL Default '0' COMMENT '角色是否已经购买了一元计划活动中80级的装备,0未购买，1已经购买',
	`YListCompleteRefreshTime` int(10) NOT NULL default 0 COMMENT '完成天之行会任务刷新任务板的次数',
	`YListIMRefreshTime` int(10) NOT NULL default 0 COMMENT '使用道具刷新天之行会任务板的次数',
	`TakeGuildRewardTime` int(10) unsigned NOT NULL default '0' COMMENT '角色领取公会战场每日在线奖励时间',
	`ExpPilularUseTimes` int(10) unsigned NOT NULL default '0' COMMENT '角色使用经验丹次数',
	`PickupModeSetting`	int(10)	unsigned NOT NULL default '4294967295' COMMENT '拾取模式设置',
	# Jason 2010-12-21
	`PlayerBack` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否领取过玩家回归奖励',
	`PlayerBackDays` smallint(6) NOT NULL DEFAULT '0' COMMENT '累计回归天数',
	# Jason 2011-1-11 v1.3.6 台湾版本需求
	`UseConstraintsMaxPKSafeGuardLevel` tinyint(1) Default '0' COMMENT '是否使用配置文件里面设置的最高pk保护等级',
	`BePlayActLayerID` int(10) unsigned NOT NULL Default '4294967295' COMMENT '被好友装扮的衣橱栏位ID',
	`BePlayActLevel` int(10) unsigned NOT NULL Default '0' COMMENT '被好友装扮的衣橱栏位等级',
	# Jason 2011-1-12
	`GodHead` int(10) unsigned NOT NULL Default '0' COMMENT '神格',
	`GodFaith` int(10) unsigned NOT NULL Default '0' COMMENT '神,信仰',
	`GodCondenced` int(10) unsigned NOT NULL Default '0' COMMENT '神,凝聚度',
	`Clergy` int(10) unsigned NOT NULL Default '0' COMMENT '角色神职',
	`PreClergy` int(10) unsigned NOT NULL Default '0' COMMENT '角色上次的神职',
	`ClergyMarsPoint` int(10) unsigned NOT NULL Default '0' COMMENT '战神系分值',
	`ClergyApolloPoint` int(10) unsigned NOT NULL Default '0' COMMENT '太阳系分值',
	`ClergyRabbiPoint` int(10) unsigned NOT NULL Default '0' COMMENT '法神系分值',
	`ClergyPeacePoint` int(10) unsigned NOT NULL Default '0' COMMENT '和平之神系分值',
	`ClergyCandidateType` tinyint(2) NOT NULL Default '-1' COMMENT '神职竞选报名状态，0战神 1太阳神 2法师之神 3和平之神',
	`Clergy4SeniorOrJunior` tinyint(2) default 0,
	`FamilyID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '家族ID',
	`KeyCodeRewarded` int(10) unsigned not null default 0 COMMENT '使用key码领取过奖励',
	`TrainDate` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族妖精修炼物品计数日期',
	`TrainNum` int(10) NOT NULL DEFAULT '0' COMMENT '家族妖精修炼当日修炼物品使用次数',
	`award_point` int(10) NOT NULL DEFAULT '0' COMMENT '角色奖励点',
	`award_flag` int(10) NOT NULL DEFAULT '0' COMMENT '礼包领取标志位',
	`BrotherEndTime` int(10) unsigned NOT NULL Default '0' COMMENT '角色结拜结束时间',
	`BrotherTeacherID` int(10) unsigned NOT NULL Default '0' COMMENT '角色结拜老师的ID',
	`PulseRemainTime` int(10) unsigned NOT NULL Default '0' COMMENT '经脉剩余学习次数',
	`TigerTime` int(10) unsigned NOT NULL Default '0' COMMENT '老虎机次数',
	`god_soul` int(10) NOT NULL Default '0' COMMENT '神魂',
	`monster_soul` int(10) NOT NULL Default '0' COMMENT '魔魂',
	`god_point` int(10) NOT NULL Default '0' COMMENT '神魂声望',
	`monster_point` int(10) NOT NULL Default '0' COMMENT '魔魂声望',
	`flower_num` int(10) NOT NULL Default '0' COMMENT '鲜花数量',
	`egg_num` int(10) NOT NULL Default '0' COMMENT '鸡蛋数量',
	`holy_value` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '圣灵值',
	`role_hit_add` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '命中',
	`role_eei_all` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '元素通用伤害',
	PRIMARY KEY (`RoleID`),
	KEY `AccountID` (`AccountID`),
	UNIQUE KEY `RoleNameCrc` (`RoleNameCrc`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='角色数据表 -- 核心表';
	

CREATE TABLE `skill` (                     
	`RoleID` int(10) unsigned NOT NULL,
	`ID` int(10) unsigned NOT NULL,
	`BiddenLevel` tinyint(3) NOT NULL default '0' COMMENT '投点等级',
	`SelfLevel` tinyint(3) NOT NULL default '0' COMMENT '固有等级',
	`Proficiency` mediumint(8) NOT NULL default '0' COMMENT '技能熟练程度',
	`CoolDown` int(10) NOT NULL COMMENT 'buff作用时间(单位:毫秒)',
	`active_time` int(10) unsigned DEFAULT '0' COMMENT '激活时间',
	PRIMARY KEY  (`RoleID`, `ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='角色所会的功夫';


CREATE TABLE `buff` (                     
	`RoleID` int(10) unsigned NOT NULL COMMENT '在谁身上',
	`SrcUnitID` int(10) unsigned NOT NULL COMMENT '源Unit的ID',
	`SrcSkillID` int(10) unsigned NOT NULL COMMENT '由哪个技能产生（大ID）',
	`ItemTypeID` int(10) unsigned NOT NULL COMMENT '物品或装备的属性ID',
	`ItemSerialID` bigint(10) NOT NULL COMMENT '由哪个物品或装备产生',
	
	`BuffID` int(10) unsigned NOT NULL COMMENT 'Buff ID',
	`CurTick` int(10) unsigned NOT NULL COMMENT '当前持续时间(单位:tick)',
	`Level` tinyint(3) NOT NULL COMMENT '等级',
	`CurLapTimes` tinyint(3) NOT NULL COMMENT '当前叠加次数',
	
	`EffectSkillID` blob COMMENT '影响该buff的技能ID',

	PRIMARY KEY  (`RoleID`, `BuffID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='角色身上的buff';


CREATE TABLE `title` (                     
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
	`ID` smallint(5) unsigned NOT NULL COMMENT '称号ID',
	`Count` int(10) unsigned NOT NULL COMMENT '计数',
	`TimeRemain` int(1) default '0' COMMENT '剩余时间',
	PRIMARY KEY  (`RoleID`, `ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='角色称号表';


CREATE TABLE `item` (                                                                                                 
          `SerialNum` bigint(20) NOT NULL COMMENT '64位id',                                                                  
          `Num` smallint(5) NOT NULL DEFAULT '1' COMMENT '数量',                                                            
          `TypeID` int(10) unsigned NOT NULL COMMENT '类型id',                                                              
          `Bind` tinyint(4) NOT NULL COMMENT '是否绑定',                                                                  
          `LockStat` tinyint(4) NOT NULL DEFAULT '0' COMMENT '物品锁定状态(1:锁定状态)',
          `UnlockTime` int(10) NOT NULL DEFAULT '0' COMMENT '当物品处于被解锁的三日之内，记录解锁的时间，其他情况下该时间为0',                
          `UseTimes` int(10) NOT NULL DEFAULT '0' COMMENT '物品使用次数',                                               
          `FirstGainTime` char(20) NOT NULL DEFAULT '2000-00-00 00:00:00' COMMENT '第一个玩家获得该道具的时间',  
          `CreateMode` tinyint(2) NOT NULL COMMENT '创建方式：GM，NPC，任务和玩家等',                            
          `CreateID` int(10) unsigned NOT NULL COMMENT '创建方式中对应的id',                                          
          `CreatorID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '创建者id',                                   
          `CreateTime` char(20) NOT NULL COMMENT '创建时间',                                                              
          `OwnerID` int(10) unsigned NOT NULL COMMENT '角色ID',                                                             
          `AccountID` int(10) unsigned NOT NULL COMMENT '帐号ID(各角色共享物品用)',                                 
          `ContainerTypeID` tinyint(4) unsigned NOT NULL COMMENT '所在容器类型',                                        
          `Suffix` smallint(3) unsigned NOT NULL COMMENT '容器中位置',                                                    
          `NameID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '名字ID',  
          `ItemSpecVal1` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品特殊属性值1',                                       
          `ItemSpecVal2` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '物品特殊属性值2',      
	  `del_time` char(20) NOT NULL DEFAULT '2011-05-17 00:00:00' COMMENT '删除道具的时间',
          PRIMARY KEY (`SerialNum`),                                                                                          
          KEY `OwnerID` (`OwnerID`),                                                                                          
          KEY `AccountID` (`AccountID`,`ContainerTypeID`)                                                                     
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='物品表';                                                      

	
CREATE TABLE `equip` (
	`SerialNum` bigint(20) NOT NULL,

	# 装备鉴定时确定
	`Quality` tinyint(4) unsigned NOT NULL default '0' COMMENT '品级(255表示未鉴定)',

	`MinUseLevel` tinyint(4) unsigned NOT NULL default '0' COMMENT '等级限制',
	`MaxUseLevel` tinyint(4) unsigned NOT NULL default '0' COMMENT '等级上限',

	`WuHun` smallint(5) NOT NULL COMMENT '武魂:内功伤害计算用',
	`MinDmg` smallint(5) NOT NULL COMMENT '武器最小伤害',
	`MaxDmg` smallint(5) NOT NULL COMMENT '武器最大伤害',
	`Armor` smallint(5) NOT NULL COMMENT '防具护甲',

	`PotVal` int(10) NOT NULL COMMENT '当前装备潜力值',
	`PotValUsed` int(10) NOT NULL default '0' COMMENT '已经消耗的装备潜力值',
	# 装备潜力值提升次数
	`PotIncTimes` smallint(10) UNSIGNED DEFAULT '0' NOT NULL,
	# 紫色品级鉴定几率 2009-12-2 Jason
	`PurpleQltyIdfPct` int(10) UNSIGNED DEFAULT '0' NOT NULL,

	`RoleAttEffect` tinyblob COMMENT '一级属性影响',
	`RoleAtt2EffectCount` tinyint unsigned NOT NULL default '0' COMMENT '装备对角色的进阶/二级属性影响个数',
	
	`LongInnerID` int(10) unsigned NOT NULL default '4294967295' COMMENT '表龙魂能力ID',
	`LongOuterID` int(10) unsigned NOT NULL default '4294967295' COMMENT '里龙魂能力ID',

	`SpecAtt` tinyint(2) unsigned NOT NULL default '255' COMMENT '特殊属性',

	# 时装生成/鉴定时确定
	`Appearance` smallint(5) NOT NULL default '0' COMMENT '仪容',
	`Rein` tinyint(3) unsigned NOT NULL default '0' COMMENT '统御',
	`Savvy` tinyint(3) unsigned NOT NULL default '0' COMMENT '悟性',
	`Fortune` tinyint(3) unsigned NOT NULL default '0' COMMENT '福缘',
	`ColorID` tinyint(3) NOT NULL default '0' COMMENT '颜色代码',
	
	# 生产时影响的属性
	`AttALimMod` smallint(5) NOT NULL default '0' COMMENT '一级属性限制值',
	`AttALimModPct` smallint(5) NOT NULL default '0' COMMENT '一级属性限制百分比',
	
	`PosyTimes` tinyint(4) NOT NULL default '0' COMMENT '铭文次数',
	`PosyEffect` tinyblob COMMENT '铭文影响(体力,真气,内外攻防)',
	
	`EngraveTimes` tinyint(4) NOT NULL default '0' COMMENT '镌刻次数',
	`EngraveAtt` tinyblob COMMENT '镌刻影响(武器:攻击/武魂;防具:护甲)',
	
	`HoleNum` tinyint(4) NOT NULL default '0' COMMENT '镶嵌孔个数',
	`HoleGemID` tinyblob COMMENT '5个孔中镶嵌的宝石ID',
	
	`BrandLevel` tinyint(4) NOT NULL default '0' COMMENT '烙印等级',
	`DerateVal` tinyblob COMMENT '伤害减免值(若人物属性枚举中伤害减免顺序调整,则该处也需调整)',
	
	`LongFuLevel` tinyint(4) NOT NULL default '0' COMMENT '龙附等级',
	`WuXing` tinyblob COMMENT '五行等级: 金木水火土',
	
	`CanCut` tinyint(2) NOT NULL default '1' COMMENT '是否可开凿(0:否; 1:可)',

	`FlareVal` tinyint(3) unsigned NOT NULL default '0' COMMENT '光晕值',

	# 装备生产时确定
	`QltyModPct` smallint(10) NOT NULL default '0' COMMENT '一级生成品级修正率(-10000,10000)',
	`QltyModPctEx` smallint(10) unsigned NOT NULL default '0' COMMENT '二级生成品级修正率[0,10000)',
	`PotValModPct` smallint(10) unsigned NOT NULL DEFAULT '10000' COMMENT '装备潜力值影响系数',
	
	`Signed` tinyint(2) NOT NULL default '0' COMMENT '是否被署名',
	`SignRoleID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '署名者id',     
	`Signature` varchar(10) COMMENT '署名内容',
	# Jason
	`SoaringFlag` tinyint(2) NOT NULL DEFAULT '0' COMMENT '飞升标志',
	`CastingTimes` tinyint(3) NOT NULL DEFAULT '0' COMMENT '熔铸次数',
	# Jason 2010-6-13 v1.0.0
	`ConsolidateLevel` tinyint(3) NOT NULL default '0' COMMENT '装备强化等级',
	# Jason 2010-6-19 v1.0.0 强化角色属性影响
	`ConsolidateAttEffect` tinyblob COMMENT '强化属性影响',
	
	# Jason 2010-9-8 神铸
	`LastGodStrengthenLevel`	int(10)	unsigned default '0' NOT NULL COMMENT '上次神铸等级',
	`GodStrengthenLevel` int(10) unsigned default '0' NOT NULL COMMENT '当前神铸等级',
	`CurGodStrengthenExp` int(10) unsigned default '0' NOT NULL COMMENT '神铸当前经验值',
	`GodStrengthenTime` int(10) unsigned default '0'NOT NULL COMMENT '上次神铸时间',

	`deltime` char(20) NOT NULL DEFAULT '2011-05-17 00:00:00' COMMENT '删除道具的时间',

	`GrowID` int(10) default 0,
	`GrowValue` int(10) default 0,
	`GrowLevel` tinyint(2) default 0,
	`GrowTimes` int(10) default 0,
	`GrowUseIMTimes` int(10) default 0,
	`GrowStars` tinyblob,
	`GrowAtt` tinyblob,
	# 宝石增幅
	`DiamondLevel` tinyint(2) default 0,
		#冲星等级
	`byStarEnhanceLvl`	tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT '冲星等级',
	`byHolyHoleNum`	tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT '元神装备当前孔数',

	PRIMARY KEY  (`SerialNum`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='装备表(装备 = 装备表 + 物品表)';
	
	
CREATE TABLE `item_cdtime` (                                                        
               `RoleID` int(10) unsigned NOT NULL COMMENT '角色id',                            
               `CDTime` blob COMMENT '冷却时间blob，格式为（typeid，冷却时间）',  
               PRIMARY KEY (`RoleID`)                                                            
             ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='物品&装备冷却时间' ;
	
CREATE TABLE `friend` (                                              
          `RoleID` int(10) unsigned NOT NULL COMMENT '玩家id',             
          `FriendID` int(10) unsigned NOT NULL COMMENT '好友id',           
          `GroupID` tinyint(2) NOT NULL COMMENT '好友所属群组',        
          PRIMARY KEY (`RoleID`,`FriendID`)                                  
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='好友列表(单向)';
	
	
CREATE TABLE `friendship` (               
	`RoleID` int(10) unsigned NOT NULL COMMENT '好友中id较小的',  
	`FriendID` int(10) unsigned NOT NULL COMMENT '好友中id较大的',
	`FriVal` mediumint(6) NOT NULL COMMENT '友好度',
	PRIMARY KEY  (`RoleID`, `FriendID`),
	KEY `FriendID` (`FriendID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='友好度(双向)';
	
	
CREATE TABLE `enemy` (                                     
          `RoleID` int(10) unsigned NOT NULL COMMENT '玩家id',   
          `EnemyID` int(10) unsigned NOT NULL COMMENT '仇敌id',  
          PRIMARY KEY (`RoleID`,`EnemyID`)                         
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='仇敌';
	
CREATE TABLE `blacklist` (                                                        
             `RoleID` int(10) unsigned NOT NULL COMMENT '角色id',                          
             `BlackID` int(10) unsigned NOT NULL COMMENT '被角色id列入黑名单的id',  
             PRIMARY KEY (`RoleID`,`BlackID`)                                                
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='黑名单';

	
CREATE TABLE `task_done` (                                                                           
             `RoleID` int(10) unsigned NOT NULL,                                                                
             `TaskID` int(10) unsigned NOT NULL COMMENT '任务id',                                             
             `Times` smallint(6) unsigned NOT NULL COMMENT '一个时期内完成的任务次数',              
             `StartTime` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '第一个任务开始时间',  
             PRIMARY KEY (`RoleID`,`TaskID`)                                                                    
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='已做过任务';

	
	
CREATE TABLE `task` (                                                                 
          `RoleID` int(10) unsigned NOT NULL,                                                 
          `TaskID` int(10) unsigned NOT NULL,                                                 
          `TaskFlag` int(10) NOT NULL DEFAULT '0' COMMENT '任务完成状态',               
          `Monster0Num` int(10) NOT NULL DEFAULT '0' COMMENT '已打怪数量1',              
          `Monster1Num` int(10) NOT NULL DEFAULT '0',                                         
          `Monster2Num` int(10) NOT NULL DEFAULT '0',                                         
          `Monster3Num` int(10) NOT NULL DEFAULT '0',                                         
          `Monster4Num` int(10) NOT NULL DEFAULT '0',                                         
          `Monster5Num` int(10) NOT NULL DEFAULT '0',                                         
          `StartTime` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '开始时间',
 	  `ChuandaoCount` int(10) unsigned Default '0' COMMENT '传道任务计数',
          `ScriptData` blob COMMENT '脚本数据',                                           
          `DynamicTarget` blob,                                                               
          PRIMARY KEY (`RoleID`,`TaskID`)                                                     
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='已接任务';

CREATE TABLE `role_del` (  
	`AccountID` int(10) unsigned NOT NULL,
	`RoleID` int(10) unsigned NOT NULL,
	`RoleName` varchar(32) NOT NULL,
	`IP` varchar(32),   
	`DeleteTime` datetime NOT NULL
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='角色删除相关记录'; 
	
	
CREATE TABLE `ybaccount` (
	`RoleID` int(10) unsigned NOT NULL,
	`YuanBao` int(10) NOT NULL default '0',
	`Gold` int(10) NOT NULL default '0',
	`Silver` int(10) NOT NULL default '0',
	`bSellOrder` tinyint(2) NOT NULL default '0' COMMENT '是否提交过出售订单',
	`bBuyOrder` tinyint(2) NOT NULL default '0' COMMENT ' 是否提交过收购订单',
	PRIMARY KEY (`RoleID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='元宝交易账户';
	
	
CREATE TABLE `yuanbaoorder` (
	`dwID` int(10) unsigned NOT NULL COMMENT '订单ID',
	`RoleID` int(10) unsigned NOT NULL COMMENT '玩家ID',
	`OrderType` tinyint(6) NOT NULL default '0' COMMENT '订单类型（收购出售）',
	`Price` int(10) NOT NULL default '0' COMMENT '订单中元宝价格',
	`Number` int(10) NOT NULL default '0' COMMENT '订单中元宝数量',
	`OrderMode` int(6) NOT NULL default '0' COMMENT '订单交易状态（委托中撤销关闭）',
	`StartTime` int(10) unsigned NOT NULL default '0' COMMENT '订单的提交时间',
	`EndTime` int(10) unsigned NOT NULL default '4294967295' COMMENT '订单的结束时间',
	`AvgPrice` int(10) NOT NULL default '0' COMMENT '最终成交的平均价格',
	`DealNum` int (10) NOT NULL default '0' COMMENT '最终的成交数量',
	PRIMARY KEY (`dwID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='元宝交易订单';

CREATE TABLE `visiting_card` (                                                 
                 `RoleID` int(11) unsigned NOT NULL COMMENT '角色id',                       
                 `Level` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '等级',                    
                 `Job` tinyint(3) NOT NULL DEFAULT '0' COMMENT '职业',                      
                 `MateRoleID` int(11) NOT NULL DEFAULT '-1' COMMENT '配偶',                 
                 `FactionID` int(11) NOT NULL DEFAULT '-1' COMMENT '帮派',                  
                 `Position` int(3) NOT NULL DEFAULT '-1' COMMENT '帮派职位',              
                 `Visibility` tinyint(1) NOT NULL DEFAULT '1' COMMENT '可见性',            
                 `Sex` tinyint(2) unsigned NOT NULL DEFAULT '3' COMMENT '性别',             
                 `Constellation` tinyint(4) unsigned NOT NULL DEFAULT '13' COMMENT '星座',  
                 `ChineseZodiac` tinyint(4) unsigned NOT NULL DEFAULT '13' COMMENT '生肖',  
                 `Area` tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT '区域',            
                 `Province` tinyint(6) unsigned NOT NULL DEFAULT '0' COMMENT '省份',        
                 `City` char(6) NOT NULL DEFAULT 'N/A' COMMENT '城市',                     
                 `HeadUrl` varchar(100) DEFAULT NULL COMMENT '头像',              
                 `Signature` varchar(200) NOT NULL DEFAULT 'N/A' COMMENT '签名',
                 `NameHistory` varchar(500) NOT NULL DEFAULT 'N/A' COMMENT '更名记录',          
                 PRIMARY KEY (`RoleID`)                                                       
               ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='角色名贴表';
               
CREATE TABLE `clan_data` (                                                                       
             `RoleID` int(11) unsigned NOT NULL,
             `TotalFame` int(20) unsigned NOT NULL DEFAULT '0' COMMENT '各氏族声望总和',
             `RemoveFlag` tinyint(1) NOT NULL default '0' COMMENT '是否处于删除状态',
						 `FamePm` smallint(3) unsigned NOT NULL DEFAULT '0' COMMENT '各氏族声望总和',                                                  
             `FameMask` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '名人堂标识',                    
             `RepRstTimeStamp` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '声望重置时间',          
             `RepXuanYuan` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '轩辕声望',                    
             `ConXuanYuan` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '轩辕贡献',                    
             `ActCntXuanYuan` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '轩辕剩余激活次数',  
             `RepShenNong` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '神农声望',                    
             `ConShenNong` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '神农贡献',                    
             `ActCntShenNong` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '神农剩余激活次数',  
             `RepFuXi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '伏羲声望',                        
             `ConFuXi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '伏羲贡献',                        
             `ActCntFuXi` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '伏羲剩余激活次数',      
             `RepSanMiao` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '三苗声望',                     
             `ConSanMiao` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '三苗贡献',                     
             `ActCntSanMiao` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '三苗剩余激活次数',   
             `RepJiuLi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '九黎声望',                       
             `ConJiuLi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '九黎贡献',                       
             `ActCntJiuLi` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '九黎剩余激活次数',     
             `RepYueZhi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '月氏声望',                      
             `ConYueZhi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '月氏贡献',                      
             `ActCntYueZhi` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '月氏剩余激活次数',    
             `RepNvWa` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '女娲声望',                        
             `ConNvWa` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '女娲贡献',                        
             `ActCntNvWa` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '女娲剩余激活次数',      
             `RepGongGong` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '共工声望',                    
             `ConGongGong` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '共工贡献',                    
             `ActCntGongGong` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '共工剩余激活次数',  
             PRIMARY KEY (`RoleID`),                                                                        
             KEY `RepFuXi` (`RepFuXi`),                                                                     
             KEY `RepXuanYuan` (`RepXuanYuan`),                                                             
             KEY `RepSanMiao` (`RepSanMiao`),                                                               
             KEY `RepShenNong` (`RepShenNong`),                                                             
             KEY `RepJiuLi` (`RepJiuLi`),                                                                   
             KEY `RepGongGong` (`RepGongGong`),                                                             
             KEY `RepNvWa` (`RepNvWa`),                                                                     
             KEY `RepYueZhi` (`RepYueZhi`)                                                                  
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='玩家氏族数据表';                                          

CREATE TABLE `famehall_act_treasure` (                               
	 `TreasureID` smallint(11) unsigned NOT NULL COMMENT '氏族珍宝ID',  
	 `ActNameID` int(11) unsigned NOT NULL COMMENT '激活该珍宝角色名字ID',                             
	 `RoleID` int(11) unsigned NOT NULL COMMENT '角色ID',                                
	 `ClanType` tinyint(1) unsigned NOT NULL COMMENT '氏族类型',                           
	 `ActTime` int(11) unsigned NOT NULL COMMENT '激活时间',                               
	 PRIMARY KEY (`ClanType`,`TreasureID`),                              
	 KEY `ActTime` (`ActTime`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='氏族珍宝激活表';

CREATE TABLE `famehall_entersnap` (         
	`RoleID` int(11) unsigned NOT NULL COMMENT '角色ID',       
	`EnterNameID` int(11) unsigned NOT NULL COMMENT '角色名字ID',  
	`ClanType` tinyint(3) unsigned NOT NULL COMMENT '氏族类型',  
	`EnterTime` int(11) unsigned NOT NULL COMMENT '进入时间',    
	PRIMARY KEY (`ClanType`,`EnterTime`),     
	KEY `ClanType` (`ClanType`)               
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;


CREATE TABLE `famehall_rep_rst_timestamp` (    
	`ClanType` tinyint(8) NOT NULL COMMENT '氏族类型',              
	`ResetTimeStamp` int(11) unsigned NOT NULL COMMENT '重置时间',  
	PRIMARY KEY (`ClanType`)                     
) ENGINE=MyISAM DEFAULT CHARSET=utf8;   
	

CREATE TABLE `guild` (                         
	`ID` int(10) unsigned NOT NULL COMMENT '帮派ID -- 名称转成小写后的CRC32值',
	`Name` blob COMMENT '名称',
	
	`FounderNameID` int(10) unsigned NOT NULL COMMENT '帮派创始人名称ID',
	`LeaderID` int(10) unsigned NOT NULL COMMENT '现任帮主ID',
	`SpecState` int(10) unsigned NOT NULL default '0' COMMENT '帮派当前特殊状态',
	
	`Level` tinyint(3) NOT NULL COMMENT '帮派当前等级',
	`HoldCity0` tinyint(3) unsigned NOT NULL default '0' COMMENT '帮派当前辖属城市编号',
	`HoldCity1` tinyint(3) unsigned NOT NULL default '0' COMMENT '',
	`HoldCity2` tinyint(3) unsigned NOT NULL default '0' COMMENT '',
	
	`Fund` int(10) NOT NULL default '0' COMMENT '当前资金',
	`Material` int(10) NOT NULL default '0' COMMENT '当前资材',
	`Rep` int(10) NOT NULL default '0' COMMENT '当前威望',
	`DailyCost` int(10) NOT NULL default '0' COMMENT '每日维护费用',
	
	`Peace` smallint(5) NOT NULL default '0' COMMENT '当前安宁度',
	`Rank` smallint(5) NOT NULL default '0' COMMENT '当前排名(0表示还未排名)',
	
	`Tenet` blob COMMENT '宗旨',
	`Symbol` blob COMMENT '标志ULR',
	
	`CreateTime` datetime NOT NULL COMMENT '创建时间',
	`GroupPurchase` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '帮派团购指数', 
	`RemainSpreadTimes` int(3) unsigned NOT NULL DEFAULT '0' COMMENT '帮务剩余发布次数',                                                           
    `Commendation` tinyint(1) NOT NULL DEFAULT '0' COMMENT '跑商嘉奖状态',
    
    `DailyRewardTakenTimes` blob COMMENT '公会各职务领取在线奖励次数，用于战场每日奖励的职位限制',
    `GuildValue1` int(10) default 0 COMMENT '策划需求数值1',
    `GuildValue2` int(10) default 0 COMMENT '策划需求数值2',
	PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='帮派属性';
	

CREATE TABLE `guild_member` (
	`RoleID` int(10) unsigned NOT NULL COMMENT '成员ID',
	`GuildID` int(10) unsigned NOT NULL COMMENT '帮派ID',
	`GuildPos` tinyint(3) unsigned NOT NULL COMMENT '帮派职位',
	
	`TotalContrib` int(10) NOT NULL default '0' COMMENT '累计帮派贡献',
	`CurContrib` int(10) NOT NULL default '0' COMMENT '当前帮派贡献',
	`Exploit` int(10) NOT NULL default '0' COMMENT '帮派功勋',
	`Salary` int(10) NOT NULL default '0' COMMENT '俸禄标准',
	`CanUseGuildWare` tinyint(1) NOT NULL DEFAULT '0' COMMENT '操作帮派仓库权限',
	`JoinTime` datetime NOT NULL COMMENT '加入时间',
	PRIMARY KEY (`RoleID`),
	KEY (`GuildID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='帮派成员';
	

CREATE TABLE `log_baibao` (
	`AccountID` int(10) unsigned NOT NULL COMMENT '账号ID',         
	`RoleID` int(10) NOT NULL COMMENT '物品来源',                   
	`LogTime` char(20) NOT NULL COMMENT '获得物品时间',             
	`n16Type` tinyint(4) unsigned NOT NULL COMMENT '物品获得方式',  
	`TypeID` int(10) unsigned NOT NULL COMMENT '物品类型',          
	`LeaveWords` tinyblob COMMENT '赠言'                            
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
            
CREATE TABLE `group_purchase` (                                       
	`GuildID` int(10) unsigned NOT NULL COMMENT '帮派ID',               
	`RoleID` int(10) NOT NULL COMMENT '发起人ID',                       
	`MallID` int(10) NOT NULL COMMENT '商城物品ID',                     
	`ItemPrice` int(10) unsigned NOT NULL COMMENT '商品价格',           
	`ParticipatorNum` tinyint(3) unsigned NOT NULL COMMENT '响应人数',  
	`RequireNum` tinyint(3) unsigned NOT NULL COMMENT '团购所需人数',   
	`RemainTime` int(10) NOT NULL COMMENT '团购剩余时间',               
	`ParticipatorList` tinyblob COMMENT '响应者列表'                    
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
               
CREATE TABLE `pet_data` (                                                                                  
            `pet_id` int(11) unsigned NOT NULL COMMENT '宠物id',
            `pet_name` varchar(32) NOT NULL COMMENT  '宠物名称',
            `pet_value` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '宠物实力值',
            `pet_pm` smallint(3) unsigned NOT NULL DEFAULT '0' COMMENT '宠物实力排名',
            `master_id` int(11) unsigned NOT NULL COMMENT '主人id',
            `type_id` int(11) unsigned NOT NULL COMMENT '原型id',
            `quality` tinyint(1) unsigned NOT NULL COMMENT '宠物品质',
            `aptitude` int(4) unsigned NOT NULL COMMENT '宠物资质',
            `potential` int(4) unsigned NOT NULL COMMENT '当前潜能',
            `cur_spirit` int(4) NOT NULL COMMENT '当前灵力',
            `cur_exp` int(20) NOT NULL DEFAULT '0' COMMENT '当前经验',
            `step` tinyint(1) NOT NULL DEFAULT '0' COMMENT '阶',
            `grade` tinyint(1) NOT NULL DEFAULT '1' COMMENT '等',
            `talent_count` int(4) NOT NULL DEFAULT '0' COMMENT '当前天资计数',
            `wuxing_energy` int(4) NOT NULL DEFAULT '0' COMMENT '五行力',
            `pet_state` int(1) NOT NULL DEFAULT '0'COMMENT '宠物状态',
            `pet_lock` tinyint(1) NOT NULL DEFAULT '0' COMMENT '宠物锁定',
            `RemoveFlag` tinyint(1) DEFAULT '0' COMMENT '删除标志位',
            `birthday` bigint(20) NOT NULL COMMENT '创建时间',
            `live` tinyint(2) NOT NULL default '1' COMMENT '宠物生存状态',
            `lifeadded` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '附加寿命',
            
            PRIMARY KEY (`pet_id`)
          ) ENGINE=MyISAM DEFAULT CHARSET=utf8 CHECKSUM=1 ROW_FORMAT=DYNAMIC COMMENT='宠物表';

CREATE TABLE `vip_stall` (                                       
             `StallID` tinyint(2) unsigned NOT NULL COMMENT 'VIP摊位序号',  
             `RoleID` int(10) unsigned NOT NULL COMMENT '所有者ID',         
             `RemainTime` int(10) NOT NULL COMMENT '剩余时间',              
             PRIMARY KEY (`StallID`)                                        
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE `pet_skill` (                                               
             `petskill_typeid` int(11) unsigned NOT NULL COMMENT '宠物技能id',  
             `petid` int(11) unsigned NOT NULL COMMENT '宠物id',                  
             `para1` int(11) unsigned DEFAULT NULL COMMENT '参数1',      
             `para2` int(11) unsigned DEFAULT NULL COMMENT '参数2',      
             PRIMARY KEY (`petskill_typeid`,`petid`),                              
             KEY `petid` (`petid`)                                              
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8;                               

CREATE TABLE `guild_commodity` (                                                                 
                   `role_id` int(10) unsigned NOT NULL COMMENT '玩家ID',                                        
                   `guild_id` int(10) unsigned NOT NULL COMMENT '所在帮派ID',                                 
                   `role_level` smallint(3) unsigned NOT NULL DEFAULT '1' COMMENT '开始跑商时玩家等级',  
                   `tael` int(10) NOT NULL DEFAULT '0' COMMENT '当前商银数量',                              
                   `goods` tinyblob COMMENT '商货信息',                                                       
                   PRIMARY KEY (`role_id`)                                                                        
                 ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
     
CREATE TABLE `guild_skill` (                                         
               `guild_id` int(10) unsigned NOT NULL COMMENT '帮派ID',           
               `skill_id` int(10) unsigned NOT NULL COMMENT '技能ID',           
               `progress` int(10) unsigned NOT NULL COMMENT '升级进度',       
               `level` int(2) unsigned NOT NULL COMMENT '当前等级',           
               `researching` tinyint(1) NOT NULL COMMENT '当前正在研究中',
               `active` tinyint(1) NOT NULL COMMENT '当前已经激活了,0表示未激活'  
             ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE `vip_netbar` (                                                                                  
              `accountid` int(11) unsigned NOT NULL COMMENT '账号id',                                                  
              `login_time` int(11) unsigned NOT NULL COMMENT '上次在金牌网吧登录时间',                        
              PRIMARY KEY (`accountid`)                                                                                  
            ) ENGINE=InnoDB DEFAULT CHARSET=utf8 CHECKSUM=1 ROW_FORMAT=DYNAMIC COMMENT='金牌网吧';


CREATE TABLE `guild_upgrade` (                                           
                 `guild_id` int(10) unsigned NOT NULL COMMENT '设施所属帮派',     
                 `type` int(2) unsigned NOT NULL COMMENT '帮派设施类型',          
                 `level` int(2) unsigned NOT NULL COMMENT '设施等级',               
                 `progress` int(10) unsigned NOT NULL COMMENT '升级进度',           
                 `item_type_1` int(10) unsigned NOT NULL COMMENT '需求物品类型',  
                 `item_neednum_1` int(10) unsigned NOT NULL COMMENT '需求的数量',  
                 `item_type_2` int(10) unsigned NOT NULL,                               
                 `item_neednum_2` int(10) unsigned NOT NULL,                            
                 `item_type_3` int(10) unsigned NOT NULL,                               
                 `item_neednum_3` int(10) unsigned NOT NULL,                            
                 `item_type_4` int(10) unsigned NOT NULL,                               
                 `item_neednum_4` int(10) unsigned NOT NULL                             
               ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
               
CREATE TABLE `commerce_rank` (                                             
                 `role_id` int(10) unsigned NOT NULL COMMENT '玩家ID',                  
                 `guild_id` int(10) unsigned NOT NULL COMMENT '帮派ID',                 
                 `times` int(10) NOT NULL DEFAULT '0' COMMENT '完成跑商次数',       
                 `tael` int(10) NOT NULL DEFAULT '0' COMMENT '为帮派贡献的商银',  
                 PRIMARY KEY (`role_id`)                                                  
               ) ENGINE=MyISAM DEFAULT CHARSET=utf8;    
               
create table `activity` (   
		 `dwID` int (10)   NOT NULL ,  
		 `ScriptData` blob   NULL  , 
		 `ScriptData1` blob   NULL  , 
		 `ScriptData2` blob   NULL  , 
		 `ScriptData3` blob   NULL  , 
		 `ScriptData4` blob   NULL  , 
		 PRIMARY KEY ( `dwID` )  
		) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='固定活动数据';           

CREATE TABLE `left_msg` (                                                                                    
            `msg_id` int(11) unsigned NOT NULL COMMENT '留言日期时间',                                           
            `roleid` int(11) unsigned NOT NULL COMMENT '玩家id',                                                     
            `msg_data` tinyblob COMMENT '留言内容',                                                                
            PRIMARY KEY (`msg_id`,`roleid`)                                                                            
          ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='留言数据';

CREATE TABLE `centralbilling` (                                    
                  `TOKENID` int(11) NOT NULL COMMENT '令牌ID',                   
                  `USERNAME` varchar(50) NOT NULL COMMENT '帐号',
                  `CHANNELID` int(10) unsigned  NOT NULL DEFAULT '0' COMMENT '运营商ID',                
                  `WORLDID` int(11) DEFAULT NULL COMMENT '服ID',                  
                  `ZONEID` int(11) NOT NULL COMMENT '区ID',                       
                  `YUANORITEM` tinyint(1) NOT NULL COMMENT '0 元宝；1 道具',  
                  `ITEMCODE` int(11) DEFAULT NULL COMMENT '道具编号',          
                  `QTY` int(11) NOT NULL DEFAULT '0' COMMENT '数量',             
                  `STATE` int(11) NOT NULL DEFAULT '0' COMMENT '状态',           
                  `INSERTDATE` datetime NOT NULL COMMENT '插入时间',           
                  PRIMARY KEY (`TOKENID`)                                          
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `centralbilling_log` LIKE `centralbilling`;

# 创建实力排行记录表
CREATE TABLE `strength_rankings` (                     
	`Num` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '名次',
	`PreNum` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '上次名次',
	`RoleName` varchar(32) character set utf8 collate utf8_general_ci default 'N/A' NOT NULL comment '角色名', 
	`YaoJingValue` int(10) unsigned NOT NULL Default '0' COMMENT '角色妖精实力值',
	`RoleID` int(10) unsigned NOT NULL Default '0' COMMENT '角色ID',
	PRIMARY KEY  (`RoleName`)
	)ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='实力排行';


#数据库增加索引
#ALTER TABLE `item_del` ADD KEY (`SerialNum`);
ALTER TABLE `yuanbaoorder` ADD KEY (`OrderMode`);
ALTER TABLE `yuanbaoorder` ADD KEY (`RoleID`);
ALTER TABLE `guild_upgrade` ADD KEY (`guild_id`);	
ALTER TABLE `guild_skill` ADD KEY (`guild_id`);
ALTER TABLE `guild_commodity` ADD KEY (`guild_id`);
ALTER TABLE `commerce_rank` ADD KEY (`guild_id`);
ALTER TABLE `roledata` ADD KEY (`RemoveFlag`);
#ALTER TABLE `item_needlog` ADD KEY (`NeedLog`);
ALTER TABLE `group_purchase` ADD KEY (`GuildID`);
ALTER TABLE `group_purchase` ADD KEY (`RoleID`);
ALTER TABLE `group_purchase` ADD KEY (`MallID`);
ALTER TABLE `pet_data` ADD KEY (`master_id`);
ALTER TABLE `yuanbaoorder` ADD KEY (`StartTime`);
#ALTER TABLE `account` ADD KEY (`worldname_crc`);
ALTER TABLE `roledata` ADD KEY (`rolename`);
ALTER TABLE `account_common` ADD KEY (`AccountName`);
ALTER TABLE `task_done` ADD KEY (`RoleID`);
ALTER TABLE `roledata` ADD KEY (`masterid`);
  
  
# Jason 外部链接表
CREATE TABLE `external_links` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `LinkName` varchar(30) NOT NULL DEFAULT '"NoName"' COMMENT '??????',
  `LinkURL` varchar(512) DEFAULT NULL COMMENT '????URL',
  PRIMARY KEY (`ID`),
  KEY `LinkName` (`LinkName`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
  
# 记录改名记录的表  
CREATE TABLE `name_history` (               
	`AccountID` int(10) unsigned NOT NULL COMMENT '账号ID',
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
	`name_1` varchar(32) NOT NULL DEFAULT '0',
	`name_2` varchar(32) NOT NULL DEFAULT '0',
	`name_3` varchar(32) NOT NULL DEFAULT '0',
	`name_4` varchar(32) NOT NULL DEFAULT '0',
	`name_5` varchar(32) NOT NULL DEFAULT '0',
	`name_6` varchar(32) NOT NULL DEFAULT '0',
	`name_7` varchar(32) NOT NULL DEFAULT '0',
	`name_8` varchar(32) NOT NULL DEFAULT '0',
	`name_9` varchar(32) NOT NULL DEFAULT '0',
	`name_10` varchar(32) NOT NULL DEFAULT '0',
	PRIMARY KEY  (`RoleID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='该表记录角色最近使用的十个名字'; 
	
# 创建声望排行记录表
CREATE TABLE `fame_rankings` (                     
	`Num` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '名次',
	`PreNum` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '上次名次',
	`Fame` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '声望',
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
	`RoleName` varchar(32) NOT NULL COMMENT '角色名字',
	KEY (`Num`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='声望排行';

# 创建等级排行记录表
CREATE TABLE `level_rankings` (                     
	`Num` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '名次',
	`PreNum` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '上次名次',
	`Level` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT '加成过的等级值，用于等级排名',
	`RoleName` varchar(32) character set utf8 collate utf8_general_ci default 'N/A' NOT NULL comment '角色名字',
	`RoleID` int(10) unsigned NOT NULL Default '0' COMMENT '角色ID',
	KEY (`Num`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='等级排行';
		
# 创建宠物排行记录表
CREATE TABLE `pet_rankings` (                     
	`Num` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '名次',
	`PreNum` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '上次名次',
	`Pet` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '宠物实力值',
	`PetID` int(11) unsigned NOT NULL COMMENT '宠物id',
	`PetName` varchar(32) NOT NULL COMMENT  '宠物名称',
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
	KEY (`Num`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='宠物排行';

# 创建装备实力排行记录表
CREATE TABLE `equip_rankings` (                     
	`Num` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '名次',
	`PreNum` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '上次名次',
	`Equip` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '装备实力值',
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
	KEY (`Num`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='装备实力排行';
					
# 创建鲜花排行记录表
CREATE TABLE `like_rankings` (                     
	`Num` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '名次',
	`PreNum` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '上次名次',
	`flowerNum` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '获得鲜花数量',
	`RoleName` varchar(32) character set utf8 collate utf8_general_ci default 'N/A' NOT NULL comment '角色名字',
	`RoleID` int(10) unsigned NOT NULL Default '0' COMMENT '角色ID',
	KEY (`Num`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='等级排行';
	
# 创建鸡蛋排行记录表
CREATE TABLE `hate_rankings` (                     
	`Num` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '名次',
	`PreNum` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '上次名次',
	`EggNum` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '加成过的等级值，用于等级排名',
	`RoleName` varchar(32) character set utf8 collate utf8_general_ci default 'N/A' NOT NULL comment '角色名字',
	`RoleID` int(10) unsigned NOT NULL Default '0' COMMENT '角色ID',
	KEY (`Num`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='等级排行';
		
#  城市	
CREATE TABLE `city` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '城市id',
  `guild_id` int(10) unsigned DEFAULT '4294967295' COMMENT '帮派id',
  `defence` int(10) DEFAULT '3000' COMMENT '防御度',
  `eudemon_tally` int(10) DEFAULT '0' COMMENT '守护神契合度',
  `tax_rate` int(10) DEFAULT '0' COMMENT '税率',
  `tax_rate_time` int(10) unsigned DEFAULT '0' COMMENT '最近一次更新税率的时间',
  `taxation` int(10) DEFAULT '0' COMMENT '税金',
  `prolificacy` int(10) DEFAULT '500' COMMENT '生产力',
  `signup_list` blob COMMENT '报名列表',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='城市';

# 师徒
create table `master` (   
		`dwID` int (10)   NOT NULL ,  
		`JingWuPoint` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '精武点数',
		`QinWuPoint` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '勤武点数',
		`LevelUpBonus` mediumint(8) NOT NULL DEFAULT '0' COMMENT '徒弟升级积分',
		`RewardNum` tinyint(3) NOT NULL DEFAULT '0' COMMENT '出师奖励次数',
		`ApprenticeData`  blob COMMENT '徒弟ID',
		 PRIMARY KEY ( `dwID` )  
		) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='师门数据';     
# 快捷栏

CREATE TABLE `shortcut_quickbar` (               
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
	`quickbardata` blob COMMENT '快捷栏数据',
	PRIMARY KEY  (`RoleID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 ; 

# 竞技场

CREATE TABLE `arena_data` (                                                                
              `serial_id` int(10) unsigned NOT NULL  COMMENT '序号',                        
              `system_id` int(10) unsigned NOT NULL  COMMENT '竞技场系统ID',                
              `serial_val` int(10) unsigned NOT NULL COMMENT '序列号值',                   
              `time_range` varchar(32) NOT NULL  COMMENT '保存的时间段',  
              `hall_persom_num` int(10) unsigned NOT NULL  COMMENT '大厅总人数',            
              `hall_room_num` int(10) unsigned NOT NULL    COMMENT '大厅房间数',              
              `fighting_room_num` int(10) unsigned NOT NULL COMMENT '战斗状态的房间数',    
              `below120Second` int(10) unsigned NOT NULL  COMMENT '低于120秒的场数',          
              `below210Second` int(10) unsigned NOT NULL  COMMENT '低于210秒的场数',        
              `below300Second` int(10) unsigned NOT NULL  COMMENT '低于300秒的场数',        
              `over300Second` int(10) unsigned NOT NULL   COMMENT '高于300秒的场数',         
              PRIMARY KEY (`serial_id`)                                                                
            ) ENGINE=MyISAM DEFAULT CHARSET=utf8  ;

# 小龙女查询表

CREATE TABLE `dragongirl_data` (                                                                
              `sign_id` int(10) unsigned NOT NULL  COMMENT '报名号',                        
              `role_id` int(10) unsigned NOT NULL  COMMENT '角色ID',                
              `role_name` varchar(32) NOT NULL  COMMENT '角色名字',  
              `point` int(10) unsigned NOT NULL COMMENT '得分',   
              `serial_id` int(10) unsigned NOT NULL COMMENT '序号ID',  
              `data_count` int(10) unsigned NOT NULL COMMENT '记录的条数',          
              PRIMARY KEY (`serial_id`)                                                                
            ) ENGINE=MyISAM DEFAULT CHARSET=utf8   ;

# 龙卫士查询表

CREATE TABLE `dragondefender_data` (                                                                                     `serial_id` int(10) unsigned NOT NULL  COMMENT '序列号ID', 
              `role_id` int(10) unsigned NOT NULL  COMMENT '角色ID',                
              `role_name` varchar(32) NOT NULL  COMMENT '角色名字',  
              `point` int(10) unsigned NOT NULL COMMENT '得分', 
              `data_count` int(10) unsigned NOT NULL COMMENT '记录的条数',
                               
              PRIMARY KEY (`role_id`,`serial_id`)                                                                
            ) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;

# 世博会等活动 通用数据排行表  2.0.0
CREATE TABLE  `activity_rankdata` (                                                                                    
              `role_id` int(10) unsigned NOT NULL  COMMENT '角色ID',                
              `role_name` varchar(32) NOT NULL  COMMENT '角色名字',  
              `role_level` varchar(32) NOT NULL  COMMENT '角色等级',  
              `datatype1` int(10) unsigned NOT NULL COMMENT '字段1类型',
              `datatype2` int(10) unsigned NOT NULL COMMENT '字段2类型',
              `datatype3` int(10) unsigned NOT NULL COMMENT '字段3类型',
              `datatype4` int(10) unsigned NOT NULL COMMENT '字段4类型',
              `datatype5` int(10) unsigned NOT NULL COMMENT '字段5类型',
              `datatype6` int(10) unsigned NOT NULL COMMENT '字段6类型',        
              `datatype7` int(10) unsigned NOT NULL COMMENT '字段7类型',
              `datatype8` int(10) unsigned NOT NULL COMMENT '字段8类型',             
                PRIMARY KEY (`role_id`)                                                                
            ) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;  
		
#台湾金牌网吧红利点
CREATE TABLE `bonus` (
  `datetime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '插入时间',
  `accountname` varchar(36) DEFAULT NULL COMMENT '账号名',
  `charname` varchar(36) DEFAULT NULL COMMENT '用户名',
  `ip` varchar(20) DEFAULT NULL COMMENT '玩家的ip',
  `bonuspoint` int(10) DEFAULT NULL COMMENT '红利点'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;		
		
#龙之试炼
CREATE TABLE `train_state`(
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
	`TrainID` int(10) unsigned NOT NULL COMMENT '试炼ID',
	`TrainState` tinyint(2) unsigned NOT NULL  default '0' COMMENT '试炼状态(0:未开启; 1:进行中; 2:已通过(已经领取完奖励); 3:已完成(没有领取奖励))',
	`Completeness` int(10) unsigned default '0' NOT NULL COMMENT '试炼完成度',
	PRIMARY KEY (`RoleID`,`TrainID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='已经开启的龙之试炼状态表'; 
		
################## 所有新表在此之上加 #######################

	
CREATE TABLE `item_needlog` (               
	`TypeID` int(10) unsigned NOT NULL COMMENT '物品的TypeID(唯一)',
	`NeedLog` tinyint(2) unsigned NOT NULL default '1' COMMENT '是否记录log(0:不记录;1:记录)',
	`MinQlty` tinyint(2) NOT NULL default '0' COMMENT '需记录log的物品的最低品级',
	PRIMARY KEY  (`TypeID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='需做log的物品TypeID表，由运营填写'; 
	

#复制物品和装备表结构
CREATE TABLE `item_baibao` SELECT * FROM `item` WHERE 0;
ALTER TABLE `item_baibao` ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='物品表';
ALTER TABLE `item_baibao` ADD PRIMARY KEY  (`SerialNum`);
ALTER TABLE `item_baibao` ADD KEY (`AccountID`);

CREATE TABLE `equip_baibao` LIKE `equip`;

CREATE TABLE `item_del` LIKE `item`;
Alter table `item_del` drop primary key;

CREATE TABLE `equip_del` LIKE `equip`;
Alter table `equip_del` drop primary key;

CREATE TABLE `roledata_del` LIKE `roledata`;
ALTER TABLE `roledata_del` DROP PRIMARY KEY;
ALTER TABLE `roledata_del` DROP INDEX `RoleNameCrc`;


# Jason 2010-2-7 增加外部链接数据
delete from external_links;
insert into external_links (linkname,linkurl ) values( '攻略站首页', 'http://www.loong3d.com/info/index.html' );
insert into external_links (linkname,linkurl ) values( '新手入门专题', 'http://loong.17173.com/zt/xs/index.shtml');
insert into external_links (linkname,linkurl ) values( '日常活动专题', 'http://loong.17173.com/zt/act/index.html');
insert into external_links (linkname,linkurl ) values( '天资加点模拟器', 'http://zq.17173.com/loong/simulator/');
insert into external_links (linkname,linkurl ) values( '属性加点模拟器', 'http://games.sina.com.cn/o/z/loong/2009-11-30/1011357027.shtml');
insert into external_links (linkname,linkurl ) values( '玩家问题互助' ,'http://bk.17173.com/bkfactory/17173bk_zt/loong/loong_zt.html');


# Jason 2010-4-14 v2.0.0 添加法宝表
CREATE TABLE `fabao` (                                          
          `SerialNum` bigint(20) NOT NULL COMMENT '装备序列号',         
          `NativeIntelligence` smallint(4) NOT NULL COMMENT '法宝资质',  
          `Stage` tinyint(4) NOT NULL COMMENT '法宝等阶',               
          `PracticeValue` int(10) NOT NULL COMMENT '修炼值',            
          `AttTypes` tinyblob COMMENT '影响人物属性类型1',              
          `AttGrowings0` tinyblob COMMENT '属性增长值0,生成时值',                 
          `AttGrowings` tinyblob COMMENT '属性增长值1',                 
          `WuxingType` tinyint(4) DEFAULT NULL COMMENT '法宝主属性',    
          `WuxingHoles` tinyblob COMMENT '五行灵槽类型1',               
          `WuxingBeadIDs` tinyblob COMMENT '灵珠typeid1',               
          `SlotAttTypes` tinyblob COMMENT '灵槽契合奖励属性类型',       
          `SlotAttValue` tinyblob COMMENT '灵槽契合奖励属性值',         
          `WuxingMatchReward` tinyblob COMMENT '五行相生奖励',          
          `UseLevel` tinyint(4) NOT NULL COMMENT '使用级别限制',        
          `SlotNum` tinyint(1) NOT NULL COMMENT '灵槽个数',             
	  `SlotLingzhuMatch` tinyint COMMENT '灵珠契合个数',
	  `SlotMacthNdx` tinyblob COMMENT '灵珠契合灵槽索引数组',
	  #`AttLingzhu` tinyblob COMMENT '灵珠镶嵌奖励属性类型数组',
	  #`AttLingzhuValue` tinyblob COMMENT '灵珠镶嵌奖励属性类型数组',
	  `Mood` int(10) Default '999' COMMENT '妖精心情',
	  `MoodTicks` int(10) Default '0' COMMENT '妖精心情计数器',
	  `MoodBuffID` int(10) unsigned Default '0' COMMENT '妖精心情相关buff',
	  # 1.2.2
	  `EleInjuryType` tinyint(4) Default '0'	COMMENT '妖精元素伤害类型',
	  `EleInjury` int(10) Default '0'		COMMENT '妖精元素伤害值',
	  `ElePierce` int(10) Default '0'		COMMENT '元素伤害穿透值',
	  `EleInjPromoteTimes` int(10) Default '0'	COMMENT '伤害提升次数',
	  `EleResPromoteTimes` int(10) Default '0'	COMMENT '抗性提升次数',
	  `EleResisFir` int(10) Default '0'		COMMENT '火元素伤害',
	  `EleResisWater` int(10) Default '0'		COMMENT '水元素伤害',
	  `EleResisEarth` int(10) Default '0'		COMMENT '土元素伤害',
	  `EleResisWind` int(10) Default '0'		COMMENT '风元素伤害',
	  `EleResisBright` int(10) Default '0'		COMMENT '光明元素伤害',
	  `EleResisDark` int(10) Default '0'		COMMENT '暗黑元素伤害',
	  `DamageUpgradeTimes` int(10) Default '0' COMMENT '妖精元素伤害提升次数',
	  `ResistanceUpgradeTimes` int(10) Default '0' COMMENT '妖精元素抗性提升次数',
	  `FairySkill1` int(10) unsigned Default '4294967295' COMMENT '妖精特技槽1',
	  `FairySkill2` int(10) unsigned Default '4294967295' COMMENT '妖精特技槽2',
	  `FairySkill3` int(10) unsigned Default '4294967295' COMMENT '妖精特技槽3',
	  `FairySkill4` int(10) unsigned Default '4294967295' COMMENT '妖精特技槽4',
	  `FairySkill5` int(10) unsigned Default '4294967295' COMMENT '妖精特技槽5',
	  `UnsureSkill` int(10) unsigned Default '4294967295' COMMENT '待确认特技',
	  `StoredExpOpen` tinyint(1) unsigned Default '0' COMMENT '经验封印是否开启,0:关闭，1:开启',
	  `CurStoredExp` int(10) unsigned Default '0' COMMENT '封印的经验值',
	  
	  # Jason 2010-12-6 1.3.0
	  `FabaoStatus` int(10) NOT NULL default '0' COMMENT '妖精状态',
	  `StatusChangeTime` int(10) unsigned NOT NULL default '0' COMMENT '妖精状态改变倒计时时间',
	  `BirthValue` int(10) NOT NULL default '0' COMMENT '妖精生育值',
	  `BirthTimes` int(10) NOT NULL default '0' COMMENT '妖精可配对次数',
          `LevelBirth` tinyblob COMMENT '妖精配对次数升级记录',     
          `IsGivenBirthTimes`	tinyint NOT NULL default '0' COMMENT '是否生育过，补偿配对次数用',                                  
	  `MaxIntelligence` smallint(3) unsigned default 0 comment '资质上限',
          PRIMARY KEY (`SerialNum`)                                     
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# 日常活动更新表
CREATE TABLE `dailytask_done` (                                                                           
             `RoleID` int(10) unsigned NOT NULL,                                                                
             `TaskID` int(10) unsigned NOT NULL COMMENT '任务id',                                             
             `Times` smallint(6) unsigned NOT NULL COMMENT '一个时期内完成的任务次数',              
             `StartTime` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '任务接取开始时间',  
             PRIMARY KEY (`RoleID`,`TaskID`)                                                                    
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='已做过日常任务';

# 角色任务板任务的存储表           
 create table quest_board (
	`RoleID` int(10) unsigned NOT NULL,                                                                
	`QuestID` int(10) unsigned NOT NULL COMMENT '任务id',                                             
	`BoardID` int(10) unsigned NOT NULL default 0 COMMENT '任务板id',         
	`State` tinyint(3) NOT NULL default 0 COMMENT '任务状态 0为未接取 1为已接',
	`BoardType` tinyint(2) unsigned NOT NULL DEFAULT 1 COMMENT '任务板类别, 1为悬赏榜，2为天之行会任务板',
	PRIMARY KEY (`RoleID`,`QuestID`) 
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='任务板任务';
	
# 强制脱离师门申请表
CREATE TABLE `force_break_out_apprentice` (
             `RoleID` int(10) unsigned NOT NULL COMMENT '申请强行脱离师门的徒弟的角色ID',
             `MasterID` int(10) unsigned NOT NULL COMMENT '师傅的角色ID',
			 `BreakoutInsertTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '申请强行脱离师门的时间',
             PRIMARY KEY (`RoleID`)
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='强制脱离师门申请';
	   

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

#公会战宣战关系表
CREATE TABLE `guild_war_declare` (                                            
                     `AttackGuildID` int(10) unsigned NOT NULL COMMENT '宣战公会ID',             
                     `DefenceGuildID` int(10) unsigned NOT NULL COMMENT '被宣战公会ID',          
                     `WarDeclareTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '宣战时间',  
					 `WarEnded` tinyint(3) unsigned NOT NULL default '0' COMMENT '是不是已经结束过了,0表示没有结束过',
                     KEY `WarDeclareTime` (`WarDeclareTime`)                                     
                   ) ENGINE=MyISAM DEFAULT CHARSET=utf8;                                          

#战场进攻竞价报名表
CREATE TABLE `battlefield_attack_signup`(
			`BattleFieldID` tinyint(3) unsigned NOT NULL COMMENT '战场ID',
			`GuildID` int(10) unsigned NOT NULL COMMENT '竞价报名公会ID',
			`BidFund` int(10) unsigned NOT NULL default '0' COMMENT '竞价报名出价',
			KEY `GuildID` (`GuildID`)
			) ENGINE=MyISAM DEFAULT CHARSET=utf8 ; 
			
#战场同盟报名表
CREATE TABLE `battlefield_alliance_signup`(
			`BattleFieldID` tinyint(3) unsigned NOT NULL COMMENT '战场ID',
			`GuildID` int(10) unsigned NOT NULL COMMENT '同盟公会ID',
			`GuildCapType` tinyint(1) unsigned NOT NULL COMMENT '公会争夺判断',
			`BeConfirmed` tinyint(1) unsigned NOT NULL default '0' COMMENT '是否被确认,1表示已经确认了,0表示未确认',
			KEY `GuildID` (`GuildID`)
			) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;

# 记录当前地图上的在线人数
CREATE TABLE `mapolinfo` (              
             `MapID` int(10) unsigned NOT NULL,    
             `IsInst` int(10) DEFAULT '0'         COMMENT '是否为副本',
             `OnlineNum` int(10) DEFAULT '0'      COMMENT '在线人数',
             `OpenNum` mediumint(6) DEFAULT '0'   COMMENT '地图开启个数对副本有意义',
             `RecTime` datetime DEFAULT NULL          COMMENT '记录时间',
             PRIMARY KEY (`MapID`)                 
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8  ;

# 统计角色每月在线时长(在线时长单位为分钟)
CREATE TABLE `role_online_time_each_month` (
		`AccountID` int(10) unsigned NOT NULL default '0' COMMENT '账号ID',
		`RoleID` int(10) unsigned NOT NULL default '0' COMMENT '角色ID',
		`Year` tinyint(2) unsigned NOT NULL default '0' COMMENT '年份，2000年之后的，例如2001年在这里就是1',
		`January` smallint(2) unsigned NOT NULL default '0' COMMENT '1月份在线时长',
		`February` smallint(2) unsigned NOT NULL default '0' COMMENT '2月份在线时长',
		`March` smallint(2) unsigned NOT NULL default '0' COMMENT '3月份在线时长',
		`April` smallint(2) unsigned NOT NULL default '0' COMMENT '4月份在线时长',
		`May` smallint(2) unsigned NOT NULL default '0' COMMENT '5月份在线时长',
		`June` smallint(2) unsigned NOT NULL default '0' COMMENT '6月份在线时长',
		`July` smallint(2) unsigned NOT NULL default '0' COMMENT '7月份在线时长',
		`August` smallint(2) unsigned NOT NULL default '0' COMMENT '8月份在线时长',
		`September` smallint(2) unsigned NOT NULL default '0' COMMENT '9月份在线时长',
		`October` smallint(2) unsigned NOT NULL default '0' COMMENT '10月份在线时长',
		`November` smallint(2) unsigned NOT NULL default '0' COMMENT '11月份在线时长',
		`December` smallint(2) unsigned NOT NULL default '0' COMMENT '12月份在线时长',
		`LastUpdateTime`  datetime DEFAULT NULL COMMENT '最后一次修改记录的时间',
		PRIMARY KEY (`AccountID`,`RoleID`,`Year`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8  ;

#增加fame_rankings_pre,equip_rankings_pre,level_rankings_pre,strength_rankings_pre,pet_rankings_pre,like_rankings_pre,hate_rankings_pre这七个表
create table fame_rankings_pre like fame_rankings;
create table equip_rankings_pre like equip_rankings;
create table level_rankings_pre like level_rankings;
create table strength_rankings_pre like strength_rankings;
create table pet_rankings_pre like pet_rankings;
create table like_rankings_pre like like_rankings;
create table hate_rankings_pre like hate_rankings;

# 分类统计各个媒体的在线人数
CREATE TABLE `mediaonline` (              
	    `DistributionID` int(10) unsigned NOT NULL Default '0' COMMENT '媒体ID',
            `OnlineNum` int(10) DEFAULT '0' COMMENT '在线人数',
            `RecTime` datetime DEFAULT NULL COMMENT '记录时间',
             PRIMARY KEY (`DistributionID`)
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8  ;

# 商城限量物品的信息
CREATE TABLE `LimitNumItemInfo`(
            `Id` int(10) unsigned NOT NULL  COMMENT '商品信息序列号',
            `ItemTypeID` int(10) unsigned NOT NULL COMMENT '物品类型id',
            `RemainNum`  int(10) unsigned NOT NULL COMMENT '该物品剩余个数',
            `IsEnd`       tinyint(2) NOT NULL COMMENT '是否销售结束',
            PRIMARY KEY(`Id`)
            )ENGINE=MyISAM DEFAULT CHARSET=utf8 ; 


# 妖精重生使用，
CREATE TABLE `fabao_extend_his` (                          
                    `SerialNum` bigint(20) NOT NULL, 
                    `AccountID` int(10) unsigned NOT NULL COMMENT '账号ID',
                    `RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',                        
                    `Stage` tinyint(4) NOT NULL                     COMMENT '妖精等级'        ,
                    `EleInjuryType` tinyint(4) NOT NULL             COMMENT '元素伤害类型'     ,  
                    `EleInjury` int(10) DEFAULT '0'                 COMMENT '元素伤害值'        ,
                    `ElePierce` int(10) Default '0'		COMMENT '元素伤害穿透值',
                    `EleInjPromoteTimes` int(10) Default '0'	COMMENT '伤害提升次数',
                    `EleResPromoteTimes` int(10) Default '0'	COMMENT '抗性提升次数',
                    `EleResisFire` int(10) DEFAULT '0'              COMMENT '火元素抗性'        ,
                    `EleResisWater` int(10) DEFAULT '0'             COMMENT '水元素抗性'        ,
                    `EleResisEarth` int(10) DEFAULT '0'             COMMENT '土元素抗性'        ,
                    `EleResisWind` int(10) DEFAULT '0'              COMMENT '风元素抗性'        ,
                    `EleResisBright` int(10) DEFAULT '0'            COMMENT '光明元素抗性'       , 
                    `EleResisDark` int(10) DEFAULT '0'              COMMENT '暗黑元素抗性'        ,
                    `DamageUpgradeTimes` int(10) DEFAULT '0'        COMMENT '元素伤害提升能力'     ,   
                    `ResistanceUpgradeTimes` int(10) DEFAULT '0'    COMMENT '元素抗性提升能力',
                    PRIMARY KEY (`SerialNum`,`Stage`),             
		      KEY `SerialNum` (`SerialNum`),
		      KEY `Stage` (`Stage`),
		     KEY `AccountID` (`AccountID`),
		     KEY `RoleID` (`RoleID`)
                  ) ENGINE=MyISAM DEFAULT CHARSET=utf8  ;
                  
# 武勋排行榜，                 
CREATE TABLE `wuxun_rankings` (                                           
                  `Num` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '名次',         
                  `PreNum` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '上次名次',  
                  `wuxun` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '武勋',      
                  `RoleName` varchar(32) NOT NULL DEFAULT 'N/A' COMMENT '角色名', 
                  `RoleID` int(10) unsigned NOT NULL Default '0' COMMENT '角色ID',        
                  PRIMARY KEY (`RoleName`)                                                
                ) ENGINE=MyISAM DEFAULT CHARSET=utf8       ;


# 武勋排行榜历史表，  
create table wuxun_rankings_pre like wuxun_rankings;

#神之竞标
create table `god_bid`(
				`RoleID` int(10) unsigned NOT NULL Default '0' COMMENT '角色ID',
				`Bidden` bigint(20) NOT NULL DEFAULT '0' COMMENT '竞标出价',
				`BidType` tinyint(1) unsigned NOT NULL Default '0' COMMENT '竞标类型,普通模式-0, 黑市金币-1, 黑市元宝-2',
				`BidID` int(10) unsigned NOT NULL Default '0' COMMENT '竞标ID',
				PRIMARY KEY (`RoleID`,`BidID`)
				)ENGINE=MyISAM DEFAULT CHARSET=utf8;

#砸金蛋
create table `egg_broker`(
				`RoleID` int(10) unsigned NOT NULL Default '0' COMMENT '角色ID',
				`CurLayer` tinyint(2) unsigned NOT NULL Default '0' COMMENT '当前可以砸的层',
				`ColourEggPos1` tinyint(2) NOT NULL Default '-1' COMMENT '彩蛋位置1',
				`ColourEggPos2` tinyint(2) NOT NULL Default '-1' COMMENT '彩蛋位置2',
				`BrokePos1` tinyint(2) NOT NULL Default '-1' COMMENT '1层砸中的位置',
				`ItemID1` int(10) unsigned NOT NULL Default '0' COMMENT '1层砸中的物品',
				`BrokePos2` tinyint(2) NOT NULL Default '-1' COMMENT '2层砸中的位置',
				`ItemID2` int(10) unsigned NOT NULL Default '0' COMMENT '2层砸中的物品',
				`BrokePos3` tinyint(2) NOT NULL Default '-1' COMMENT '3层砸中的位置',
				`ItemID3` int(10) unsigned NOT NULL Default '0' COMMENT '3层砸中的物品',
				`BrokePos4` tinyint(2) NOT NULL Default '-1' COMMENT '4层砸中的位置',
				`ItemID4` int(10) unsigned NOT NULL Default '0' COMMENT '4层砸中的物品',
				`BrokePos5` tinyint(2) NOT NULL Default '-1' COMMENT '5层砸中的位置',
				`ItemID5` int(10) unsigned NOT NULL Default '0' COMMENT '5层砸中的物品',
				`BrokePos6` tinyint(2) NOT NULL Default '-1' COMMENT '6层砸中的位置',
				`ItemID6` int(10) unsigned NOT NULL Default '0' COMMENT '6层砸中的物品',
				`BrokePos7` tinyint(2) NOT NULL Default '-1' COMMENT '7层砸中的位置',
				`ItemID7` int(10) unsigned NOT NULL Default '0' COMMENT '7层砸中的物品',
				PRIMARY KEY (`RoleID`)
				)ENGINE=MyISAM DEFAULT CHARSET=utf8;

# 衣橱系统
create table `wardrobe`(
				`RoleID` int(10) unsigned NOT NULL Default '0' COMMENT '角色ID',
				`LayerID` int(10) unsigned NOT NULL Default '0' COMMENT '衣橱栏位ID',
				`Level` tinyint(2) NOT NULL Default '1' COMMENT '衣橱栏等级',
				`State` tinyint(2) NOT NULL Default '0' COMMENT '衣橱栏状态,0-空，一件时装也没有;1-没有集齐，有几件时装;2-集齐，未使用;3-自己穿戴中;4-好友装扮中',
				`PlayActStartTime` int(10) unsigned NOT NULL Default '0' COMMENT '装扮开始时间',
				`PlayActFriendID` int(10) unsigned NOT NULL Default '0' COMMENT '装扮的好友ID',
				PRIMARY KEY (`RoleID`,`LayerID`)
				)ENGINE=MyISAM DEFAULT CHARSET=utf8;

#神职竞选
drop table if EXISTS Clergy_Mars;
create table `Clergy_Mars` (
				`Num` tinyint(2) unsigned NOT NULL COMMENT '名次',
				`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
				`Sex` tinyint(2) unsigned NOT NULL DEFAULT '3' COMMENT '性别',
				`Clergy` int(10) unsigned NOT NULL COMMENT '职位',
				`ClergyPoint` int(10) unsigned NOT NULL COMMENT '分值',
				`Clergy4SeniorOrJunior` tinyint(2) default 0,
				`RecvOrNot` bool Default 0 COMMENT '职位是否被领取',
				KEY (`Num`)			
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

drop table if EXISTS Clergy_Apollo;
create table `Clergy_Apollo` (
				`Num` tinyint(2) unsigned NOT NULL COMMENT '名次',
				`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
				`Sex` tinyint(2) unsigned NOT NULL DEFAULT '3' COMMENT '性别',
				`Clergy` int(10) unsigned NOT NULL COMMENT '职位',
				`ClergyPoint` int(10) unsigned NOT NULL COMMENT '分值',
				`Clergy4SeniorOrJunior` tinyint(2) default 0,
				`RecvOrNot` bool Default 0 COMMENT '职位是否被领取',
				KEY (`Num`)			
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

drop table if EXISTS Clergy_Rabbi;
create table `Clergy_Rabbi` (
				`Num` tinyint(2) unsigned NOT NULL COMMENT '名次',
				`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
				`Sex` tinyint(2) unsigned NOT NULL DEFAULT '3' COMMENT '性别', 
				`Clergy` int(10) unsigned NOT NULL COMMENT '职位',
				`ClergyPoint` int(10) unsigned NOT NULL COMMENT '分值',
				`Clergy4SeniorOrJunior` tinyint(2) default 0,
				`RecvOrNot` bool Default 0 COMMENT '职位是否被领取',
				KEY (`Num`)			
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

drop table if EXISTS Clergy_Peace;
create table `Clergy_Peace` (
				`Num` tinyint(2) unsigned NOT NULL COMMENT '名次',
				`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID',
				`Sex` tinyint(2) unsigned NOT NULL DEFAULT '3' COMMENT '性别', 
				`Clergy` int(10) unsigned NOT NULL COMMENT '职位',
				`ClergyPoint` int(10) unsigned NOT NULL COMMENT '分值',
				`Clergy4SeniorOrJunior` tinyint(2) default 0,
				`RecvOrNot` bool Default 0 COMMENT '职位是否被领取',
				KEY (`Num`)			
)ENGINE=MyISAM DEFAULT CHARSET=utf8;


drop table if EXISTS area;
CREATE TABLE `area` (                                                        
          `AreaID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '领域ID',  
          `RoleID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色ID',  
          `Activation` tinyint(3) unsigned DEFAULT '0' COMMENT '激活状态', 
	  `CDTime` int(10) unsigned DEFAULT '0' COMMENT 'cd时间',
          PRIMARY KEY (`AreaID`,`RoleID`)                              
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

drop table if EXISTS rarearea;
CREATE TABLE `rarearea` (                                               
            `AreaID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '领域表ID',        
            `ItemID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '领域卡表ID',      
            `DataID` bigint(20) NOT NULL DEFAULT '0' COMMENT '领域卡唯一ID或角色ID',  
            `State` smallint(3) DEFAULT '0' COMMENT '状态',                           
            `LeaveTime` int(10) DEFAULT '0' COMMENT '剩余有效时间',                   
            `RecordTime` int(10) unsigned DEFAULT '0' COMMENT '下线时间',                    
            PRIMARY KEY (`AreaID`,`DataID`)                        
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;


############################## F-Plan 1.4.1 start############################################


############################## F-Plan 1.4.1 end############################################


############################## F-Plan 1.4.2 start############################################
#家族表
DROP TABLE IF EXISTS `family`;
CREATE TABLE `family` (
		`FamilyID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族ID',
		`FamilyName` varchar(32) NOT NULL  COMMENT '家族名字',
		`LeaderID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '族长ID',
		`FounderID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '创始人ID',
		`Active`  int(10) NOT NULL DEFAULT '0' COMMENT '家族活跃度',
		`CreateTime` datetime  NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '家族创建时间',
		PRIMARY KEY(`FamilyID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;
#家族成员表
DROP TABLE IF EXISTS `family_member`;
CREATE TABLE `family_member` (
		`RoleID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色ID',  
		`FamilyID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族ID',
		`JoinTime` datetime  NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '加入家族时间',
		`IsLeader` tinyint(2) NOT NULL DEFAULT '0' COMMENT '是否是族长,0:否， 1:是',
		`BeRewarded` tinyint(2) NOT NULL DEFAULT '0' COMMENT '是否领取过奖励,0:否， 1:是',
		PRIMARY KEY(`FamilyID`,`RoleID`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8;


drop table if EXISTS family_sprite;
CREATE TABLE `family_sprite` (                                                        
          `FamilyID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族ID',  
          `Level` tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT '守护妖精等级', 
	  `Exp` bigint(20) NOT NULL DEFAULT '0' COMMENT '守护妖精成长度',  
	  `Name` varchar(32) NOT NULL COMMENT '守护妖精名称',  

	  `HP` int(10) DEFAULT '0' COMMENT '体力能力',
	  `EXAttack` int(10) DEFAULT '0' COMMENT '物理攻击能力',
	  `InAttack` int(10) DEFAULT '0' COMMENT '法术攻击',
	  `EXDefense` int(10) DEFAULT '0' COMMENT '物理防御',
	  `InDefense` int(10) DEFAULT '0' COMMENT '法术防御',
	  `EXAttackDeeper` int(10) DEFAULT '0' COMMENT '物理伤害加深',
	  `InAttackDeeper` int(10) DEFAULT '0' COMMENT '法术伤害加深',
	  `EXAttackResistance` int(10) DEFAULT '0' COMMENT '物理伤害减免',
	  `InAttackResistance` int(10) DEFAULT '0' COMMENT '法术伤害减免',
	  `Toughness` int(10) DEFAULT '0' COMMENT '韧性',
	  `CritDes` int(10) DEFAULT '0' COMMENT '暴击抵消',
	  `ControleffectDeepen` int(10) DEFAULT '0' COMMENT '控制效果加深',
	  `ControleffectResistance` int(10) DEFAULT '0' COMMENT '控制效果抵抗',
	  `SlowingeffectDeepen` int(10) DEFAULT '0' COMMENT '减速效果强化',
	  `SlowingeffectResistance` int(10) DEFAULT '0' COMMENT '减速效果抵抗',
	  `FixedeffectDeepen` int(10) DEFAULT '0' COMMENT '固定伤害强化',
	  `FixedeffectResistance` int(10) DEFAULT '0' COMMENT '固定伤害抵抗',
	  `AgingeffectDeepen` int(10) DEFAULT '0' COMMENT '衰弱效果强化',
	  `AgingeffectResistance` int(10) DEFAULT '0' COMMENT '衰弱效果抵抗',
	  
          PRIMARY KEY (`FamilyID`)                              
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;


drop table if EXISTS family_sprite_requirement;
CREATE TABLE `family_sprite_requirement` (                                                        
          `FamilyID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族ID', 
	  `RequirementID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '修炼需求ID',
	  `Complete` tinyint(3) unsigned DEFAULT '0' COMMENT '完成状态',
	  `RoleName` varchar(32) DEFAULT '' COMMENT '完成者名称', 
	  `Result` int(10) DEFAULT '0' COMMENT '实际完成值',
	  
          PRIMARY KEY (`FamilyID`,`RequirementID`)                              
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

drop table if EXISTS family_role_sprite;
CREATE TABLE `family_role_sprite` (                                                        
          `FamilyID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族ID', 
	  `SpriteID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '妖精TypeID',
	  `RoleName` varchar(32) DEFAULT '' COMMENT '登记者名称', 
	  `Shili` float(10) DEFAULT '0' COMMENT '实力值',
	  `Gongming` float(10) DEFAULT '0' COMMENT '共鸣值',
	  `RoleID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色ID', 
	  `Level` tinyint(4) unsigned DEFAULT '0' COMMENT '登记妖精等级',  
	  `Quality` tinyint(1) unsigned DEFAULT '0' COMMENT '登记妖精品阶',
          PRIMARY KEY (`FamilyID`,`RoleID`)                              
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# add by zr
drop table if EXISTS family_quest;
create table family_quest (
	`FamilyID` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '家族ID',
	LastQuestProcess smallint(5) not null default 0 COMMENT '昨天的任务完成度',
	IssueFlag tinyint(3) not null default 0 COMMENT '0未发布 1已经发布',
	QuestID1 int(10) unsigned not null default 0 COMMENT '家族任务1',
	QuestNum1 int(10) unsigned not null default 0 COMMENT '家族任务1计数',
	QuestID2 int(10) unsigned not null default 0 COMMENT '家族任务2',
	QuestNum2 int(10) unsigned not null default 0 COMMENT '家族任务2计数', 
	QuestID3 int(10) unsigned not null default 0 COMMENT '家族任务3',
	QuestNum3 int(10) unsigned not null default 0 COMMENT '家族任务3计数', 
	QuestID4 int(10) unsigned not null default 0 COMMENT '家族任务4',
	QuestNum4 int(10) unsigned not null default 0 COMMENT '家族任务4计数', 
	PRIMARY KEY (`FamilyID`)     
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

# add by zr
drop table if EXISTS role_buy_mall_item_count;
create table role_buy_mall_item_count(
	`RoleID` int(10) unsigned NOT NULL COMMENT '角色ID', 
	`MallItemID` int(10) unsigned NOT NULL comment '商城物品的ID',
	`Num` int(10) unsigned not null comment '购买次数'
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

############################## F-Plan 1.4.2 end############################################

# gm工具使用的表
drop table if exists gm_server_data;      
create table gm_server_data
(
	`data_name` char(32) not null,
	`data_value` bigint(20) default 0,
	key(`data_name`) 
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
insert into gm_server_data value ('MinItemSerial',0);
insert into gm_server_data value ('TigerProfit',0);

############################## F-Plan 1.4.9 start############################################
# by ZR
create table log_world_data
(
`Date` int(10) not null,
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
data100	int(10) unsigned,
unique key (`Date`)
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
insert into log_world_data (`Date`) values(1);

# by zr
create table log_role_data
(
RoleID	int(10) unsigned not null,
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
data20	int(10) unsigned
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

# by zr 1.5.0
create table SoulCrystal
(
	`SerialNum` bigint(20) NOT NULL COMMENT '64位id',
	`SoulLevel` tinyint(2) not null default 0,
	`SoulQlty` tinyint(3) unsigned not null,
	`SoulSkillID` int(10) unsigned not null,
	`SoulAttID1` int(10) unsigned default 0,
	`SoulAttID2` int(10) unsigned default 0,
	`SoulAttID3` int(10) unsigned default 0,
	`SoulAttID4` int(10) unsigned default 0,
	`SoulAttID5` int(10) unsigned default 0,
	`SoulAttID6` int(10) unsigned default 0,
	`SoulAttID7` int(10) unsigned default 0,
	`SoulAttID8` int(10) unsigned default 0,
	primary key(`SerialNum`)
)ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='魂晶';

create table Account_Reactive
(
	accountid int(10) unsigned NOT NULL COMMENT '账号ID',
	keycode int(10) unsigned not null,
	reward int(10) unsigned default 0,
	rewardlevel tinyint(2) unsigned default 0,
	primary key(accountid),
	key(keycode)
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

create table Account_Reactiver
(
	accountid int(10) unsigned NOT NULL COMMENT '账号ID',
	keycode int(10) unsigned not null default 0,
	level tinyint(3) unsigned not null default 0,
	exp int(10) unsigned not null default 0,
	primary key(accountid)
)ENGINE=MyISAM DEFAULT CHARSET=utf8;

############################# F-Plan 1.5.12 新Key码相关###################################
CREATE TABLE `UseKeyCode_roleID` (                         
             `RoleID` int(10) unsigned NOT NULL COMMENT '角色id', 
	      primary key(`RoleID`)           
           ); 
##元神表
CREATE TABLE  `HolySoulInfo`(  
              `SoulID` int(10) unsigned NOT NULL  COMMENT '当前元神ID',
	      `RoleID`  int(10) unsigned NOT NULL  COMMENT '角色ID',
              `CurLevelExp` bigint(20) NOT NULL COMMENT '当前经验等级',
              `CurSoulValue` int(10) unsigned NOT NULL  COMMENT '当前元神值',
              `CurCovalue` int(10) unsigned NOT NULL  COMMENT '当前默契值',       
              `HolySoulState`   int(10)  NOT NULL  COMMENT '元神状态',
              `RebornLv`  int(10)  NOT NULL  COMMENT '转身等级',
              `SoulLevel`  int(10)  NOT NULL  COMMENT '元神等级',
              `Demage`  int(10)  NOT NULL  COMMENT '元神绝技类伤害',
              `Hit`  int(10)  NOT NULL  COMMENT '攻击命中',
              `Crit`  int(10)  NOT NULL  COMMENT '攻击致命',
              `CritAmount`  int(10) unsigned NOT NULL  COMMENT '攻击致命量',
	      `Exattack`  int(10) unsigned NOT NULL  COMMENT '外功攻击',
              `Inattack`  int(10) unsigned NOT NULL  COMMENT '内功攻击',
              `Accrate`  int(10) unsigned NOT NULL  COMMENT '精准',
              `Exdamage`  int(10) unsigned NOT NULL  COMMENT '额外伤害',
              `Toughness`  int(10) unsigned  NOT NULL  COMMENT '忽视韧性',
              `Morale`  int(10) unsigned  NOT NULL  COMMENT '士气',
               PRIMARY KEY (`RoleID`)                                
            ) ENGINE=MyISAM DEFAULT CHARSET=utf8 ; 

############################## F-Plan 1.6.0 end############################################ 

##圣灵表
CREATE TABLE `holyman` (                                          
          `SerialNum` bigint(20) NOT NULL COMMENT '装备序列号',   
	  `DevourNum` int(10) NOT NULL COMMENT '吸收妖精的当前次数', 
	  `EquipmentNumber` smallint(4) NOT NULL COMMENT '圣灵可装备道具数量',
	  `ToDayDevourNum` int(10) NOT NULL COMMENT '当天吸收妖精的次数', 
	  `EquipSerialIDs` tinyblob COMMENT '圣纹64位Id', 
	  `CostHoly` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '装备圣纹所消耗的灵能',
	  `CoValue` int(10) NOT NULL COMMENT '默契值', 	  
	  `HolyDmg` int(10) NOT NULL COMMENT '圣灵伤害值', 
	  `HolyDef` int(10) NOT NULL COMMENT '圣灵防御值', 
	  `Crit` int(10) NOT NULL COMMENT '致命', 
	  `HolyCritRate` int(10) NOT NULL COMMENT '致命量', 
	  `ExDamage` int(10) NOT NULL COMMENT '伤害加深', 
	  `AttackTec` int(10) NOT NULL COMMENT '精准', 
	  `NeglectToughness` int(10) NOT NULL COMMENT '灵巧', 
	  `HolyValue` int(10) NOT NULL COMMENT '灵能值', 
	  `MaxDevourNum` int(10) NOT NULL COMMENT '最大吃妖精次数',   
          PRIMARY KEY (`SerialNum`)                                     
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

##圣纹表
CREATE TABLE `holyequip` (                                          
          `SerialNum` bigint(20) NOT NULL COMMENT '装备序列号',   
	  `CostHoly` int(10) NOT NULL COMMENT '装备该圣纹要消耗的灵能', 	
	  `EnhanceCount` smallint(4) unsigned NOT NULL DEFAULT '0' COMMENT '圣纹的充能次数',  
	  `HolyDmgChg` int(10) NOT NULL COMMENT '圣灵伤害影响值', 
	  `HolyDefChg` int(10) NOT NULL COMMENT '圣灵防御影响值', 
	  `CritChg` int(10) NOT NULL COMMENT '致命影响值', 
	  `HolyCritRateChg` int(10) NOT NULL COMMENT '致命量影响值', 
	  `ExDamageChg` int(10) NOT NULL COMMENT '伤害加深影响值', 
	  `AttackTecChg` int(10) NOT NULL COMMENT '精准影响值', 
	  `NeglectToughnessChg` int(10) NOT NULL COMMENT '灵巧影响值', 	   
          PRIMARY KEY (`SerialNum`)                                     
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

############################# F-Plan 1.6.10 ###################################
CREATE TABLE `ReciveYuanBaoDaiBi` (                         
             `RoleID` int(10) unsigned NOT NULL COMMENT '角色id' ,
             `Num` int(10) unsigned NOT NULL COMMENT '可领取的数量' ,
	      primary key(`RoleID`)           
           ) 
