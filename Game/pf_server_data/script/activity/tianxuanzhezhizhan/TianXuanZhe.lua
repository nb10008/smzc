--对象定义

--战场对象，可能开启多个战场
TianXuan_Battle = {}

--阵营对象
TianXuan_Side = {}

--玩家对象
TianXuan_Role = {}

--刷新道具区域对象
TianXuan_Area = {}

TianXuan_Roletotal = 0

--对象定义

--战场对象，可能开启多个战场
KuaFuPK_Battle = {}

--阵营对象
KuaFuPK_Side = {}

--玩家对象
KuaFuPK_Role = {}

--刷新道具区域对象
KuaFuPK_Area = {}

KuaFuPK_Roletotal = 0
--记录战场中工会成员数量

GuildNum = {}

TianXuan_Exp = {}
TianXuan_Exp[50]= 243000
TianXuan_Exp[51]= 255000
TianXuan_Exp[52]= 265000
TianXuan_Exp[53]= 276000
TianXuan_Exp[54]= 289000
TianXuan_Exp[55]= 314000
TianXuan_Exp[56]= 327000
TianXuan_Exp[57]= 339000
TianXuan_Exp[58]= 352000
TianXuan_Exp[59]= 388000
TianXuan_Exp[60]= 507000
TianXuan_Exp[61]= 525000
TianXuan_Exp[62]= 544000
TianXuan_Exp[63]= 564000
TianXuan_Exp[64]= 582000
TianXuan_Exp[65]= 606000
TianXuan_Exp[66]= 627000
TianXuan_Exp[67]= 648000
TianXuan_Exp[68]= 670000
TianXuan_Exp[69]= 720000
TianXuan_Exp[70]= 1073000
TianXuan_Exp[71]= 1107000
TianXuan_Exp[72]= 1143000
TianXuan_Exp[73]= 1180000
TianXuan_Exp[74]= 1217000
TianXuan_Exp[75]= 1256000
TianXuan_Exp[76]= 1293000
TianXuan_Exp[77]= 1331000
TianXuan_Exp[78]= 1370000
TianXuan_Exp[79]= 2116000
TianXuan_Exp[80]= 2178000
TianXuan_Exp[81]= 2241000
TianXuan_Exp[82]= 2303000
TianXuan_Exp[83]= 2370000
TianXuan_Exp[84]= 2438000
TianXuan_Exp[85]= 2505000
TianXuan_Exp[86]= 2572000
TianXuan_Exp[87]= 2640000
TianXuan_Exp[88]= 2709000
TianXuan_Exp[89]= 2782000
TianXuan_Exp[90]= 3490000
TianXuan_Exp[91]= 3546000
TianXuan_Exp[92]= 3602000
TianXuan_Exp[93]= 3662000
TianXuan_Exp[94]= 3720000
TianXuan_Exp[95]= 3777000
TianXuan_Exp[96]= 3833000
TianXuan_Exp[97]= 3887000
TianXuan_Exp[98]= 3941000
TianXuan_Exp[99]= 3996000
TianXuan_Exp[100]= 4050000



--玩家进入竞技场
function S59_OnPlayerEnter(MapID, InstanceID, RoleID)

    if InstanceID ~= 9999 then

		if InstanceID == 100 or InstanceID == 101 then
			local curhour = tonumber(os.date("%H"))--小时
			local curmin = tonumber(os.date("%M"))--分钟
			if curhour == 19 and curmin < 30 then
				unit.AddBuff(MapID, InstanceID, RoleID,6941101,RoleID)
			end
			local RoleObj = KuaFuPK_Role[RoleID]
			RoleObj:GetAttBuff_KuaFuPK()
			RoleObj:WuDi_KuaFuPK()
			RoleObj:SetColor_KuaFuPK()
			--RoleObj:ShowKuaFuPKTime()
			RoleObj:PlayerEnterKuaFuPK()
			--RoleObj.State = "InSide"
			local BattleObj = KuaFuPK_Battle[InstanceID]
			BattleObj:SendAllInfo_KuaFuPK()
			for k,v in pairs(GuildNum) do
			if v.guildID == KuaFuPK_Role[RoleID].Guild then
				if v.instanceID == nil or v.instanceID == 0 then
					v.instanceID = KuaFuPK_Role[RoleID].InstanceID
				end
			end
		end
		else
			local RoleObj = TianXuan_Role[RoleID]
			RoleObj:GetAttBuff_TianXuan()
			RoleObj:WuDi_TianXuan()
			RoleObj:SetColor_TianXuan()
			RoleObj:ShowTianXuanTime()
			RoleObj:PlayerEnterTianXuan()
			--RoleObj.State = "InSide"
			local BattleObj = TianXuan_Battle[InstanceID]
			BattleObj:SendAllInfo_TianXuan()
		end
    end
end

--玩家在竞技场中杀人
function s59_KillPeople(MapID, InstanceID, RoleID, KillerID)

	if unit.IsPlayer(KillerID) and unit.IsPlayer(RoleID) and InstanceID ~= 9999 and RoleID ~= KillerID then
		if InstanceID == 100 or InstanceID == 101 then
			KuaFuPK_Role[RoleID].die = 1
			local RoleObj = KuaFuPK_Role[KillerID]
			RoleObj:GetKillExp_KuaFuPK()
			local point = 1
			local guildID = guild.GetRoleGuildID(KillerID)
			for k1,v1 in pairs(GuildNum) do
				if v1.guildID == guildID then
					if RoleObj.Side == "Red" then
						KuaFuPK_Battle[RoleObj.InstanceID].RedPlayer.Point = KuaFuPK_Battle[RoleObj.InstanceID].RedPlayer.Point + point
					elseif RoleObj.Side == "Blue" then
						KuaFuPK_Battle[RoleObj.InstanceID].BluePlayer.Point = KuaFuPK_Battle[RoleObj.InstanceID].BluePlayer.Point + point
					end
				end
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 206726)          --<color=0xffffff00>你<color=0xfffff7e0>杀死了<color=0xffff0000>XXX<color=0xfffff7e0>
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.DispatchRoleMsgEvent(KillerID, MsgID)     --发送消息

			local BattleObj = KuaFuPK_Battle[InstanceID]
			BattleObj:SendAllInfo_KuaFuPK()
		else
			local RoleObj = TianXuan_Role[KillerID]
			RoleObj:GetKillExp_TianXuan()
			local point = 1
			if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61095) then
				point = 10
			end
			if RoleObj.Side == "Red" then
				TianXuan_Battle[RoleObj.InstanceID].RedPlayer.Point = TianXuan_Battle[RoleObj.InstanceID].RedPlayer.Point + point
			elseif RoleObj.Side == "Blue" then
				TianXuan_Battle[RoleObj.InstanceID].BluePlayer.Point = TianXuan_Battle[RoleObj.InstanceID].BluePlayer.Point + point
			end

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 206726)          --<color=0xffffff00>你<color=0xfffff7e0>杀死了<color=0xffff0000>XXX<color=0xfffff7e0>
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.DispatchRoleMsgEvent(KillerID, MsgID)     --发送消息

			local BattleObj = TianXuan_Battle[InstanceID]
			BattleObj:SendAllInfo_TianXuan()
		end
	end

end

--玩家离开竞技场

function s59_LeaveMap(MapID, InstanceID, RoleID)
    if InstanceID ~= 9999 then
		unit.UnsetState(MapID, InstanceID, RoleID, 10)
		unit.UnsetState(MapID, InstanceID, RoleID, 11)
		if InstanceID == 100 or InstanceID == 101 then
			local RoleObj = KuaFuPK_Role[RoleID]
			if RoleObj ~= nil then
				if KuaFuPK_Battle[InstanceID] ~= nil then
					RoleObj:PlayerLeaveKuaFuPK()
					local BattleObj = KuaFuPK_Battle[InstanceID]
					BattleObj:SendAllInfo_KuaFuPK()
				end
			end
		else
			local RoleObj = TianXuan_Role[RoleID]
			if RoleObj ~= nil then
				RoleObj:PlayerLeaveTianXuan()
				local BattleObj = TianXuan_Battle[InstanceID]
				BattleObj:SendAllInfo_TianXuan()
			end
		end
	end
end

--玩家复活脚本
function s59_RoloRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)

    ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
	ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)
	local k = math.random(9)
	if InstanceID ~= 9999 then
		if InstanceID == 100 or InstanceID == 101 then
			local RoleObj = KuaFuPK_Role[RoleID]
			KuaFuPK_Role[RoleID].die = 0
			RoleObj:WuDi_KuaFuPK()
		else
			local RoleObj = TianXuan_Role[RoleID]
			RoleObj:WuDi_TianXuan()
		end
	end
	return ReviveHP, ReviveMP,S59_Point[k].x, S59_Point[k].y*51,S59_Point[k].z, MapID

end

-- 竞技场的敌我关系判断 -- 除同色以外全是敌人
function s59_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
    if unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) then
		if InstanceID == 9999 then
			local TeamID1 = role.IsRoleHaveTeam(MapID, InstanceID, SrcID)
			local TeamID2 = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
			if TeamID1 ~= 4294967295 and TeamID1 ~= nil and TeamID1 == TeamID2 then
			    return true, false, false, true
			else
			    return false, true, false, true
			end
		else
			if InstanceID == 100 or InstanceID == 101 then
				local RoleObj1 = GetKuaFuPKRoleObj(SrcID)
				local RoleObj2 = GetKuaFuPKRoleObj(TargetID)
				if RoleObj1 ~= nil and RoleObj2 ~= nil then
					if RoleObj1.Side ==  RoleObj2.Side then
						return true, false, false, true
					else
						return false, true, false, true
					end
				end
			else
				local RoleObj1 = GetTianXuanRoleObj(SrcID)
				local RoleObj2 = GetTianXuanRoleObj(TargetID)
				if RoleObj1 ~= nil and RoleObj2 ~= nil then
					if RoleObj1.Side ==  RoleObj2.Side then
						return true, false, false, true
					else
						return false, true, false, true
					end
				end
			end
		end
	end

	return false, false, false, false
end



--注册该活动的相关事件
aux.RegisterMapEvent("s59", 2, "S59_OnPlayerEnter")
aux.RegisterMapEvent("s59", 5, "s59_KillPeople")
aux.RegisterMapEvent("s59", 3, "s59_LeaveMap")
aux.RegisterMapEvent("s59", 11, "s59_RoloRevive")
aux.RegisterMapEvent("s59", 16, "s59_FriendEnemy")
