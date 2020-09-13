
KuaFuPKchsr = 0
KuaFuPKtalk = false
function KuaFuPKzhe_OnStart(actID)
	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	for k1,v1 in pairs(GuildNum) do
		guild.SetGuildValue(v1.guildID,0,0)
	end
	if curyear == 2012 and ((curmon == 3 and curday >= 6) or curmon > 3) then
		KuaFuPKtalk = true
		KuaFuPKzhe_baoming = 0
		local enemy = {}
		enemy[4800283] = {guildID = 0,enemyID = 0}
		enemy[4800284] = {guildID = 0,enemyID = 0}
		enemy[4800285] = {guildID = 0,enemyID = 0}
		enemy[4800286] = {guildID = 0,enemyID = 0}
		for k1,v1 in pairs(KuaFuPK_Role) do
			local num1 =role.GetRoleItemNum(v1.RoleID, 4800283)
			local num2 =role.GetRoleItemNum(v1.RoleID, 4800284)
			local num3 =role.GetRoleItemNum(v1.RoleID, 4800285)
			local num4 =role.GetRoleItemNum(v1.RoleID, 4800286)
			if num1 > 0 then
				enemy[4800283].guildID = guild.GetRoleGuildID(v1.RoleID)
				enemy[4800284].enemyID = guild.GetRoleGuildID(v1.RoleID)
			elseif num2 > 0 then
				enemy[4800284].guildID = guild.GetRoleGuildID(v1.RoleID)
				enemy[4800283].enemyID = guild.GetRoleGuildID(v1.RoleID)
			elseif num3 > 0 then
				enemy[4800285].guildID = guild.GetRoleGuildID(v1.RoleID)
				enemy[4800286].enemyID = guild.GetRoleGuildID(v1.RoleID)
			elseif num4 > 0 then
				enemy[4800286].guildID = guild.GetRoleGuildID(v1.RoleID)
				enemy[4800285].enemyID = guild.GetRoleGuildID(v1.RoleID)
			end
		end
		for k1,v1 in pairs(enemy) do
			if v1.guildID ~= 0 and v1.enemyID == 0 then
				guild.SetGuildValue(v1.guildID,1,0)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 211040)  --一方未参赛
				msg.AddMsgEvent(MsgID, 19, v1.guildID)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
		end
		KuaFuPK_ChooseSide()
		for k,v in pairs(KuaFuPK_Role) do
			local guildID = guild.GetRoleGuildID(v.RoleID)
			if guildID == nil or guildID == 4294967295 or guildID == -1 then
				table.remove(KuaFuPK_Role,k)
			end
			for k1,v1 in pairs(GuildNum) do
				if guildID == v1.guildID then
					v1.side = v.Side
				end
			end
			local Target_MapID, Target_InstanceID = role.GetRoleMapID(k)
			if Target_MapID ~= nil and Target_InstanceID ~= nil then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 211039)  --即将开始
				msg.DispatchRoleMsgEvent(v.RoleID, MsgID)
			end
		end
	end

end

function KuaFuPKzhe_OnTimerMin(actID)
	local curhour = tonumber(os.date("%H"))--小时
	local curmin = tonumber(os.date("%M"))--分钟
	if curhour == 19 and curmin == 30 then
		local RoleTblIni1 = {}
		local RoleTblIni2 = {}
		RoleTblIni1 = map.GetMapAllRoleID(3373743144, 100)
		RoleTblIni2 = map.GetMapAllRoleID(3373743144, 101)
		if RoleTblIni1 ~= nil then
			for k,v in pairs(RoleTblIni1) do
				unit.CancelBuff(3373743144, 100,v, 6941101)
			end
		end
		if RoleTblIni2 ~= nil then
			for k,v in pairs(RoleTblIni2) do
				unit.CancelBuff(3373743144, 101, v, 6941101)
			end
		end
		for k,v in pairs(KuaFuPK_Role) do
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 211020)    --战斗开始了，向前冲吧！
			msg.DispatchRoleMsgEvent(v.RoleID, MsgID)
		end
	end
	for k,v in pairs(KuaFuPK_Role) do
		if v.die == 1 then
			v.die = 2
		elseif v.die == 2 then
			role.ForcedToRevive(S59_MapID, v.InstanceID, v.RoleID)
			v.die = 0
		end
		local guildID = guild.GetRoleGuildID(v.RoleID)
		if guildID == nil or guildID == 4294967295 or guildID == -1 then
			local Target_MapID, Target_InstanceID = role.GetRoleMapID(v.RoleID)
			role.RoleGotoNewMap(Target_MapID, Target_InstanceID,v.RoleID, 3424073512, 610,23094,669)
		end
	end
	if ((curhour == 19 and curmin >= 30) or (curhour == 20)) and  curmin % 5 == 0 then
		for k1,v1 in pairs(GuildNum) do
			if v1.instanceID == 100 and KuaFuPK_Battle[100] ~= nil and  v1.curNum ~= 0 then
				if v1.side == 1 or v1.side == "Red" then
					v1.score = KuaFuPK_Battle[100].RedPlayer.Point
				else
					v1.score = KuaFuPK_Battle[100].BluePlayer.Point
				end
			elseif v1.instanceID == 101 and KuaFuPK_Battle[101] ~= nil and  v1.curNum ~= 0 then
				if v1.side == 1 or v1.side == "Red" then
					v1.score = KuaFuPK_Battle[101].RedPlayer.Point
				else
					v1.score = KuaFuPK_Battle[101].BluePlayer.Point
				end
			end
		end
		for k1,v1 in pairs(GuildNum) do
			v1.tempnum = v1.lastNum
			v1.lastNum = v1.curNum
			v1.curNum = 0
		end
		for kx,vx in pairs(KuaFuPK_Battle) do
			local curins = vx.instanceID
			for k,v in pairs(KuaFuPK_Role) do
				local guildID = guild.GetRoleGuildID(v.RoleID)
				for k1,v1 in pairs(GuildNum) do
					if guildID ~= nil and guildID ~= 4294967295 and guildID == v1.guildID then
						v1.curNum = v1.curNum + 1
					end
				end
			end
		end
		for k1,v1 in pairs(GuildNum) do
		local tempbaoming = true
			if KuaFuPK_Battle[v1.instanceID] ~= nil then
				if v1.curNum < 15 then
					v1.tag = false
					if v1.tempnum < 15 then
						for k,v in pairs(KuaFuPK_Role) do
							local guildID = guild.GetRoleGuildID(v.RoleID)
							if guildID == nil or guildID == 4294967295 or guildID == v1.guildID then
								local Target_MapID, Target_InstanceID = role.GetRoleMapID(v.RoleID)
								local num1 =role.GetRoleItemNum(v.RoleID, 4800283)
								local num2 =role.GetRoleItemNum(v.RoleID, 4800284)
								local num3 =role.GetRoleItemNum(v.RoleID, 4800285)
								local num4 =role.GetRoleItemNum(v.RoleID, 4800286)
								if num1 > 0 then
									role.RemoveFromRole(Target_MapID, Target_InstanceID, v.RoleID, 4800283, num1, 101)
								end
								if num2 > 0 then
									role.RemoveFromRole(Target_MapID, Target_InstanceID, v.RoleID, 4800284, num2, 101)
								end
								if num3 > 0 then
									role.RemoveFromRole(Target_MapID, Target_InstanceID, v.RoleID, 4800285, num3, 101)
								end
								if num4 > 0 then
									role.RemoveFromRole(Target_MapID, Target_InstanceID, v.RoleID, 4800286, num4, 101)
								end
								role.RoleGotoNewMap(Target_MapID, Target_InstanceID, v.RoleID, 3424073512,610,23094,669)
							end
						end
						v1.curNum = 0
						local tempid = 0
						local tempnum = 0
						local tempt = false
						for k2,v2 in pairs (GuildNum) do
							if v2.guildID ~= v1.guildID and v2.instanceID == v1.instanceID then
								tempid = v2.guildID
								tempnum = v2.curNum
								tempt = true
								v1.score = -1
								if v1.instanceID == 100 and KuaFuPK_Battle[100] ~= nil then
									if v1.side == 1 or v1.side == "Red"  then
										v1.score = KuaFuPK_Battle[100].RedPlayer.Point
										v2.score = KuaFuPK_Battle[100].BluePlayer.Point
									else
										v1.score = KuaFuPK_Battle[100].BluePlayer.Point
										v2.score = KuaFuPK_Battle[100].RedPlayer.Point
									end
								elseif v1.instanceID == 101 and KuaFuPK_Battle[101] ~= nil then
									if v1.side == 1 or v1.side == "Red"  then
										v1.score = KuaFuPK_Battle[101].RedPlayer.Point
										v2.score = KuaFuPK_Battle[101].BluePlayer.Point
									else
										v1.score = KuaFuPK_Battle[101].BluePlayer.Point
										v2.score = KuaFuPK_Battle[101].RedPlayer.Point
									end
								end
								if tempnum >= 15 then
									guild.SetGuildValue(v1.guildID,2,v1.score)
									guild.SetGuildValue(v2.guildID,1,v2.score)
								else
									guild.SetGuildValue(v1.guildID,2,v1.score)
									guild.SetGuildValue(v2.guildID,2,v2.score)
								end
							end
						end
						if tempt == false then
							guild.SetGuildValue(v1.guildID,1,0)
						end
						if KuaFuPK_Battle[v1.instanceID] ~= nil then
							KuaFuPK_Battle[v1.instanceID] = nil
						end
						map.DeleteInstance(S59_MapID, v1.instanceID)
						if tempid == nil or tempid == 0 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 100, 211040)  --一方未参赛
							msg.AddMsgEvent(MsgID, 19, v1.guildID)
							msg.DispatchBroadcast(MsgID,-1,-1,-1)
							for k,v in pairs(KuaFuPK_Role) do
								if guild.GetRoleGuildID(v.RoleID) == v1.guildID then
									local MsgID1 = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID1, 71, 211047)  --一方未参赛
									msg.DispatchRoleMsgEvent(v.RoleID, MsgID1)
								end
							end
						elseif tempnum < 15 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 100, 211036)  --双方都放弃比赛
							msg.AddMsgEvent(MsgID, 19, v1.guildID)
							msg.AddMsgEvent(MsgID, 19, tempid)
							msg.DispatchBroadcast(MsgID,-1,-1,-1)
							for k,v in pairs(KuaFuPK_Role) do
								if guild.GetRoleGuildID(v.RoleID) == v1.guildID or guild.GetRoleGuildID(v.RoleID) == tempid then
									local MsgID1 = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID1, 71, 211046)  --一方未参赛
									msg.DispatchRoleMsgEvent(v.RoleID, MsgID1)
								end
							end
						else
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 100, 211022)  --一方放弃比赛，另一方获胜
							msg.AddMsgEvent(MsgID, 19, v1.guildID)
							msg.AddMsgEvent(MsgID, 19, tempid)
							msg.DispatchBroadcast(MsgID,-1,-1,-1)
							for k,v in pairs(KuaFuPK_Role) do
								if guild.GetRoleGuildID(v.RoleID) == v1.guildID  then
									local MsgID1 = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID1, 71, 211044)  --一方未参赛
									msg.DispatchRoleMsgEvent(v.RoleID, MsgID1)
								elseif guild.GetRoleGuildID(v.RoleID) == tempid then
									local MsgID1 = msg.BeginMsgEvent()
									msg.AddMsgEvent(MsgID1, 71, 211045)  --一方未参赛
									msg.DispatchRoleMsgEvent(v.RoleID, MsgID1)
								end
							end
						end
					end
				else
					v1.tag = true
				end
			end
		end
	end
    local Min = tonumber(os.date("%M"))
	if curhour == 20 and (Min == 27 or Min == 28 or Min == 29) then
	    for k,v in pairs(KuaFuPK_Battle) do
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 211034)  --天选者之战将在XX分钟后结束
			msg.AddMsgEvent(MsgID, 9, 30-Min)
			msg.DispatchMapMsgEvent(MsgID, S59_MapID,k)
		end
	end
	for k,v in pairs(KuaFuPK_Battle) do
		if KuaFuPK_Battle[k] ~= nil then
			local BattleObj = KuaFuPK_Battle[k]
			BattleObj:SetPlayerPoint_KuaFuPK()
			BattleObj:ResetInSidePlayer_KuaFuPK()
		end
	end

end

function KuaFuPKzhe_OnEnd(actID)
	local record = {}
	record[100] = {redguild = 0 ,redpoint = 0 , blueguild = 0 , bluepoint = 0}
	record[101] = {redguild = 0 ,redpoint = 0 , blueguild = 0 , bluepoint = 0}
	local temp  = 0
	for k1,v1 in pairs(GuildNum) do
		if v1.instanceID == 100 and KuaFuPK_Battle[100] ~= nil then
			if v1.side == 1 or v1.side == "Red"  then
				v1.score = KuaFuPK_Battle[100].RedPlayer.Point
				record[100].redguild = v1.guildID
				record[100].redpoint = v1.score
			else
				v1.score = KuaFuPK_Battle[100].BluePlayer.Point
				record[100].blueguild = v1.guildID
				record[100].bluepoint = v1.score
			end
		elseif v1.instanceID == 101 and KuaFuPK_Battle[101] ~= nil then
			if v1.side == 1 or v1.side == "Red"  then
				v1.score = KuaFuPK_Battle[101].RedPlayer.Point
				record[101].redguild = v1.guildID
				record[101].redpoint = v1.score
			else
				v1.score = KuaFuPK_Battle[101].BluePlayer.Point
				record[101].blueguild = v1.guildID
				record[101].bluepoint = v1.score
			end
		end
		temp = guild.GetGuildValue(v1.guildID)
		guild.SetGuildValue(v1.guildID,temp,v1.score)
	end

	local tempMsg = 0
	for k,v in pairs(record) do
		if v.redguild ~= nil and v.redguild ~= 0 and v.blueguild ~= nil and v.blueguild ~= 0 then
			if v.redpoint > v.bluepoint then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 211021)  --宣布比赛结果，一方获胜
				msg.AddMsgEvent(MsgID, 19, v.redguild)
				msg.AddMsgEvent(MsgID, 9, v.redpoint)
				msg.AddMsgEvent(MsgID, 19, v.blueguild)
				msg.AddMsgEvent(MsgID, 9, v.bluepoint)
				msg.AddMsgEvent(MsgID, 19, v.redguild)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
				for k2,v2 in pairs(KuaFuPK_Role) do
					if guild.GetRoleGuildID(v2.RoleID) == v.redguild then
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 71, 211042)  --宣布比赛结果，一方获胜
						msg.AddMsgEvent(MsgID1, 9, v.redpoint)
						msg.AddMsgEvent(MsgID1, 9, v.bluepoint)
						msg.DispatchRoleMsgEvent(v2.RoleID, MsgID1)
					elseif guild.GetRoleGuildID(v2.RoleID) == v.blueguild then
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 71, 211043)  --宣布比赛结果，一方获胜
						msg.AddMsgEvent(MsgID1, 9, v.bluepoint)
						msg.AddMsgEvent(MsgID1, 9,  v.redpoint)
						msg.DispatchRoleMsgEvent(v2.RoleID, MsgID1)
					end
				end
				tempMsg = 211021
			elseif v.redpoint < v.bluepoint then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 211021)  --宣布比赛结果，另一方获胜
				msg.AddMsgEvent(MsgID, 19, v.redguild)
				msg.AddMsgEvent(MsgID, 9, v.redpoint)
				msg.AddMsgEvent(MsgID, 19, v.blueguild)
				msg.AddMsgEvent(MsgID, 9, v.bluepoint)
				msg.AddMsgEvent(MsgID, 19, v.blueguild)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
				for k2,v2 in pairs(KuaFuPK_Role) do
					if guild.GetRoleGuildID(v2.RoleID) == v.redguild then
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 71, 211043)  --宣布比赛结果，一方获胜
						msg.AddMsgEvent(MsgID1, 9, v.redpoint)
						msg.AddMsgEvent(MsgID1, 9, v.bluepoint)
						msg.DispatchRoleMsgEvent(v2.RoleID, MsgID1)
					elseif guild.GetRoleGuildID(v2.RoleID) == v.blueguild then
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 71, 211042)  --宣布比赛结果，一方获胜
						msg.AddMsgEvent(MsgID1, 9, v.bluepoint)
						msg.AddMsgEvent(MsgID1, 9, v.redpoint)
						msg.DispatchRoleMsgEvent(v2.RoleID, MsgID1)
					end
				end
				tempMsg = 211021
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 211037)  --宣布比赛结果，平局
				msg.AddMsgEvent(MsgID, 19, v.redguild)
				msg.AddMsgEvent(MsgID, 9, v.redpoint)
				msg.AddMsgEvent(MsgID, 19, v.blueguild)
				msg.AddMsgEvent(MsgID, 9, v.bluepoint)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
				for k2,v2 in pairs(KuaFuPK_Role) do
					if guild.GetRoleGuildID(v2.RoleID) == v.redguild or guild.GetRoleGuildID(v2.RoleID) == v.blueguild then
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 71, 211041)  --宣布比赛结果，平局
						msg.AddMsgEvent(MsgID1, 9, v.redpoint)
						msg.AddMsgEvent(MsgID1, 9, v.bluepoint)
						msg.DispatchRoleMsgEvent(v2.RoleID, MsgID1)
					end
				end
				tempMsg = 211037
			end
		end
	end



	if KuaFuPKchsr ~= nil and KuaFuPKchsr ~= 0 then
		map.MapDeleteCreature(3424073512, -1, KuaFuPKchsr)
	end
    for k,v in pairs(KuaFuPK_Battle) do
		if KuaFuPK_Battle[k] ~= nil then
			--local BattleObj = KuaFuPK_Battle[k]
			--BattleObj:SetPlayerFinalPoint_KuaFuPK()
			map.DeleteInstance(S59_MapID, k)
		end
	end


    --战场对象，可能开启多个战场
	KuaFuPK_Battle = {}
	--阵营对象
	KuaFuPK_Side = {}
	--玩家对象
	KuaFuPK_Role = {}
	--刷新道具区域对象
	KuaFuPK_Area = {}
    KuaFuPK_Roletotal = 0
	GuildNum = {}
	KuaFuPKtalk = false
end



--注册该活动的相关事件
aux.RegisterActEvent(54, 3, "KuaFuPKzhe_OnEnd")
aux.RegisterActEvent(54, 2, "KuaFuPKzhe_OnStart")
aux.RegisterActEvent(54, 4, "KuaFuPKzhe_OnTimerMin")


--斗技场NPC对话脚本
function x5610105_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    local tbl = {}
	tbl[30] = 21000
	tbl[31] = 21600
	tbl[32] = 23000
	tbl[33] = 24600
	tbl[34] = 26100
	tbl[35] = 27700
	tbl[36] = 29300
	tbl[37] = 30900
	tbl[38] = 31600
	tbl[39] = 33000
	tbl[40] = 35600
	tbl[41] = 37500
	tbl[42] = 39600
	tbl[43] = 41700
	tbl[44] = 55300
	tbl[45] = 57900
	tbl[46] = 60500
	tbl[47] = 63000
	tbl[48] = 65600
	tbl[49] = 68400
	tbl[50] = 71300
	tbl[51] = 74100
	tbl[52] = 77000
	tbl[53] = 80100
	tbl[54] = 83300
	tbl[55] = 86400
	tbl[56] = 89500
	tbl[57] = 92700
	tbl[58] = 95800
	tbl[59] = 117500
	tbl[60] = 121500
	tbl[61] = 125600
	tbl[62] = 129600
	tbl[63] = 134000
	tbl[64] = 138000
	tbl[65] = 142800
	tbl[66] = 147200
	tbl[67] = 151600
	tbl[68] = 156000
	tbl[69] = 189300
	tbl[70] = 194800
	tbl[71] = 200400
	tbl[72] = 206000
	tbl[73] = 211900
	tbl[74] = 217900
	tbl[75] = 253400
	tbl[76] = 260100
	tbl[77] = 266900
	tbl[78] = 273600
	tbl[79] = 280800
	tbl[80] = 288000
	tbl[81] = 295200
	tbl[82] = 302400
	tbl[83] = 310100
	tbl[84] = 317700
	tbl[85] = 368800
	tbl[86] = 377400
	tbl[87] = 386100
	tbl[88] = 394800
	tbl[89] = 404000
	tbl[90] = 413100
	tbl[91] = 422300
	tbl[92] = 431500
	tbl[93] = 441200
	tbl[94] = 450900
	tbl[95] = 508000
	tbl[96] = 518700
	tbl[97] = 529400
	tbl[98] = 540000
	tbl[99] = 551300
	tbl[100] = 562500
	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	local curhour = tonumber(os.date("%H"))--小时
	local curmin = tonumber(os.date("%M"))--分钟
    --获取玩家等级
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel > 100 then
	    rolelevel = 100
	end
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()

		if curyear == 2012 and ((curmon == 2 and curday >= 28) or (curmon == 3 and curday <= 4)) then
			msg.AddMsgEvent(MsgID, 20,	211002)		--“跨服争霸预选人：\n    跨服争霸赛马上就要开始，现在是准备期间，赶紧去收集对决之证吧”
			msg.AddMsgEvent(MsgID, 24,	TargetID)
			msg.AddMsgEvent(MsgID, 21,	5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	211008)		--“提交对决之证”
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	211006)		--“查看提交排名”
			msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	211007)		--“查看参与规则”
			msg.AddMsgEvent(MsgID, 21,	8)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	211031)		--“查看提交数量”

		elseif curyear == 2012 and curmon == 3 and curday > 4 then
			msg.AddMsgEvent(MsgID, 20,	211005)		--“跨服争霸预选人：\n    跨服争霸赛已经开始，详细赛事请去官网查询”
			msg.AddMsgEvent(MsgID, 24,	TargetID)
			msg.AddMsgEvent(MsgID, 21,	6)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	211006)		--“查看提交排名”
			msg.AddMsgEvent(MsgID, 21,	7)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	211007)		--“查看参与规则”
			msg.AddMsgEvent(MsgID, 21,	8)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	211031)		--“查看提交数量”
		end
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 5 then
		local guildID = guild.GetRoleGuildID(RoleID)
		local num = role.GetRoleItemNum(RoleID, 4800282)
		local guildNu = guild.GetGuildValue(guildID)
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if guildNu == nil then
			guildNu = 0
		end
		if guildID == nil or guildID == -1 or guildID == 4294967295 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211018)		--“跨服争霸预选人：\n    你没有公会。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif num > 0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 4800282, 1, 420)
			TiJiaoDuijue(guildID)
			guildNu = guild.GetGuildValue(guildID)
			if level >= 30 then
				role.AddRoleExp(MapID, InstanceID, RoleID, tbl[level])
			else
				role.AddRoleExp(MapID, InstanceID, RoleID, level*700)
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211024)		--“跨服争霸预选人：\n    提交成功。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 9,	guildNu)
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211010)		--“跨服争霸预选人：\n    你没有对决之证。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 6 then
		local k = nil
		local guildRank = {}
		guildRank[1] = {}
		guildRank[2] = {}
		local r = 0
		for i = 0, 9 do
			local guildID = act.GetActScriptData(54, 1, i)
			if guildID ~= nil and guildID ~= 0 and guildID ~= 4294967295 and guild.IsHaveGuild(guildID) then
				k = i
				guildRank[1][r] = act.GetActScriptData(54, 1, i)
				guildRank[2][r] = act.GetActScriptData(54, 1, 10+i)
				r = r + 1
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211015) -- "跨服争霸预选人：\n    目前还没有公会提交对决之证"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211011) -- "跨服争霸预选人：\n    目前为止，只有一个公会提交了对决之证：\n    &lt;p1&gt;  &lt;p2&gt;个
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 19, guildRank[1][0])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][0])
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211012) -- "跨服争霸预选人：\n    目前为止，只有两个公会提交了对决之证：\n    &lt;p1&gt;  &lt;p2&gt;个\n    &lt;p3&gt;  &lt;p4&gt;个
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 19, guildRank[1][0])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][0])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][1])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][1])
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211013) -- "跨服争霸预选人：\n    目前为止，只有三个公会提交了对决之证：\n    &lt;p1&gt;  &lt;p2&gt;个\n    &lt;p3&gt;  &lt;p4&gt;个\n    &lt;p5&gt;  &lt;p6&gt;个
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 19, guildRank[1][0])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][0])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][1])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][1])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][2])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][2])
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 3 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211014) -- "跨服争霸预选人：\n    目前为止，只有四个公会提交了对决之证：\n    &lt;p1&gt;  &lt;p2&gt;个\n    &lt;p3&gt;  &lt;p4&gt;个\n    &lt;p5&gt;  &lt;p6&gt;个\n    &lt;p7&gt;  &lt;p8&gt;个
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 19, guildRank[1][0])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][0])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][1])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][1])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][2])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][2])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][3])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][3])
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k >= 4 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211001) -- "跨服争霸预选人：\n    目前为止，提交对决之证最多的五个公会是：\n    &lt;p1&gt;  &lt;p2&gt;个\n    &lt;p3&gt;  &lt;p4&gt;个\n    &lt;p5&gt;  &lt;p6&gt;个\n    &lt;p7&gt;  &lt;p8&gt;个\n    &lt;p9&gt;  &lt;p10&gt;个
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 19, guildRank[1][0])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][0])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][1])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][1])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][2])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][2])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][3])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][3])
			msg.AddMsgEvent(MsgID, 19, guildRank[1][4])
			msg.AddMsgEvent(MsgID, 9, guildRank[2][4])
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end
	elseif TalkIndex == 7 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 211004) -- "跨服争霸预选人：\n    跨服争霸赛的参与规则如下\n    公会必须通过预选获得参与资格（即提交对决之证的数量排名前五）\n    公会至少要有15个80级以上的角色（80级以上的角色才能进入跨服争霸赛场景）\n    根据官方安排，按时参与跨服争霸赛的角逐（详细流程将会在官网公布）
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
		msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	elseif TalkIndex == 8 then
		local guildID = guild.GetRoleGuildID(RoleID)
		local guildNu = guild.GetGuildValue(guildID)
		if guildNu == nil then
			guildNu = 0
		end
		if guildID == nil or guildID == -1 or guildID == 4294967295 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211032)		--“跨服争霸预选人：\n    你没有公会。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211033)		--“跨服争霸预选人：\n    你所在公会&lt;p1&gt;提交了&lt;p2&gt;个对决之证”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 19,	guildID)
			msg.AddMsgEvent(MsgID, 9,	guildNu)
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end

aux.RegisterCreatureEvent(5610105, 7, "x5610105_OnTalk")


function x5610106_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	local curhour = tonumber(os.date("%H"))--小时
	local curmin = tonumber(os.date("%M"))--分钟
	local guildID = guild.GetRoleGuildID(RoleID)
    --获取玩家等级
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel > 100 then
	    rolelevel = 100
	end
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	211019)		--“跨服争霸传送人：\n    战斗马上就要开始，你准备好了吗？
		msg.AddMsgEvent(MsgID, 24,	TargetID)
		if KuaFuPKtalk == false then
			msg.AddMsgEvent(MsgID, 21,	8)			--报名参赛
			msg.AddMsgEvent(MsgID, 1,	211025)
		else
			msg.AddMsgEvent(MsgID, 21,	9)			--进入争霸场景
			msg.AddMsgEvent(MsgID, 1,	211009)		--
		end
		msg.AddMsgEvent(MsgID, 21,	10)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100001)		--“
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 8 then
		if role.GetRoleLevel(MapID, InstanceID, RoleID) < 80 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211027)		--“跨服争霸传送人：\n    你等级不足80级，无法进入场景。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    elseif role.GetRoleItemNum(RoleID, 4800283) < 1 and role.GetRoleItemNum(RoleID, 4800284) < 1 and role.GetRoleItemNum(RoleID, 4800285) < 1 and role.GetRoleItemNum(RoleID, 4800286) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211026)		--“跨服争霸预选人：\n    你没有对决之令，无法进入场景。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif guildID == nil or guildID == -1 or guildID == 4294967295 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211028)		--跨服争霸传送人：\n    你没有公会，无法进入场景
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if KuaFuPK_Role[RoleID] ~= nil then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	211029)		--跨服争霸传送人：\n    你已经报名了，无需重复报名赛
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				CreatKuaFuPKRoleObj(RoleID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20,	211030)		--跨服争霸传送人：\n    你没有公会，无法进入场景
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
				msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
        end
	elseif TalkIndex == 9 then
		if role.GetRoleLevel(MapID, InstanceID, RoleID) < 80 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211017)		--“跨服争霸传送人：\n    你等级不足80级，无法进入场景。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    elseif role.GetRoleItemNum(RoleID, 4800283) < 1 and role.GetRoleItemNum(RoleID, 4800284) < 1 and role.GetRoleItemNum(RoleID, 4800285) < 1 and role.GetRoleItemNum(RoleID, 4800286) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211016)		--“跨服争霸预选人：\n    你没有对决之令，无法进入场景。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif guildID == nil or guildID == -1 or guildID == 4294967295 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	211023)		--跨服争霸传送人：\n    你没有公会，无法进入场景
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if Xunlianchang_KuaFu == nil then
				Xunlianchang_KuaFu = {}
			end
			Xunlianchang_KuaFu[RoleID] = 1
			local k = math.random(9)
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, S59_MapID, S59_Point[k].x, S59_Point[k].y, S59_Point[k].z)      --传送
        end
	end
end
aux.RegisterCreatureEvent(5610106, 7, "x5610106_OnTalk")

function TiJiaoDuijue(guildID)
    local i = guild.GetGuildValue(guildID)
	local guildNu = guild.GetGuildValue(guildID)
	if i == nil then
	    i = 0
	end
	for q=0,9 do                       --容错性修改
	    if act.GetActScriptData(54, 1, q) == guildID then
		    if act.GetActScriptData(54, 1, q+10) >= i then
			    i = act.GetActScriptData(54, 1, q+10)
			end
		end
	end
	i = i + 1
	guild.SetGuildValue(guildID,guildNu+1,0)

	local arrID = {}
	local arrNum = {}
	local bNew = 1
	for l=0,9 do
		arrID[l] = act.GetActScriptData(54, 1, l)
		if arrID[l] == nil then
		    arrID[l] = 0
		end
		if arrID[l] == guildID  then
			arrNum[l] = i
			bNew = 0
		else
			arrNum[l] = act.GetActScriptData(54, 1, l+10)
			if arrNum[l] == nil then
		        arrNum[l] = 0
		    end
		end
	end

	if bNew == 1 then
		arrID[10] = guildID;
		arrNum[10] = i;
	else
		arrID[10] = 0;
		arrNum[10] = 0;
	end

	for m=0, 9 do
		for n=m+1,10 do
			if arrNum[n] > arrNum[m] then
				local temp = arrNum[m]
				arrNum[m] = arrNum[n]
				arrNum[n] = temp
				temp = arrID[m]
				arrID[m] = arrID[n]
				arrID[n] = temp
			end
		end
	end

	for j=0,9 do
		act.SetActScriptData(54, 1, j, arrID[j])
		act.SetActScriptData(54, 1, j+10, arrNum[j])
	end

	act.SaveActScriptData(54)

end
