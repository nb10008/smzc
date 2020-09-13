--20607
--需求1：使用物品墓碑[4010144]后刷出墓碑[6010022]，并获得坟头的土[4010172]


--接去任务
function i4010144_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4010172, 1, 0, 0, 300)
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	if m == nil then
		m = 0
	end
	if m < 5 then
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 6010022, x+1, y, z+1)
	map.SetMapScriptData(MapID,InstanceID,1,1,m+1)
	end
	if map.GetMapScriptData(MapID,InstanceID,1,1) >= 4 then
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 148)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
	bRet = 32
	end


end

function i4010144_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
		return bRet, bIgnore
	end
end


--注册
aux.RegisterItemEvent(4010144, 1, "i4010144_QuestUsable")
aux.RegisterItemEvent(4010144, 0, "i4010144_CanUse")


