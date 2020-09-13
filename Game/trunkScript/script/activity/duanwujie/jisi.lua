--活动开始
jiesi_id = 0
function jiesi_OnStart(actID)
	--[[local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	if jiesi_id == 0 then
		if curyear == 2011 and curmon == 6 and (curday >= 2 and curday <= 14) then]]
			jiesi_id = map.MapCreateCreature(3424073512, -1, 5600065, 505, 23510, 759)
		--[[end
	else
		if curyear ~= 2011 or curmon ~= 6 or curday == 1 or curday > 14 then
			map.MapDeleteCreature(3424073256, -1, jiesi_id)
		end
	end]]

end
aux.RegisterActEvent(40, 2, "jiesi_OnStart")
--活动结束
function jiesi_OnEnd(actID)
	if jiesi_id ~= 0 then
		map.MapDeleteCreature(3424073512, -1, jiesi_id)
	end
end
aux.RegisterActEvent(40, 3, "jiesi_OnEnd")

function q30787_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)

    local CurTime = tonumber(os.date("%j"))
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["duanwujie_time"])--时间

	if CurTime~=UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["duanwujie"], 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["duanwujie_time"], CurTime)
	end

	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["duanwujie"])--任务
	if 0 == TotalNum then
		TotalNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["duanwujie"], TotalNum)
	end

	if TotalNum > 3 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444018)--您每天只能完成X次该任务！
		msg.AddMsgEvent(MsgID, 9, 3)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	else
		return 1
	end
end

aux.RegisterQuestEvent(30787, 4, "q30787_CheckAccept")

--任务接取
function ganxiexin_CanUse(MapID, InstanceID, TypeID, RoleID)
	local a = role.GetRoleQuestNum(RoleID)
	local levle = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local bRet, bIgnore = 0, false
--[[
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["duanwujie"])--任务
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["duanwujie_time"])--时间
	if 0 == TotalNum then
		TotalNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["duanwujie"], TotalNum)
	end
	local CurTime = tonumber(os.date("%j"))
	if CurTime~=UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["duanwujie_time"], 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["duanwujie_time"], CurTime)
	end]]

	if a >= 30 then
		bRet = 21
	else
		if levle < 40 then
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 100008)--等级不足，无法使用物品
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			bRet = 32
		elseif levle >= 40 then
			local a = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30789)
			if a == true then
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 100007)--您已经接取了任务
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
				bRet = 32
			--[[else
				if TotalNum > 3 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, )--每天只能完成3次该任务！
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					bRet = 32
				end]]
			end
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4800033, 0, "ganxiexin_CanUse")

function ganxiexin_onUse(MapID, InstanceID, TypeID, RoleID)
	role.AddQuest(RoleID, 30789)
end
aux.RegisterItemEvent(4800033, 1, "ganxiexin_onUse")

-- 任务完成
function duanwujie_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local Step = role.GetRoleScriptData(RoleID, 1, RoleDataType["duanwujie"])
	Step = Step + 1
	role.SetRoleScriptData(RoleID, 1, RoleDataType["duanwujie"], Step)

end
aux.RegisterQuestEvent(30787, 1, "duanwujie_OnComplete")

--任务放弃
function duanwujie_OnCancel(MapID, InstanceID, QuestID, RoleID)
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["duanwujie_time"])
	local CurTime = tonumber(os.date("%j"))
	if CurTime ~= UpdateTime then
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["duanwujie"], 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["duanwujie_time"], UpdateTime)
	end
end
aux.RegisterQuestEvent(30787, 2, "duanwujie_OnCancel")

function I4800130_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local chengsexuanshang = {}
	chengsexuanshang[40] = 76800
	chengsexuanshang[41] = 63890
	chengsexuanshang[42] = 63890
	chengsexuanshang[43] = 89910
	chengsexuanshang[44] = 94680
	chengsexuanshang[45] = 99470
	chengsexuanshang[46] = 104310
	chengsexuanshang[47] = 109180
	chengsexuanshang[48] = 114080
	chengsexuanshang[49] = 149400
	chengsexuanshang[50] = 156250
	chengsexuanshang[51] = 163150
	chengsexuanshang[52] = 170100
	chengsexuanshang[53] = 177740
	chengsexuanshang[54] = 185420
	chengsexuanshang[55] = 193170
	chengsexuanshang[56] = 200960
	chengsexuanshang[57] = 208820
	chengsexuanshang[58] = 216720
	chengsexuanshang[59] = 270400
	chengsexuanshang[60] = 280800
	chengsexuanshang[61] = 291280
	chengsexuanshang[62] = 301830
	chengsexuanshang[63] = 313240
	chengsexuanshang[64] = 336290
	chengsexuanshang[65] = 336290
	chengsexuanshang[66] = 347930
	chengsexuanshang[67] = 359650
	chengsexuanshang[68] = 371450
	chengsexuanshang[69] = 448160
	chengsexuanshang[70] = 463050
	chengsexuanshang[71] = 478050
	chengsexuanshang[72] = 493140
	chengsexuanshang[73] = 509290
	chengsexuanshang[74] = 525540
	chengsexuanshang[75] = 541890
	chengsexuanshang[76] = 558350
	chengsexuanshang[77] = 574920
	chengsexuanshang[78] = 591590
	chengsexuanshang[79] = 696390
	chengsexuanshang[80] = 716800
	chengsexuanshang[81] = 737350
	chengsexuanshang[82] = 758020
	chengsexuanshang[83] = 779950
	chengsexuanshang[84] = 802020
	chengsexuanshang[85] = 824220
	chengsexuanshang[86] = 846560
	chengsexuanshang[87] = 891650
	chengsexuanshang[88] = 891650
	chengsexuanshang[89] = 915560
	chengsexuanshang[90] = 1057050
	chengsexuanshang[91] = 1084270
	chengsexuanshang[92] = 1111650
	chengsexuanshang[93] = 1140510
	chengsexuanshang[94] = 1169540
	chengsexuanshang[95] = 1331930
	chengsexuanshang[96] = 1364560
	chengsexuanshang[97] = 1397390
	chengsexuanshang[98] = 1430400
	chengsexuanshang[99] = 1465100
	chengsexuanshang[100] = 1500000


	local rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if rolelevel < 40 then
	    rolelevel = 40
	elseif rolelevel > 100 then
	    rolelevel = 100
	end
	local roleexp = math.floor(chengsexuanshang[rolelevel]*0.2)
	role.AddRoleExp(MapID, InstanceID, TargetID, roleexp)

end

aux.RegisterItemEvent(4800130, 1, "I4800130_GiftBag")

function I4800035_GiftBag(MapID, InstanceID, TypeID, TargetID)
	map.MapCreateColCreature(MapID, InstanceID, 1100011, 620, 4394, 474, true, "")
end

function I4800035_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if MapID ~= 1266371662 or x < 570 or x > 670 or z < 424 or z > 524 then
		--提示玩家背包空间不足
		bRet = 32
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800035, 1, "I4800035_GiftBag")
aux.RegisterItemEvent(4800035, 0, "I4800035_CanUseGiftBag")
