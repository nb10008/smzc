S59_Point = {}
S59_Point[1] = {x=184,y=67,z=857}
S59_Point[2] = {x=557,y=34,z=842}
S59_Point[3] = {x=791,y=66,z=809}
S59_Point[4] = {x=850,y=35,z=492}
S59_Point[5] = {x=490,y=66,z=567}
S59_Point[6] = {x=189,y=30,z=568}
S59_Point[7] = {x=204,y=64,z=197}
S59_Point[8] = {x=515,y=44,z=255}
S59_Point[9] = {x=806,y=67,z=186}


S59_MapID = 3373743144

S59_ExitMapID = 3424073512
S59_ExitPoint = {x=610,y=23094,z=669}

Xunlianchang_TXZ = {}
Xunlianchang_KuaFu = {}

S59_Area = {}
S59_Area[1] = 450
S59_Area[2] = 451
S59_Area[3] = 452
S59_Area[4] = 453
S59_Area[5] = 454
S59_Area[6] = 455
S59_Area[7] = 456
S59_Area[8] = 457
S59_Area[9] = 476


S59_ItemPoint = {}
S59_ItemPoint[S59_Area[1]] = {x=502,y=2222,z=228}
S59_ItemPoint[S59_Area[2]] = {x=573,y=3326,z=495}
S59_ItemPoint[S59_Area[3]] = {x=815,y=3378,z=214}
S59_ItemPoint[S59_Area[4]] = {x=161,y=3358,z=880}
S59_ItemPoint[S59_Area[5]] = {x=173,y=3386,z=210}
S59_ItemPoint[S59_Area[6]] = {x=214,y=1438,z=525}
S59_ItemPoint[S59_Area[7]] = {x=837,y=1751,z=512}
S59_ItemPoint[S59_Area[8]] = {x=820,y=3333,z=834}
S59_ItemPoint[S59_Area[9]] = {x=541,y=1723,z=855}


--找到最合适的副本实例
function s59_GetOnePerfectMap(MapID, InstanceID, RoleID)
    local temp = false
    if Xunlianchang_TXZ[RoleID] ~= nil and Xunlianchang_TXZ[RoleID] == 1 then
	    Xunlianchang_TXZ[RoleID] = nil
		return 9999
	else
		for k,v in pairs(KuaFuPK_Role) do
	        if v.RoleID == RoleID then
	             temp = true
	        end
	    end
        if temp ==true then
			return KuaFuPK_Role[RoleID].InstanceID
		else
			return TianXuan_Role[RoleID].InstanceID
		end
	end

end
aux.RegisterMapEvent("s59",  14, "s59_GetOnePerfectMap")


--玩家在活动中掉线后上线
function s59_CanTakeOverWhenOnline(MapID, RoleID)
	--修正玩家的上线地图和坐标todo
	return S59_ExitMapID,S59_ExitPoint.x, S59_ExitPoint.y*51,S59_ExitPoint.z
end

aux.RegisterMapEvent("s59",  12, "s59_CanTakeOverWhenOnline")

-------------------------------------------------------------------------------------------------------
--找到地图的出口和坐标
function s59_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, S59_ExitMapID, S59_ExitPoint.x, S59_ExitPoint.y*51,S59_ExitPoint.z
end

aux.RegisterMapEvent("s59",  13, "s59_GetExporMapAndCoord")

-------------------------------------------------------------------------------------------------------
--检测玩家是否能够进入活动场景
function s59_CanEnter(MapID, InstanceID, RoleID)

	if Xunlianchang_TXZ[RoleID] == 1 or (TianXuan_Role[RoleID] ~= nil and TianXuan_Role[RoleID] ~= 0) or (KuaFuPK_Role[RoleID] ~= nil and KuaFuPK_Role[RoleID] ~= 0)  then
		return 1
	else
	    return 0
	end

end

aux.RegisterMapEvent("s59",  15, "s59_CanEnter")


--玩家进入区域
function s59_OnRoleEnterArea(MapID, InstanceID, RoleID, ObjAreaID)
    if InstanceID ~= 9999 then
		if InstanceID == 100 or InstanceID == 101 then
			local AreaObj = GetKuaFuPKAreaObjFromBattle(KuaFuPK_Battle[InstanceID])
			AreaObj:IntoKuaFuPKArea(ObjAreaID,RoleID)
		else
			local AreaObj = GetTianXuanAreaObjFromBattle(TianXuan_Battle[InstanceID])
			AreaObj:IntoTianXuanArea(ObjAreaID,RoleID)
		end
	end
end
aux.RegisterMapEvent("s59", 7, "s59_OnRoleEnterArea")
