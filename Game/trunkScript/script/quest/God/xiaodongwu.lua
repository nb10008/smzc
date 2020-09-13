--野猪
function c5610004_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		if cre.IsHaveBuff(MapID, InstanceID, TargetID, 61002) == true then
			msg.AddMsgEvent(MsgID, 20, 442043)		--这是一只心智暴躁的野猪，使用一个净化药剂可以让它平静下来
			msg.AddMsgEvent(MsgID, 21, 4)			--确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001)		--“确定”
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.AddMsgEvent(MsgID, 24, TargetID)
		else
			msg.AddMsgEvent(MsgID, 20, 442044)		--呼~~呼~~
		    msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		end
	elseif TalkIndex == 4 then
	    if cre.IsHaveBuff(MapID, InstanceID, TargetID, 61002) == true then
			if role.GetRoleItemNum(RoleID, 4810005) < 1 then
				msg.AddMsgEvent(MsgID, 20, 442042)		--缺少净化药剂
			    msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
			elseif role.GetBagFreeSize(RoleID) < 1 then
				msg.AddMsgEvent(MsgID, 20, 100003)		--缺少净化药剂
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 9, 1)
			    msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
				unit.CancelBuff(MapID, InstanceID, TargetID, 6100201)
				getFaith(MapID, InstanceID, 20, RoleID)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4810019, 1, -1, 1, 104)
				msg.AddMsgEvent(MsgID, 15, 442045)		--暴躁的野猪已经恢复了平静
			    msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
			end
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- 注册
aux.RegisterCreatureEvent(5610004, 7, "c5610004_OnTalk")


--野熊
function c5610005_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local MsgID = msg.BeginMsgEvent()
	if TalkIndex == -1 then
		if cre.IsHaveBuff(MapID, InstanceID, TargetID, 61002) == true then
			msg.AddMsgEvent(MsgID, 20, 442046)		--这是一只心智暴躁的野熊，使用一个净化药剂可以让它平静下来
			msg.AddMsgEvent(MsgID, 21, 4)			--确定按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.AddMsgEvent(MsgID, 24, TargetID)
		else
			msg.AddMsgEvent(MsgID, 20, 442047)		--吼~~吼~~
			msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
		end
	elseif TalkIndex == 4 then
	    if cre.IsHaveBuff(MapID, InstanceID, TargetID, 61002) == true then
			if role.GetRoleItemNum(RoleID, 4810005) < 1 then
				msg.AddMsgEvent(MsgID, 20, 300001)		--缺少净化药剂
				msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
			elseif role.GetBagFreeSize(RoleID) < 1 then
				msg.AddMsgEvent(MsgID, 20, 100003)		--缺少净化药剂
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 9, 1)
			    msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
			else
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4810005, 1, 401)
				unit.CancelBuff(MapID, InstanceID, TargetID, 6100201)
				getFaith(MapID, InstanceID, 15, RoleID)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4810019, 1, -1, 1, 104)
			 	msg.AddMsgEvent(MsgID, 20, 442048)		--暴躁的野熊已经恢复了平静
			    msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,100001)		--“确定”
			end
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
-- 注册
aux.RegisterCreatureEvent(5610005, 7, "c5610005_OnTalk")
