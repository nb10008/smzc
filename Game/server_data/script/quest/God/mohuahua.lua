--魔化雪莲花
function c5610007_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		msg.AddMsgEvent(MsgID, 20, 442034)		--这是一株被污染的雪莲花，使用一个净化药剂可以将其净化
		msg.AddMsgEvent(MsgID, 21, 4)			--确定按钮
		msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.AddMsgEvent(MsgID, 24, TargetID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 4810005) < 1 then
			msg.AddMsgEvent(MsgID, 20, 442042)		--缺少净化药剂
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
			getFaith(MapID, InstanceID, 50, RoleID)
			local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
			map.MapCreateColCreature(MapID, InstanceID, 6040007, x, y, z, 1, "")
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			msg.AddMsgEvent(MsgID, 20, 442038)		--被污染的雪莲花获得了新生
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- 注册
aux.RegisterCreatureEvent(5610007, 7, "c5610007_OnTalk")


--魔化银鹃花
function c5610008_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		msg.AddMsgEvent(MsgID, 20, 442035)		--这是一株被污染的银鹃花，使用一个净化药剂可以将其净化
		msg.AddMsgEvent(MsgID, 21, 4)			--确定按钮
		msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.AddMsgEvent(MsgID, 24, TargetID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 4810005) < 1 then
			msg.AddMsgEvent(MsgID, 26, 442042)		--缺少净化药剂
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
			getFaith(MapID, InstanceID, 50, RoleID)
			local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
			map.MapCreateColCreature(MapID, InstanceID, 6040008, x, y, z, 1, "")
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			msg.AddMsgEvent(MsgID, 20, 442039)		--被污染的银鹃花获得了新生
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- 注册
aux.RegisterCreatureEvent(5610008, 7, "c5610008_OnTalk")


--魔化聆语花
function c5610009_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		msg.AddMsgEvent(MsgID, 20, 442036)		--这是一株被污染的聆语花，使用一个净化药剂可以将其净化
		msg.AddMsgEvent(MsgID, 21, 4)			--确定按钮
		msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.AddMsgEvent(MsgID, 24, TargetID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 4810005) < 1 then
			msg.AddMsgEvent(MsgID, 26, 442042)		--缺少净化药剂
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
			getFaith(MapID, InstanceID, 50, RoleID)
			local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
			map.MapCreateColCreature(MapID, InstanceID, 6040009, x, y, z, 1, "")
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			msg.AddMsgEvent(MsgID, 20, 442040)		--被污染的聆语花获得了新生
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- 注册
aux.RegisterCreatureEvent(5610009, 7, "c5610009_OnTalk")


--魔化夙语花
function c5610010_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		msg.AddMsgEvent(MsgID, 20, 442037)		--这是一株被污染的雪莲花，使用一个净化药剂可以将其净化
		msg.AddMsgEvent(MsgID, 21, 4)			--确定按钮
		msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.AddMsgEvent(MsgID, 24, TargetID)
	elseif TalkIndex == 4 then
		if role.GetRoleItemNum(RoleID, 4810005) < 1 then
			msg.AddMsgEvent(MsgID, 26, 442042)		--缺少净化药剂
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
			getFaith(MapID, InstanceID, 50, RoleID)
			local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
			map.MapCreateColCreature(MapID, InstanceID, 6040010, x, y, z, 1, "")
			map.MapDeleteCreature(MapID, InstanceID, TargetID)
			msg.AddMsgEvent(MsgID, 20, 442041)		--被污染的夙语花获得了新生
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- 注册
aux.RegisterCreatureEvent(5610010, 7, "c5610010_OnTalk")
