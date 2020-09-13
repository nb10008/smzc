--清空玩家异常数据
function s51_OnPlayerEnter(MapID, InstanceID,RoleID)
	local n = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmEnter"])
	if n == 0 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmEnter"], 1)
	end
end
aux.RegisterMapEvent("s51", 2, "s51_OnPlayerEnter")

--注册参加种植的玩家的数据
plant_data = {}

--[[
plant_data[RoleID] = {}
plant_data[RoleID] = nil]]

--存储经验
farm_exp = {}
farm_exp[0] = 0
farm_exp[1] = 10
farm_exp[2] = 640
farm_exp[3] = 7290
farm_exp[4] = 40960
farm_exp[5] = 156250
farm_exp[6] = 466560
farm_exp[7] = 1176490
farm_exp[8] = 2621440
farm_exp[9] = 0
--对应经验
seed_exp = {}
--二期收获
seed_exp[5800010] = 8
seed_exp[5800014] = 160
seed_exp[5800018] = 160
seed_exp[5800022] = 160
seed_exp[5800026] = 160
seed_exp[5800030] = 160
seed_exp[5800034] = 160
seed_exp[5800038] = 4
seed_exp[5800042] = 4
seed_exp[5800046] = 8
seed_exp[5800050] = 16
seed_exp[5800054] = 40
seed_exp[5800058] = 80
seed_exp[5800062] = 160
seed_exp[5800066] = 240
seed_exp[5800070] = 320
--护理
seed_exp[5800008] = 10
seed_exp[5800012] = 200
seed_exp[5800016] = 200
seed_exp[5800020] = 200
seed_exp[5800024] = 200
seed_exp[5800028] = 200
seed_exp[5800032] = 200
seed_exp[5800036] = 5
seed_exp[5800040] = 5
seed_exp[5800044] = 10
seed_exp[5800048] = 20
seed_exp[5800052] = 50
seed_exp[5800056] = 100
seed_exp[5800060] = 200
seed_exp[5800064] = 300
seed_exp[5800068] = 400
--一期收获
seed_exp[5800009] = 2
seed_exp[5800013] = 40
seed_exp[5800017] = 40
seed_exp[5800021] = 40
seed_exp[5800025] = 40
seed_exp[5800029] = 40
seed_exp[5800033] = 40
seed_exp[5800037] = 1
seed_exp[5800041] = 1
seed_exp[5800045] = 2
seed_exp[5800049] = 4
seed_exp[5800053] = 10
seed_exp[5800057] = 20
seed_exp[5800061] = 40
seed_exp[5800065] = 60
seed_exp[5800069] = 80



--[[定义全局变量
mound_point = {}
mound_point["MapID"]={}

mound_point = {}
mound_point.MapID ={}

mound_point = {MapID={}}
]]

--[[刷新
function f05_OnInit( MapID , InstanceID )
	local mound = {}
	for i=1 , 5 do
		mound[i] = {x = mound_point[i].x , y = mound_point[i].y , z = mound_point[i].z }
		map.MapCreateCreature(MapID, InstanceID, 5800001, mound[i].x , mound[i].y , mound[i].z)
	end
end
--注册农场地图开启
aux.RegisterMapEvent("s51", 0, "f05_OnInit")]]


-------------------------------播种任务完成--------------------------------------------------------
function q40053_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local questdata = (zero%1000-zero%100)/100
	if questdata ~= 0 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero-100)
	end
end

function q40053_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local bRet = 0
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local questdata = (zero%1000-zero%100)/100
	if questdata == 1 then
		bRet = 1
	else
		bRet = 0
	end
	return bRet
end
aux.RegisterQuestEvent(40053, 0, "q40053_OnAccept")
aux.RegisterQuestEvent(40053, 5, "q40053_CheckComplete")

function q40054_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local questdata = (zero%10000-zero%1000)/1000
	if questdata ~= 0 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero-1000)
	end
end

function q40054_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local bRet = 0
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local questdata = (zero%10000-zero%1000)/1000
	if questdata == 1 then
		bRet = 1
	else
		bRet = 0
	end
	return bRet
end
aux.RegisterQuestEvent(40054, 0, "q40054_OnAccept")
aux.RegisterQuestEvent(40054, 5, "q40054_CheckComplete")
----------------低级播种---------------------------------------------------------------------------------------
function mound_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local owner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local questdata = (zero%1000-zero%100)/100
	local farmnum = plant_data[RoleID]	--读取该玩家的已种植数量
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if farmnum == nil then	--如果这个玩家没有种植,给他的种植数量置0
		farmnum = 0
	end
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420001)	--您是否要播种？
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 12)		--香果之树返回12
		msg.AddMsgEvent(MsgID, 1,  420003)	--“我要种植香果之树”
		msg.AddMsgEvent(MsgID, 21, 13)		--火桃之树返回13
		msg.AddMsgEvent(MsgID, 1,  420004)	--“我要种植火桃之树”
		msg.AddMsgEvent(MsgID, 21, 11)		--元素之树返回11
		msg.AddMsgEvent(MsgID, 1,  420002)	--“我要种植元素之树”
		msg.AddMsgEvent(MsgID, 21, 14)		--玉子之树返回14
		msg.AddMsgEvent(MsgID, 1,  420005)	--“我要种植玉子之树”
		msg.AddMsgEvent(MsgID, 21, 15)		--水晶之树返回15
		msg.AddMsgEvent(MsgID, 1,  420006)	--“我要种植水晶之树”
		msg.AddMsgEvent(MsgID, 21, 16)		--紫梅之树返回16
		msg.AddMsgEvent(MsgID, 1,  420007)	--“我要种植紫梅之树”
		msg.AddMsgEvent(MsgID, 21, 17)		--椰米之树返回17
		msg.AddMsgEvent(MsgID, 1,  420008)	--“我要种植椰米之树”
		msg.AddMsgEvent(MsgID, 21, 10)		--取消返回10
		msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 11 then		--元素之种
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 2 then
				if farmnum < farmlevel then
					seed = 4100101
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--读取土坑坐标
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--删除种子
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--删除土坑
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800007, x, y, z)--在原土坑处刷新播种土堆
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--第一位记录播种角色id
						plant_data[RoleID] = farmnum  --把种植数量记入全局变量
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--提示没有种子
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--提示种植数量已满,不能种植
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--提取农场等级
					msg.AddMsgEvent(MsgID, 9, farmnum)			--提取种植数量
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--您的等级不足，无法种植
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--提示在这个地点的收获时间不足15分钟,无法再次种植
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then		--香果之树
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 1 then
				if farmnum < farmlevel then
					seed = 4100108
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--记录土坑坐标
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--删除种子
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--删除土坑
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800035, x, y, z)--在原土坑处刷新播种土堆
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--第一位记录播种角色id
						plant_data[RoleID] = farmnum  --把种植数量*10记入人物数据
						if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40053) == true then
							role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40053, 2)
							if questdata == 0 then
								role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+100)
							else
								role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
							end
						end
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--提示没有种子
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--提示种植数量已满,不能种植
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--提取农场等级
					msg.AddMsgEvent(MsgID, 9, farmnum)			--提取种植数量
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--您的等级不足，无法种植
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--提示在这个地点的收获时间不足15分钟,无法再次种植
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 13 then		--火桃之树
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 1 then
				if farmnum < farmlevel then
					seed = 4100109
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--记录土坑坐标
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--删除种子
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--删除土坑
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800039, x, y, z)--在原土坑处刷新播种土堆
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--第一位记录播种角色id
						plant_data[RoleID] = farmnum --把种植数量*10记入人物数据
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--提示没有种子
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--提示种植数量已满,不能种植
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--提取农场等级
					msg.AddMsgEvent(MsgID, 9, farmnum)			--提取种植数量
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID,  1, 420030)			--您的等级不足，无法种植
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--提示在这个地点的收获时间不足15分钟,无法再次种植
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 14 then		--玉子之树
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 2 then
				if farmnum < farmlevel then
					seed = 4100110
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--记录土坑坐标
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--删除种子
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--删除土坑
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800043, x, y, z)--在原土坑处刷新播种土堆
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--第一位记录播种角色id
						plant_data[RoleID] = farmnum	 --把种植数量*10记入人物数据
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--提示没有种子
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--提示种植数量已满,不能种植
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--提取农场等级
					msg.AddMsgEvent(MsgID, 9, farmnum)			--提取种植数量
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--您的等级不足，无法种植
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--提示在这个地点的收获时间不足15分钟,无法再次种植
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 15 then		--水晶之树
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 3 then
				if farmnum < farmlevel then
					seed = 4100111
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--记录土坑坐标
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--删除种子
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--删除土坑
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800047, x, y, z)--在原土坑处刷新播种土堆
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--第一位记录播种角色id
						plant_data[RoleID] = farmnum  --把种植数量*10记入人物数据
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--提示没有种子
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--提示种植数量已满,不能种植
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--提取农场等级
					msg.AddMsgEvent(MsgID, 9, farmnum)			--提取种植数量
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--您的等级不足，无法种植
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--提示在这个地点的收获时间不足15分钟,无法再次种植
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 16 then		--紫梅之树
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 4 then
				if farmnum < farmlevel then
					seed = 4100112
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--记录土坑坐标
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--删除种子
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--删除土坑
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800051, x, y, z)--在原土坑处刷新播种土堆
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--第一位记录播种角色id
						plant_data[RoleID] = farmnum 	  --把种植数量*10记入人物数据
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--提示没有种子
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--提示种植数量已满,不能种植
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--提取农场等级
					msg.AddMsgEvent(MsgID, 9, farmnum)			--提取种植数量
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--您的等级不足，无法种植
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--提示在这个地点的收获时间不足15分钟,无法再次种植
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 17 then		--yemi
		local cTime = os.time()
		local s = 0
		if lTime == 0 then
			s = 60*15
		else
			s = os.difftime(cTime,lTime)
		end
		if s >= 60*15 or owner ~= RoleID then
			if farmlevel >= 5 then
				if farmnum < farmlevel then
					seed = 4100113
					local num = role.GetRoleItemNum(RoleID, seed)
					if num >= 1 then
						local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)		--记录土坑坐标
						role.RemoveFromRole(MapID, InstanceID, RoleID, seed, 1, 420)	--删除种子
						map.MapDeleteCreature(MapID, InstanceID, TargetID)				--删除土坑
						local seedmound = map.MapCreateCreature(MapID, InstanceID, 5800055, x, y, z)--在原土坑处刷新播种土堆
						farmnum = farmnum + 1
						cre.SetCreatureScriptData(MapID, InstanceID, seedmound, 1, 1, RoleID)		--第一位记录播种角色id
						plant_data[RoleID] = farmnum  --把种植数量*10记入人物数据
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 1)
						msg.AddMsgEvent(MsgID, 1, 420010)			--提示没有种子
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420011)			--提示种植数量已满,不能种植
					msg.AddMsgEvent(MsgID, 9, farmlevel)		--提取农场等级
					msg.AddMsgEvent(MsgID, 9, farmnum)			--提取种植数量
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420030)			--您的等级不足，无法种植
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420012)			--提示在这个地点的收获时间不足15分钟,无法再次种植
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和低级土坑对话
aux.RegisterCreatureEvent(5800005, 7, "mound_OnTalk")


----------------高级播种---------------------------------------------------------------------------------------
function mound_advanced_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local owner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local questdata = (zero%1000-zero%100)/100
	local farmnum = plant_data[RoleID]	--读取该玩家的已种植数量
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if farmnum == nil then	--如果这个玩家没有种植,给他的种植数量置0
		farmnum = 0
	end
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420001)	--您是否要播种？
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 11)		--元素之树返回11
		msg.AddMsgEvent(MsgID, 1,  420039)	--“我要种植元素之树”
		msg.AddMsgEvent(MsgID, 21, 12)		--香果之树返回12
		msg.AddMsgEvent(MsgID, 1,  420040)	--“我要种植香果之树”
		msg.AddMsgEvent(MsgID, 21, 13)		--火桃之树返回13
		msg.AddMsgEvent(MsgID, 1,  420041)	--“我要种植火桃之树”
		msg.AddMsgEvent(MsgID, 21, 14)		--玉子之树返回14
		msg.AddMsgEvent(MsgID, 1,  420042)	--“我要种植玉子之树”
		msg.AddMsgEvent(MsgID, 21, 15)		--水晶之树返回15
		msg.AddMsgEvent(MsgID, 1,  420043)	--“我要种植水晶之树”
		msg.AddMsgEvent(MsgID, 21, 16)		--紫梅之树返回16
		msg.AddMsgEvent(MsgID, 1,  420044)	--“我要种植紫梅之树”
		msg.AddMsgEvent(MsgID, 21, 17)		--椰米之树返回17
		msg.AddMsgEvent(MsgID, 1,  420045)	--“我要种植椰米之树”
		msg.AddMsgEvent(MsgID, 21, 18)		--椰米之树返回17
		msg.AddMsgEvent(MsgID, 1,  420046)	--“我要种植椰米之树”
		msg.AddMsgEvent(MsgID, 21, 19)		--椰米之树返回17
		msg.AddMsgEvent(MsgID, 1,  420047)	--“我要种植椰米之树”
		msg.AddMsgEvent(MsgID, 21, 20)		--椰米之树返回17
		msg.AddMsgEvent(MsgID, 1,  420048)	--“我要种植椰米之树”
		msg.AddMsgEvent(MsgID, 21, 10)		--取消返回10
		msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex ~= 10 then		--尚未开放
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420049)	--您是否要播种？
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 10)		--取消返回10
		msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
--注册和高级土坑对话
aux.RegisterCreatureEvent(5800006, 7, "mound_advanced_OnTalk")


-------------浇水-------------------------------------------------------------------------------------------
--元素之树浇水
function yuansu_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
--	cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2, 0)
--	local waternum = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--提示可以浇水,以及水的来源
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420014)	--“浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--这不是你的播种,是XXX的
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--获得玩家身上的水的数量
		local cTime = os.time() --当前时间
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--删除水
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800008, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--记录幼苗出生的时间
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和播种土堆对话
aux.RegisterCreatureEvent(5800007, 7, "yuansu_seed_OnTalk")

--香果之树浇水
function xiangguo_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local questdata = (zero%10000-zero%1000)/1000
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--提示可以浇水,以及水的来源
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420014)	--“浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--这不是你的播种,是XXX的
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--获得玩家身上的水的数量
		local cTime = os.time() --当前时间
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--删除水
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800036, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--记录幼苗出生的时间
			if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40054) == true then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40054, 2)
				if questdata == 0 then
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1000)
				else
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				end
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和播种土堆对话
aux.RegisterCreatureEvent(5800035, 7, "xiangguo_seed_OnTalk")

--火桃之树浇水
function huotao_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--提示可以浇水,以及水的来源
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420014)	--“浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--这不是你的播种,是XXX的
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--获得玩家身上的水的数量
		local cTime = os.time() --当前时间
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--删除水
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800040, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--记录幼苗出生的时间
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和播种土堆对话
aux.RegisterCreatureEvent(5800039, 7, "huotao_seed_OnTalk")

--玉子之树浇水
function yuzi_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--提示可以浇水,以及水的来源
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420014)	--“浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--这不是你的播种,是XXX的
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--获得玩家身上的水的数量
		local cTime = os.time() --当前时间
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--删除水
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800044, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--记录幼苗出生的时间
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和播种土堆对话
aux.RegisterCreatureEvent(5800043, 7, "yuzi_seed_OnTalk")

--水晶之树浇水
function shuijing_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--提示可以浇水,以及水的来源
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420014)	--“浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--这不是你的播种,是XXX的
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--获得玩家身上的水的数量
		local cTime = os.time() --当前时间
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--删除水
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800048, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--记录幼苗出生的时间
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和播种土堆对话
aux.RegisterCreatureEvent(5800047, 7, "shuijing_seed_OnTalk")

--紫梅之树浇水
function zimei_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--提示可以浇水,以及水的来源
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420014)	--“浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--这不是你的播种,是XXX的
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--获得玩家身上的水的数量
		local cTime = os.time() --当前时间
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--删除水
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800052, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--记录幼苗出生的时间
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和播种土堆对话
aux.RegisterCreatureEvent(5800051, 7, "zimei_seed_OnTalk")

--椰米之树浇水
function yemi_seed_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420013)	--提示可以浇水,以及水的来源
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420014)	--“浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420015)	--这不是你的播种,是XXX的
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100007)		--获得玩家身上的水的数量
		local cTime = os.time() --当前时间
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100007, 1, 420)			--删除水
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800056, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)				--记录幼苗出生的时间
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420017)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和播种土堆对话
aux.RegisterCreatureEvent(5800055, 7, "yemi_seed_OnTalk")

--------------小树消失,刷新大树----------------------------------------------------------------------------------------
--元素
function yuansuseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800007, 13, "yuansuseed_On_Disappear")

--香果
function xiangguoseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800035, 13, "xiangguoseed_On_Disappear")

--火桃
function huotaoseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800039, 13, "huotaoseed_On_Disappear")

--玉子
function yuziseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800043, 13, "yuziseed_On_Disappear")

--水晶
function shuijingseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800047, 13, "shuijingseed_On_Disappear")

--紫梅
function zimeiseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800051, 13, "zimeiseed_On_Disappear")

--椰米
function yemiseed_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800055, 13, "yemiseed_On_Disappear")



------------护理------------------------------------------------------------------------------------------------
--元素
function yuansu_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --当前时间
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800009, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--您的树还有XXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420018)	--“拉芙艾娜的爱之泉”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--这是XXX的树,还有XXXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--浇水返回12
			msg.AddMsgEvent(MsgID, 1,  420020)	--“我要帮它浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--删除绑定水
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--删除不绑定的水
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800009, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--删除水
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和幼苗说话
aux.RegisterCreatureEvent(5800008, 7, "yuansu_sapling_OnTalk")

--香果
function xiangguo_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --当前时间
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800037, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--您的树还有XXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420018)	--“拉芙艾娜的爱之泉”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--这是XXX的树,还有XXXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--浇水返回12
			msg.AddMsgEvent(MsgID, 1,  420020)	--“我要帮它浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--删除绑定水
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--删除不绑定的水
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800037, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
		local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--删除水
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和幼苗说话
aux.RegisterCreatureEvent(5800036, 7, "xiangguo_sapling_OnTalk")

--火桃
function huotao_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --当前时间
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800041, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--您的树还有XXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420018)	--“拉芙艾娜的爱之泉”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--这是XXX的树,还有XXXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--浇水返回12
			msg.AddMsgEvent(MsgID, 1,  420020)	--“我要帮它浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--删除绑定水
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--删除不绑定的水
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800041, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--删除水
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和幼苗说话
aux.RegisterCreatureEvent(5800040, 7, "huotao_sapling_OnTalk")

--玉子
function yuzi_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --当前时间
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800045, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--您的树还有XXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420018)	--“拉芙艾娜的爱之泉”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--这是XXX的树,还有XXXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--浇水返回12
			msg.AddMsgEvent(MsgID, 1,  420020)	--“我要帮它浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--删除绑定水
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--删除不绑定的水
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800045, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--删除水
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和幼苗说话
aux.RegisterCreatureEvent(5800044, 7, "yuzi_sapling_OnTalk")

--水晶
function shuijing_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --当前时间
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800049, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--您的树还有XXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420018)	--“拉芙艾娜的爱之泉”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--这是XXX的树,还有XXXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--浇水返回12
			msg.AddMsgEvent(MsgID, 1,  420020)	--“我要帮它浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--删除绑定水
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--删除不绑定的水
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800049, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--删除水
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和幼苗说话
aux.RegisterCreatureEvent(5800048, 7, "shuijing_sapling_OnTalk")

--紫梅
function zimei_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --当前时间
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800053, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--您的树还有XXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420018)	--“拉芙艾娜的爱之泉”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--这是XXX的树,还有XXXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--浇水返回12
			msg.AddMsgEvent(MsgID, 1,  420020)	--“我要帮它浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--删除绑定水
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--删除不绑定的水
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800053, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--删除水
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和幼苗说话
aux.RegisterCreatureEvent(5800052, 7, "zimei_sapling_OnTalk")

--椰米
function yemi_sapling_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local cTime = os.time() --当前时间
	local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)
	local s = os.difftime(cTime,lTime)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
	    if s >= 3600 then
		    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800057, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		elseif seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420016)	--您的树还有XXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 11)		--浇水返回11
			msg.AddMsgEvent(MsgID, 1,  420018)	--“拉芙艾娜的爱之泉”
			msg.AddMsgEvent(MsgID, 21, 10)		--不能浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420019)	--这是XXX的树,还有XXXX时间成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 9, math.floor(60-s/60))
			msg.AddMsgEvent(MsgID, 21, 12)		--浇水返回12
			msg.AddMsgEvent(MsgID, 1,  420020)	--“我要帮它浇水”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local num = role.GetRoleItemNum(RoleID, 4100901)
		local num2 = role.GetRoleItemNum(RoleID, 4100902)
		if num > 0 or num2 > 0 then
			if num2 > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100902, 1, 420)			--删除绑定水
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4100901, 1, 420)			--删除不绑定的水
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
			local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800057, x, y, z)		--刷新幼苗
			cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, RoleID)				--记录幼苗的所属人
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420021)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 12 then
				local num = role.GetRoleItemNum(RoleID, 4100903)
		if num >0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4100903, 1, 420)			--删除水
			if farmlevel < 9 then
				farmexp = farmexp + seed_exp[TargetTypeID]
				if farmexp >= farm_exp[farmlevel] then
					farmexp = farmexp - farm_exp[farmlevel]
					zero = zero + 1
					farmlevel = farmlevel + 1
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
					msg.AddMsgEvent(MsgID, 9, farmlevel)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local ex = farm_exp[farmlevel] - farmexp
					role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 1)
					msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
					msg.AddMsgEvent(MsgID, 9, farmexp)
					msg.AddMsgEvent(MsgID, 9, ex)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420022)	--提示没有水,不能浇水
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
--注册和幼苗说话
aux.RegisterCreatureEvent(5800056, 7, "yemi_sapling_OnTalk")

--------------小树消失,刷新大树----------------------------------------------------------------------------------------
--元素
function yuansu_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800009, x, y, z)		--刷新幼苗
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--您的XXX成熟了,请前往采摘！
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800008, 13, "yuansu_On_Disappear")

--香果
function xiangguo_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800037, x, y, z)		--刷新幼苗
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--您的XXX成熟了,请前往采摘！
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800036, 13, "xiangguo_On_Disappear")

--火桃
function huotao_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800041, x, y, z)		--刷新幼苗
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--您的XXX成熟了,请前往采摘！
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800040, 13, "huotao_On_Disappear")

--玉子
function yuzi_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800045, x, y, z)		--刷新幼苗
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--您的XXX成熟了,请前往采摘！
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800044, 13, "yuzi_On_Disappear")

--水晶
function shuijing_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800049, x, y, z)		--刷新幼苗
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--您的XXX成熟了,请前往采摘！
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800048, 13, "shuijing_On_Disappear")

--紫梅
function zimei_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800053, x, y, z)		--刷新幼苗
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--您的XXX成熟了,请前往采摘！
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800052, 13, "zimei_On_Disappear")

--椰米
function yemi_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800057, x, y, z)		--刷新幼苗
	cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420023)--您的XXX成熟了,请前往采摘！
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)
end
aux.RegisterCreatureEvent(5800056, 13, "yemi_On_Disappear")

------------第一次收获------------------------------------------------------------------------------------------------
--元素
function yuansu_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--这是XXX的树,是否想偷取收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--偷摘返回12
			msg.AddMsgEvent(MsgID, 1,  420025)	--“偷摘”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 199 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400001, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400001, 1, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400003, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800010, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
				if a <= 199 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400001, 1, -1, 8, 102)
		else a = 200
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400001, 1, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400003, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800010, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--您的XXXX被XXX偷走了一部分！剩下的果实将在1小时候消失,请及时采摘！
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800009, 7, "yuansu_first_OnTalk")

--香果
function xiangguo_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--这是XXX的树,是否想偷取收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--偷摘返回12
			msg.AddMsgEvent(MsgID, 1,  420025)	--“偷摘”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800038, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800038, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--您的XXXX被XXX偷走了！剩下的果实将在1小时候消失,请及时采摘！
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800037, 7, "xiangguo_first_OnTalk")

--火桃
function huotao_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--这是XXX的树,是否想偷取收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--偷摘返回12
			msg.AddMsgEvent(MsgID, 1,  420025)	--“偷摘”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800042, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800042, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--您的XXXX被XXX偷走了！剩下的果实将在1小时候消失,请及时采摘！
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800041, 7, "huotao_first_OnTalk")

--玉子
function yuzi_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--这是XXX的树,是否想偷取收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--偷摘返回12
			msg.AddMsgEvent(MsgID, 1,  420025)	--“偷摘”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800046, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800046, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--您的XXXX被XXX偷走了！剩下的果实将在1小时候消失,请及时采摘！
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800045, 7, "yuzi_first_OnTalk")

--水晶
function shuijing_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--这是XXX的树,是否想偷取收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--偷摘返回12
			msg.AddMsgEvent(MsgID, 1,  420025)	--“偷摘”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800050, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800050, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--您的XXXX被XXX偷走了！剩下的果实将在1小时候消失,请及时采摘！
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800049, 7, "shuijing_first_OnTalk")

--紫梅
function zimei_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--这是XXX的树,是否想偷取收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--偷摘返回12
			msg.AddMsgEvent(MsgID, 1,  420025)	--“偷摘”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800054, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800054, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--您的XXXX被XXX偷走了！剩下的果实将在1小时候消失,请及时采摘！
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800053, 7, "zimei_first_OnTalk")


--椰米果
function yemi_first_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420026)	--这是XXX的树,是否想偷取收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 12)		--偷摘返回12
			msg.AddMsgEvent(MsgID, 1,  420025)	--“偷摘”
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800058, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	elseif TalkIndex == 12 then
		local a = math.random(1,200)
		if a <= 100 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 1, -1, 8, 102)
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 2, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800058, x, y, z)	--刷新第二期成熟
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)			--记录幼苗的所属人
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420027)--您的XXXX被XXX偷走了！剩下的果实将在1小时候消失,请及时采摘！
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(seedowner, MsgID)
	end
end
aux.RegisterCreatureEvent(5800057, 7, "yemi_first_OnTalk")

---------------------------------成熟第二期采摘--------------------------------------------------
--元素
function yuansu_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--这是XXX的树,无法收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--记录时间,为以后的种植cd做准备
		local a = math.random(1,1000)
		if a <= 300 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400001, 2, -1, 8, 102)
		elseif a <= 860 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400002, 1, -1, 8, 102)
		elseif a <= 963 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400003, 1, -1, 8, 102)
		elseif a <= 983 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400004, 1, -1, 8, 102)
		elseif a <= 988 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4400005, 1, -1, 8, 102)
		elseif a <= 990 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100107, 1, -1, 8, 102)
		elseif a <= 992 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100106, 1, -1, 8, 102)
		elseif a <= 994 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100105, 1, -1, 8, 102)
		elseif a <= 996 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100104, 1, -1, 8, 102)
		elseif a <= 998 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100103, 1, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100102, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--记录幼苗的所属人
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800010, 7, "yuansu_second_OnTalk")

--香果
function xiangguo_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--这是XXX的树,无法收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--记录时间,为以后的种植cd做准备
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100201, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100108, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--记录幼苗的所属人
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800038, 7, "xiangguo_second_OnTalk")

--火桃
function huotao_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--这是XXX的树,无法收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--记录时间,为以后的种植cd做准备
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100202, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100109, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--记录幼苗的所属人
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800042, 7, "huotao_second_OnTalk")

--玉子
function yuzi_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--这是XXX的树,无法收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--记录时间,为以后的种植cd做准备
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100203, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100110, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--记录幼苗的所属人
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800046, 7, "yuzi_second_OnTalk")

--水晶
function shuijing_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--这是XXX的树,无法收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--记录时间,为以后的种植cd做准备
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100204, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100111, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--记录幼苗的所属人
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800050, 7, "shuijing_second_OnTalk")

--紫梅
function zimei_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--这是XXX的树,无法收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--记录时间,为以后的种植cd做准备
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100205, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100112, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--记录幼苗的所属人
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800054, 7, "zimei_second_OnTalk")

--椰米
function yemi_second_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[RoleID]
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end
	if TalkIndex == -1 then
		if seedowner == RoleID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420024)	--您的XXX已经成熟
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 21, 11)		--收获返回11
			msg.AddMsgEvent(MsgID, 1,  420025)	--“收获”
			msg.AddMsgEvent(MsgID, 21, 10)		--不收获返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 420028)	--这是XXX的树,无法收获
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 2, seedowner)
			msg.AddMsgEvent(MsgID, 21, 10)		--不浇水返回10
			msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 11 then
		local cTime = os.time()					--记录时间,为以后的种植cd做准备
		local a = math.random(1,1000)
		if a <= 600 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 5, -1, 8, 102)
		elseif a <= 1000 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100206, 5, -1, 8, 102)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4100113, 1, -1, 8, 102)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)			--删除播种的土堆
		local seedlings = map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 1, seedowner)				--记录幼苗的所属人
		cre.SetCreatureScriptData(MapID, InstanceID, seedlings, 1, 2, cTime)
		farmnum = farmnum - 1
		if farmnum < 1 then
			plant_data[RoleID] = nil
		else
			plant_data[RoleID] = farmnum
		end
		if farmlevel < 9 then
			farmexp = farmexp + seed_exp[TargetTypeID]
			if farmexp >= farm_exp[farmlevel] then
				farmexp = farmexp - farm_exp[farmlevel]
				zero = zero + 1
				farmlevel = farmlevel + 1
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
				msg.AddMsgEvent(MsgID, 9, farmlevel)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local ex = farm_exp[farmlevel] - farmexp
				role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
				msg.AddMsgEvent(MsgID, 9, farmexp)
				msg.AddMsgEvent(MsgID, 9, ex)
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
			msg.DispatchRoleMsgEvent(seedowner, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(5800058, 7, "yemi_second_OnTalk")

-----------时间到后自己消失,并且刷新新种植点-------------------------------------------------------------------
--元素
function yuansutree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--您的XXX已过期,无法再次收获
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800010, 13, "yuansutree_On_Disappear")

--香果
function xiangguotree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--您的XXX已过期,无法再次收获
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800038, 13, "xiangguotree_On_Disappear")

--huotao
function huotaotree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--您的XXX已过期,无法再次收获
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800042, 13, "huotaotree_On_Disappear")

--yuzi
function yuzitree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--您的XXX已过期,无法再次收获
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800046, 13, "yuzitree_On_Disappear")

--shuijing
function shuijingtree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--您的XXX已过期,无法再次收获
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800050, 13, "shuijingtree_On_Disappear")

--zimei
function zimeitree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--您的XXX已过期,无法再次收获
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800054, 13, "zimeitree_On_Disappear")

--yemi
function yemitree_On_Disappear(MapID, InstanceID, TargetID, Ai)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	local farmnum = plant_data[seedowner]
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 5800005, x, y, z)		--刷新幼苗
--[[local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 420029)--您的XXX已过期,无法再次收获
	msg.AddMsgEvent(MsgID, 5, TargetID)
	msg.DispatchRoleMsgEvent(seedowner, MsgID)]]
	farmnum = farmnum - 1
	if farmnum < 1 then
		plant_data[seedowner] = nil
	else
		plant_data[seedowner] = farmnum
	end
end
aux.RegisterCreatureEvent(5800058, 13, "yemitree_On_Disappear")
-----------------------查询农场等级-----------------------------------------------
function guanli_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
	end
	if farmlevel == 9 then
		farmexp = 0
	end
	local lexp = farm_exp[farmlevel] - farmexp
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420034)	--您好，我是妖精农场的管理员，如果您有想查阅自己的农场属性，来找我就对了！
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 11)		--查看农场等级返回11
		msg.AddMsgEvent(MsgID, 1,  420035)	--“查看农场等级”
		msg.AddMsgEvent(MsgID, 21, 12)		--查看能种植的种子返回12
		msg.AddMsgEvent(MsgID, 1,  420036)	--查看能种植的种子
		msg.AddMsgEvent(MsgID, 21, 13)		--查看能种植的种子返回12
		msg.AddMsgEvent(MsgID, 1,  420050)	--查看能种植的种子
		msg.AddMsgEvent(MsgID, 21, 10)		--取消返回10
		msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 11 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420037)	--您的农场等级为x，当前已积累XX经验，还剩XXX经验即可升级
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 9, farmlevel)
		msg.AddMsgEvent(MsgID, 9, farmexp)
		msg.AddMsgEvent(MsgID, 9, lexp)
		msg.AddMsgEvent(MsgID, 21, 10)		--取消返回10
		msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 12 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420038)	--您的农场等级为X级，可种植以下植物：
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 10)		--取消返回10
		msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 13 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 420051)	--您的农场等级为X级，可种植以下植物：
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 10)		--取消返回10
		msg.AddMsgEvent(MsgID, 1,  420009)	--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterCreatureEvent(5800086, 7, "guanli_OnTalk")

--------------------奇异披萨----------------------------------------------------
function s4100904_Use(MapID, InstanceID, TypeID, RoleID)
	local zero = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"])
	local farmlevel = zero%10
	local farmexp = role.GetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"])
	if farmlevel == 0 then
		farmlevel = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero+1)
	end

	if farmlevel < 9 then
		farmexp = farmexp + 10
		if farmexp >= farm_exp[farmlevel] then
			farmexp = farmexp - farm_exp[farmlevel]
			zero = zero + 1
			farmlevel = farmlevel + 1
			role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmLevel"], zero)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420031)--恭喜你，您的农场等级升至X级，请去农场管理员处查阅新等级农场权限
			msg.AddMsgEvent(MsgID, 9, farmlevel)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local ex = farm_exp[farmlevel] - farmexp
			role.SetRoleScriptData(RoleID, 1, RoleDataType["FarmExp"], farmexp)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 420032)--您的农场经验积累到XX，还差XX升级
			msg.AddMsgEvent(MsgID, 9, farmexp)
			msg.AddMsgEvent(MsgID, 9, ex)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 420033)--您的农场等级已经升至最高，无法获得农场经验
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterItemEvent(4100904, 1, "s4100904_Use")

