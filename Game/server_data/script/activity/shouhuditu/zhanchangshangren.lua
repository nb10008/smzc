function NPC6040050_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取玩家等级
	local x = role.GetWarGold(MapID, InstanceID,RoleID)
	if x < 10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 207123)		--战场BUFF商:\n    你身上不足10战场金币，无法兑换
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 14)			
		msg.AddMsgEvent(MsgID, 1, 100001)		--确定
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 207122)		--战场BUFF商:\n    使用10战场金币可以换取更强BUFF支持与保护
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 4)			
			msg.AddMsgEvent(MsgID, 1, 207112)		--体质增加10
			msg.AddMsgEvent(MsgID, 21, 5)			
			msg.AddMsgEvent(MsgID, 1, 207113)		--武力增加10
			msg.AddMsgEvent(MsgID, 21, 6)			
			msg.AddMsgEvent(MsgID, 1, 207114)		--真元增加10
			msg.AddMsgEvent(MsgID, 21, 7)			
			msg.AddMsgEvent(MsgID, 1, 207115)		--法力增加10
			msg.AddMsgEvent(MsgID, 21, 8)			
			msg.AddMsgEvent(MsgID, 1, 207116)		--最大体力增加150
			msg.AddMsgEvent(MsgID, 21, 9)			
			msg.AddMsgEvent(MsgID, 1, 207117)		--最大真元增加150
			msg.AddMsgEvent(MsgID, 21, 10)			
			msg.AddMsgEvent(MsgID, 1, 207118)		--精准增加50
			msg.AddMsgEvent(MsgID, 21, 11)			
			msg.AddMsgEvent(MsgID, 1, 207119)		--灵巧增加50
			msg.AddMsgEvent(MsgID, 21, 12)			
			msg.AddMsgEvent(MsgID, 1, 207120)		--伤害加深增加5
			msg.AddMsgEvent(MsgID, 21, 13)			
			msg.AddMsgEvent(MsgID, 1, 207121)		--伤害减免增加5
			msg.AddMsgEvent(MsgID, 21, 14)			
			msg.AddMsgEvent(MsgID, 1, 100002)		--取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif TalkIndex == 4 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3922801, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 5 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3922901, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 6 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923001, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 7 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923101, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 8 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923201, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 9 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923301, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 10 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923401, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 11 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923501, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 12 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923601, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		elseif TalkIndex == 13 then
			unit.AddBuff(MapID, InstanceID, RoleID, 3923701, RoleID)
			role.SetWarGold(MapID, InstanceID,RoleID,-10)
		end
	end
end
aux.RegisterCreatureEvent(6040050, 7, "NPC6040050_OnTalk")

function NPC6040057_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取玩家等级
	local x = role.GetRoleItemNum(RoleID, 4820269)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 207124)		--战场金币商:\n    一个兑换卷可以兑换500战场金币
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			
		msg.AddMsgEvent(MsgID, 1, 100001)		--体质增加10
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 100002)		--取消
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if x < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 207125)		--战场金币商:\n    战场金币兑换卷少于1个
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 5)
			msg.AddMsgEvent(MsgID, 1, 100001)		--取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4820269, 1, 420)
			role.SetWarGold(MapID, InstanceID,RoleID,500)
		end
	end
end
aux.RegisterCreatureEvent(6040057, 7, "NPC6040057_OnTalk")
Zshouweishibing = 0
ZLshouweishibing = 0 
function NPC6040051_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取玩家等级
	local table1 = {}
	table1[1] = {800,4200,140}
	table1[2] = {811,4200,140}
	table1[3] = {848,4200,145}
	table1[4] = {811,4200,103}
	local x = role.GetWarGold(MapID, InstanceID,RoleID)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 207126)		--战场金币商:\n    200战场金币可以兑换4个守卫士兵，500金币可以兑换4个老练的守卫士兵
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			
		msg.AddMsgEvent(MsgID, 1, 207128)		--招募守卫士兵4020176
		msg.AddMsgEvent(MsgID, 21, 5)			
		msg.AddMsgEvent(MsgID, 1, 207129)		--招募老练的守卫士兵4020177
		msg.AddMsgEvent(MsgID, 21, 6)
		msg.AddMsgEvent(MsgID, 1, 100002)		--取消
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if x < 200 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 207127)		--战场金币商:\n    战场金币兑换卷少于200个
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100001)		--取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if Zshouweishibing > 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 207131)		--战场金币商:\n    守卫士兵已经出现，不能在召唤
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 100001)		--取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if ZLshouweishibing > 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 207132)		--战场金币商:\n    老练的守卫士兵和守卫士兵不能同时出现
					msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21, 6)
					msg.AddMsgEvent(MsgID, 1, 100001)		--取消
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					for i = 1,4 do
						Zshouweishibing = Zshouweishibing + 1
						map.MapCreateColCreature(MapID, InstanceID,4020176, table1[i][1], table1[i][2], table1[i][3], 1)
					end
					role.SetWarGold(MapID, InstanceID,RoleID,-200)
				end
			end
		end
	elseif TalkIndex == 5 then
		if x < 500 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 207130)		--战场金币商:\n    战场金币兑换卷少于500个
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100001)		--取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if ZLshouweishibing > 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 207133)		--战场金币商:\n    老练的守卫士兵已经出现，不能在召唤
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, 6)
				msg.AddMsgEvent(MsgID, 1, 100001)		--取消
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if Zshouweishibing > 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 207132)		--战场金币商:\n    老练的守卫士兵和守卫士兵不能同时出现
					msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21, 6)
					msg.AddMsgEvent(MsgID, 1, 100001)		--取消
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					for i = 1,4 do
						ZLshouweishibing = ZLshouweishibing + 1
						map.MapCreateColCreature(MapID, InstanceID,4020177, table1[i][1], table1[i][2], table1[i][3], 1)
					end
					role.SetWarGold(MapID, InstanceID,RoleID,-500)
				end
			end
		end
	end
end
aux.RegisterCreatureEvent(6040051, 7, "NPC6040051_OnTalk")
function zhanchangshouwei_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Zshouweishibing = Zshouweishibing -1
end
aux.RegisterCreatureEvent(4020176, 4, "zhanchangshouwei_OnDie")
function Lzhanchangshouwei_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	ZLshouweishibing = ZLshouweishibing -1
end
aux.RegisterCreatureEvent(4020177, 4, "Lzhanchangshouwei_OnDie")