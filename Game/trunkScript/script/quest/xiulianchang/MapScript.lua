--scriptdata 0 记录现在杀了多少个怪
--scriptdata 1 记录现在杀了多少个中型怪

XiuLian_Fire = {}

--XiuLian_Role = {}

XiuLian_Pos = {}
XiuLian_Pos[1] = {x = 243,y = 10000,z = 304}
XiuLian_Pos[2] = {x = 251,y = 10000,z = 304}
XiuLian_Pos[3] = {x = 273,y = 10000,z = 304}
XiuLian_Pos[4] = {x = 275,y = 10000,z = 293}
XiuLian_Pos[5] = {x = 263,y = 10000,z = 290}
XiuLian_Pos[6] = {x = 245,y = 10000,z = 286}
XiuLian_Pos[7] = {x = 243,y = 10000,z = 276}
XiuLian_Pos[8] = {x = 254,y = 10000,z = 271}
XiuLian_Pos[9] = {x = 265,y = 10000,z = 269}
XiuLian_Pos[10] = {x = 277,y = 10000,z = 264}
XiuLian_Pos[11] = {x = 282,y = 10000,z = 256}
XiuLian_Pos[12] = {x = 272,y = 10000,z = 249}
XiuLian_Pos[13] = {x = 259,y = 10000,z = 246}
XiuLian_Pos[14] = {x = 241,y = 10000,z = 249}
XiuLian_Pos[15] = {x = 240,y = 10000,z = 239}
XiuLian_Pos[16] = {x = 252,y = 10000,z = 231}
XiuLian_Pos[17] = {x = 261,y = 10000,z = 227}
XiuLian_Pos[18] = {x = 261,y = 10000,z = 227}
XiuLian_Pos[19] = {x = 284,y = 10000,z = 218}
XiuLian_Pos[20] = {x = 269,y = 10000,z = 210}
XiuLian_Pos[21] = {x = 266,y = 10000,z = 227}
XiuLian_Pos[22] = {x = 252,y = 10000,z = 246}
XiuLian_Pos[23] = {x = 277,y = 10000,z = 247}
XiuLian_Pos[24] = {x = 270,y = 10000,z = 266}
XiuLian_Pos[25] = {x = 253,y = 10000,z = 268}
XiuLian_Pos[26] = {x = 260,y = 10000,z = 250}
XiuLian_Pos[27] = {x = 240,y = 10428,z = 289}
XiuLian_Pos[28] = {x = 240,y = 10428,z = 250}
XiuLian_Pos[29] = {x = 240,y = 10428,z = 215}
XiuLian_Pos[30] = {x = 287,y = 10428,z = 215}
XiuLian_Pos[31] = {x = 287,y = 10428,z = 250}
XiuLian_Pos[32] = {x = 287,y = 10428,z = 289}

-- 修炼场敌我判断
function Xiulian_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	-- 都是玩家
	if unit.IsPlayer(SrcID) and not unit.IsPlayer(TargetID) then
	    local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, TargetID)
	    if TypeID >= 1099301 and TypeID <= 1099309 then
			local x, y ,z = unit.GetPosition(MapID, InstanceID, SrcID)
			if x < 183 or x > 340 or z < 166 or z > 336 then
				return true, false, false, true
			end
		else
		    return false, false, false, false
		end
	end

	return false, false, false, false
end



--注册该活动的相关事件

aux.RegisterMapEvent("s55_1", 16, "Xiulian_FriendEnemy")
aux.RegisterMapEvent("s55_2", 16, "Xiulian_FriendEnemy")
aux.RegisterMapEvent("s55_3", 16, "Xiulian_FriendEnemy")


function Xiulianchang_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    local k = (TargetTypeID - 1099301) % 3
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if tonumber(os.date("%H"))%2 == 0 then
		if k == 0 then
			local num = map.GetMapScriptData(MapID,InstanceID,1,0) + 1
			map.SetMapScriptData(MapID,InstanceID,1,0,num)
			if num <= 600 then
				if num%100 == 0 then
					for i = 21,25 do
						map.MapCreateColCreature(MapID, InstanceID, TargetTypeID+1, XiuLian_Pos[i].x, XiuLian_Pos[i].y, XiuLian_Pos[i].z, 1)
					end
				end
				local index = math.random(20)
				map.MapCreateColCreature(MapID, InstanceID, TargetTypeID, XiuLian_Pos[index].x, XiuLian_Pos[index].y, XiuLian_Pos[index].z, 1)
			end
			if role.GetBagFreeSize(RoleID) > 0 and ((rolelevel<=80 and MapID == 2748433913) or (rolelevel<=90 and MapID == 2748434169) or MapID == 2748434425) then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4800134, 1, -1, 8, 102)
			end
		elseif k == 1 then
		    local num = map.GetMapScriptData(MapID,InstanceID,1,1) + 1
			map.SetMapScriptData(MapID,InstanceID,1,1,num)
			if num <= 30 then
				if num % 5 == 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 605013)          --墙上的一盏油灯被点燃了
					msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
					if XiuLian_Fire[MapID] == nil then
						XiuLian_Fire[MapID] = {}
					end
					XiuLian_Fire[MapID][num%5] = map.MapCreateColCreature(MapID, InstanceID, 5610048, XiuLian_Pos[26+num/5].x, XiuLian_Pos[26+num/5].y, XiuLian_Pos[26+num/5].z, 1)
				end
				if num == 30 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 605014)      --当所有的油灯都被点燃，一个强大的炼魂殿灵出现了
					msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
					map.MapCreateColCreature(MapID, InstanceID, TargetTypeID+1, XiuLian_Pos[26].x, XiuLian_Pos[26].y, XiuLian_Pos[26].z, 1)
				end
			end
			if role.GetBagFreeSize(RoleID) > 0 and ((rolelevel<=80 and MapID == 2748433913) or (rolelevel<=90 and MapID == 2748434169) or MapID == 2748434425) then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4800135, 1, -1, 8, 102)
			end
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 605015)      --XXX击杀了最强大的炼魂殿灵
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		end
	end
end
aux.RegisterCreatureEvent(1099301, 4, "Xiulianchang_OnDie")
aux.RegisterCreatureEvent(1099302, 4, "Xiulianchang_OnDie")
aux.RegisterCreatureEvent(1099303, 4, "Xiulianchang_OnDie")
aux.RegisterCreatureEvent(1099304, 4, "Xiulianchang_OnDie")
aux.RegisterCreatureEvent(1099305, 4, "Xiulianchang_OnDie")
aux.RegisterCreatureEvent(1099306, 4, "Xiulianchang_OnDie")
aux.RegisterCreatureEvent(1099307, 4, "Xiulianchang_OnDie")
aux.RegisterCreatureEvent(1099308, 4, "Xiulianchang_OnDie")
aux.RegisterCreatureEvent(1099309, 4, "Xiulianchang_OnDie")

--玩家进入竞技场
function Xiulian_OnPlayerEnter(MapID, InstanceID, RoleID)
    --XiuLian_Role[RoleID] = 1
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["XiuLianChang_Time"])
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 16,	28)								--
	msg.AddMsgEvent(MsgID, 16,	k*60)							--剩余时间
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
	--进入修炼场总人数
	local curTime = tonumber(os.date("%j"))
	if lianHun_Roles[RoleID] == nil then
		UpdateSeverData(54, 1)
		lianHun_Roles[RoleID] = curTime
	elseif lianHun_Roles[RoleID] ~= curTime then
		lianHun_Roles = {}
		lianHun_Roles[RoleID] = curTime
		UpdateSeverData(54, 1)
	end
end

--[[玩家离开竞技场

function Xiulian_LeaveMap(MapID, InstanceID, RoleID)
	XiuLian_Role[RoleID] = nil
end]]

aux.RegisterMapEvent("s55_1", 2, "Xiulian_OnPlayerEnter")
aux.RegisterMapEvent("s55_2", 2, "Xiulian_OnPlayerEnter")
aux.RegisterMapEvent("s55_3", 2, "Xiulian_OnPlayerEnter")
--aux.RegisterMapEvent("s55_1", 3, "Xiulian_LeaveMap")
--aux.RegisterMapEvent("s55_2", 3, "Xiulian_LeaveMap")
--aux.RegisterMapEvent("s55_3", 3, "Xiulian_LeaveMap")
