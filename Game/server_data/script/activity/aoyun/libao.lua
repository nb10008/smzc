function I4810050_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(10000)
	local temp = 0
	local index = 1
	local tbl = {}

	tbl[1]={3020011,1,800}
	tbl[2]={3020012,1,600}
	tbl[3]={3070028,1,700}
	tbl[4]={4800131,1,800}
	tbl[5]={4820047,1,450}
	tbl[6]={3070078,1,100}
	tbl[7]={3020007,1,800}
	tbl[8]={3020010,1,800}
	tbl[9]={3500010,1,500}
	tbl[10]={4820046,1,600}
	tbl[11]={3070051,1,750}
	tbl[12]={4500002,3,750}
	tbl[13]={3500201,1,950}
	tbl[14]={3500202,1,1100}
	tbl[15]={4530095,1,50}
	tbl[16]={3070067,1,250}

	for i = 1,16 do
	    temp = temp + tbl[i][3]
		if temp >= r then
		    index = i
			break
		end
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], -1, 8, 420)

end
aux.RegisterItemEvent(4810050, 1, "I4810050_GiftBag")

function I4810050_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4810050, 0, "I4810050_CanUseGiftBag")


function I4810047_GiftBag(MapID, InstanceID, TypeID, TargetID)		--95级圣衣包
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(24)
	local tbl = {}

	tbl[1]= 8050906 
	tbl[2]= 8050907 
	tbl[3]= 8051906 
	tbl[4]= 8051907 
	tbl[5]= 8060906 
	tbl[6]= 8060907 
	tbl[7]= 8061906 
	tbl[8]= 8061907 
	tbl[9]= 8070906 
	tbl[10]= 8070907 
	tbl[11]= 8071906 
	tbl[12]= 8071907 
	tbl[13]= 8080906 
	tbl[14]= 8080907 
	tbl[15]= 8081906 
	tbl[16]= 8081907 
	tbl[17]= 8090906 
	tbl[18]= 8090907 
	tbl[19]= 8091906 
	tbl[20]= 8091907 
	tbl[21]= 8101311 
	tbl[22]= 8101312 
	tbl[23]= 8101313 
	tbl[24]= 8101314 

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[r], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4810047, 1, "I4810047_GiftBag")

function I4810047_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4810047, 0, "I4810047_CanUseGiftBag")

function I4810048_GiftBag(MapID, InstanceID, TypeID, TargetID)		--85级圣衣包
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(24)
	local tbl = {}

	tbl[1]=  8061904
	tbl[2]=  8061905
	tbl[3]=  8070904
	tbl[4]=  8070905
	tbl[5]=  8071904
	tbl[6]=  8071905
	tbl[7]=  8080904 
	tbl[8]=  8080905 
	tbl[9]=  8081904 
	tbl[10]=  8081905 
	tbl[11]=  8090904 
	tbl[12]=  8090905 
	tbl[13]=  8091904 
	tbl[14]=  8091905 
	tbl[15]=  8101307 
	tbl[16]=  8101308 
	tbl[17]=  8101309 
	tbl[18]=  8101310 
	tbl[19]=  8050904 
	tbl[20]=  8050905 
	tbl[21]=  8051904 
	tbl[22]=  8051905 
	tbl[23]=  8060904 
	tbl[24]=  8060905 

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[r], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4810048, 1, "I4810048_GiftBag")

function I4810048_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4810048, 0, "I4810048_CanUseGiftBag")

function I4810049_GiftBag(MapID, InstanceID, TypeID, TargetID)		--82级圣衣包
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(12)
	local tbl = {}

	tbl[1]=  8050903
	tbl[2]=  8051903
	tbl[3]=  8060903
	tbl[4]=  8061903
	tbl[5]=  8070903
	tbl[6]=  8071903
	tbl[7]=  8080903 
	tbl[8]=  8081903 
	tbl[9]=  8090903 
	tbl[10]=  8091903 
	tbl[11]=  8101305 
	tbl[12]=  8101306 

	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[r], 1, -1, 8, 420)

end
aux.RegisterItemEvent(4810049, 1, "I4810049_GiftBag")

function I4810049_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4810049, 0, "I4810049_CanUseGiftBag")



function n6040141_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local lev = role.GetRoleLevel(MapID, InstanceID,RoleID)

	if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211173) -- 奥运岛传送人：\n    奥运金牌活动开始了，50级以上的玩家都可以到奥运岛参加晚上进行的各种比赛。怎么样，你也想去吗？
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 4) 
			msg.AddMsgEvent(MsgID, 1, 211174) -- 传送至奥运岛
			msg.DispatchRoleMsgEvent(RoleID, MsgID) 
	elseif TalkIndex == 4 then
		if lev < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211175) -- 奥运岛传送人：\n    你的等级不足50级,去了也是白去。
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3390517288, 466, 2850, 249)
		end
	end
end
aux.RegisterCreatureEvent(6040141, 7, "n6040141_OnTalk")

function n6040142_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local baoming = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_baoming"])
	local got = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_got"])

	if TalkIndex == -1 then
		if got == 1 or baoming < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211180) -- 跑步开启人：\n    要在我这里开启比赛才算你的成绩！
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211180) -- 跑步开启人：\n    要在我这里开启比赛才算你的成绩！
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 4) 
			msg.AddMsgEvent(MsgID, 1, 211181) -- 开启比赛
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 4 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211182) -- 跑步开启人：\n    冲吧！加油！
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			if baoming == 1 or baoming == 2 then
			 	--玩家是否已报名
				local ishavebuff = role.IsRoleHaveBuff(MapID,-1,RoleID,59473)		--专业运动员buff
				if baoming == 2 and (not ishavebuff) then
					unit.AddBuff(MapID,-1,RoleID,5947301,RoleID)
				end
				unit.AddBuff(MapID,-1,RoleID,5945501,RoleID)		--比赛进行中 buff
				role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunpaobu_shiduan"], 1)
			end
	end
end
aux.RegisterCreatureEvent(6040142, 7, "n6040142_OnTalk")

function n6040143_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local baoming = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_baoming"])
	local got = role.GetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_got"])

	if TalkIndex == -1 then
		if got == 1 or baoming < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211183) -- 游泳开启人：\n    要在我这里开启比赛才算你的成绩！
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211183) -- 游泳开启人：\n    要在我这里开启比赛才算你的成绩！
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 4) 
			msg.AddMsgEvent(MsgID, 1, 211181) -- 开启比赛
			msg.DispatchRoleMsgEvent(RoleID, MsgID) 
		end
	elseif TalkIndex == 4 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 211184) -- 游泳开启人：\n    冲吧！加油！
			msg.AddMsgEvent(MsgID, 24, TargetID) 
			msg.AddMsgEvent(MsgID, 21, 99)
			msg.AddMsgEvent(MsgID, 1, 100001) -- 取消
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			if baoming == 1 or baoming == 2 then
			 	--玩家是否已报名
				local ishavebuff = role.IsRoleHaveBuff(MapID,-1,RoleID,59473)		--专业运动员buff
				if baoming == 2 and (not ishavebuff) then
					unit.AddBuff(MapID,-1,RoleID,5947301,RoleID)
				end
				unit.AddBuff(MapID,-1,RoleID,5945501,RoleID)		--比赛进行中 buff
				role.SetRoleScriptData(RoleID, 1, RoleDataType["aoyunyouyong_shiduan"], 1)
			end
	end
end
aux.RegisterCreatureEvent(6040143, 7, "n6040143_OnTalk")
