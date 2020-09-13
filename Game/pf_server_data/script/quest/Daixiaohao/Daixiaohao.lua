--scriptdata 第0位记录第几波
--scriptdata 第1位记录还剩几个怪物
--scriptdata 第2位记录是否刷了第一个NPC
--scriptdata 第3位记录刷新60s计时器
--传送进副本

function x5810004_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取玩家等级
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206401)		--“晶石使者：\n     必须要满足队伍中同时有50级以上的玩家以及10级至29级的玩家太能进入滚石洞穴。\n    必须满足队伍中同时有50级以上的玩家以及30级至40级的玩家太能进入碎石洞穴。\n    50级以上玩家每天能进入滚石洞穴和碎石洞穴的总次数是5次。\n    当你拥有熔岩晶核和冰岩晶核的时候，可以在我这里用3个三等的材料（精铁，玄铁或幻钢）来兑换3个相应的四等材料。50级以上的玩家可以通过每天在悬赏使者处接取社交任务来获得熔岩晶核和冰岩晶核。”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--确定按钮
		if MapID == 3424071720 then
		    msg.AddMsgEvent(MsgID, 21, 5)			--确定按钮
			msg.AddMsgEvent(MsgID, 1, 206402)		--“进入碎石洞穴”
		else
		    msg.AddMsgEvent(MsgID, 21, 4)			--确定按钮
			msg.AddMsgEvent(MsgID, 1, 206403)		--“进入滚石洞穴”
		end
		if rolelevel > 49 then
			msg.AddMsgEvent(MsgID, 21, 6)			--确定按钮
			msg.AddMsgEvent(MsgID, 1, 206404)		--“万能钥匙兑换”
		end
		msg.AddMsgEvent(MsgID, 21,	10)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    local bool = false
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		if TeamID ~= nil and TeamID ~= 4294967295 then
			local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
			local Role = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
			local Num = 0
			for i=1, 6 do
				if Role[i] and Role[i]~= 4294967295 then
					local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
					local level = role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])
					if level >= 50 then
						Num = Num + 10
					elseif level >= 10 and level <= 29  then
					    Num  = Num + 1
					end
				end
			end
			if Num > 10 and Num%10 >= 1 then
			    bool = true
			end
		end
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if bool == true then
			if level > 49 then
				local CurTime = tonumber(os.date("%j"))
				local UpdateTime = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["Chuandao"])/10)
				if CurTime~=UpdateTime then
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Chuandao"], CurTime*10)
				end
				if CurTime == UpdateTime and  role.GetRoleScriptData(RoleID, 1, RoleDataType["Chuandao"]) % 10 > 4 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 	206405)	--晶石使者：\n     您今天进入滚石洞穴和碎石洞穴的总次数已经超过了<p1>次，请明天再来吧。
					msg.AddMsgEvent(MsgID, 9, 	5)
					msg.AddMsgEvent(MsgID, 21, 	-1)		--取消按钮
					msg.AddMsgEvent(MsgID, 1, 	100001)		--取消
					msg.AddMsgEvent(MsgID, 24,	TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Chuandao"],role.GetRoleScriptData(RoleID, 1, RoleDataType["Chuandao"])+1)
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, 619033786, 162, 4, 50)
				end
			else
			    role.RoleGotoNewMap(MapID, InstanceID, RoleID, 619033786, 162, 4, 50)
			end
		else
		    local MsgID = msg.BeginMsgEvent()
			if level > 50 then
				msg.AddMsgEvent(MsgID, 20,	206406)		--“晶石使者：\n     你必须和10至29级的玩家组队才能进入滚石洞穴。”
			elseif level > 9 and level <= 29 then
			    msg.AddMsgEvent(MsgID, 20,	206407)		--“晶石使者：\n     你必须和50级以上的玩家组队才能进入滚石洞穴。”
			else
			    msg.AddMsgEvent(MsgID, 20,	206408)		--“晶石使者：\n     你的队伍中必须有至少一个10至29级的玩家以及一个50级以上的玩家才能进入滚石洞穴。”
			end
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 5 then
	    local bool = false
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		if TeamID ~= nil and TeamID ~= 4294967295 then
			local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
			local Role = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
			local Num = 0
			for i=1, 6 do
				if Role[i] and Role[i]~= 4294967295 then
					local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
					local level = role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])
					if level >= 50 then
						Num = Num + 10
					elseif level >= 30 and level <= 49  then
					    Num  = Num + 1
					end
				end
			end
			if Num > 10 and Num%10 >= 1 then
			    bool = true
			end
		end

		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if bool == true then
			if level > 49 then
				local CurTime = tonumber(os.date("%j"))
				local UpdateTime = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["Chuandao"])/10)
				if CurTime~=UpdateTime then
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Chuandao"], CurTime*10)
				end
				if CurTime == UpdateTime and  role.GetRoleScriptData(RoleID, 1, RoleDataType["Chuandao"]) % 10 > 4 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 	206405)	--晶石使者：\n     您今天进入滚石洞穴和碎石洞穴的总次数已经超过了<p1>次，请明天再来吧。
					msg.AddMsgEvent(MsgID, 9, 	5)
					msg.AddMsgEvent(MsgID, 21, 	-1)		--取消按钮
					msg.AddMsgEvent(MsgID, 1, 	100001)		--取消
					msg.AddMsgEvent(MsgID, 24,	TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Chuandao"],role.GetRoleScriptData(RoleID, 1, RoleDataType["Chuandao"])+1)
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, 870054631, 162, 4, 50)
				end
			else
			    role.RoleGotoNewMap(MapID, InstanceID, RoleID, 870054631, 162, 4, 50)
			end
		else
		    local MsgID = msg.BeginMsgEvent()
			if level > 50 then
				msg.AddMsgEvent(MsgID, 20,	206409)		--“晶石使者：\n     你必须和10至29级的玩家组队才能进入滚石洞穴。”
			elseif level > 29 and level <= 49 then
			    msg.AddMsgEvent(MsgID, 20,	206410)		--“晶石使者：\n     你必须和50级以上的玩家组队才能进入滚石洞穴。”
			else
			    msg.AddMsgEvent(MsgID, 20,	206411)		--“晶石使者：\n     你的队伍中必须有至少一个10至29级的玩家以及一个50级以上的玩家才能进入滚石洞穴。”
			end
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 6 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206412)		--“晶石使者：\n     兑换需要以下材料：\n三等相应材料*3\n熔岩晶核*1\n冰岩晶核*1\n    请选择你要兑换的材料：”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	7)			--
		msg.AddMsgEvent(MsgID, 1,	100001)		--确定
		msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 7 then
	   -- local a = role.GetRoleItemNum(RoleID,3050011)
		local b = role.GetRoleItemNum(RoleID,4400413)
		local c = role.GetRoleItemNum(RoleID,4400414)

		if role.GetBagFreeSize(RoleID) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206417)		--“晶石使者：\n     请保证背包中有一个以上剩余空间再来兑换材料。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif b*c > 0 then
		   -- role.RemoveFromRole(MapID, InstanceID, RoleID, 3050011, 3, 101)
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4400413, 1, 101)
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4400414, 1, 101)
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4800239, 1, -1, 8, 420)
	        local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206416)		--“晶石使者：\n     如果你有更多的材料的话，还可以来找我兑换。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206418)		--“晶石使者：\n     你的材料不足，请准备好再来。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	--[[elseif TalkIndex == 8 then
	    local a = role.GetRoleItemNum(RoleID,3050016)
		local b = role.GetRoleItemNum(RoleID,4400413)
		local c = role.GetRoleItemNum(RoleID,4400414)

		if role.GetBagFreeSize(RoleID) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206417)		--“晶石使者：\n     请保证背包中有一个以上剩余空间再来兑换材料。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif a*b*c > 0 and a >= 3 then
		    role.RemoveFromRole(MapID, InstanceID, RoleID, 3050016, 3, 101)
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4400413, 1, 101)
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4400414, 1, 101)
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3050017, 3, -1, 8, 420)
	        local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206416)		--“晶石使者：\n     如果你有更多的材料的话，还可以来找我兑换。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206418)		--“晶石使者：\n     你的材料不足，请准备好再来。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 9 then
	    local a = role.GetRoleItemNum(RoleID,3050021)
		local b = role.GetRoleItemNum(RoleID,4400413)
		local c = role.GetRoleItemNum(RoleID,4400414)

		if role.GetBagFreeSize(RoleID) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206417)		--“晶石使者：\n     请保证背包中有一个以上剩余空间再来兑换材料。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif a*b*c > 0 and a >= 3 then
		    role.RemoveFromRole(MapID, InstanceID, RoleID, 3050021, 3, 101)
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4400413, 1, 101)
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4400414, 1, 101)
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3050022, 3, -1, 8, 420)
	        local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206416)		--“晶石使者：\n     如果你有更多的材料的话，还可以来找我兑换。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206418)		--“晶石使者：\n     你的材料不足，请准备好再来。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end]]
	end
end
--
aux.RegisterCreatureEvent(5810004, 7, "x5810004_OnTalk")




function x5810001_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
	    if MapID == 619033786 then
		    if rolelevel >= 10 and rolelevel <= 29 then
		        local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206419)		--“     召唤结晶中的岩石傀儡吗？”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	4)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.AddMsgEvent(MsgID, 21,	10)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206420)		--“     这是一块充满能力的岩石结晶，也许可以召唤出强大的岩石傀儡。\n    提示：只有10级至29级的玩家可以进行召唤。”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	10)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif MapID == 870054631 then
		    if rolelevel >= 30 and rolelevel <= 49 then
		        local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206419)		--“     召唤结晶中的岩石傀儡吗？”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	4)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.AddMsgEvent(MsgID, 21,	10)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	206421)		--“     这是一块充满能力的岩石结晶，也许可以召唤出强大的岩石傀儡。\n    提示：只有10级至29级的玩家可以进行召唤。”
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	10)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 4 then
        local bool = false
	    if MapID == 619033786 then
		    if rolelevel >= 10 and rolelevel <= 29 then
		        bool = true
			end
		elseif MapID == 870054631 then
		    if rolelevel >= 30 and rolelevel <= 49 then
		        bool = true
			end
		end
		if bool then
			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
			--刷新第一个60s计时器
			if map.GetMapScriptData(MapID, InstanceID, 1, 3) == 0 then
				map.MapCreateColCreature(MapID, InstanceID, 5610025, 1, 1, 1, 1, "")
				map.SetMapScriptData(MapID, InstanceID, 1, 3, 1)
			end
			local maxlevel = 50
			if TeamID ~= nil and TeamID ~= 4294967295 then
				local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				local Role = {}
				Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
				local Num = 0
				for i=1, 6 do
					if Role[i] and Role[i]~= 4294967295 then
						local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
						local level = role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])
						if level >50 then
							maxlevel = level
							if maxlevel > 100 then
								maxlevel = 100
							end
						end
					end
				end
			end

			local chuandao_mon = {}
			chuandao_mon[619033786] = {}
			chuandao_mon[619033786][5810001] = {1096001,5}
			chuandao_mon[619033786][5810002] = {1096012,3}
			chuandao_mon[619033786][5810003] = {1096023,1}
			chuandao_mon[870054631] = {}
			chuandao_mon[870054631][5810001] = {1096034,5}
			chuandao_mon[870054631][5810002] = {1096045,3}
			chuandao_mon[870054631][5810003] = {1096056,1}

			local chuandao_pos = {}
			chuandao_pos[1] = {x=45,y=177,z=184}
			chuandao_pos[2] = {x=66,y=280,z=127}
			chuandao_pos[3] = {x=126,y=280,z=155}
			chuandao_pos[4] = {x=95,y=245,z=201}
			chuandao_pos[5] = {x=62,y=280,z=157}

			local wave = map.GetMapScriptData(MapID, InstanceID, 1, 0)+1
			map.SetMapScriptData(MapID, InstanceID, 1, 0, wave)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, chuandao_mon[MapID][TargetTypeID][2])
			local index = math.floor(maxlevel/5)-10
			for i = 1,chuandao_mon[MapID][TargetTypeID][2] do
			    local monsterid = 0
                if 	chuandao_mon[MapID][TargetTypeID][2] ~= 1 then
					monsterid = map.MapCreateColCreature(MapID, InstanceID, chuandao_mon[MapID][TargetTypeID][1]+index, chuandao_pos[i].x, chuandao_pos[i].y, chuandao_pos[i].z, 1, "")
				else
				    monsterid = map.MapCreateColCreature(MapID, InstanceID, chuandao_mon[MapID][TargetTypeID][1]+index, 88, 280, 171, 1, "")
				end
				unit.AddBuff(MapID, InstanceID, monsterid, 2054302, monsterid)
			end
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			if role.GetBagFreeSize(RoleID) > 0 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4400415, 1, -1, 8, 102)
			end
		end
	end
end
--注册
aux.RegisterCreatureEvent(5810001, 7, "x5810001_OnTalk")
aux.RegisterCreatureEvent(5810002, 7, "x5810001_OnTalk")
aux.RegisterCreatureEvent(5810003, 7, "x5810001_OnTalk")

function x5810005_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
	    local MsgID = msg.BeginMsgEvent()
		if TargetTypeID == 5810005 then
		    msg.AddMsgEvent(MsgID, 20,	206422)     --“     只有10级至49级的玩家才能开启这个宝箱领取奖励。你想开启宝箱吗？”
		elseif TargetTypeID == 5810006 then
			msg.AddMsgEvent(MsgID, 20,	206423)     --“     只有50级以上的玩家才能开启这个宝箱领取奖励。你想开启宝箱吗？”
		end
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
		msg.AddMsgEvent(MsgID, 21,	10)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    local bool = false
        if TargetTypeID == 5810005 and rolelevel >= 10 and rolelevel <= 49 then
		    bool = true
		elseif TargetTypeID == 5810006 and rolelevel >= 50 then
		    bool = true
		end
		if not bool then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206424)		--“     你不符合开启该宝箱的等级要求，无法开启宝箱。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.GetBagFreeSize(RoleID) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206425)		--“     你需要身上至少有一个以上的背包空间才能开启宝箱。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local box = {}
			box[5810006] = {id = {4090013,4090008,4090014,4090009,3050011,3050012,3050016,3050017,3030002,3030010,3030073,3030081},per={19,5,19,5,19,5,19,5,1,1,1,1}}
			box[5810005] = {id = {3020009,3020014,3020015,3050006,3050007,3050008,3050011,3050012,3030001,3030009,3030072,3030080},per={19,12,5,19,12,5,19,5,1,1,1,1}}
		    local k = math.random(100)
			local percent = 0
			for i = 1,12 do
			    percent = box[TargetTypeID].per[i] + percent
				if percent >= k then
				    role.AddRoleItem(MapID, InstanceID, RoleID, box[TargetTypeID].id[i], 1, -1, 8, 102)
				    map.MapDeleteCreature(MapID, InstanceID, TargetID)
					break
				--[[elseif percent == k then
				    role.AddRoleItem(MapID, InstanceID, RoleID, box[TargetTypeID].id[i], 1, -1, 8, 102)
				    map.MapDeleteCreature(MapID, InstanceID, TargetID)
				    break]]
				end
			end
		end
	end
end
--注册
aux.RegisterCreatureEvent(5810005, 7, "x5810005_OnTalk")
aux.RegisterCreatureEvent(5810006, 7, "x5810005_OnTalk")

--杀掉特殊小怪后的BOSS刷新判断
function ChuanDao_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    map.SetMapScriptData(MapID, InstanceID, 1, 1, map.GetMapScriptData(MapID, InstanceID, 1, 1)-1)
	local chuandao_pos = {}
	chuandao_pos[1] = {x=45,y=177,z=184}
	chuandao_pos[2] = {x=66,y=280,z=127}
	chuandao_pos[3] = {x=126,y=280,z=155}
	chuandao_pos[4] = {x=95,y=245,z=201}
	chuandao_pos[5] = {x=62,y=280,z=157}
	if map.GetMapScriptData(MapID, InstanceID, 1, 1) == 0 then
	    if map.GetMapScriptData(MapID, InstanceID, 1, 0) < 3 then
			local k = math.random(5)
			map.MapCreateColCreature(MapID, InstanceID, 5810001, chuandao_pos[k].x, chuandao_pos[k].y, chuandao_pos[k].z, 1, "")
		elseif map.GetMapScriptData(MapID, InstanceID, 1, 0) < 5 then
		    local k = math.random(5)
			map.MapCreateColCreature(MapID, InstanceID, 5810002, chuandao_pos[k].x, chuandao_pos[k].y, chuandao_pos[k].z, 1, "")
		elseif map.GetMapScriptData(MapID, InstanceID, 1, 0) < 6 then
		    local k = math.random(5)
			map.MapCreateColCreature(MapID, InstanceID, 5810003, chuandao_pos[k].x, chuandao_pos[k].y, chuandao_pos[k].z, 1, "")
		end
	end
	if ((TargetTypeID >= 1096023 and TargetTypeID <= 1096033) or (TargetTypeID >= 1096056 and TargetTypeID <= 1096066)) and math.random(100) > 95 then
	    if math.random(2) == 2 then
			map.MapCreateColCreature(MapID, InstanceID, 5810005, chuandao_pos[3].x, chuandao_pos[3].y, chuandao_pos[3].z, 1, "")
		else
		    map.MapCreateColCreature(MapID, InstanceID, 5810006, chuandao_pos[3].x, chuandao_pos[3].y, chuandao_pos[3].z, 1, "")
		end
	end
end

aux.RegisterCreatureEvent(1096001, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096002, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096003, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096004, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096005, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096006, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096007, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096008, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096009, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096010, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096011, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096012, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096013, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096014, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096015, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096016, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096017, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096018, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096019, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096020, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096021, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096022, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096023, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096024, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096025, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096026, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096027, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096028, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096029, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096030, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096031, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096032, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096033, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096034, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096035, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096036, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096037, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096038, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096039, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096040, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096041, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096042, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096043, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096044, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096045, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096046, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096047, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096048, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096049, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096050, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096051, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096052, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096053, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096054, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096055, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096056, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096057, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096058, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096059, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096060, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096061, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096062, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096063, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096064, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096065, 4, "ChuanDao_OnDie")
aux.RegisterCreatureEvent(1096066, 4, "ChuanDao_OnDie")

function h06_OnPlayerEnter(MapID, InstanceID, RoleID)
    if map.GetMapScriptData(MapID, InstanceID, 1, 2) == 0 then
	    local chuandao_pos = {}
		chuandao_pos[1] = {x=45,y=177,z=184}
		chuandao_pos[2] = {x=66,y=280,z=127}
		chuandao_pos[3] = {x=126,y=280,z=155}
		chuandao_pos[4] = {x=95,y=245,z=201}
		chuandao_pos[5] = {x=62,y=280,z=157}
	    local k = math.random(5)
		map.MapCreateColCreature(MapID, InstanceID, 5810001, chuandao_pos[k].x, chuandao_pos[k].y, chuandao_pos[k].z, 1, "")
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 1)
	end
end
aux.RegisterMapEvent("h06", 2, "h06_OnPlayerEnter")
aux.RegisterMapEvent("h06_1", 2, "h06_OnPlayerEnter")

--玩家复活脚本
function h06_RoloRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)
    if Type == 0 then
		ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
		ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)
		x = 162
		y = 200
		z = 50
	end

	return ReviveHP, ReviveMP, x, y, z, MapID

end
aux.RegisterMapEvent("h06", 11, "h06_RoloRevive")
aux.RegisterMapEvent("h06_1", 11, "h06_RoloRevive")

--接取任务
function q30717_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	local Quest_DaiXiaoHao = {}
	Quest_DaiXiaoHao[10]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=20000,RewXP=10000,MsgID=16001}
	Quest_DaiXiaoHao[11]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=20000,RewXP=10000,MsgID=16001}
	Quest_DaiXiaoHao[12]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=30000,RewXP=12000,MsgID=16001}
	Quest_DaiXiaoHao[13]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=30000,RewXP=12000,MsgID=16001}
	Quest_DaiXiaoHao[14]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=30000,RewXP=12000,MsgID=16001}
	Quest_DaiXiaoHao[15]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=40000,RewXP=14000,MsgID=16001}
	Quest_DaiXiaoHao[16]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=40000,RewXP=15000,MsgID=16001}
	Quest_DaiXiaoHao[17]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=40000,RewXP=16000,MsgID=16001}
	Quest_DaiXiaoHao[18]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=40000,RewXP=17000,MsgID=16001}
	Quest_DaiXiaoHao[19]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=40000,RewXP=18000,MsgID=16001}
	Quest_DaiXiaoHao[20]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=50000,RewXP=20000,MsgID=16001}
	Quest_DaiXiaoHao[21]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=50000,RewXP=24000,MsgID=16001}
	Quest_DaiXiaoHao[22]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=50000,RewXP=26000,MsgID=16001}
	Quest_DaiXiaoHao[23]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=50000,RewXP=28000,MsgID=16001}
	Quest_DaiXiaoHao[24]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=60000,RewXP=30000,MsgID=16001}
	Quest_DaiXiaoHao[25]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=60000,RewXP=32000,MsgID=16001}
	Quest_DaiXiaoHao[26]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=60000,RewXP=34000,MsgID=16001}
	Quest_DaiXiaoHao[27]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=60000,RewXP=38000,MsgID=16001}
	Quest_DaiXiaoHao[28]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=70000,RewXP=46000,MsgID=16001}
	Quest_DaiXiaoHao[29]={TargetType=QuestTargetType["Collect"],TargetID1=4400401,Num1=15,TargetID2=4400402,Num2=6,TargetID3=4400403,Num3=1,RewMoney=70000,RewXP=60000,MsgID=16001}
	Quest_DaiXiaoHao[30]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=70000,RewXP=100000,MsgID=16002}
	Quest_DaiXiaoHao[31]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=70000,RewXP=120000,MsgID=16002}
	Quest_DaiXiaoHao[32]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=80000,RewXP=140000,MsgID=16002}
	Quest_DaiXiaoHao[33]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=80000,RewXP=160000,MsgID=16002}
	Quest_DaiXiaoHao[34]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=80000,RewXP=180000,MsgID=16002}
	Quest_DaiXiaoHao[35]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=80000,RewXP=200000,MsgID=16002}
	Quest_DaiXiaoHao[36]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=90000,RewXP=220000,MsgID=16002}
	Quest_DaiXiaoHao[37]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=90000,RewXP=240000,MsgID=16002}
	Quest_DaiXiaoHao[38]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=90000,RewXP=260000,MsgID=16002}
	Quest_DaiXiaoHao[39]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=90000,RewXP=280000,MsgID=16002}
	Quest_DaiXiaoHao[40]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=100000,RewXP=300000,MsgID=16002}
	Quest_DaiXiaoHao[41]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=100000,RewXP=320000,MsgID=16002}
	Quest_DaiXiaoHao[42]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=100000,RewXP=340000,MsgID=16002}
	Quest_DaiXiaoHao[43]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=100000,RewXP=360000,MsgID=16002}
	Quest_DaiXiaoHao[44]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=110000,RewXP=380000,MsgID=16002}
	Quest_DaiXiaoHao[45]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=110000,RewXP=400000,MsgID=16002}
	Quest_DaiXiaoHao[46]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=110000,RewXP=420000,MsgID=16002}
	Quest_DaiXiaoHao[47]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=110000,RewXP=440000,MsgID=16002}
	Quest_DaiXiaoHao[48]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=120000,RewXP=460000,MsgID=16002}
	Quest_DaiXiaoHao[49]={TargetType=QuestTargetType["Collect"],TargetID1=4400404,Num1=15,TargetID2=4400405,Num2=6,TargetID3=4400406,Num3=1,RewMoney=120000,RewXP=480000,MsgID=16002}

	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local Index = 10
	if rolelevel < 50 and rolelevel > 9 then
	    Index = rolelevel
	end
	--初始化任务的动态数据
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_DaiXiaoHao, Index))
	--同步客户端总任务环数和任务说明
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_DaiXiaoHao[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterQuestEvent(30717, 7, "q30717_OnQuestInit")
aux.RegisterQuestEvent(30718, 7, "q30717_OnQuestInit")

function q30717_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local FreeSize = role.GetBagFreeSize(RoleID)
	if FreeSize < 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end
aux.RegisterQuestEvent(30717, 5, "q30717_OnCheckComplete")

function q30718_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local FreeSize = role.GetBagFreeSize(RoleID)
	if FreeSize < 2 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 2)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end
aux.RegisterQuestEvent(30718, 5, "q30718_OnCheckComplete")

--完成任务
function q30717_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

    role.AddRoleItem(MapID, InstanceID, RoleID, 3070026, 1, -1, 8, 102)

end
aux.RegisterQuestEvent(30717, 1, "q30717_OnComplete")

--完成任务
function q30718_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

    role.AddRoleItem(MapID, InstanceID, RoleID, 3020057, 2, -1, 8, 102)
    role.AddRoleItem(MapID, InstanceID, RoleID, 3020009, 2, -1, 8, 102)

end
aux.RegisterQuestEvent(30718, 1, "q30718_OnComplete")

function I4400415_CanUse(MapID, InstanceID, TypeID, TargetID)
	if MapID == 619033786 or MapID == 870054631 then
		--提示玩家背包空间不足
		return 0, false
	else
		return 43,false
	end
end

aux.RegisterItemEvent(4400415, 0, "I4400415_CanUse")

function c5610025_On_Disappear(MapID, InstanceID, TargetID, TargetTypeID)
	if MapID == 619033786 then
		local pos1 = {}
		pos1[1] = {x=53, y = 280, z = 155}
		pos1[2] = {x=95, y = 241, z = 136}
		pos1[3] = {x=70, y = 260, z = 184}
		pos1[4] = {x=120, y = 260, z = 177}
		pos1[5] = {x=97, y = 245, z = 194}
		pos1[6] = {x=114, y = 239, z = 148}
		pos1[7] = {x=59, y = 260, z = 172}
		pos1[8] = {x=75, y = 260, z = 137}
		pos1[9] = {x=71, y = 280, z = 154}
		pos1[10] = {x=99, y = 260, z = 181}
		pos1[11] = {x=109, y = 260, z = 163}
		pos1[12] = {x=93, y = 241, z = 151}
		pos1[13] = {x=53, y = 280, z = 155}
		for i=1, 3 do
			local n = math.random(table.maxn(pos1))
			map.MapCreateColCreature(MapID, InstanceID, 5610024, pos1[n].x, pos1[n].y, pos1[n].z, 1, "")
			table.remove(pos1, n)
		end
		if map.GetMapScriptData(MapID, InstanceID, 1, 0) < 5 then
			map.MapCreateColCreature(MapID, InstanceID, 5610025, 1, 1, 1, 1, "")
		end
	elseif MapID == 870054631 then
		local pos2 = {}
		pos2[1] = {x =76, y = 260, z = 186}
		pos2[2] = {x =107, y = 260, z = 165}
		pos2[3] = {x =107, y = 239, z = 203}
		pos2[4] = {x =53, y = 177, z = 200}
		pos2[5] = {x =85, y = 239, z = 202}
		pos2[6] = {x =117, y = 260, z = 181}
		pos2[7] = {x =117, y = 280, z = 155}
		pos2[8] = {x =63, y = 301, z = 143}
		pos2[9] = {x =48, y = 321, z = 140}
		pos2[10] = {x =65, y = 293, z = 124}
		pos2[11] = {x =45, y = 260, z = 157}
		pos2[12] = {x =38, y = 198, z = 179}
		pos2[13] = {x =52, y = 239, z = 170}
		pos2[14] = {x =77, y = 239, z = 126}
		pos2[15] = {x =103, y = 239, z = 132}
		pos2[16] = {x =79, y = 260, z = 146}
		for i=1, 3 do
			local n = math.random(table.maxn(pos2))
			map.MapCreateColCreature(MapID, InstanceID, 5610024, pos2[n].x, pos2[n].y, pos2[n].z, 1, "")
			table.remove(pos2, n)
		end
		if map.GetMapScriptData(MapID, InstanceID, 1, 0) < 5 then
			map.MapCreateColCreature(MapID, InstanceID, 5610025, 1, 1, 1, 1, "")
		end
	end
end
aux.RegisterCreatureEvent(5610025, 13, "c5610025_On_Disappear")

function c5610024_On_Invest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	local lev = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if lev > 100 then
		lev = 100
	end
	role.AddRoleExp(MapID, InstanceID, RoleID, monster_exp[lev]*3.5)
end
aux.RegisterCreatureEvent(5610024, 6, "c5610024_On_Invest")

function h06_OnPlayerLeave(MapID, InstanceID, RoleID)
	local num1 = role.GetRoleItemNum(RoleID, 4400416)
	local num2 = role.GetRoleItemNum(RoleID, 4400415)
    if num1 > 0 then
	    role.RemoveFromRole(MapID, InstanceID, RoleID, 4400416, num1, 310)
	end
	if num2 > 0 then
	    role.RemoveFromRole(MapID, InstanceID, RoleID, 4400415, num2, 310)
	end
end
aux.RegisterMapEvent("h06", 3, "h06_OnPlayerLeave")
aux.RegisterMapEvent("h06_1", 3, "h06_OnPlayerLeave")










