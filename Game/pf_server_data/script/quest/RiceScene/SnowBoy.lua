


--雪人调查事件
function C6010047_OnInVest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	--检测玩家是否有五色石，如果有则直接扣除

	local monsterHP = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0)

	local nRet = 0
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(RoleID)
	if FreeSize > 0 then
		nRet = role.RemoveFromRole(MapID, InstanceID, RoleID, 4080079, 1, 1000)
		if 0 == nRet then
			local i = math.random(100)

			if i<2 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 3110001, 1, -1, 8, 102)
			elseif i>2 and i<4 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 3110008, 1, -1, 8, 102)
			elseif i>4 and i<6 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 3110013, 1, -1, 8, 102)
			end

			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			if level == 30 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 780*5)--为原来的5倍
			elseif level == 31 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 810*5)
			elseif level == 32 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 870*5)
			elseif level == 33 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 900*5)
			elseif level == 34 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 960*5)
			elseif level == 35 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 1170*5)
			elseif level == 36 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 1230*5)
			elseif level == 37 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 1470*5)
			elseif level == 38 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 1560*5)
			elseif level == 39 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 1620*5)
			elseif level == 40 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 1890*5)
			elseif level == 41 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 2010*5)
			elseif level == 42 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 2100*5)
			elseif level == 43 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 2190*5)
			elseif level == 44 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 2310*5)
			elseif level == 45 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 2880*5)
			elseif level == 46 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 3030*5)
			elseif level == 47 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 3150*5)
			elseif level == 48 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 3270*5)
			elseif level == 49 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 3420*5)
			elseif level == 50 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 3840*5)
			elseif level > 50 and level < 59 then
			role.AddRoleExp(MapID, InstanceID, RoleID, 4500*5)
			elseif level > 59 and level < 69  then
			role.AddRoleExp(MapID, InstanceID, RoleID, 7500*5)
			elseif level > 69 and level < 79  then
			role.AddRoleExp(MapID, InstanceID, RoleID, 12000*5)
			elseif level > 79   then
			role.AddRoleExp(MapID, InstanceID, RoleID, 17100*5)

			end


			monsterHP=monsterHP+1
			cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0, monsterHP)

			if monsterHP%10 == 0 then
				unit.AddBuff(MapID, InstanceID, CreatureID, 9001401, CreatureID)
				local MsgID = msg.BeginMsgEvent();
				msg.AddMsgEvent(MsgID,102,100103)--场景广播：超级哇啦雪人雕像忽然变大了，请大家继续努力,接下来会发生什么呢？很让人期待啊！
				msg.DispatchBroadcast(MsgID,3424071720,-1,-1);
			end
		else
			unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 404001)--提示没有北洲之雪，无法调查
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)--需要背包空间
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(6010047, 6, "C6010047_OnInVest")

function C6010047_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, CreatureID)

	if MapID==3424071720 then
		map.MapCreateColCreature(MapID, InstanceID, 1091004, x, y, z,1)
	end

	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100104)--场景广播：超级哇啦雪人雕像忽然变大了，请大家继续努力,接下来会发生什么呢？很让人期待啊！
	msg.DispatchBroadcast(MsgID,3424071720,-1,-1);

end

aux.RegisterCreatureEvent(6010047, 4, "C6010047_OnDie")





