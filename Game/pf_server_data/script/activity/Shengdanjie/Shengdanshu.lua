


--圣诞树调查事件
function C6030001_OnInVest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	--检测玩家是否有圣诞袜，如果有则扣除

	local monsterHP = cre.GetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0)

	local nRet = 0
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(RoleID)
	if FreeSize > 0 then
		nRet = role.RemoveFromRole(MapID, InstanceID, RoleID, 4800024, 1, 1000)
		if 0 == nRet then
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			local i = math.random(100)

			if i <= 5 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800004, 1, -1, 8, 102)
			elseif i > 5 and i<=25 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800008, 1, -1, 8, 102)
			elseif i>=50 and i<54 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800240, 1, -1, 8, 102)
			elseif i>=97 and i<98 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4800003, 1, -1, 8, 102)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 300024)  --[<P1>]在圣诞树下获得了一颗闪闪发光的雪之钻，真是幸运！
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchBroadcast(MsgID, -1, -1, -1)
			else
				if level >= 30 and level <= 34 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 15000)
				elseif level > 34 and level <= 39 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 15000)
				elseif level >= 40 and level <= 49 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 30000)
				elseif level >= 50 and level <= 59 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 55000)
				elseif level >= 60 and level <= 69 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 80000)
				elseif level >= 70 and level <= 79 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 100000)
				elseif level >= 80 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 120000)
				end
			end



			monsterHP=monsterHP+1
			cre.SetCreatureScriptData(MapID, InstanceID, CreatureID, 1, 0, monsterHP)

			if monsterHP%300 == 0 then
				unit.AddBuff(MapID, InstanceID, CreatureID, 9004401, CreatureID)--体型缩小BUFF添加
				local MsgID = msg.BeginMsgEvent();
				msg.AddMsgEvent(MsgID,102, 300025)--神秘居然圣诞树缩小了，看来上面的礼物也越来越少了，再不来圣诞树上的礼物就要被别人抢光了呀！
				msg.DispatchBroadcast(MsgID, -1, -1, -1)
			end
			if monsterHP == 1000 then
			unit.CancelBuff(MapID, InstanceID, CreatureID, 6936601)
			unit.CancelBuff(MapID, InstanceID, CreatureID, 6936801)
			end
		else
			unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 206316)--提示没有圣诞袜，无法调查
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)--需要背包空间
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(6030001, 6, "C6030001_OnInVest")






function c6030001_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			yaojing = map.MapCreateCreature(MapID, InstanceID, 1092011, x+1, y, z+1)
			unit.AddBuff(MapID, InstanceID, yaojing, 5932801, yaojing)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300026)    --神秘圣诞树消失了，圣诞妖精米米拉绚丽登场！3分钟后即可击杀，通过契约可获得珍贵的圣诞妖精！
			msg.DispatchWorldMsgEvent(MsgID)


end

function c1092011_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

			local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
			map.MapCreateCreature(MapID, InstanceID, 6010071, x+1, y, z+1)
			map.MapCreateCreature(3424073512, InstanceID, 5600020, 627, 23510, 585)
			local MsgID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID1, 102, 300027)  --圣诞兔子已经出现在斗战天城的<585，600>，请大家前去领取圣诞宝宝的祝福奖励，此祝福可使获得经验提升1.5倍
			msg.DispatchWorldMsgEvent(MsgID1)
end

aux.RegisterCreatureEvent(6030001,4,"c6030001_OnDie")
aux.RegisterCreatureEvent(1092011,4,"c1092011_OnDie")
