--夫妻系统需要的一些全局定义

--结婚好友度限制
marriage_FriendShipVal  = 720

--申请者结婚金钱 银
marriage_GetMarriageDecSilver = 660000
--申请者离婚金钱 银
marriage_BreakMarriageDecSilver = 1000000
--申请婚礼金钱 银
marriage_WeddingDecSilver = 1000000

--1361107	月老之红线
--1361108	红娘之红线
--1361109	未开封的婚戒
--1361110	未开封的礼服
--1361111	大婚礼包      男的
--1361112	大婚礼包      女的
--8990101	[婚]君子
--8990102	[婚]君子
--8990103	[婚]君子
--8990111	[婚]淑女
--8990112	[婚]淑女
--8990113	[婚]淑女

--结婚
function Marriage_GetMarriageEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--[[删除婚前未开封的物品
	role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 1361109 , 1, 101)
	role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 1361110 , 1, 101)

	role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 1361109 , 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 1361110 , 1, 101)

	--添加婚后物品 0:girl，1:boy(便于以后扩展)
	--男方 	戒指：君子	物品包：男性礼服套装
	--女方	戒指：淑女	物品包：女性婚纱套装
	local MaleRoleID = 0 --男方的roleid
	local FeMaleRoleID = 0 --女方的roleid

	if role.GetRoleSex(ApplicantRoleID) == 0 and role.GetRoleSex(TargetRoleID) == 1 then
		--女
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 1361112	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 8990111	, 1, -1, 3, 104)

		--男
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 1361111	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 8990101	, 1, -1, 3, 104)

		MaleRoleID = TargetRoleID
		FeMaleRoleID = ApplicantRoleID
	elseif role.GetRoleSex(ApplicantRoleID) == 1 and role.GetRoleSex(TargetRoleID) == 0 then
		--男
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 1361111	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 8990101	, 1, -1, 3, 104)
		--女
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 1361112	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 8990111	, 1, -1, 3, 104)

		MaleRoleID = ApplicantRoleID
		FeMaleRoleID = TargetRoleID
	elseif role.GetRoleSex(ApplicantRoleID) == 1 and role.GetRoleSex(TargetRoleID) == 1 then
		--男
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 1361111	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 8990101	, 1, -1, 3, 104)
		--女
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 1361111	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 8990101	, 1, -1, 3, 104)

		MaleRoleID = ApplicantRoleID
		FeMaleRoleID = TargetRoleID
	elseif role.GetRoleSex(ApplicantRoleID) == 0 and role.GetRoleSex(TargetRoleID) == 0 then
		--男
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 1361112	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 8990111	, 1, -1, 3, 104)
		--女
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 1361112	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 8990111	, 1, -1, 3, 104)

		MaleRoleID = ApplicantRoleID
		FeMaleRoleID = TargetRoleID

	end
	

	--减申请者的钱钱
	role.DecRoleSilver(MapID, InstanceID, ApplicantRoleID, marriage_GetMarriageDecSilver, 101)
]]
    role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 4800110 , 1, 101)

    local MapID1,InstanceID1 = role.GetRoleMapID(TargetRoleID)

	role.RemoveFromRole(MapID1,InstanceID1, TargetRoleID, 4800110 , 1, 101)

    role.AddRoleItem(MapID1,InstanceID1, TargetRoleID, 4800111, 1, -1, 3, 104)
	role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 4800111, 1, -1, 3, 104)

    if role.GetRoleSex(ApplicantRoleID) == 0 then
		MaleRoleID = TargetRoleID
		FeMaleRoleID = ApplicantRoleID
	else
		MaleRoleID = ApplicantRoleID
		FeMaleRoleID = TargetRoleID
	end
	--全服广播
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 500)	-- 全服广播###（男方）与###（女方）正式结为夫妻，祝他们白头携老，永结同心！
	msg.AddMsgEvent(MsgID, 2, MaleRoleID)	-- 男方roleid
	msg.AddMsgEvent(MsgID, 2, FeMaleRoleID)	-- 女方roleid
	msg.DispatchWorldMsgEvent(MsgID)


	--刷各种地物
	MapID,InstanceID = role.GetRoleMapID(TargetRoleID)

    if Marriage_Monster ~= nil and #Marriage_Monster ~= 0 then
	    for i = 1, #Marriage_Monster do
		    map.MapDeleteCreature(MapID, InstanceID, Marriage_Monster[i])
		end
	end
	Marriage_Monster = {}
	local yanhua = {}
	yanhua[1] = {14550,3132,20650}
	yanhua[2] = {14300,3132,20850}
	yanhua[3] = {14050,3132,21050}
	yanhua[4] = {15950,3480,20700}
	yanhua[5] = {15800,3480,21150}
	yanhua[6] = {15400,3480,21650}
	yanhua[7] = {15100,3480,21850}
	yanhua[8] = {14750,3480,22100}
	yanhua[9] = {11550,3132,20650}
	yanhua[10] = {11800,3132,20850}
	yanhua[11] = {12050,3132,21050}
	yanhua[12] = {10150,3480,20700}
	yanhua[13] = {10300,3480,21150}
	yanhua[14] = {10700,3480,21650}
	yanhua[15] = {11000,3480,21850}
	yanhua[16] = {11350,3480,22100}


	local huacao = {}
	huacao[1] = {x = 250,y = 3199,z = 374}
	huacao[2] = {x = 250,y = 3199,z = 364}
	huacao[3] = {x = 250,y = 3199,z = 351}
	huacao[4] = {x = 250,y = 3199,z = 343}
	huacao[5] = {x = 272,y = 3199,z = 374}
	huacao[6] = {x = 272,y = 3199,z = 364}
	huacao[7] = {x = 272,y = 3199,z = 351}
	huacao[8] = {x = 272,y = 3199,z = 343}
	huacao[9] = {x = 260,y = 3403,z = 426}
	huacao[10] = {x = 256,y = 3403,z = 426}
	huacao[11] = {x = 264,y = 3403,z = 426}

	local buffid = {6001401,6001701,6001801,6001901}

	for i = 1,16 do
		Marriage_Monster[i] = map.MapCreateColCreature(MapID, InstanceID, 5600064, math.floor(yanhua[i][1]/50), yanhua[i][2], math.floor(yanhua[i][3]/50), true, "")
		unit.AddBuff(MapID, InstanceID, Marriage_Monster[i], buffid[math.random(4)], Marriage_Monster[i])
	end

	for i = 1,8 do
		Marriage_Monster[i+#yanhua] = map.MapCreateColCreature(MapID, InstanceID, 5600060, huacao[i].x, huacao[i].y, huacao[i].z, true, "")
	end

	Marriage_Monster[9 + #yanhua] = map.MapCreateColCreature(MapID, InstanceID, 5600062, huacao[9].x, huacao[9].y, huacao[9].z, true, "")
	Marriage_Monster[10 + #yanhua] = map.MapCreateColCreature(MapID, InstanceID, 5600061, huacao[10].x, huacao[10].y, huacao[10].z, true, "")
	Marriage_Monster[11 + #yanhua] = map.MapCreateColCreature(MapID, InstanceID, 5600061, huacao[11].x, huacao[11].y, huacao[11].z, true, "")

	local qiqiu = {}
	qiqiu[1] = {x = 14616,y = 3080,z = 17946}
	qiqiu[2] = {x = 14464,y = 3082,z = 19760}
	qiqiu[3] = {x = 13796,y = 3403,z = 20842}
	qiqiu[4] = {x = 13797,y = 3403,z = 21348}
	qiqiu[5] = {x = 12298,y = 3403,z = 21350}
	qiqiu[6] = {x = 12295,y = 3403,z = 20836}
	qiqiu[7] = {x = 11517,y = 3089,z = 19524}
	qiqiu[8] = {x = 11320,y = 3093,z = 19085}
	qiqiu[9] = {x = 11596,y = 3115,z = 17906}
	qiqiu[10] = {x = 14681,y = 3093,z = 18795}

	for i = 1,10 do
		Marriage_Monster[i+#yanhua+#huacao] = map.MapCreateColCreature(MapID, InstanceID, 5610049, math.floor(qiqiu[i].x/50), qiqiu[i].y, math.floor(qiqiu[i].z/50), true, "")
	end
end

--结婚检查
function Marriage_GetMarriageCheckEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--性别是否相同
	--[[if role.GetRoleSex(ApplicantRoleID) == role.GetRoleSex(TargetRoleID) then
		return 2 --性别相同
	end]]

	--[[检查好友度
	if role.GetFriendShipValue(ApplicantRoleID,TargetRoleID) < marriage_FriendShipVal then
		return 3 --好友度不够
	end]]

	--检查申请者是否已婚
	if role.IsRoleMarried(ApplicantRoleID) == true then
		return 4 --申请者已婚
	end

	if role.IsRoleMarried(TargetRoleID) == true then
		return 5 --被申请者已婚
	end

	--[[申请者金钱是否足够
	if role.GetRoleSilver(MapID,InstanceID,ApplicantRoleID) < marriage_GetMarriageDecSilver then
		return 7 --申请者必须拥有66金
	end

	--是否有婚前未开封的物品
	local num1 = role.GetRoleItemNum(ApplicantRoleID,1361109)
	local num2 = role.GetRoleItemNum(ApplicantRoleID,1361110)

	local num3 = role.GetRoleItemNum(TargetRoleID,1361109)
	local num4 = role.GetRoleItemNum(TargetRoleID,1361110)

	if num1<1 or num2<1 or num3<1 or num4 <1 then
		return  10 --缺少必要物品
	end]]

	local num5 = role.GetBagFreeSize(ApplicantRoleID)
	local num6 = role.GetBagFreeSize(TargetRoleID)

	if num5<1 or num6<1 then
		return 12 --背包空间不够
	end

	return 0 --成功

end

--删除结婚戒指
function Marriage_DelRing(MapID, InstanceID, ApplicantRoleID, TargetRoleID)
	--删除婚后物品 0:girl，1:boy(便于以后扩展)
	--男方 	戒指：君子
	--女方	戒指：淑女
	--[[戒指有可能已经升级了 所以要把所有款式的戒指都有删一遍
	if role.GetRoleSex(ApplicantRoleID) == 0 then
		--女背包
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990111	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990112	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990113	, 1, -1, 3, 104)
		--女仓库
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990111	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990112	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990113	, 1, -1, 3, 104)
		--男背包
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990101	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990102	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990103	, 1, -1, 3, 104)
		--男仓库
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990101	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990102	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990103	, 1, -1, 3, 104)
	else
		--男背包
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990101	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990102	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990103	, 1, -1, 3, 104)
		--男仓库
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990101	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990102	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990103	, 1, -1, 3, 104)
		--女背包
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990111	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990112	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990113	, 1, -1, 3, 104)
		--女仓库
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990111	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990112	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990113	, 1, -1, 3, 104)
	end]]
end

--离婚通用检查
function Marriage_BreakMarriageCommonCheck(ApplicantRoleID, TargetRoleID)

	--性别是否相同
	if role.GetRoleSex(ApplicantRoleID) == role.GetRoleSex(TargetRoleID) then
		return 2 --性别相同
	end

	--检查申请者是否已婚
	if role.IsRoleMarried(ApplicantRoleID) == false then
		return 11 --申请者未婚
	end

	return 0 --成功

end

--离婚
function Marriage_BreakMarriageEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--删除结婚戒指
	--Marriage_DelRing(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--减申请者的钱钱
	role.DecRoleSilver(MapID, InstanceID, ApplicantRoleID, 31880000, 101)

	role.RemoveSkill(ApplicantRoleID, 90006)
	role.RemoveSkill(ApplicantRoleID, 90007)

	local MapID1,InstanceID1 = role.GetRoleMapID(TargetRoleID)
	if MapID1 ~= nil then
		if role.IsRoleOnLine(MapID1,InstanceID1,TargetRoleID) then
		    role.RemoveSkill(TargetRoleID, 90006)
			role.RemoveSkill(TargetRoleID, 90007)
		end
	end

end

--离婚检查
function Marriage_BreakMarriageCheckEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--离婚通用检查
	CommonCheckResult = Marriage_BreakMarriageCommonCheck(ApplicantRoleID, TargetRoleID)
	if CommonCheckResult ~= 0 then
		return CommonCheckResult
	end

	--[[申请者金钱是否足够
	if role.GetRoleSilver(MapID,InstanceID,ApplicantRoleID) < marriage_BreakMarriageDecSilver then
		return 7 --申请者必须拥有总够的离婚费用
	end]]

	return 0 --成功
end

--强制离婚
function Marriage_ForceBreakMarriageEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--删除结婚戒指
	--Marriage_DelRing(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--从申请者背包内扣除绝情丹,typeid=1360118
	--role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 1360118, 1, 101);

    --减申请者的钱钱
	role.DecRoleSilver(MapID, InstanceID, ApplicantRoleID, 31880000, 101)

	role.RemoveSkill(ApplicantRoleID, 90006)
	role.RemoveSkill(ApplicantRoleID, 90007)

	local MapID1,InstanceID1 = role.GetRoleMapID(TargetRoleID)
	if MapID1 ~= nil then
		if role.IsRoleOnLine(MapID1,InstanceID1,TargetRoleID) then
		    role.RemoveSkill(TargetRoleID, 90006)
			role.RemoveSkill(TargetRoleID, 90007)
		end
	end

end

--强制离婚检查
function Marriage_ForceBreakMarriageCheckEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--离婚通用检查
	CommonCheckResult = Marriage_BreakMarriageCommonCheck(ApplicantRoleID, TargetRoleID)
	if CommonCheckResult ~= 0 then
		return CommonCheckResult
	end

	--[[检查角色身上是否有绝情丹，typeid=1360118
	if role.GetRoleItemNum(ApplicantRoleID, 1360118) <= 0 then
		return 13
	end]]

	return 0 --成功
end

--怪物消失，置空表
function Marriage_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    for k,v in pairs(Marriage_Monster) do
	    if v == TargetID then
		    table.remove(Marriage_Monster,k)
		end
	end
end

aux.RegisterCreatureEvent(5600060, 13, "Marriage_OnDisappear")
aux.RegisterCreatureEvent(5600061, 13, "Marriage_OnDisappear")
aux.RegisterCreatureEvent(5600062, 13, "Marriage_OnDisappear")
aux.RegisterCreatureEvent(5600064, 13, "Marriage_OnDisappear")

aux.RegisterRoleEvent(11, "Marriage_GetMarriageEvent")
aux.RegisterRoleEvent(12, "Marriage_GetMarriageCheckEvent")

aux.RegisterRoleEvent(13, "Marriage_BreakMarriageEvent")
aux.RegisterRoleEvent(14, "Marriage_BreakMarriageCheckEvent")

aux.RegisterRoleEvent(42, "Marriage_ForceBreakMarriageEvent")
aux.RegisterRoleEvent(43, "Marriage_ForceBreakMarriageCheckEvent")
