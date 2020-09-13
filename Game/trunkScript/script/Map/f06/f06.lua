---[0]记录是否刷出地物
---[1]381, 15076, 235坐标的地物  517
---[2]393, 15076, 235坐标的地物  518
---[3]405, 15076, 235坐标的地物  519
---[4]381, 15076, 247坐标的地物  520

---[5]393, 15076, 247坐标的地物  521
---[6]405, 15076, 247坐标的地物  522
---[7]381, 15076, 259坐标的地物  523
---[8]393, 15076, 259坐标的地物  524
---[9]405, 15076, 259坐标的地物  525


--[10]按位记录1至9坐标的地物状态，1表示有buff 0表示无buff


--[11]记录因踩点刷出怪物的数量，上限60

--[12]记录是否解除机关

--[13]记录是否刷出老一3040103
--[14]记录老一3040103死亡与否
--[15]记录老二3040108死亡与否
--[16]记录老三3040109死亡与否
--[17]记录老四3040116死亡与否
--[18]记录老四3040119死亡与否


function f06_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100092)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,13)
	if monsterhave==0 then
		map.OpenCloseDoor(MapID, InstanceID, 441, nil)
		map.MapCreateCreature(MapID, InstanceID, 3040103, 257, 12631, 243)
		map.SetMapScriptData(MapID,InstanceID,1,13,1)
	end
	unit.AddBuff(MapID,InstanceID,RoleID,5930901,RoleID)
	--副本统计
	--instanceData(6, RoleID, 29, 36)
	--玩家统计
	--UpdateRoleData(RoleID, 16, 1)
	--加体验buff
	if experienceTeams ~= nil then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		for k in pairs(experienceTeams) do
			if experienceTeams[k].teamid == TeamID then
				local lev = math.floor(role.GetRoleLevel(MapID, InstanceID, RoleID)/10)
				if lev == 4 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109704, RoleID)
				elseif lev == 5 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109707, RoleID)
				elseif lev == 6 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109710, RoleID)
				elseif lev == 7 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109712, RoleID)
				end
				unit.AddBuff(MapID, InstanceID, RoleID, 6109601, RoleID)
				break
			end
		end
	end
end

--注册
aux.RegisterMapEvent("f06", 2, "f06_OnPlayerEnter")






function fuckjapan_01(MapID, InstanceID,objNum)
	local j=map.GetMapScriptData(MapID,InstanceID,1,0)
	local i=objNum
	if j==1 then
		if i<10 and i>0 then
			local triggleID = map.GetMapScriptData(MapID,InstanceID,1,i)

			local status = map.GetMapScriptData(MapID,InstanceID,1,10)
			local state = math.floor(status/math.pow(2,i-1))%2



			if state==1 then
				unit.CancelBuff(MapID, InstanceID, triggleID, 5055801)
				map.SetMapScriptData(MapID,InstanceID,1,10,(status-math.pow(2,i-1)))


			else
				unit.AddBuff(MapID,InstanceID,triggleID,5055801,triggleID)
				map.SetMapScriptData(MapID,InstanceID,1,10,(status+math.pow(2,i-1)))


			end
		end
	end
end




function f06_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	local triggle={}
	triggle[1]=map.GetMapScriptData(MapID,InstanceID,1,1)
	triggle[2]=map.GetMapScriptData(MapID,InstanceID,1,2)
	triggle[3]=map.GetMapScriptData(MapID,InstanceID,1,3)
	triggle[4]=map.GetMapScriptData(MapID,InstanceID,1,4)
	triggle[5]=map.GetMapScriptData(MapID,InstanceID,1,5)
	triggle[6]=map.GetMapScriptData(MapID,InstanceID,1,6)
	triggle[7]=map.GetMapScriptData(MapID,InstanceID,1,7)
	triggle[8]=map.GetMapScriptData(MapID,InstanceID,1,8)
	triggle[9]=map.GetMapScriptData(MapID,InstanceID,1,9)

	local trush=3040120
	local nummonster = map.GetMapScriptData(MapID,InstanceID,1,11)
	local j=map.GetMapScriptData(MapID,InstanceID,1,0)
	local bgot=map.GetMapScriptData(MapID,InstanceID,1,12)

	if ObjID==517 and bgot == 0 then

		fuckjapan_01(MapID, InstanceID,1)
		fuckjapan_01(MapID, InstanceID,2)
		fuckjapan_01(MapID, InstanceID,4)

		if  j==1 then
			if nummonster<30 then
				map.MapCreateCreature(MapID, InstanceID, trush, 381, 15076, 235)
				nummonster=nummonster+1
				map.SetMapScriptData(MapID,InstanceID,1,11,nummonster)
			end

			local objtarget = map.GetMapScriptData(MapID,InstanceID,1,10)
			if objtarget == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100097)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

				map.OpenCloseDoor(MapID, InstanceID, 441, 1)
				map.SetMapScriptData(MapID,InstanceID,1,12,1)
			end
		end
	end



	if ObjID==518 and bgot == 0 then

		fuckjapan_01(MapID, InstanceID,1)
		fuckjapan_01(MapID, InstanceID,2)
		fuckjapan_01(MapID, InstanceID,3)
		fuckjapan_01(MapID, InstanceID,5)



		if  j==1 then
			if nummonster<30 then
				map.MapCreateCreature(MapID, InstanceID, trush, 393, 15076, 235)
				nummonster=nummonster+1
				map.SetMapScriptData(MapID,InstanceID,1,11,nummonster)
			end

			local objtarget = map.GetMapScriptData(MapID,InstanceID,1,10)
			if objtarget == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100097)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

				map.OpenCloseDoor(MapID, InstanceID, 441, 1)
				map.SetMapScriptData(MapID,InstanceID,1,12,1)

			end
		end
	end

	if ObjID==519 and bgot == 0 then

		fuckjapan_01(MapID, InstanceID,3)
		fuckjapan_01(MapID, InstanceID,2)
		fuckjapan_01(MapID, InstanceID,6)
		if  j==1 then

			if nummonster <30 then
				map.MapCreateCreature(MapID, InstanceID, trush, 405, 15076, 235)
				nummonster=nummonster+1
				map.SetMapScriptData(MapID,InstanceID,1,11,nummonster)
			end

			local objtarget = map.GetMapScriptData(MapID,InstanceID,1,10)
			if objtarget == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100097)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

				map.OpenCloseDoor(MapID, InstanceID, 441, 1)
				map.SetMapScriptData(MapID,InstanceID,1,12,1)
			end
		end
	end

	if ObjID==520 and bgot == 0 then

		fuckjapan_01(MapID, InstanceID,4)
		fuckjapan_01(MapID, InstanceID,1)
		fuckjapan_01(MapID, InstanceID,7)
		fuckjapan_01(MapID, InstanceID,5)
		if  j==1 then
			if nummonster <30 then
				map.MapCreateCreature(MapID, InstanceID, trush, 381, 15076, 247)
				nummonster=nummonster+1
				map.SetMapScriptData(MapID,InstanceID,1,11,nummonster)
			end

			local objtarget = map.GetMapScriptData(MapID,InstanceID,1,10)
			if objtarget == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100097)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

				map.OpenCloseDoor(MapID, InstanceID, 441, 1)
				map.SetMapScriptData(MapID,InstanceID,1,12,1)
			end
		end

	end
	if ObjID==521 and bgot == 0 then

		fuckjapan_01(MapID, InstanceID,6)
		fuckjapan_01(MapID, InstanceID,8)
		fuckjapan_01(MapID, InstanceID,2)
		fuckjapan_01(MapID, InstanceID,4)
		fuckjapan_01(MapID, InstanceID,5)
		if  j==1 then
			if nummonster <30 then
				map.MapCreateCreature(MapID, InstanceID, trush, 393, 15076, 247)
				nummonster=nummonster+1
				map.SetMapScriptData(MapID,InstanceID,1,11,nummonster)
			end

			local objtarget = map.GetMapScriptData(MapID,InstanceID,1,10)
			if objtarget == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100097)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

				map.OpenCloseDoor(MapID, InstanceID, 441, 1)
				map.SetMapScriptData(MapID,InstanceID,1,12,1)
			end
		end
	end

	if ObjID==522 and bgot == 0 then

		fuckjapan_01(MapID, InstanceID,3)
		fuckjapan_01(MapID, InstanceID,5)
		fuckjapan_01(MapID, InstanceID,6)
		fuckjapan_01(MapID, InstanceID,9)
		if  j==1 then

			if nummonster <30 then
				map.MapCreateCreature(MapID, InstanceID, trush, 405, 15076, 247)
				nummonster=nummonster+1
				map.SetMapScriptData(MapID,InstanceID,1,11,nummonster)
			end

			local objtarget = map.GetMapScriptData(MapID,InstanceID,1,10)
			if objtarget == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100097)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

				map.OpenCloseDoor(MapID, InstanceID, 441, 1)
				map.SetMapScriptData(MapID,InstanceID,1,12,1)
			end
		end
	end
	if ObjID==523 and bgot == 0 then

		fuckjapan_01(MapID, InstanceID,4)
		fuckjapan_01(MapID, InstanceID,7)
		fuckjapan_01(MapID, InstanceID,8)
		if  j==1 then
			if nummonster <30 then
				map.MapCreateCreature(MapID, InstanceID, trush, 381, 15076, 259)
				nummonster=nummonster+1
				map.SetMapScriptData(MapID,InstanceID,1,11,nummonster)
			end

			local objtarget = map.GetMapScriptData(MapID,InstanceID,1,10)
			if objtarget == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100097)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

				map.OpenCloseDoor(MapID, InstanceID, 441, 1)
				map.SetMapScriptData(MapID,InstanceID,1,12,1)
			end
		end

	end

	if ObjID==524 and bgot == 0 then

		fuckjapan_01(MapID, InstanceID,5)
		fuckjapan_01(MapID, InstanceID,7)
		fuckjapan_01(MapID, InstanceID,8)
		fuckjapan_01(MapID, InstanceID,9)
		if  j==1 then
			if nummonster <30 then
				map.MapCreateCreature(MapID, InstanceID, trush, 393, 15076, 259)
				nummonster=nummonster+1
				map.SetMapScriptData(MapID,InstanceID,1,11,nummonster)
			end
			local objtarget = map.GetMapScriptData(MapID,InstanceID,1,10)
			if objtarget == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100097)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

				map.OpenCloseDoor(MapID, InstanceID, 441, 1)
				map.SetMapScriptData(MapID,InstanceID,1,12,1)
			end
		end

	end
	if ObjID==525 and bgot == 0 then

		fuckjapan_01(MapID, InstanceID,6)
		fuckjapan_01(MapID, InstanceID,8)
		fuckjapan_01(MapID, InstanceID,9)
		if  j==1 then
			if nummonster <30 then

				map.MapCreateCreature(MapID, InstanceID, trush, 405, 15076, 259)
				nummonster=nummonster+1
				map.SetMapScriptData(MapID,InstanceID,1,11,nummonster)
			end

			local objtarget = map.GetMapScriptData(MapID,InstanceID,1,10)
			if objtarget == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100097)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

				map.OpenCloseDoor(MapID, InstanceID, 441, 1)
				map.SetMapScriptData(MapID,InstanceID,1,12,1)
			end
		end

	end



end
aux.RegisterMapEvent("f06", 6, "f06_OnEnterTrigger")
