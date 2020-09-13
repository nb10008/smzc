gushenmozu_lootNUM = {}
function World_ItemLoop(CreatureID, RoleID, eType, ItemTypeID, SetID)
	if	ItemTypeID == 4800137 then
	    local k = act.GetActScriptData(41,1,2)       --记录冰字是否已经掉落
	    if k >= 1 then
			return 0
		else
			act.SetActScriptData(41, 1, 2, k+1)
			act.SaveActScriptData(41)
			return 1
		end
	elseif	ItemTypeID == 4800141 then
	    local k = act.GetActScriptData(41,1,3)       --记录盛字是否已经掉落
	    if k >= 19 then
			return 0
		else
			act.SetActScriptData(41, 1, 3, k+1)
			act.SaveActScriptData(41)
			return 1
		end
	end

	--新增野外橙装和紫色掉落判断
	local level_suit = role.GetRoleScriptData(RoleID, 1, RoleDataType["Yewaizhuangbei"])
	if(SetID == 739) then
		if(level_suit % 10 > 0) then
			if(math.random(100) <= 20) then
				return 1
			else
				return 0
			end
		elseif(level_suit % 10 == 0) then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["Yewaizhuangbei"], level_suit+1)
			return 1
		end
	end
	if(SetID == 740) then
		if(level_suit%100 >= 10) then
			if(math.random(100) <= 20) then
				return 1
			else
				return 0
			end
		elseif(level_suit%100 < 10) then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["Yewaizhuangbei"], level_suit+10)
			return 1
		end
	end
	if(SetID == 741) then
		if(level_suit%1000 >= 100) then
			if(math.random(100) <= 3) then
				return 1
			else
				return 0
			end
		elseif(level_suit%1000 < 100) then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["Yewaizhuangbei"], level_suit+100)
			return 1
		end
	end

	--猜拳挑战勋章和经验券掉落判断（只有30级以上怪物才掉落）

	local mapID, instanceID = role.GetRoleMapID(RoleID)
	if(ItemTypeID == 4400703) then
		if(mapID == 3424071720 or mapID == 3424072488 or mapID == 3424072232 or mapID == 3424075048) then
			return 1
		else
			return 0
		end
	end
	if(ItemTypeID == 4820043) then
	    local CreatureID = cre.GetCreatureTypeID(mapID, instanceID, CreatureID)
		if (CreatureID >=1040001 and CreatureID <=1060013) or (CreatureID >=1060033 and CreatureID <=1060037) then
			return 1
		else
			return 0
		end
	end
	--天涯明月掉落判断
	if(ItemTypeID == 4820060) then
		local record = role.GetRoleScriptData(RoleID, 1, RoleDataType["tianyamingyue"])
		local lastDate = (record - record%100)/100
		local curDate = tonumber(os.date("%j"))
		local maxNum = record%10
		local curNum = (record%100 - record%10)/10
		if(lastDate ~= curDate) then
			local temp = math.random(100)
			if(temp <= 50) then
				maxNum = 1
			elseif(temp <= 80) then
				maxNum = 2
			elseif(temp <= 95) then
				maxNum = 3
			else
				maxNum = 4
			end
			lastDate = curDate
			curNum = 1
		else
			if(curNum >= maxNum) then
				return 0
			else
				local temp = math.random(10*curNum)
				if(temp <= 1) then
					curNum =curNum+1
				else
					return 0
				end
			end
		end
		record = curDate*100+curNum*10+maxNum
		role.SetRoleScriptData(RoleID, 1, RoleDataType["tianyamingyue"], record)
	end
	if(ItemTypeID == 4820069 and ItemTypeID == 4820070 ) then
	    local CreatureID = cre.GetCreatureTypeID(mapID, instanceID, CreatureID)
		if (CreatureID >=1050001 and CreatureID <=1050045) or (CreatureID >=1060003 and CreatureID <=1070089) then
			return 1
		else
			return 0
		end
	end
	--全世界每天掉落15个守护符，不绑定
	if ItemTypeID == 3020071 or (ItemTypeID >= 4820163 and ItemTypeID <= 4820170) then
	    local curDate = tonumber(os.date("%j"))
    	if gushenmozu_lootNUM.data == nil then
		    gushenmozu_lootNUM.data = curDate
			gushenmozu_lootNUM.shouhuNum = 0
			gushenmozu_lootNUM.shuNum = 0
		elseif gushenmozu_lootNUM.data ~= curDate then
		    gushenmozu_lootNUM.data = curDate
			gushenmozu_lootNUM.shouhuNum = 0
			gushenmozu_lootNUM.shuNum = 0
		end

		if ItemTypeID == 3020071 then
			gushenmozu_lootNUM.shouhuNum = gushenmozu_lootNUM.shouhuNum + 1
			if gushenmozu_lootNUM.shouhuNum > 10 then
				return 0
			else
				return 1
			end
		elseif ItemTypeID >= 4820163 and ItemTypeID <= 4820170 then
		    gushenmozu_lootNUM.shuNum = gushenmozu_lootNUM.shuNum + 1
			if gushenmozu_lootNUM.shuNum > 1 then
				return 0
			else
				return 1
			end
		end
	end
	--体验副本中不产出领域技能书
	if ItemTypeID >= 4640001 and ItemTypeID <= 4643301 then
		local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleID)
		if role.IsRoleHaveBuff(Target_MapID, Target_InstanceID, RoleID, 61096) == true then
			return 0
		end
	end

	return 1
end

aux.RegisterWorldEvent(2, "World_ItemLoop")
