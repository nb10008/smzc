function s5938701_Cast(MapID, InstanceID, SkillID, OwnerID)
	if MapID==1266370126 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 10037)
		map.MapCreateCreature(MapID, InstanceID, 3040252, 324,1151,831)
		map.MapCreateCreature(MapID, InstanceID, 3040253, 350,1151,831)
		map.MapCreateCreature(MapID, InstanceID, 3040254, 339,1151,859)
	end
	local temp = math.random(3)
	local tianjieBOss = map.GetMapScriptData(MapID,InstanceID,1,23)
	if(tianjieBOss ~= nil and tianjieBOss ~= 0) then
		if(temp == 1) then
			unit.AddBuff(MapID, InstanceID, tianjieBOss,5942002,tianjieBOss)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100176)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		elseif(temp == 2) then
			unit.AddBuff(MapID, InstanceID, tianjieBOss,5942102,tianjieBOss)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100177)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		else
			unit.AddBuff(MapID, InstanceID, tianjieBOss,5942202,tianjieBOss)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100178)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		end
	end
end
aux.RegisterSkillEvent(5938701, 1, "s5938701_Cast")


function c3040252_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)

	local tianjieBOss = map.GetMapScriptData(MapID,InstanceID,1,23)
	if(tianjieBOss ~= nil and tianjieBOss ~= 0) then
		if(cre.IsHaveBuff(MapID, InstanceID, TargetID,59420)) then
			unit.CancelBuff(MapID, InstanceID, tianjieBOss, 5942002)
		else
			unit.AddBuff(MapID, InstanceID, tianjieBOss,5942001,tianjieBOss)
		end
	end
	if(cre.IsHaveBuff(MapID, InstanceID, TargetID,59420) )then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100182)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100179)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end

end
aux.RegisterCreatureEvent(3040252, 4, "c3040252_OnDie")


function c3040253_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	local tianjieBOss = map.GetMapScriptData(MapID,InstanceID,1,23)
	if(tianjieBOss ~= nil and tianjieBOss ~= 0) then
		if(cre.IsHaveBuff(MapID, InstanceID, TargetID,59421)) then
			unit.CancelBuff(MapID, InstanceID, tianjieBOss, 5942102)
		else
			unit.AddBuff(MapID, InstanceID, tianjieBOss,5942101,tianjieBOss)
		end
	end
	if(cre.IsHaveBuff(MapID, InstanceID, TargetID,59421) )then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100183)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100180)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end

end
aux.RegisterCreatureEvent(3040253, 4, "c3040253_OnDie")



function c3040254_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	local tianjieBOss = map.GetMapScriptData(MapID,InstanceID,1,23)
	if(tianjieBOss ~= nil and tianjieBOss ~= 0) then
		if(cre.IsHaveBuff(MapID, InstanceID, TargetID,59422)) then
			unit.CancelBuff(MapID, InstanceID, tianjieBOss, 5942202)
		else
			unit.AddBuff(MapID, InstanceID, tianjieBOss,5942201,tianjieBOss)
		end
	end
	if(cre.IsHaveBuff(MapID, InstanceID, TargetID,59422) )then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100184)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100181)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end

end
aux.RegisterCreatureEvent(3040254, 4, "c3040254_OnDie")




function c6011031_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = table.getn(tianjiebaozang)
	if(temp ~= nil and temp > 0) then
		for i=1, temp do  --判断是否是天界宝藏
			if(tianjiebaozang[i] == TargetID) then
				local lastDate = role.GetRoleScriptData(RoleID, 1, RoleDataType["zuihoudezhanyi"])
				local theDays = tonumber(os.date("%j"))
				role.SetRoleScriptData(RoleID, 1, RoleDataType["zuihoudezhanyi"],theDays)
				table.remove(tianjiebaozang,i)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800247, 1, -1, 8, 420)
			end
		end
	end

end
aux.RegisterCreatureEvent(6011031, 4, "c6011031_OnDie")
