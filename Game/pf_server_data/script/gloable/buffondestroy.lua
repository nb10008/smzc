--[[function Buff_OnDestroy ( MapID, InstanceID, RoleID, BuffID )	--buff消失

--两小时结束，第一个buff消失，添加第二个buff
	if BuffID == 9700101 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9700201, RoleID)

--三小时结束，第二个buff消失，添加第三个buff
	elseif BuffID == 9700201 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9700301, RoleID)
------发放三小时奖励
		role.BaibaoAddRoleItem(MapID, InstanceID, RoleID, 6020001, 2, -1, 12, 1200)
--六小时结束，第三个buff消失，添加第四个buff
	elseif BuffID == 9700301 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9700401, RoleID)
------发放六小时奖励
		role.BaibaoAddRoleItem(MapID, InstanceID, RoleID, 3070010, 1, -1, 12, 1200)
	end
end

aux.RegisterBuffEvent( 0, "Buff_OnDestroy" )]]
