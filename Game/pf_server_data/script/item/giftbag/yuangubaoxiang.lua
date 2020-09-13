--远古宝箱
function I4820174_Use(MapID, InstanceID, TypeID, TargetID)

	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820162, 1, 101)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820130+math.random(9)+(math.random(2)-1)*20, 1, -1, 8, 420)
	local ITEM = 0
	local index = math.random(100)
	if index >= 80 then
	    local tbl = {8010017,8020017,8030017,8040017,8050017,8051017,8060017,8061017,8070017,8071017,8080017,8081017,8090017,8091017,8100017,8110017,8111017,8121017,8122017,8130017}
	    ITEM = tbl[math.random(20)]
		role.AddRoleItem(MapID, InstanceID, TargetID, ITEM, 1, 4, 8, 420)
		ITEM = 0
	elseif index >= 60 then
	    local tbl = {8100310,8100410,8100510,8100610,8110310,8111310,8121310,8122310,8130310,8130410,8110310,8111310,8121310,8122310,8130310,8130410,8110310,8111310,8110310,8111310,8050310,8050410,8051310,8051410,8060310,8060410,8061310,8061410,8070310,8070410,8071310,8071410,8080310,8080410,8081310,8081410,8090310,8090410,8091310,8091410,8013010,8023010,8033010,8043010}
        ITEM = tbl[math.random(44)]
		role.AddRoleItem(MapID, InstanceID, TargetID, ITEM, 1, 5, 8, 420)
		ITEM = 0
	elseif index >= 40 then
	    ITEM = 4820191 + math.random(36)
	elseif index >= 30 then
	    ITEM = 4820162 + math.random(8)
	end

	--每15天最多掉出1个稀有戒指
	if yuangujiezhi == nil or yuangujiezhi ~= tonumber(os.date("%j")) then
	    local day = act.GetActScriptData(29, 1, 4) - 1
	    yuangujiezhi = tonumber(os.date("%j"))
		act.SetActScriptData(29, 1, 4, day)
		if day <= 0 then
		    act.SetActScriptData(29, 1, 4, 15)
			act.SetActScriptData(29, 1, 5, 1)
		end
		act.SaveActScriptData(29)
	end
	if act.GetActScriptData(29, 1, 5) == 1 then
	    if math.random(30) == 13 then
		    ITEM = 0
			act.SetActScriptData(29, 1, 5, 0)
		    if math.random(10) >= 9 then
			    local tbl = {8110916,8110919,8111917,8111918}
				role.AddRoleItem(MapID, InstanceID, TargetID, tbl[math.random(4)], 1, 6, 8, 420)
			else
			    local tbl = {8111916,8111919,8110917,8110918}
				role.AddRoleItem(MapID, InstanceID, TargetID, tbl[math.random(4)], 1, 6, 8, 420)
			end
			act.SaveActScriptData(29)
		end
	end

	if ITEM ~= 0 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, ITEM, 1, -1, 8, 420)
	end
	role.AddRoleSilver(MapID, InstanceID, TargetID, math.random(50,100)*100000, 102)
end

function I4820174_CanUse(MapID, InstanceID, TypeID, TargetID)
	local b = role.GetRoleItemNum(TargetID,4820162)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		return 40, false
	end
	if b < 1 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4820174, 1, "I4820174_Use")
aux.RegisterItemEvent(4820174, 0, "I4820174_CanUse")
