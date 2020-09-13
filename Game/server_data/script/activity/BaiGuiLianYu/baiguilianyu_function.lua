BaiGui_expe = {}
BaiGui_expe[40] = 55000
BaiGui_expe[45] =76000
BaiGui_expe[50] =96000
BaiGui_expe[55] =111000
BaiGui_expe[60] =136000
BaiGui_expe[65] =175000
BaiGui_expe[70] =237000
BaiGui_expe[75] =307000
BaiGui_expe[80] =359000
BaiGui_expe[85] =445000
BaiGui_expe[90] =543000
BaiGui_expe[95] =650000
BaiGui_expe[100] =650000

function BaiGui_addexp(MapID, InstanceID,RoleID,Wave)--完成一波之后给予奖励
    local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	if TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
				if Target_MapID == MapID then
					local rolelevel = role.GetRoleLevel(MapID, InstanceID, Role[i])

					if rolelevel >100 then
						rolelevel = 100
					else
						rolelevel = math.floor(rolelevel/5)*5
					end
					if Wave == 10 then
					    local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
						role.AddRoleExp(MapID, InstanceID, Role[i], expe)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
						msg.AddMsgEvent(MsgID, 9, Wave)
						msg.AddMsgEvent(MsgID, 9, expe)
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					elseif Wave == 9 then
					    local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
						role.AddRoleExp(MapID, InstanceID, Role[i], expe)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
						msg.AddMsgEvent(MsgID, 9, Wave)
						msg.AddMsgEvent(MsgID, 9, expe)
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					elseif Wave == 8 then
						local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
						role.AddRoleExp(MapID, InstanceID, Role[i], expe)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
						msg.AddMsgEvent(MsgID, 9, Wave)
						msg.AddMsgEvent(MsgID, 9, expe)
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					elseif Wave == 7 then
						local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
						role.AddRoleExp(MapID, InstanceID, Role[i], expe)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
						msg.AddMsgEvent(MsgID, 9, Wave)
						msg.AddMsgEvent(MsgID, 9, expe)
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					elseif Wave == 6 then
						local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
						role.AddRoleExp(MapID, InstanceID, Role[i], expe)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
						msg.AddMsgEvent(MsgID, 9, Wave)
						msg.AddMsgEvent(MsgID, 9, expe)
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					elseif Wave == 5 then
						local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
						role.AddRoleExp(MapID, InstanceID, Role[i], expe)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
						msg.AddMsgEvent(MsgID, 9, Wave)
						msg.AddMsgEvent(MsgID, 9, expe)
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					elseif Wave == 4 then
						local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
						role.AddRoleExp(MapID, InstanceID, Role[i], expe)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
						msg.AddMsgEvent(MsgID, 9, Wave)
						msg.AddMsgEvent(MsgID, 9, expe)
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					elseif Wave == 3 then
						local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
						role.AddRoleExp(MapID, InstanceID, Role[i], expe)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
						msg.AddMsgEvent(MsgID, 9, Wave)
						msg.AddMsgEvent(MsgID, 9, expe)
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					elseif Wave == 2 then
						local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
						role.AddRoleExp(MapID, InstanceID, Role[i], expe)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
						msg.AddMsgEvent(MsgID, 9, Wave)
						msg.AddMsgEvent(MsgID, 9, expe)
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					elseif Wave == 1 then
						local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
						role.AddRoleExp(MapID, InstanceID, Role[i], expe)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
						msg.AddMsgEvent(MsgID, 9, Wave)
						msg.AddMsgEvent(MsgID, 9, expe)
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					end
				end
			end
		end
	else
	    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if rolelevel >100 then
			rolelevel = 100
		else
			rolelevel = math.floor(rolelevel/5)*5
		end
		if Wave == 10 then
			local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
			role.AddRoleExp(MapID, InstanceID, RoleID, expe)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, expe)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Wave == 9 then
			local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
			role.AddRoleExp(MapID, InstanceID, RoleID, expe)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, expe)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Wave == 8 then
			local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
			role.AddRoleExp(MapID, InstanceID, RoleID, expe)
			local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, expe)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Wave == 7 then
			local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
			role.AddRoleExp(MapID, InstanceID, RoleID, expe)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, expe)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Wave == 6 then
			local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
			role.AddRoleExp(MapID, InstanceID, RoleID, expe)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, expe)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Wave == 5 then
			local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
			role.AddRoleExp(MapID, InstanceID, RoleID, expe)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, expe)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Wave == 4 then
			local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
			role.AddRoleExp(MapID, InstanceID, RoleID, expe)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, expe)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Wave == 3 then
			local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
			role.AddRoleExp(MapID, InstanceID, RoleID, expe)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, expe)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Wave == 2 then
			local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
			role.AddRoleExp(MapID, InstanceID, RoleID, expe)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, expe)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Wave == 1 then
			local expe = math.floor(BaiGui_expe[rolelevel]/55*Wave)
			role.AddRoleExp(MapID, InstanceID, RoleID, expe)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 204014)  --百鬼炼狱第&lt;p1&gt;波通过！您获得经验&lt;p2&gt;点！
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, expe)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end

function BaiGui_CreateCreature(MapID, InstanceID, Average_Level, Wave)--刷出一波新怪
    local num = 0
	if Average_Level >= 100 then
	    num = 11
	elseif Average_Level >= 95 and Average_Level <= 99 then
	    num = 10
    elseif Average_Level >= 90 and Average_Level <= 94 then
	    num = 9
	elseif Average_Level >= 85 and Average_Level <= 89 then
	    num = 8
	elseif Average_Level >= 80 and Average_Level <= 84 then
	    num = 7
	elseif Average_Level >= 75 and Average_Level <= 79 then
	    num = 6
	elseif Average_Level >= 70 and Average_Level <= 74 then
	    num = 5
	elseif Average_Level >= 65 and Average_Level <= 69 then
	    num = 4
	elseif Average_Level >= 60 and Average_Level <= 64 then
	    num = 3
	elseif Average_Level >= 55 and Average_Level <= 59 then
	    num = 2
	elseif Average_Level >= 50 and Average_Level <= 54 then
	    num = 1
	elseif Average_Level >= 40 and Average_Level <= 49 then
	    num = 0
	end
	if Wave == 1 then
		local xiaoguaiid = 2021001 + num*4
		local bossid = 2022001 + num*8
	    map.MapCreateColCreature(MapID, InstanceID, bossid, BaiGui_Pos[53].x, BaiGui_Pos[53].y/50, BaiGui_Pos[53].z, 1)
		for i=3,12 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		if num >= 3 then
		    local p = math.random(5)
			if p >= 4 then
			    local ID1 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[13].x, BaiGui_Pos[13].y/50, BaiGui_Pos[13].z, 1)
				local ID2 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[14].x, BaiGui_Pos[14].y/50, BaiGui_Pos[14].z, 1)
				unit.AddBuff(MapID, InstanceID, ID1, 6100401, ID1)
				unit.AddBuff(MapID, InstanceID, ID2, 6100401, ID2)
			end
		end
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 11)
	elseif Wave == 2 then
	    local xiaoguaiid = 2021001 + num*4
		local bossid = 2022001 + num*8 + 1
	    map.MapCreateColCreature(MapID, InstanceID, bossid, BaiGui_Pos[53].x, BaiGui_Pos[53].y/50, BaiGui_Pos[53].z, 1)
		for i=3,17 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		if num >= 3 then
		    local p = math.random(5)
			if p >= 4 then
			    local ID1 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[18].x, BaiGui_Pos[18].y/50, BaiGui_Pos[18].z, 1)
				local ID2 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[19].x, BaiGui_Pos[19].y/50, BaiGui_Pos[19].z, 1)
				unit.AddBuff(MapID, InstanceID, ID1, 6100401, ID1)
				unit.AddBuff(MapID, InstanceID, ID2, 6100401, ID2)
			end
		end
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 16)
	elseif Wave == 3 then
	    local xiaoguaiid = 2021001 + num*4 +1
		local bossid = 2022001 + num*8 + 2
	    map.MapCreateColCreature(MapID, InstanceID, bossid, BaiGui_Pos[53].x, BaiGui_Pos[53].y/50, BaiGui_Pos[53].z, 1)
		for i=3,12 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		for i=13,22 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		if num >= 3 then
		    local p = math.random(5)
			if p >= 4 then
			    local ID1 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[23].x, BaiGui_Pos[23].y/50, BaiGui_Pos[23].z, 1)
				local ID2 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[24].x, BaiGui_Pos[24].y/50, BaiGui_Pos[24].z, 1)
				unit.AddBuff(MapID, InstanceID, ID1, 6100401, ID1)
				unit.AddBuff(MapID, InstanceID, ID2, 6100401, ID2)
			end
		end
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 21)
	elseif Wave == 4 then
	    local xiaoguaiid = 2021001 + num*4 +1
		local bossid = 2022001 + num*8 + 3
	    map.MapCreateColCreature(MapID, InstanceID, bossid, BaiGui_Pos[53].x, BaiGui_Pos[53].y/50, BaiGui_Pos[53].z, 1)
		for i=3,12 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		for i=13,22 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		if num >= 3 then
		    local p = math.random(5)
			if p >= 4 then
			    local ID1 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[23].x, BaiGui_Pos[23].y/50, BaiGui_Pos[23].z, 1)
				local ID2 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[24].x, BaiGui_Pos[24].y/50, BaiGui_Pos[24].z, 1)
				unit.AddBuff(MapID, InstanceID, ID1, 6100401, ID1)
				unit.AddBuff(MapID, InstanceID, ID2, 6100401, ID2)
			end
		end
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 21)
	elseif Wave == 5 then
	    local xiaoguaiid = 2021001 + num*4 +2
		local bossid = 2022001 + num*8 + 4
	    map.MapCreateColCreature(MapID, InstanceID, bossid, BaiGui_Pos[53].x, BaiGui_Pos[53].y/50, BaiGui_Pos[53].z, 1)
		----unit.AddBuff(MapID, InstanceID, bossid, 9429501, bossid)
		for i=3,12 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		for i=13,22 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		if num >= 3 then
		    local p = math.random(5)
			if p >= 4 then
			    local ID1 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[23].x, BaiGui_Pos[23].y/50, BaiGui_Pos[23].z, 1)
				local ID2 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[24].x, BaiGui_Pos[24].y/50, BaiGui_Pos[24].z, 1)
				unit.AddBuff(MapID, InstanceID, ID1, 6100401, ID1)
				unit.AddBuff(MapID, InstanceID, ID2, 6100401, ID2)
			end
		end
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 21)
	elseif Wave == 6 then
	    local xiaoguaiid = 2021001 + num*4 +2
		local bossid = 2022001 + num*8 + 5
	    map.MapCreateColCreature(MapID, InstanceID, bossid, BaiGui_Pos[53].x, BaiGui_Pos[53].y/50, BaiGui_Pos[53].z, 1)
		----unit.AddBuff(MapID, InstanceID, bossid, 9429501, bossid)
		for i=3,12 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		for i=13,22 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		if num >= 3 then
		    local p = math.random(5)
			if p >= 4 then
			    local ID1 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[23].x, BaiGui_Pos[23].y/50, BaiGui_Pos[23].z, 1)
				local ID2 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[24].x, BaiGui_Pos[24].y/50, BaiGui_Pos[24].z, 1)
				unit.AddBuff(MapID, InstanceID, ID1, 6100401, ID1)
				unit.AddBuff(MapID, InstanceID, ID2, 6100401, ID2)
			end
		end
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 21)
	elseif Wave == 7 then
	    local xiaoguaiid = 2021001 + num*4 +3
		local bossid = 2022001 + num*8 + 6
	    map.MapCreateColCreature(MapID, InstanceID, bossid, BaiGui_Pos[53].x, BaiGui_Pos[53].y/50, BaiGui_Pos[53].z, 1)
		--unit.AddBuff(MapID, InstanceID, bossid, 9429501, bossid)
		for i=3,12 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		for i=13,22 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		if num >= 3 then
		    local p = math.random(5)
			if p >= 4 then
			    local ID1 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[23].x, BaiGui_Pos[23].y/50, BaiGui_Pos[23].z, 1)
				local ID2 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[24].x, BaiGui_Pos[24].y/50, BaiGui_Pos[24].z, 1)
				unit.AddBuff(MapID, InstanceID, ID1, 6100401, ID1)
				unit.AddBuff(MapID, InstanceID, ID2, 6100401, ID2)
			end
		end
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 21)
	elseif Wave == 8 then
	    local xiaoguaiid = 2021001 + num*4 +3
		local bossid = 2022001 + num*8 + 7
	    map.MapCreateColCreature(MapID, InstanceID, bossid, BaiGui_Pos[53].x, BaiGui_Pos[53].y/50, BaiGui_Pos[53].z, 1)
		--unit.AddBuff(MapID, InstanceID, bossid, 9429501, bossid)
		for i=3,12 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		for i=13,22 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		if num >= 3 then
		    local p = math.random(5)
			if p >= 4 then
			    local ID1 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[23].x, BaiGui_Pos[23].y/50, BaiGui_Pos[23].z, 1)
				local ID2 = map.MapCreateColCreature(MapID, InstanceID, 1099002, BaiGui_Pos[24].x, BaiGui_Pos[24].y/50, BaiGui_Pos[24].z, 1)
				unit.AddBuff(MapID, InstanceID, ID1, 6100401, ID1)
				unit.AddBuff(MapID, InstanceID, ID2, 6100401, ID2)
			end
		end
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 21)
	elseif Wave == 9 then
	    local bossid = 2023001 + num
	    map.MapCreateColCreature(MapID, InstanceID, bossid, BaiGui_Pos[53].x, BaiGui_Pos[53].y/50, BaiGui_Pos[53].z, 1)
		--unit.AddBuff(MapID, InstanceID, bossid, 9429501, bossid)
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 1)
	elseif Wave == 10 then
	    local xiaoguaiid = 2021001 + num*4 +3
	    map.MapCreateColCreature(MapID, InstanceID, 2023014, BaiGui_Pos[53].x, BaiGui_Pos[53].y/50, BaiGui_Pos[53].z, 1)
		--unit.AddBuff(MapID, InstanceID, bossid, 9429901, bossid)
		for i=3,52 do
		    map.MapCreateColCreature(MapID, InstanceID, xiaoguaiid, BaiGui_Pos[i].x, BaiGui_Pos[i].y/50, BaiGui_Pos[i].z, 1)
		end
		map.SetMapScriptData(MapID, InstanceID, 1, 2, 50)
    end
end
