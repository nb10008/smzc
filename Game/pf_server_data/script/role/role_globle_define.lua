--玩家脚本数据定义
--玩家脚本数据一共为200个，后面可按需求扩充
--版本对外之后，已经设定索引的脚本数据不能再换索引

RoleDataType = {}
RoleDataType["Injury"]								= 0		    --内伤治疗状态
RoleDataType["Chest_Rate"]							= 1	--玩家开启宝箱的节点掉率
RoleDataType["Chest_Step"]							= 2	--玩家开启宝箱的步骤
RoleDataType["BattleField_TalkIndex"]				= 3	--战场传送人对话步骤
RoleDataType["ShiMen_Step"]							= 4	--师门任务步骤
RoleDataType["ShiMen_Update"]						= 5	--师门任务更新时间
RoleDataType["DuoBaoQiBing_Bag"]					= 6	--玩家开启夺宝奇兵宝箱的次数
RoleDataType["Quest_50010"]							= 7	--玩家完成任务50010任务的次数
RoleDataType["Quest_50011"]							= 8	--玩家完成任务50011任务的次数
RoleDataType["Quest_50012"]							= 9	--玩家完成任务50012任务的次数
RoleDataType["Quest_50013"]							= 10	--玩家完成任务50013任务的次数
RoleDataType["Quest_50014"]							= 11	--玩家完成任务50014任务的次数
RoleDataType["Quest_50015"]							= 12	--玩家完成任务50015任务的次数
RoleDataType["Quest_50016"]							= 13	--玩家完成任务50016任务的次数
RoleDataType["Quest_50017"]							= 14	--玩家完成任务50017任务的次数
RoleDataType["Quest_50018"]							= 15	--玩家完成任务50018任务的次数
RoleDataType["Quest_50019"]							= 16	--玩家完成任务50019任务的次数
RoleDataType["Quest_50020"]							= 17	--玩家完成任务50020任务的次数
RoleDataType["Quest_50021"]							= 18	--玩家完成任务50021任务的次数
RoleDataType["Quest_50022"]							= 19	--玩家完成任务50022任务的次数
RoleDataType["Quest_50023"]							= 20	--玩家完成任务50023任务的次数
RoleDataType["Quest_50024"]							= 21	--玩家完成任务50024任务的次数
RoleDataType["Quest_50025"]							= 22	--玩家完成任务50025任务的次数
RoleDataType["Quest_50026"]							= 23	--玩家完成任务50026任务的次数
RoleDataType["TotleTimes_Arena"]    				= 24   --玩家参与竞技场的总次数
RoleDataType["WinTimes_Arena"]      				= 25   --玩家在竞技场中获得胜利的次数
RoleDataType["TimesToday_Arena"]    				= 26   --玩家今天参加竞技场比赛的次数
RoleDataType["Date_Arena"]          				= 27   --竞技场专用的今天的日期记录
RoleDataType["Point_Arena"]         				= 28   --记录玩家被预先扣掉但是还没有扣的分数
RoleDataType["TangZhi"]          					= 29   --记录玩家身上糖纸的数量
RoleDataType["RewardBoardQuestLvl"]          		= 30   --记录玩家身上悬赏榜的等级
RoleDataType["GuildBoardQuestLvl"]			  		= 31   --记录玩家身上公会任务板的等级
RoleDataType["YellowListBoardQuestLvl"]      		= 32   --记录玩家身上天之行会任务板的等级
RoleDataType["TodayYellowListQuestTimes"]      		= 33   --记录玩家当天接取天之行会任务板任务的次?
RoleDataType["ActivityZhiYin"]      				= 34   --百位为百鬼，十位为魔族入侵，各位为勇闯
RoleDataType["FarmLevel"]							= 35   --1,2位记录玩家农场等级；第3位记录播种，第4位记录浇水
RoleDataType["FarmExp"]								= 36   --农场经验
RoleDataType["FarmEnter"]							= 37   --农场进入等级初始化记录
RoleDataType["Chuandao"]							= 38   --传道任务每天进入场景的次数
RoleDataType["Xinnianshipin"]                       = 39   --交付新年大餐的次数
RoleDataType["Xinnianshipin_time"]                  = 40   --属于那一天的哪次活动
RoleDataType["Tianhuozhixia10_34"]                  = 41   --10-34级的记录
RoleDataType["Tianhuozhixia35_59"]                  = 42   --35-59级的记录
RoleDataType["Tequanrenwu"]                  		= 43   --玩家每天获得特权道具的个数
RoleDataType["LaBaZhou_Time"]                  		= 44   --玩家上次获得腊八粥材料的时间
RoleDataType["Tiandigong_quest"]                  	= 45   --春节活动天地宫兑换任务每天完成的次数
RoleDataType["Qingrenjie_quest"]                  	= 46   --情人节任务每天完成的次数
RoleDataType["Rose_Times"]                  	    = 47   --玫瑰的种植次数
RoleDataType["Qingrenjie_update"]                  	= 48   --情人节任务
RoleDataType["YYYJCollect"]                         = 49   --梦境粉尘任务次数
RoleDataType["XinyangCollect"]                      = 50   --信仰收集任务
RoleDataType["canAddFaith"]                         = 51   --当前获得信仰的天数
RoleDataType["canGetCoin"]                          = 52   --今天是否已经兑换了动物代币
RoleDataType["FaithToday"]                          = 53   --今天获得了多少信仰
RoleDataType["God_Quest"]                           = 54   --几个指引任务的完成情况
RoleDataType["God_Equip"]                           = 55   --玩家是否已经获得过一个圣衣武器
RoleDataType["God_SkillBook"]                       = 56   --记录玩家是否领取过某本书
RoleDataType["IsToday"]                             = 57   --今天是否更新过小活动的计数
RoleDataType["FengZhiShiZhe"]                       = 58   --今天完成了多少次风之使者
RoleDataType["MoJieZhiHua"]                         = 59   --今天完成了多少次魔界之花
RoleDataType["InstantKillCre"]                      = 60   --今天完成了多少次杀戮密令
RoleDataType["GuessFingers"]                        = 61   --今天完成了多少次猜拳
RoleDataType["BeiZhouTianXue"]                      = 62   --20300是否完成
RoleDataType["huntijifen"]                          = 63   --母亲节任务积分
RoleDataType["muqinyaojing"]                        = 64   --母亲节记录兑换魂体结晶个数
RoleDataType["muqin_time"]                          = 65   --玩家上次兑换的时间
RoleDataType["duanwujie"]                           = 66   --玩家上次兑换的时间
RoleDataType["duanwujie_time"]                      = 67   --玩家上次兑换的时间
RoleDataType["XiuLianChang_LingQu"]                 = 68   --玩家今天是否领取过免费门票
RoleDataType["XiuLianChang_Time"]                   = 69   --玩家身上的剩余修炼时间
RoleDataType["ShiMen_Step_qixu"]                    = 70   --师门次数记载
RoleDataType["QiZhiCePing"]							= 71   --气质测评领取奖励
RoleDataType["ShiShangPingLun"]						= 72   --时尚评论领取奖励
RoleDataType["Qixi"]								= 73   --七夕老人任务(相关内容已删除)
RoleDataType["Yewaizhuangbei"]						= 74   --野外30级、50级橙装和65级紫色戒指掉落记录（个位是30级橙装、十位是50级橙装、百位是65紫装）
RoleDataType["xinshouyaojing"]						= 75   --新手杀巨型怪刷妖精，个位记录25-30之间第一次击杀巨型怪，十位记录32-35之间第一次击杀巨型怪
RoleDataType["MidAutumn"]							= 76   --记录每日收集月光宝鉴数量
RoleDataType["MidAutumnTimes"]						= 77   --记录是否是当日
RoleDataType["tianyamingyue"]						= 78   --记录天涯明月的掉落（个位是掉落上限。十位是当天已掉落数量，百位是日期）
RoleDataType["manyuezhiguang"]						= 79   --记录满月之光的开启次数
RoleDataType["curExp"]								= 80   --记录积累经验
RoleDataType["gotExp"]								= 81   --记录当前已经领取奖励
RoleDataType["guoqingbaodao"]						= 82   --最后两位是报到天数,往前两位是缺勤天数，再往前一位是当日任务完成情况，再往前三位是最后一次报到的日期（每年的第几天），第一位记录当前经验扣除情况
RoleDataType["totalExp"]							= 83   --根据报到情况的理论总经验
RoleDataType["baiguiVIP"]							= 84   --记录百鬼炼狱完成日期
RoleDataType["zhenjiaVIP"]							= 85   --记录真假魔王完成日期
RoleDataType["shaluVIP"]							= 86   --记录杀戮密令完成日期
RoleDataType["mojieVIP"]							= 87   --记录魔界之花完成日期
RoleDataType["mozubaoku"]							= 88   --魔族宝库开启宝箱日期
RoleDataType["Tianxuanzhe_Point"]				    = 89   --天选者之战的分数
RoleDataType["ganenqifuxianling"]				    = 90   --感恩祈福活动领取仙令日期
RoleDataType["ganenqifulibao"]				    	= 91   --感恩祈福活动领取礼包日期
RoleDataType["Time_JBZDY"]				    	    = 92   --参与金币总动员的日期
RoleDataType["zuihoudezhanyi"]				    	= 93   --最后的战役开启天界宝箱日期
RoleDataType["shenqicifu"]				    		= 94   --神器赐福
RoleDataType["SDWI_State"]				    		= 95   --圣诞武器现在各个强化的状态
RoleDataType["SDWI_Times"]				    		= 96   --圣诞武器被强化的次数
RoleDataType["xinchunbaodao"]				    	= 97   --记录玩家上次领取签到令牌的日期
RoleDataType["clgjingyan"]				    		= 98   --记录玩家上次领取传令官经验的日期
RoleDataType["jileijingyan"]				    	= 99   --记录玩家经验积累的天数
RoleDataType["clgbaodao"]				    	    = 100   --记录玩家上次签到的日期
RoleDataType["baodaotianshu"]				    	= 101   --记录玩家连续报到的天数
RoleDataType["Jiaozishipin"]				    	= 102   --玩家交饺子个数
RoleDataType["jiaozishipin_time"]				    = 103   --玩家所处时间段
RoleDataType["jiaofucishu"]				   			= 104   --限定玩家每次交付次数
RoleDataType["jubaopen"]				   			= 105   --玩家开启聚宝盆次数
RoleDataType["qingrenjieqinwenID"]				   	= 106   --记录情人节任务中玩家上次亲吻玩家ID
RoleDataType["qingrenjieqinwenNum"]				   	= 107   --记录情人节任务中玩家亲吻玩家数量
RoleDataType["piaoliuping"]				   			= 108   --记录玩家调查漂流瓶的日期和次数
RoleDataType["kaiqipiaoliuping"]				   	= 109   --记录玩家开启漂流瓶的日期和次数
RoleDataType["qingrenjiezhi"]				   		= 110   --玩家是否领取过情人戒指
RoleDataType["shuijingxinlingqu"]				   	= 111   --水晶心领取情况记录
RoleDataType["jinqiantequan"]				   		= 112   --金钱特权领取记录
RoleDataType["YXmozubaoku"]							= 113   --英雄魔族宝库开启宝箱标记
RoleDataType["YXmozubaokuNUM"]							= 114   --英雄魔族宝库开启宝箱次数
RoleDataType["shenglingzhiyin"]							= 115   --圣灵指引任务时间


--[[
RoleDataType["RolePoint"]           = 25		-- 答题活动中玩家的总得分数
RoleDataType["RoleMapID"]           = 26        -- 答题活动中玩家所在的地图id
RoleDataType["RoleInstanceID"]      = 27        -- 答题活动中玩家所在的地图instanceid



RoleDataType["Act14_Expire"]        = 36	    --玩家上次参加英雄乱武活动的届数
RoleDataType["Act18_Expire"]        = 37	    --玩家上次参加群雄逐鹿活动的届数
RoleDataType["Act18_TeamKey"]       = 38        --玩家参加群雄乱舞活动时的小队编号

RoleDataType["Shengxiao_Val"]		= 40		--玩家生肖传说的完成记录


RoleDataType["PKstate"]		= 78	--玩家进入修炼场前的PK状态

RoleDataType["FristTime_GiveBag"]    = 79    --玩家是否第一次获得12908，12909，12911任务的包裹

RoleDataType["SDYL_Done"]			= 80	-- 完成一轮十殿阎罗活动

RoleDataType["YueYao_Pos"]                      =81    --记录月妖的随机坐标
RoleDataType["YueYaoZhiYing_Date"]                      =82    --记录月妖之影当日是否已经领取
RoleDataType["WeiDao_Exp"]		= 83	--当前卫道任务Index
RoleDataType["YueYaoZhiJing_Date"]                      =84    --记录月妖之镜当日是否已经领取
RoleDataType["FDGJ_QuestType"]                      =85    --记录接取的超度亡魂任务的类型。1为普通经验，2为稍好，3为高经验
RoleDataType["FDGJ_QuestID"]                        =86    --记录接取的超度亡魂任务的Index
RoleDataType["FishingLevel"]                        =87    --记录玩家的钓鱼熟练度等级
RoleDataType["FishingNum"]                     	   	=88    --记录玩家的钓鱼熟练度，升级后置0
RoleDataType["JinRuXiuLianChang"]                   =89    --记录玩家进入修炼场的次数

RoleDataType["CS_FX_TalkIndex"]		= 90	-- 城战的对话步骤
RoleDataType["fangwei"]				= 91	-- 城战任务环数
RoleDataType["fangwei_Update"]		= 92	-- 城战任务更新时间
RoleDataType["YinYangNum"]		= 93	--炼化太极活动中向阴阳太极鼎提交道具次数
RoleDataType["BiWuQieCuo_Kill"]		= 94	--比武切磋中获得的连杀数
RoleDataType["jinjieshili"]		= 95	--记录玩家获得的进阶实力值，副本

RoleDataType["NianHuo_Total"]			= 96	--年货任务总环数
RoleDataType["NianHuo_Update"]		= 97	--年货任务上次更新时间，用于检测今日完成次数

RoleDataType["XSXLC_EnterType"]		= 98	--玩家进入新手修炼场的模式

RoleDataType["jinjieshili_A"]		= 99	--记录玩家获得的进阶实力值,活动
RoleDataType["jinjieshili_Q"]		= 100	--记录玩家获得的进阶实力值，随机和每日任务
RoleDataType["jinjieshili_K"]		= 101	--记录玩家获得的进阶实力值，击杀非白名玩家
RoleDataType["PuTiYe_Num"]			= 102	--记录菩提叶任务每天的完成次数
RoleDataType["PuTiYe_Update"]		= 103	--记录菩提叶任务上次更新时间

RoleDataType["LeiFengMao"]			= 104	--记录雷锋帽的总共获取数量

RoleDataType["GraduateReward"]		= 105	--记录是否可以领取出师奖励

RoleDataType["ChuShiJiangLi"]		= 106	--出师奖励人NPC对话状态
RoleDataType["UseJingWuPoint"]		= 107	--记录玩家是否使用精武点数升级
RoleDataType["HaveGraduate"]		= 108	--徒弟是否领取过出师奖励


RoleDataType["TotleTimes_Arena"]    = 110   --玩家参与竞技场的总次数
RoleDataType["WinTimes_Arena"]      = 111   --玩家在竞技场中获得胜利的次数
RoleDataType["TimesToday_Arena"]      = 112   --玩家今天参加竞技场比赛的次数
RoleDataType["Date_Arena"]      = 113        --竞技场专用的今天的日期记录
RoleDataType["RankToday_Arena"]      = 114   --玩家今天获得的武勋

RoleDataType["Salary_Times"]			= 115    --玩家本周已领取工资的次数
RoleDataType["Last_Salary_M"]			= 116    --玩家上次领取工资的月份
RoleDataType["Have_FreePoint"]			= 117	--标记玩家是否已经领取35级免费赠点
RoleDataType["QingMing_Total"]			= 118	--清明活动任务本日完成次数，后世博字帖兑换使用
RoleDataType["QingMing_Update"]			= 119	--清明活动任务上次更新日期，后世博字帖兑换使用
RoleDataType["YuRen_Update"]			= 120	--清明愚人牌上次更新日期
RoleDataType["YuRen_Total"]				= 121	--清明愚人牌近日使用次数

RoleDataType["ShopDate_Arena"]      = 122        --竞技场专用的装备购买记录
RoleDataType["BuyEquip_Arena"]      = 123        --是否买了装备

RoleDataType["XJ_Dati_Date"]      = 124        --记录最后一次答题的时间

RoleDataType["Last_Recv_D"]			= 125		--上一次领取神迹经验奖励时间

RoleDataType["Last_PaoSang_D"]		= 126		--上一次领取跑商日期
RoleDataType["Last_PaoSang_C"]		= 127		-- 次跑商日期内接受跑商次

RoleDataType["Last_Instance"]		= 128		--上次领取副本完成奖励的副本id

RoleDataType["Last_JCXL_Day"]		= 129		--上次进入禁城修炼场日期

RoleDataType["FaBaoShiLian"]		= 130		--法宝的相关历练值

RoleDataType["VoteDate"]		= 131		--法宝的相关历练值
RoleDataType["SupportTeam"]		= 132		--法宝的相关历练值
RoleDataType["VoteNum"]			= 133		--法宝的相关历练值
RoleDataType["LastBuyWine"]		= 134		--法宝的相关历练值
RoleDataType["Wine_Fee_A"]		= 135		--法宝的相关历练值
RoleDataType["Wine_Fee_B"]		= 136		--法宝的相关历练值

RoleDataType["FeiSheng_shilian"]		= 137		--飞升相关试炼记录：是否杀掉BOSS：个位，炼器数量：十位百位 熔铸：千位。
RoleDataType["FeiShengDuihuan_times"]           = 138       --玩家兑换材料的次数
RoleDataType["FeiShengDuihuan_Update"]  = 139        --玩家上次兑换材料的时间

RoleDataType["LingPaiLingQu_Update"]           = 140       --玩家上次领取清剿匪类令德时间
RoleDataType["ChengZhuJingYan_Update"]           = 141       --玩家上次领取城主发布经验的时间
RoleDataType["FengXiangXiaoXiao_Update"]           = 142       --玩家上次领取帮派活动经验

RoleDataType["Q20315_Times"]           = 143       --玩家完成20315的次数
]]
