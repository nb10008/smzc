function Buff_OnDestroy ( MapID, InstanceID, RoleID, BuffID )	--buff消失

--[[--两小时结束，第一个buff消失，添加第二个buff
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
	end]]
	if BuffID == 3206001 then--长兵
		local t = math.random(100)
		if t <= 5 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206101,RoleID)
		elseif t > 5 and t <= 30 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206301,RoleID)
		elseif t > 30 and t <= 50 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206401,RoleID)
		elseif t > 50 and t <= 70 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206601,RoleID)
		elseif t > 70 and t <= 100 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206701,RoleID)
		end
	end
	if BuffID == 3207101 then--弓
		local t = math.random(100)
		if t <= 5 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206101,RoleID)
		elseif t > 5 and t <= 30 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206801,RoleID)
		elseif t > 30 and t <= 50 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206401,RoleID)
		elseif t > 50 and t <= 70 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206601,RoleID)
		elseif t > 70 and t <= 100 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206701,RoleID)
		end
	end
	if BuffID == 3207201 then--琴
		local t = math.random(100)
		if t <= 5 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206101,RoleID)
		elseif t > 5 and t <= 30 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206901,RoleID)
		elseif t > 30 and t <= 50 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206401,RoleID)
		elseif t > 50 and t <= 70 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206601,RoleID)
		elseif t > 70 and t <= 100 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206701,RoleID)
		end
	end
	if BuffID == 3207301 then--法器
		local t = math.random(100)
		if t <= 5 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206101,RoleID)
		elseif t > 5 and t <= 30 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206301,RoleID)
		elseif t > 30 and t <= 50 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206401,RoleID)
		elseif t > 50 and t <= 70 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206601,RoleID)
		elseif t > 70 and t <= 100 then
			unit.AddBuff(MapID, InstanceID, RoleID,3206701,RoleID)
		end
	end
------仲夏夜活动
	if BuffID == 9008101 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30798)
		if quest == true then
			unit.AddBuff(MapID, InstanceID, RoleID,9008201,RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 0)
		end
	end
	if BuffID == 9008201 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30798)
		if quest == true then
			unit.AddBuff(MapID, InstanceID, RoleID,9008301,RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 0)
		end
	end
	if BuffID == 9008301 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30798)
		if quest == true then
			unit.AddBuff(MapID, InstanceID, RoleID,9008401,RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 0)
		end
	end
	if BuffID == 9008401 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30798)
		if quest == true then
			unit.AddBuff(MapID, InstanceID, RoleID,9008501,RoleID)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 0)
		end
	end
	if BuffID == 9008501 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30798)
		if quest == true then
			role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 30798, 2)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 1)
			return
		end
	end
end

aux.RegisterBuffEvent( 0, "Buff_OnDestroy" )

function q30798_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local zhongxiayehuodong = role.GetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"])
	if zhongxiayehuodong == 1 then
		return 1
	end
end
aux.RegisterQuestEvent(30798, 5, "q30798_OnComplete")
function q30798_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	unit.AddBuff(MapID, InstanceID, RoleID,9008101,RoleID)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongxiaye"], 0)
end
aux.RegisterQuestEvent(30798, 0, "q30798_Complete")

function q30798_OnCancel(MapID, InstanceID, QuestID, RoleID, NPCID)
	unit.CancelBuff(MapID, InstanceID,RoleID,9008101)
	unit.CancelBuff(MapID, InstanceID,RoleID,9008201)
	unit.CancelBuff(MapID, InstanceID,RoleID,9008301)
	unit.CancelBuff(MapID, InstanceID,RoleID,9008401)
	unit.CancelBuff(MapID, InstanceID,RoleID,9008501)
end
aux.RegisterQuestEvent(30798, 2, "q30798_OnCancel")