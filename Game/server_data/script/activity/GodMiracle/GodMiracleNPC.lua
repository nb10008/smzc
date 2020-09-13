function GodMiracle_On_Can_Buy(MapID, InstanceID, ShopID, RoleID, ItemID)

	local MasterGuildID = GetGuildByGodMiracle(MapID)

	local GuildID = guild.GetRoleGuildID(RoleID)

	if MasterGuildID~=GuildID then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1601)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	for i=1,8 do
		if ItemID == 3200818+i then

			local Role_Expoit = role.GetGuildExploit(RoleID)
			local Exploit_Needed = {}
			Exploit_Needed[3200819]=360
			Exploit_Needed[3200820]=360
			Exploit_Needed[3200821]=1080
			Exploit_Needed[3200822]=1080
			Exploit_Needed[3200823]=3240
			Exploit_Needed[3200824]=3240
			Exploit_Needed[3200825]=16200
			Exploit_Needed[3200826]=16200

			if Role_Expoit<Exploit_Needed[ItemID] then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1603)
				msg.AddMsgEvent(MsgID, 9, Exploit_Needed[ItemID])
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return 0
			end
		end
	end

	return 1

end

function SendConfirmMsg(MsgContentID, RoleID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 20, MsgContentID)
	msg.AddMsgEvent(MsgID, 21, 5)
	msg.AddMsgEvent(MsgID, 1, 100001)
	msg.AddMsgEvent(MsgID, 24, TargetID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

function SendYesNoMsg(MsgContentID, RoleID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 20, MsgContentID)
	msg.AddMsgEvent(MsgID, 21, 4)
	msg.AddMsgEvent(MsgID, 1, 100001)
	msg.AddMsgEvent(MsgID, 21, 5)
	msg.AddMsgEvent(MsgID, 1, 100002)
	msg.AddMsgEvent(MsgID, 24, TargetID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

function CheckEnterBattleField(godMiracleID, guildID, RoleID, TargetID)
	local stepindex = 0
	--取公会阵营类型，1表示防守，2表示攻击，0表示没有参战权
	local guildAlliance = guild.GetGuildAllianceType(godMiracleID, guildID)
	if guildAlliance == nil then
		SendConfirmMsg(303005, RoleID, TargetID)				--提示“您的公会没有本次战场的参与权，目前无法进入”
	else
		if guildAlliance == 1 or guildAlliance == 2 then		--属于参战公会
			SendYesNoMsg(303003, RoleID, TargetID)				--是否进入天神冢战场？
			stepindex = 3
		else
			SendConfirmMsg(303005, RoleID, TargetID)			--提示“您的公会没有本次战场的参与权，目前无法进入”
			stepindex = 0
		end
	end
	return stepindex;
end

function BattleFieldTransmitor_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	--如果是第一次对话
	if TalkIndex == -1 then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["BattleField_TalkIndex"], 1)
	end

	local stepindex = role.GetRoleScriptData(RoleID, 1, RoleDataType["BattleField_TalkIndex"])
	local godMiracleID = GodMiracleTypeID[TargetTypeID].GodMiracleTypeID

	if stepindex == 1 then
		--先判断角色是否属于某一个公会
		local guildID = guild.GetRoleGuildID(RoleID)
		if guildID == nil or guildID == 4294967295 then
			SendConfirmMsg(303001, RoleID, TargetID)					--您不属于任何公会，不能进入战场，只有公会成员才能进入战场
			stepindex = 0
		else
			local godCurState = guild.GetGodMiracleCurState(godMiracleID)
			if godCurState == 0 then									--未被占领
				SendYesNoMsg(303002, RoleID, TargetID)					--天神冢战场处于未被占领状态，是否进入？
				stepindex = 2
			elseif godCurState == 1 then								--休战	
				local WeekDay = os.date("%A")
				local Hour = tonumber(os.date("%H"))
				local Min  = tonumber(os.date("%M"))
				
				--时间为周六19：50之后，所有参战成员可进，并且所有玩家都被传出战场了
				if WeekDay=="Saturday" and Hour == 19 and Min >= 50 then -- and broadCastGlag.flag2 == 1
					--检查阵营并进入战场
					stepindex = CheckEnterBattleField(godMiracleID, guildID, RoleID, TargetID)
				else															--时间为周六19：50之前，只有占领公会成员可进
				    local defendGuildID = guild.GetDefendGuildID(godMiracleID)
					if guildID == defendGuildID then		                    --是属于占领公会
						SendYesNoMsg(303003, RoleID, TargetID)					--是否进入天神冢战场？
						stepindex = 2
					else
						SendConfirmMsg(303004, RoleID, TargetID)				--提示“您的公会没有占领该战场，目前无法进入”
						stepindex = 0
					end
				end
			elseif godCurState == 2 then										--争夺中
				stepindex = CheckEnterBattleField(godMiracleID, guildID, RoleID, TargetID)	--检查阵营并进入战场
			end
		end

	--所有人都传送到固定地点
	elseif stepindex == 2 then
		if TalkIndex ~= 4 then
			return
		end

		local DestMapID = GodMiracleTypeID[TargetTypeID].MapID;

		role.RoleGotoNewMap(MapID, InstanceID, RoleID, DestMapID, RoleEnter_Pos[DestMapID].x2, RoleEnter_Pos[DestMapID].y2, RoleEnter_Pos[DestMapID].z2)

		stepindex = 0

	--根据阵营传送到两个不同的地点
	elseif stepindex == 3 then
		if TalkIndex ~= 4 then
			return
		end

		local guildID = guild.GetRoleGuildID(RoleID);
		if guildID ~= nil and guildID ~= -1 then

			--取公会阵营类型，1表示防守，2表示攻击，0表示没有参战权
			local guildAlliance = guild.GetGuildAllianceType(godMiracleID, guildID)
			if guildAlliance ~= nil then
				--取要传送到的地图ID
				local DestMapID = GodMiracleTypeID[TargetTypeID].MapID;
				if guildAlliance == 1 then			--公会属于防守阵营
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, DestMapID, RoleEnter_Pos[DestMapID].x2, RoleEnter_Pos[DestMapID].y2, RoleEnter_Pos[DestMapID].z2)
				elseif guildAlliance == 2 then		--公会属于攻击阵营
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, DestMapID, RoleEnter_Pos[DestMapID].x1, RoleEnter_Pos[DestMapID].y1, RoleEnter_Pos[DestMapID].z1)
				elseif guildAlliance == 0 then		--公会没有参战权
					SendConfirmMsg(602004, RoleID, TargetID)					--提示“您的公会没有本次战场的参与权，目前无法进入”
				end
			end
			stepindex = 0
		end
	end

	role.SetRoleScriptData(RoleID, 1, RoleDataType["BattleField_TalkIndex"], stepindex)

end

function DZTC_Transmitor_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		SendYesNoMsg(303006, RoleID, TargetID)		--“您是否要前往斗战天城？”
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID_S02, S02_Trans_Pos.x, S02_Trans_Pos.y, S02_Trans_Pos.z)
	end
end

aux.RegisterCreatureEvent(4900504, 16, "GodMiracle_On_Can_Buy")
aux.RegisterCreatureEvent(4900505, 16, "GodMiracle_On_Can_Buy")
aux.RegisterCreatureEvent(4900506, 16, "GodMiracle_On_Can_Buy")
aux.RegisterCreatureEvent(4900507, 16, "GodMiracle_On_Can_Buy")
aux.RegisterCreatureEvent(4900508, 16, "GodMiracle_On_Can_Buy")

aux.RegisterCreatureEvent(5010110, 7, "BattleFieldTransmitor_OnTalk")		-- 天神冢战场战场传送人
aux.RegisterCreatureEvent(5010054, 7, "DZTC_Transmitor_OnTalk")			-- 斗战天城传送人
