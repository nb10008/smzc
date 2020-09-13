
function XNbilu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local a = role.GetRoleItemNum(RoleID, 4800010)--优质面粉
	local b = role.GetRoleItemNum(RoleID, 4800011)--盐粒
	local c = role.GetRoleItemNum(RoleID, 4800012)--黄油
	local d = role.GetRoleItemNum(RoleID, 4800013)--小猪饼干
	local e = role.GetRoleItemNum(RoleID, 4800014)--烟熏火腿
	local f = role.GetRoleItemNum(RoleID, 4800015)--圣诞火鸡
	local g = role.GetRoleItemNum(RoleID, 4800016)--帕斯图红酒
	local h = role.GetRoleItemNum(RoleID, 4800020)--高级火腿
	local i = role.GetRoleItemNum(RoleID, 4800021)--肥美火鸡

	local FreeBag = role.GetBagFreeSize(RoleID)
	local x = act.GetActIsStart(36)--新年活动
	if x == true then
		if FreeBag < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 100003)	-- 请保证背包内有一个空位再来合成
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.AddMsgEvent(MsgID, 21, -1)	    -- 选项一
			msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if TalkIndex == -1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206317)	-- 请选择你要制作的食物
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 206318)	-- 制作小猪饼干
				msg.AddMsgEvent(MsgID, 21, 5)	-- 选项二
				msg.AddMsgEvent(MsgID, 1, 206319)	-- 制作烟熏火腿
				msg.AddMsgEvent(MsgID, 21, 6)	-- 选项三
				msg.AddMsgEvent(MsgID, 1, 206320)	-- 制作圣诞火鸡
				msg.AddMsgEvent(MsgID, 21, 7)	-- 选项四
				msg.AddMsgEvent(MsgID, 1, 206321)	-- 制作新年大餐
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
			if TalkIndex == 4 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206322)	-- 制作小猪饼干：每制作一个小猪饼干就需要优质面粉2个、盐粒1个、黄油2个。你确定要制作小猪饼干么？
				msg.AddMsgEvent(MsgID, 21, 8)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
				msg.AddMsgEvent(MsgID, 21, -1)	-- 选项二
				msg.AddMsgEvent(MsgID, 1, 100002)	-- 取消
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			end
			if TalkIndex == 8 then
				if a >= 2 and b >= 1 and c >= 2 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800010, 2, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800011, 1, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800012, 2, 402)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800013, 1, 0, 0, 402)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 405023)	-- 身上物品不够无法合成，请集齐物品再来合成
					msg.AddMsgEvent(MsgID, 21, -1)	    -- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
			if TalkIndex == 5 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206330)	-- 制作烟熏火腿,每制作一个烟熏火腿就需要黄油2个、盐粒4个、高级火腿1个。你确定要制作烟熏火腿么？
				msg.AddMsgEvent(MsgID, 21, 9)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
				msg.AddMsgEvent(MsgID, 21, -1)	-- 选项二
				msg.AddMsgEvent(MsgID, 1, 100002)	-- 取消
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
			if TalkIndex == 9 then
				if c >= 2 and b >= 4 and h >= 1 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800012, 2, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800011, 4, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800020, 1, 402)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800014, 1, 0, 0, 402)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 405023)	-- 身上物品不够无法合成，请集齐物品再来合成
					msg.AddMsgEvent(MsgID, 21, -1)	    -- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
			if TalkIndex == 6 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206323)	-- 制作圣诞火鸡：每制作一个圣诞火鸡就需要黄油5个、盐粒5个、肥美火鸡1个。你确定要制作圣诞火鸡么？
				msg.AddMsgEvent(MsgID, 21, 10)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
				msg.AddMsgEvent(MsgID, 21, -1)	-- 选项二
				msg.AddMsgEvent(MsgID, 1, 100002)	-- 取消
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			end
			if TalkIndex == 10 then
				if c >= 5 and b >= 5 and i >= 1 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800012, 5, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800011, 5, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800021, 1, 402)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800015, 1, 0, 0, 402)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 405023)	-- 身上物品不够无法合成，请集齐物品再来合成
					msg.AddMsgEvent(MsgID, 21, -1)	    -- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
			if TalkIndex == 7 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206324)	-- 制作新年大餐：每制作一个新年大餐就需要帕图斯红酒1个、烟熏火腿2个、圣诞火鸡1个 小猪饼干5个。你确定要制作新年大餐么？
				msg.AddMsgEvent(MsgID, 21, 11)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
				msg.AddMsgEvent(MsgID, 21, -1)	-- 选项二
				msg.AddMsgEvent(MsgID, 1, 100002)	-- 取消
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
			if TalkIndex == 11 then
				if g >= 1 and e >= 1 and f >= 1 and d >= 5 then
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800016, 1, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800014, 1, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800015, 1, 402)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4800013, 5, 402)
					role.AddRoleItem(MapID, InstanceID, RoleID, 4800017, 1, 0, 0, 402)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 405023)	-- 身上物品不够无法合成，请集齐物品再来合成
					msg.AddMsgEvent(MsgID, 21, -1)	    -- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- 确定
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		local QuestID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(QuestID1, 13, 0)
		msg.AddMsgEvent(QuestID1, 1, 206331)--活动还没有开始
		msg.DispatchRoleMsgEvent(RoleID, QuestID1)
	end
end
aux.RegisterCreatureEvent(5600014, 7, "XNbilu_OnTalk")
