--灵魂净化各种判断
function linghunjinghua_canuse(MapID, InstanceID, SkillID, RoleID, TargetID)
--role.IsRoleHaveBuff(地图ID,副本ID,玩家ID,buffID(前五位ID))
	local bool = false
    if unit.IsPlayer(TargetID) then
	    if role.IsRoleHaveBuff(MapID, InstanceID, TargetID, 61003) or role.IsRoleHaveBuff(MapID, InstanceID, TargetID, 61001) or role.IsRoleHaveBuff(MapID, InstanceID, TargetID, 60006) then
	        bool = true
	    end
	else
	    local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, TargetID)
		if TypeID == 1099002 then
		    bool = true
		elseif TypeID == 1099001 and cre.IsHaveBuff(MapID, InstanceID, TargetID, 61000) then
		    bool = true
		elseif TypeID >= 2051001 and TypeID <= 2051060 and cre.IsHaveBuff(MapID, InstanceID, TargetID, 61000) then
	        bool = true
		elseif TypeID >= 2012001 and TypeID <= 2013034 and cre.IsHaveBuff(MapID, InstanceID, TargetID, 61000) then
			bool = true
		elseif TypeID >= 1080001 and TypeID <= 1080140 and cre.IsHaveBuff(MapID, InstanceID, TargetID, 61002) then
		    bool = true
		elseif TypeID == 1099003 and cre.IsHaveBuff(MapID, InstanceID, TargetID, 61004) then
		    bool = true
		--[[elseif TypeID >= 5610007 and TypeID <= 5610010 then
		    bool = true]]
		elseif TypeID == 1098403 and cre.IsHaveBuff(MapID, InstanceID, TargetID, 61004) then
		    bool = true
		--[[elseif TypeID >= 5610004 and TypeID <= 5610006 and cre.IsHaveBuff(MapID, InstanceID, TargetID, 61002) then
		    bool = true]]
		elseif ((TypeID >= 1099004 and TypeID <= 1099020) or (TypeID >= 1098001 and TypeID <= 1098009)) and cre.IsHaveBuff(MapID, InstanceID, TargetID, 60007) then
		    bool = true
		elseif TypeID == 1099021 and cre.IsHaveBuff(MapID, InstanceID, TargetID, 61008) then
		    bool = true
		end
	end
	if not bool then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440101)		--不能对没有魔气萦绕状态的目标使用该技能
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 999
	elseif role.GetRoleItemNum(RoleID, 4810005) < 1 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440102)		--你没有足够的净化药剂来使用灵魂净化
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 999
	else
	    return 0
	end
end
--注册是否可使用
aux.RegisterSkillEvent(2010701, 0, "linghunjinghua_canuse")

--灵魂净化
function linghunjinghua_Cast(MapID, InstanceID, SkillID, RoleID, TargetID)
    role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 420)
    if unit.IsPlayer(TargetID) then
	    if role.IsRoleHaveBuff(MapID, InstanceID, TargetID, 61003) then
		    unit.CancelBuff(MapID, InstanceID,TargetID,6100301)
			getFaith(MapID, InstanceID, 40, RoleID)
		elseif role.IsRoleHaveBuff(MapID, InstanceID, TargetID, 61001) then
		    unit.CancelBuff(MapID, InstanceID,TargetID,6100101)
			getFaith(MapID, InstanceID, 40, RoleID)
		elseif role.IsRoleHaveBuff(MapID, InstanceID, TargetID, 60006) then
	        unit.CancelBuff(MapID, InstanceID,TargetID,6000601)
			getFaith(MapID, InstanceID, 40, RoleID)
	    end
	else
	    local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, TargetID)
		if TypeID == 1099002 then
		    unit.AddBuff(MapID, InstanceID,TargetID,6100501,TargetID)
			getFaith(MapID, InstanceID, 20, RoleID)
		elseif TypeID == 1099001 then
		    unit.AddBuff(MapID, InstanceID,TargetID,6101301,TargetID)
			getFaith(MapID, InstanceID, 10, RoleID)
		elseif TypeID >= 2051001 and TypeID <= 2051060 then
		    unit.AddBuff(MapID, InstanceID,TargetID,6100701,TargetID)
			getFaith(MapID, InstanceID, 20, RoleID)
		elseif TypeID >= 2012001 and TypeID <= 2013034 then
		    unit.AddBuff(MapID, InstanceID,TargetID,6100701,TargetID)
			getFaith(MapID, InstanceID, 20, RoleID)
		elseif TypeID >= 1080001 and TypeID <= 1080140 then
		    unit.CancelBuff(MapID, InstanceID,TargetID,6100201)
			getFaith(MapID, InstanceID, 30, RoleID)
		elseif TypeID == 1099003 then
		    unit.AddBuff(MapID, InstanceID,TargetID,6100601,TargetID)
			getFaith(MapID, InstanceID, 20, RoleID)
			local maxhp = unit.GetUnitAttValue(MapID, InstanceID, TargetID, 8)
			local nowhp = unit.GetUnitAttValue(MapID, InstanceID, TargetID, 9)
			if nowhp > 0 and maxhp/nowhp >= 2 then
			    unit.CancelBuff(MapID, InstanceID,TargetID,6100401)
			end
		--[[elseif TypeID >= 5610007 and TypeID <= 5610010 then
			getFaith(MapID, InstanceID, 10, RoleID)
			local x,y,z = GetPosition(MapID, InstanceID,TargetID)
			MapCreateCreature(MapID, InstanceID, TargetID+430000,x,y,z)
			unit.ChangeHP(MapID, InstanceID, TargetID, -100)]]
		elseif TypeID == 1098403 then
		    unit.AddBuff(MapID, InstanceID,TargetID,6101301,TargetID)
			getFaith(MapID, InstanceID, 20, RoleID)
		--[[elseif TypeID >= 5610004 and TypeID <= 5610006 then
		    unit.CancelBuff(MapID, InstanceID,TargetID,6100201)
			getFaith(MapID, InstanceID, 10, RoleID)
			role.AddRoleItem(MapID, InstanceID, RoleID, 4810019, 1, -1, 1, 104)]]
		elseif ((TypeID >= 1099004 and TypeID <= 1099020) or (TypeID >= 1098001 and TypeID <= 1098009)) then
		    unit.CancelBuff(MapID, InstanceID,TargetID,6000701)
			getFaith(MapID, InstanceID, 40, RoleID)
		elseif TypeID == 1099021 then
		    unit.CancelBuff(MapID, InstanceID,TargetID,6100801)
			--unit.AddBuff(MapID, InstanceID,TargetID,6101001,TargetID)
			unit.AddBuff(MapID, InstanceID,TargetID,6100901,TargetID)
			getFaith(MapID, InstanceID, 20, RoleID)
		end
	end
    return 1
--unit.GetUnitAttValue(地图ID, 副本ID, 怪物ID, 属性索引)
--unit.AddBuff(MapID, InstanceID,unvisiblecre,1042201,unvisiblecre)

--cre.IsHaveBuff(地图ID, 副本ID, 怪物ID, BuffID)
end

aux.RegisterSkillEvent(2010701, 5, "linghunjinghua_Cast")

