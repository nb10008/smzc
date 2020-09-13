
--玩家等级改变
function x_RoleLevelChange(MapID, InstanceID, RoleID)

	local classid = role.GetRoleClass(RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local devil = role.GetRoleSpecFlag(MapID, InstanceID, RoleID)  --获得玩家账号标识（判断是否是恶魔玩家）

	if level % 10 == 0 and level > 9 then
		role.NotifyFBBS(RoleID,2,level)
	end

	if level >20 and classid == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 71, 402001)--您现在的职业为新手，20级之后可进行转职，请尽快前往斗战天城接取转职任务！
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	--[[elseif level == 60 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 71, 402002)--60级后可进行职业二转，请前往斗战天城接取二转职业任务！
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		]]
	elseif level >60 and (classid >= 2 and classid <= 5) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 71, 402002)--60级后可进行职业二转，请前往斗战天城接取二转职业任务！
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

    if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69370) then
	    if level >= 60 then
		    unit.CancelBuff(MapID, InstanceID, RoleID, 6937001)
		elseif level == 32 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 206430)--新的特权任务可以接取！\n您现在可以在斗战天城的新人特权使者处领取新的特权任务（使用10个北洲之雪换取25个契约道具）！
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif level == 42 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 206431)--新的特权任务可以接取！\n您现在可以在斗战天城的新人特权使者处领取新的特权任务（完成一次百鬼炼狱活动然后领取6个勇者之证）！
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
	--5倍悬赏任务经验奖励buff判断（80以上玩家去掉buff）
    if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61050) then
	    if level >= 80 then
		    unit.CancelBuff(MapID, InstanceID, RoleID, 6105001)
		end
	end

	--80级以上玩家获得声望加倍buff
	if shengwangjiabei ~= 0 and level >= 80 and not role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69374) then
	    unit.AddBuff(MapID, InstanceID, RoleID,6937399+shengwangjiabei,RoleID) --80级以上玩家每次上线获得声望加倍buff
	end

	if ((devil == 1 or devil == 2) and level >= 70 and level < 80) then  --来自恶魔的玩家，到达70级添加龙神祝福buff
		unit.AddBuff(MapID, InstanceID, RoleID,6104501,RoleID)
	end
	if (role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 61045)) then --80级取消龙神祝福
		if level >= 80 then
			unit.CancelBuff(MapID, InstanceID, RoleID, 6104501)
		end
	end
	---------------------------------------成就相关----------------------------------------------
	role.SetTrialCompleteNum(RoleID, 1001, level)
	role.SetTrialCompleteNum(RoleID, 1002, level)
	role.SetTrialCompleteNum(RoleID, 2001, level)
	role.SetTrialCompleteNum(RoleID, 2002, level)
	role.SetTrialCompleteNum(RoleID, 3001, level)
	role.SetTrialCompleteNum(RoleID, 3002, level)
	role.SetTrialCompleteNum(RoleID, 4001, level)
	role.SetTrialCompleteNum(RoleID, 4002, level)
	role.SetTrialCompleteNum(RoleID, 5001, level)
	role.SetTrialCompleteNum(RoleID, 5002, level)
	role.SetTrialCompleteNum(RoleID, 6001, level)
	role.SetTrialCompleteNum(RoleID, 6002, level)
	role.SetTrialCompleteNum(RoleID, 7001, level)
	role.SetTrialCompleteNum(RoleID, 7002, level)
	role.SetTrialCompleteNum(RoleID, 8001, level)
	role.SetTrialCompleteNum(RoleID, 8002, level)
	role.SetTrialCompleteNum(RoleID, 8003, level)
	role.SetTrialCompleteNum(RoleID, 8004, level)
	role.SetTrialCompleteNum(RoleID, 8005, level)
	role.SetTrialCompleteNum(RoleID, 9001, level)
	role.SetTrialCompleteNum(RoleID, 9002, level)
	role.SetTrialCompleteNum(RoleID, 9003, level)
	role.SetTrialCompleteNum(RoleID, 9004, level)
	role.SetTrialCompleteNum(RoleID, 9005, level)
	role.SetTrialCompleteNum(RoleID, 9006, level)
	role.SetTrialCompleteNum(RoleID, 9007, level)
	role.SetTrialCompleteNum(RoleID, 9008, level)
	role.SetTrialCompleteNum(RoleID, 9009, level)
	role.SetTrialCompleteNum(RoleID, 9010, level)

	if level >= 10 and level <= 59 then

		if level >= 10 and level <= 19 then

		    if role.GetTrialState(RoleID, 1001) == 0 then
			    --role.OpenNewTrialType(RoleID, 10)
				for i= 1,11 do
				    if i ~= 7 then
						role.OpenTrain(RoleID, 1000+i)
						if role.GetTrialCompleteNum(RoleID,1000+i) >= Trail_maxnum[1000+i] then
							role.TrialComplete(RoleID,1000+i)
						end
					end
				end

			end

		elseif level >= 20 and level <= 29 then

		    if role.GetTrialState(RoleID, 2001) == 0 then
                --role.OpenNewTrialType(RoleID, 20)
				for i= 1,16 do
				    if i ~= 5 then
						role.OpenTrain(RoleID, 2000+i)
						if role.GetTrialCompleteNum(RoleID,2000+i) >= Trail_maxnum[2000+i] then
							role.TrialComplete(RoleID,2000+i)
						end
					end
				end
			end

		elseif level >= 30 and level <= 39 then

		    if role.GetTrialState(RoleID, 3001) == 0 then
                --role.OpenNewTrialType(RoleID, 30)
				for i= 1,14 do
				    if i ~= 5 then
						role.OpenTrain(RoleID, 3000+i)
						if role.GetTrialCompleteNum(RoleID,3000+i) >= Trail_maxnum[3000+i] then
							role.TrialComplete(RoleID,3000+i)
						end
					end
				end
			end


		elseif level >= 40 and level <= 49 then

		    if role.GetTrialState(RoleID, 4001) == 0 then
                --role.OpenNewTrialType(RoleID, 40)
				for i= 1,17 do
				    role.OpenTrain(RoleID, 4000+i)
					if role.GetTrialCompleteNum(RoleID,4000+i) >= Trail_maxnum[4000+i] then
					    role.TrialComplete(RoleID,4000+i)
					end
				end
			end

		elseif level >= 50 and level <= 59 then

		    if role.GetTrialState(RoleID, 5001) == 0 then
               -- role.OpenNewTrialType(RoleID, 50)
				for i= 1,13 do
				    role.OpenTrain(RoleID, 5000+i)
					if role.GetTrialCompleteNum(RoleID,5000+i) >= Trail_maxnum[5000+i] then
					    role.TrialComplete(RoleID,5000+i)
					end
				end
			end

		end

	elseif level >= 60 then

	    if level >= 60 and level <= 69 then
		    if level == 60 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID,102,100106)
				msg.AddMsgEvent(MsgID,2,RoleID)
				if math.random(100) < 81 then
					msg.DispatchBroadcast(MsgID,MapID, InstanceID,-1)
				else
				    msg.DispatchBroadcast(MsgID, -1, -1, -1)
				end
			end

		    if role.GetTrialState(RoleID, 6001) == 0 then
                --role.OpenNewTrialType(RoleID, 60)
				for i= 1,20 do
				    role.OpenTrain(RoleID, 6000+i)
					if role.GetTrialCompleteNum(RoleID,6000+i) >= Trail_maxnum[6000+i] then
					    role.TrialComplete(RoleID,6000+i)
					end
				end
			end

		elseif level >= 70 and level <= 79 then

            if level == 70 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID,102,100107)
				msg.AddMsgEvent(MsgID,2,RoleID)
				if math.random(100) < 51 then
					msg.DispatchBroadcast(MsgID,MapID, InstanceID,-1)
				else
				    msg.DispatchBroadcast(MsgID, -1, -1, -1)
				end
			end

		    if role.GetTrialState(RoleID, 7001) == 0 then
                --role.OpenNewTrialType(RoleID, 70)
				for i= 1,15 do
				    role.OpenTrain(RoleID, 7000+i)
					if role.GetTrialCompleteNum(RoleID,7000+i) >= Trail_maxnum[7000+i] then
					    role.TrialComplete(RoleID,7000+i)
					end
				end
			end

		elseif level >= 80 and level <= 89 then

			if level == 80 or level == 85 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID,102,100108)
				msg.AddMsgEvent(MsgID,2,RoleID)
				msg.AddMsgEvent(MsgID,9,level)
				msg.DispatchBroadcast(MsgID, -1, -1, -1)
			end

		    if role.GetTrialState(RoleID, 8001) == 0 then
                --role.OpenNewTrialType(RoleID, 80)
				for i= 1,28 do
				    role.OpenTrain(RoleID, 8000+i)
					if role.GetTrialCompleteNum(RoleID,8000+i) >= Trail_maxnum[8000+i] then
					    role.TrialComplete(RoleID,8000+i)
					end
				end
			end

		elseif level >= 90 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID,102,100109)
			msg.AddMsgEvent(MsgID,2,RoleID)
			msg.AddMsgEvent(MsgID,9,level)
			msg.DispatchBroadcast(MsgID, -1, -1, -1)

		    if role.GetTrialState(RoleID, 9001) == 0 then
                --role.OpenNewTrialType(RoleID, 90)
				for i= 1,34 do
				    role.OpenTrain(RoleID, 9000+i)
					if role.GetTrialCompleteNum(RoleID,9000+i) >= Trail_maxnum[9000+i] then
					    role.TrialComplete(RoleID,9000+i)
					end
				end
			end
		end
	end

	if level >= 15 then
		if role.GetTrialState(RoleID, 1001) == 1 then
			role.TrialComplete(RoleID,1001)
		end
	end
	if level >= 20 then
		if role.GetTrialState(RoleID, 1002) == 1 then
			role.TrialComplete(RoleID,1002)
		end
	end
	if level >= 25 then
		if role.GetTrialState(RoleID, 2001) == 1 then
			role.TrialComplete(RoleID,2001)
		end
	end
	if level >= 30 then
		if role.GetTrialState(RoleID, 2002) == 1 then
			role.TrialComplete(RoleID,2002)
		end
	end
	if level >= 35 then
		if role.GetTrialState(RoleID, 3001) == 1 then
			role.TrialComplete(RoleID,3001)
		end
	end
	if level >= 40 then
		if role.GetTrialState(RoleID, 3002) == 1 then
			role.TrialComplete(RoleID, 3002)
		end
	end
	if level >= 45 then
		if role.GetTrialState(RoleID, 4001) == 1 then
			role.TrialComplete(RoleID, 4001)
		end
	end
	if level >= 50 then
		if role.GetTrialState(RoleID, 4002) == 1 then
			role.TrialComplete(RoleID, 4002)
		end
	end
	if level >= 55 then
		if role.GetTrialState(RoleID, 5001) == 1 then
			role.TrialComplete(RoleID, 5001)
		end
	end
	if level >= 60 then
		if role.GetTrialState(RoleID, 5002) == 1 then
			role.TrialComplete(RoleID, 5002)
		end
	end
	if level >= 65 then
		if role.GetTrialState(RoleID, 6001) == 1 then
			role.TrialComplete(RoleID, 6001)
		end
	end
	if level >= 70 then
		if role.GetTrialState(RoleID, 6002) == 1 then
			role.TrialComplete(RoleID, 6002)
		end
	end
	if level >= 75 then
		if role.GetTrialState(RoleID, 7001) == 1 then
			role.TrialComplete(RoleID, 7001)
		end
	end
	if level >= 80 then
		if role.GetTrialState(RoleID, 7002) == 1 then
			role.TrialComplete(RoleID, 7002)
		end
	end
	if level >= 82 then
		if role.GetTrialState(RoleID, 8001) == 1 then
			role.TrialComplete(RoleID, 8001)
		end
	end
	if level >= 84 then
		if role.GetTrialState(RoleID, 8002) == 1 then
			role.TrialComplete(RoleID, 8002)
		end
	end
	if level >= 86 then
		if role.GetTrialState(RoleID, 8003) == 1 then
			role.TrialComplete(RoleID, 8003)
		end
	end
	if level >= 88 then
		if role.GetTrialState(RoleID, 8004) == 1 then
			role.TrialComplete(RoleID, 8004)
		end
	end
	if level >= 90 then
		if role.GetTrialState(RoleID, 8005) == 1 then
			role.TrialComplete(RoleID, 8005)
		end
	end
	if level >= 91 then
		if role.GetTrialState(RoleID, 9001) == 1 then
			role.TrialComplete(RoleID, 9001)
		end
	end
	if level >= 92 then
		if role.GetTrialState(RoleID, 9002) == 1 then
			role.TrialComplete(RoleID, 9002)
		end
	end
	if level >= 93 then
		if role.GetTrialState(RoleID, 9003) == 1 then
			role.TrialComplete(RoleID, 9003)
		end
	end
	if level >= 94 then
		if role.GetTrialState(RoleID, 9004) == 1 then
			role.TrialComplete(RoleID, 9004)
		end
	end
	if level >= 95 then
		if role.GetTrialState(RoleID, 9005) == 1 then
			role.TrialComplete(RoleID, 9005)
		end
	end
	if level >= 96 then
		if role.GetTrialState(RoleID, 9006) == 1 then
			role.TrialComplete(RoleID, 9006)
		end
	end
	if level >= 97 then
		if role.GetTrialState(RoleID, 9007) == 1 then
			role.TrialComplete(RoleID, 9007)
		end
	end
	if level >= 98 then
		if role.GetTrialState(RoleID, 9008) == 1 then
			role.TrialComplete(RoleID, 9008)
		end
	end
	if level >= 99 then
		if role.GetTrialState(RoleID, 9009) == 1 then
			role.TrialComplete(RoleID, 9009)
		end
	end
	if level >= 100 then
		if role.GetTrialState(RoleID, 9010) == 1 then
			role.TrialComplete(RoleID, 9010)
		end
	end
	---------------------------------------成就END-----------------------------------------------
end

aux.RegisterRoleEvent(6, "x_RoleLevelChange")

