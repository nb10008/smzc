Quest_Guild = {}
Quest_Guild[1]={TargetType=QuestTargetType["Kill"],TargetID1=1040021,Num1=40,RewMoney=0,RewXP=25200,MsgID=15001,}--魔狼【北洲天雪】
Quest_Guild[2]={TargetType=QuestTargetType["Kill"],TargetID1=1040023,Num1=40,RewMoney=0,RewXP=30100,MsgID=15002,}--魅影刺客【北洲天雪】
Quest_Guild[3]={TargetType=QuestTargetType["Kill"],TargetID1=1050001,Num1=40,RewMoney=0,RewXP=44100,MsgID=15003,}--幻林野猪【西洲幻林】
Quest_Guild[4]={TargetType=QuestTargetType["Kill"],TargetID1=1050007,Num1=40,RewMoney=0,RewXP=51450,MsgID=15004,}--巨熊【西洲幻林】
Quest_Guild[5]={TargetType=QuestTargetType["Kill"],TargetID1=1050017,Num1=60,RewMoney=0,RewXP=59850,MsgID=15005,}--三鬼教徒【西洲幻林】
Quest_Guild[6]={TargetType=QuestTargetType["Kill"],TargetID1=1060005,Num1=60,RewMoney=0,RewXP=77700,MsgID=15006,}--丛林袭击者【南洲迷沼】
Quest_Guild[7]={TargetType=QuestTargetType["Kill"],TargetID1=1060011,Num1=75,RewMoney=0,RewXP=107100,MsgID=15007,}--丛林暗杀者【南洲迷沼】
Quest_Guild[8]={TargetType=QuestTargetType["Kill"],TargetID1=1060023,Num1=75,RewMoney=0,RewXP=140700,MsgID=15008,}--嗜血蛇妖【南洲迷沼】
Quest_Guild[9]={TargetType=QuestTargetType["Kill"],TargetID1=1060032,Num1=90,RewMoney=0,RewXP=182000,MsgID=15009,}--原神教卫兵【东洲冥山】
Quest_Guild[10]={TargetType=QuestTargetType["Kill"],TargetID1=1070011,Num1=90,RewMoney=0,RewXP=225750,MsgID=15010,}--原神教教徒【东洲冥山】的【山隐秘窟】
Quest_Guild[11]={TargetType=QuestTargetType["Kill"],TargetID1=1070023,Num1=90,RewMoney=0,RewXP=275450,MsgID=15011,}--冥山龙【东洲冥山】
Quest_Guild[12]={TargetType=QuestTargetType["Kill"],TargetID1=1070048,Num1=90,RewMoney=0,RewXP=331100,MsgID=15012,}--地狱巨岩【东洲冥山】
Quest_Guild[13]={TargetType=QuestTargetType["Kill"],TargetID1=1040048,Num1=1,RewMoney=0,RewXP=25200,MsgID=15013,}--蛇妖杜莎拉【北洲天雪】的【冷心洞】
Quest_Guild[14]={TargetType=QuestTargetType["Kill"],TargetID1=1040049,Num1=1,RewMoney=0,RewXP=30100,MsgID=15014,}--北洲人王【北洲天雪】
Quest_Guild[15]={TargetType=QuestTargetType["Kill"],TargetID1=1050037,Num1=1,RewMoney=0,RewXP=44100,MsgID=15015,}--幻林蝶王【西洲幻林】
Quest_Guild[16]={TargetType=QuestTargetType["Kill"],TargetID1=1050041,Num1=1,RewMoney=0,RewXP=51450,MsgID=15016,}--痛苦之王【西洲幻林】
Quest_Guild[17]={TargetType=QuestTargetType["Kill"],TargetID1=1050042,Num1=1,RewMoney=0,RewXP=59850,MsgID=15017,}--魔蝶紫罗兰【西洲幻林】
Quest_Guild[18]={TargetType=QuestTargetType["Kill"],TargetID1=1060048,Num1=1,RewMoney=0,RewXP=77700,MsgID=15018,}--妖迷王【南洲迷沼】
Quest_Guild[19]={TargetType=QuestTargetType["Kill"],TargetID1=1060052,Num1=1,RewMoney=0,RewXP=107100,MsgID=15019,}--九天石魔【南洲迷沼】的【石狼洞】
Quest_Guild[20]={TargetType=QuestTargetType["Kill"],TargetID1=1060054,Num1=1,RewMoney=0,RewXP=140700,MsgID=15020,}--摄魂女皇【南洲迷沼】
Quest_Guild[21]={TargetType=QuestTargetType["Kill"],TargetID1=1070073,Num1=1,RewMoney=0,RewXP=182000,MsgID=15021,}--魔舞之花【东洲冥山】
Quest_Guild[22]={TargetType=QuestTargetType["Kill"],TargetID1=1070077,Num1=1,RewMoney=0,RewXP=225750,MsgID=15022,}--万年走尸【东洲冥山】
Quest_Guild[23]={TargetType=QuestTargetType["Kill"],TargetID1=1070080,Num1=1,RewMoney=0,RewXP=275450,MsgID=15023,}--鬼箭【东洲冥山】
Quest_Guild[24]={TargetType=QuestTargetType["Kill"],TargetID1=1070085,Num1=1,RewMoney=0,RewXP=331100,MsgID=15024,}--毒血【东洲冥山】


--接取任务
function q30002_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	local Index = 0
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if rolelevel >= 40 and rolelevel<= 99 then
		local temp = math.floor((rolelevel-40)/5)
		Index = temp + 1
	elseif rolelevel >= 100 then
		Index = 12
	end

	--初始化任务的动态数据
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_Guild, Index))
	--同步客户端总任务环数和任务说明
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_Guild[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

--接取任务
function q30003_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	local Index = 0
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if rolelevel >= 40 and rolelevel<= 99 then
		local temp = math.floor((rolelevel-40)/5)
		Index = temp + 13
	elseif rolelevel >= 100 then
		Index = 24
	end

	--初始化任务的动态数据
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_Guild, Index))
	--同步客户端总任务环数和任务说明
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_Guild[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

aux.RegisterQuestEvent(30001, 7, "q30001_OnQuestInit")
aux.RegisterQuestEvent(30002, 7, "q30002_OnQuestInit")
aux.RegisterQuestEvent(30003, 7, "q30003_OnQuestInit")

--完成任务
function qGuild_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	local Guild_Reward = {} --此表中奖励为3次之和
	Guild_Reward[1] = {Money=10929,Resource=6}
	Guild_Reward[2] = {Money=14995,Resource=12}
	Guild_Reward[3] = {Money=19063,Resource=12}
	Guild_Reward[4] = {Money=23202,Resource=12}
	Guild_Reward[5] = {Money=27769,Resource=15}

	--奖励玩家公会贡献
	local GuildID = guild.GetRoleGuildID(RoleID)
	local GuildLevel = guild.GetGuildAtt(GuildID)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel > 100 then
		rolelevel = 100
	end
	local a = 8--个人贡献,公会资金，公会资源
	local b = 5000
	local c = 5
	if QuestID == 30001 then
	a = 8
	elseif QuestID == 30002 then
	a = 15
	elseif QuestID == 30003 then
	a = 10
	end
	b = (Guild_Reward[GuildLevel].Money/3)*(1+rolelevel/400)
	c = Guild_Reward[GuildLevel].Resource/3

	guild.ModifyRoleContribute(GuildID, RoleID, a, 102)
	guild.ModifyGuildFund(GuildID, RoleID, b,102)
	guild.ModifyGuildMaterial(GuildID, RoleID, c, 102)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 403001)
	msg.AddMsgEvent(MsgID, 9, a)
	msg.AddMsgEvent(MsgID, 9, b)
	msg.AddMsgEvent(MsgID, 9, c)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

end
aux.RegisterQuestEvent(30001, 1, "qGuild_OnComplete")
aux.RegisterQuestEvent(30002, 1, "qGuild_OnComplete")
aux.RegisterQuestEvent(30003, 1, "qGuild_OnComplete")

function qGuild_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	--判断玩家是否已加入帮派
	local GuildID = guild.GetRoleGuildID(RoleID)
	if GuildID==4294967295 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 403002)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end
aux.RegisterQuestEvent(30001, 4, "qGuild_OnCheckAccept")
aux.RegisterQuestEvent(30002, 4, "qGuild_OnCheckAccept")
aux.RegisterQuestEvent(30003, 4, "qGuild_OnCheckAccept")

function qGuild_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local GuildID = guild.GetRoleGuildID(RoleID)
	if GuildID==4294967295 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 403002)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end
aux.RegisterQuestEvent(30002, 5, "qGuild_OnCheckComplete")
aux.RegisterQuestEvent(30003, 5, "qGuild_OnCheckComplete")
