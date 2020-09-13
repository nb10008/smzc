
function I4800241_OnUse(MapID, InstanceID, TypeID, TargetID)
	map.CreateInstance(3390518824, TargetID)
	role.RoleGotoNewMap(MapID, InstanceID, TargetID, 3390518824,124,1,139 )

end

function I4800241_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	local curMapId = role.GetRoleMapID(TargetID)
	if(curMapId ~= 3424071720) then
		bRet = 47
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4800241, 1, "I4800241_OnUse")
aux.RegisterItemEvent(4800241, 0, "I4800241_CanUse")



--找到最合适的副本实例
function s63_GetOnePerfectMap(MapID, InstanceID, RoleID)

	return RoleID

end
aux.RegisterMapEvent("s63",  14, "s63_GetOnePerfectMap")

--玩家在活动中掉线后上线
function s63_CanEnterWhenOnline(MapID, RoleID)
	--修正玩家的上线地图和坐标todo
	map.DeleteInstance(MapID, RoleID)
	return 3424073512,610,23094,669
end

aux.RegisterMapEvent("s63",  12, "s63_CanEnterWhenOnline")

-------------------------------------------------------------------------------------------------------
--找到地图的出口和坐标
function s63_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3424073512,610,23094,669
end

aux.RegisterMapEvent("s63",  13, "s63_GetExporMapAndCoord")

--检测玩家是否能够进入活动场景
function s63_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("s63",  15, "s63_CanEnter")


function s63_OnPlayerEnter(MapID, InstanceID,RoleID)
	map.MapCreateColCreature(MapID, InstanceID,5600004 ,140,32,152, true, "");
	local class = role.GetRoleClass(RoleID)
	if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
		map.MapCreateColCreature(MapID, InstanceID,1092001 ,126,31,162, true, "");
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
		map.MapCreateColCreature(MapID, InstanceID,1092006 ,126,31,162, true, "");
    end
end

--注册
aux.RegisterMapEvent("s63", 2, "s63_OnPlayerEnter")

function s63_OnPlayerLeave(MapID, InstanceID,RoleID)
	map.DeleteInstance(MapID, RoleID)
end

--注册
aux.RegisterMapEvent("s63", 3, "s63_OnPlayerLeave")
