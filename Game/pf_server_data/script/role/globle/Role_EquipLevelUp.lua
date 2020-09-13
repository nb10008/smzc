moshen_BGS = 0             --本日鉴定魔神装的件数
moshen_BGS_Data = 0        --魔神装的日期记录

-- 法宝
function x_RoleFAbaoLevelup(MapID, InstanceID, RoleID, FabaoID, Type, FBVal)
	if Type == 0 then
	    local Trail = {2004,3004,4003,5003,6004,7003,8006,9011}
		if role.GetTrialCompleteNum(RoleID,2004) < 100 then
			for i = 1,8 do
				if role.GetTrialCompleteNum(RoleID,Trail[i]) < FBVal then
					role.SetTrialCompleteNum(RoleID, Trail[i], FBVal)
				end
				if role.GetTrialState(RoleID, Trail[i]) == 1 and role.GetTrialCompleteNum(RoleID,Trail[i]) >= Trail_maxnum[Trail[i]] then
					role.TrialComplete(RoleID,Trail[i])
				end
			end
		end
		if FBVal%10 == 0 and FBVal >19 then
			role.NotifyFBBS(RoleID,4,FBVal)
		end
		-- 关于妖精等级的任务的相关逻辑
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40003) then
			if FBVal >= 10 then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40003, 2)
			end
		end
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40005) then
			if FBVal >= 20 then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40005, 2)
			end
		end
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40006) then
			if FBVal >= 30 then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40006, 2)
			end
		end
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40007) then
			if FBVal >= 40 then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40007, 2)
			end
		end
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40008) then
			if FBVal >= 50 then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40008, 2)
			end
		end
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40009) then
			if FBVal >= 60 then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40009, 2)
			end
		end

	--[[elseif Type == 1 or Type == 2 then
	    local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"])
		if k < 10 then
		    role.SetRoleScriptData(RoleID, 1, RoleDataType["FaBaoShiLian"], 10 + k)
		end]]
	end
end


function x_RoleLianQi(MapID, InstanceID, RoleID, ItemID, level)
    if role.GetTrialState(RoleID, 8007) == 1 then
		role.SetTrialCompleteNum(RoleID, 8007, 1)
	end
	if role.GetTrialState(RoleID, 8007) == 1 and role.GetTrialCompleteNum(RoleID,8007) >= Trail_maxnum[8007] then
		role.TrialComplete(RoleID,8007)
	end
end

function x_OnEquipConsolidateLevelUp(MapID, InstanceID, RoleID, TypeID, LevelBefore, LevelNow)
    if LevelNow > LevelBefore then
	    local lv = {5,8,12,14,20}
		for i = 1,5 do
		    if LevelNow == lv[i] then
			    role.NotifyFBBS(RoleID,7,LevelNow)
			end
		end
		local Trail = {3006,4004,5004,6005,7004,8008,9012,9013}
		for i = 1,8 do
			if role.GetTrialCompleteNum(RoleID,Trail[i]) < LevelNow and role.GetTrialState(RoleID, Trail[i]) == 1 then
				role.SetTrialCompleteNum(RoleID, Trail[i], LevelNow)
			end
			if role.GetTrialState(RoleID, Trail[i]) == 1 and role.GetTrialCompleteNum(RoleID,Trail[i]) >= Trail_maxnum[Trail[i]] then
				role.TrialComplete(RoleID,Trail[i])
			end
		end
	end
end

function x_OnEquipDigHole(MapID, InstanceID, RoleID, TypeID, Sucess, SlotNum)
    if Sucess == 1 then
	    local Trail = {4005,5005,6006,7005}
		for i = 1,4 do
			if role.GetTrialCompleteNum(RoleID,Trail[i]) < SlotNum and role.GetTrialState(RoleID, Trail[i]) >= 1 then
				role.SetTrialCompleteNum(RoleID, Trail[i], SlotNum)
			end
			if role.GetTrialState(RoleID, Trail[i]) == 1 and role.GetTrialCompleteNum(RoleID,Trail[i]) >= Trail_maxnum[Trail[i]] then
				role.TrialComplete(RoleID,Trail[i])
			end
		end
	end
end


function x_OnProduceEquip(MapID, InstanceID, RoleID, TypeID, Quality)
    if Quality >= 3 then
		if role.GetTrialCompleteNum(RoleID,3012) < 1 and role.GetTrialState(RoleID, 3012) >= 1 then
			role.SetTrialCompleteNum(RoleID, 3012, 1)
		end
		if role.GetTrialState(RoleID, 3012) == 1 and role.GetTrialCompleteNum(RoleID,3012) >= Trail_maxnum[3012] then
			role.TrialComplete(RoleID,3012)
		end
	end
end

function x_OnCutStone(MapID, InstanceID, RoleID, DianJinID, TypeID, NewStoneTypeID)

     --磨砂纸
    local Item_ID = {}
	Item_ID[3030001] = {ID = {3050084},Percent = {100},num = {1}}
	Item_ID[3030002] = {ID = {3030001,3050085,3050084},Percent = {100,5,100},num = {1,1,1}}
	Item_ID[3030003] = {ID = {3030002,3050085,3050084},Percent = {100,10,100},num = {1,1,1}}
	Item_ID[3030004] = {ID = {3030003,3050085,3050084},Percent = {100,15,100},num = {1,1,1}}
	Item_ID[3030009] = {ID = {3050084},Percent = {100},num ={1}}
	Item_ID[3030010] = {ID = {3030009,3050085,3050084},Percent = {100,5,100},num = {1,1,1}}
	Item_ID[3030011] = {ID = {3030010,3050085,3050084},Percent = {100,10,100},num = {1,1,1}}
	Item_ID[3030012] = {ID = {3030011,3050085,3050084},Percent = {100,15,100},num = {1,1,1}}
	Item_ID[3030017] = {ID = {3050084},Percent = {100},num = {5}}
	Item_ID[3030018] = {ID = {3030017,3050085,3050084},Percent = {100,20,100},num = {1,1,2}}
	Item_ID[3030023] = {ID = {3050084,3050085},Percent = {100,20},num = {5,1}}
	Item_ID[3030024] = {ID = {3030023,3050085,3050084},Percent = {100,20,100},num = {1,1,2}}
	Item_ID[3030029] = {ID = {3050084},Percent = {100},num = {1}}
	Item_ID[3030030] = {ID = {3030029,3050085,3050084},Percent = {100,5,100},num = {1,1,1}}
	Item_ID[3030031] = {ID = {3030030,3050085,3050084},Percent = {100,10,100},num = {1,1,1}}
	Item_ID[3030032] = {ID = {3030031,3050085,3050084},Percent = {100,15,100},num = {1,1,1}}
	Item_ID[3030037] = {ID = {3050084},Percent = {100},num = {1}}
	Item_ID[3030038] = {ID = {3030037,3050085,3050084},Percent = {100,5,100},num = {1,1,1}}
	Item_ID[3030039] = {ID = {3030038,3050085,3050084},Percent = {100,10,100},num = {1,1,1}}
	Item_ID[3030040] = {ID = {3030039,3050085,3050084},Percent = {100,15,100},num = {1,1,1}}
	Item_ID[3030045] = {ID = {3050084,3040085},Percent = {100,20},num = {5,1}}
	Item_ID[3030046] = {ID = {3030045,3050084,3040085},Percent = {100,100,20},num = {1,2,1}}
	Item_ID[3030051] = {ID = {3050084,3050085},Percent = {100,20},num = {5,1}}
	Item_ID[3030052] = {ID = {3030051,3050085,3050084},Percent = {100,20,100},num = {1,1,2}}
	Item_ID[3030057] = {ID = {3050084,3050085},Percent = {100,20},num = {8,1}}
	Item_ID[3030062] = {ID = {3050084,3050085},Percent = {100,20},num = {8,1}}
	Item_ID[3030067] = {ID = {3050084,3050085},Percent = {100,20},num = {8,1}}
	Item_ID[3030072] = {ID = {3050084},Percent = {100},num = {1}}
	Item_ID[3030073] = {ID = {3030072,3050085,3050084},Percent = {100,5,100},num = {1,1,1}}
	Item_ID[3030074] = {ID = {3030073,3050085,3050084},Percent = {100,10,100},num = {1,1,1}}
	Item_ID[3030075] = {ID = {3030074,3050085,3050084},Percent = {100,15,100},num = {1,1,1}}
	Item_ID[3030080] = {ID = {3050084},Percent = {100},num = {1}}
	Item_ID[3030081] = {ID = {3030080,3050085,3050084},Percent = {100,5,100},num = {1,1,1}}
	Item_ID[3030082] = {ID = {3030081,3050085,3050084},Percent = {100,10,100},num = {1,1,1}}
	Item_ID[3030083] = {ID = {3030082,3050085,3050084},Percent = {100,15,100},num = {1,1,1}}
	Item_ID[3030088] = {ID = {3050084,3050085},Percent = {100,20},num = {8,1}}
	Item_ID[3030093] = {ID = {3050084,3050085},Percent = {100,20},num = {5,1}}
	Item_ID[3030094] = {ID = {3030093,3050085,3050084},Percent = {100,20,100},num = {1,1,2}}
	Item_ID[3030096] = {ID = {3050084,3050085},Percent = {100,20},num = {1,2}}
	Item_ID[3030097] = {ID = {3030096,3050085,3050084},Percent = {100,20,100},num = {1,1,2}}
	Item_ID[3030099] = {ID = {3050084,3050085},Percent = {100,20},num = {5,1}}
	Item_ID[3030100] = {ID = {3030099,3050085,3050084},Percent = {100,20,100},num = {1,1,2}}
	Item_ID[3030111] = {ID = {3050084},Percent = {100},num = {1}}

	if NewStoneTypeID == 3050087 and Item_ID[TypeID] ~= nil then
	    local FreeSize = role.GetBagFreeSize(RoleID)
		if Item_ID[TypeID].ID[1] ~= nil  and math.random(100) <= Item_ID[TypeID].Percent[1] then
		    role.AddRoleItem(MapID, InstanceID, RoleID, Item_ID[TypeID].ID[1],Item_ID[TypeID].num[1], -1, 8, 420)
		end
		local FreeSize = role.GetBagFreeSize(RoleID)
		if Item_ID[TypeID].ID[2] ~= nil and FreeSize > 1 and math.random(100) <= Item_ID[TypeID].Percent[2] then
		    role.AddRoleItem(MapID, InstanceID, RoleID, Item_ID[TypeID].ID[2],Item_ID[TypeID].num[2], -1, 8, 420)
		end
		local FreeSize = role.GetBagFreeSize(RoleID)
		if Item_ID[TypeID].ID[3] ~= nil and FreeSize > 1 and math.random(100) <= Item_ID[TypeID].Percent[3] then
		    role.AddRoleItem(MapID, InstanceID, RoleID, Item_ID[TypeID].ID[3],Item_ID[TypeID].num[3], -1, 8, 420)
		end
		return 0
	end
	if NewStoneTypeID == 8220301 and TypeID ~= 8220201 then
		local temp = math.random(100)
		local guoqing1 = math.random(20)
		if(guoqing1 == 10) then
			local guoqing11 = math.random(5)
			if(guoqing11 == 3 ) then
				local guoqing16 = math.random(10)
				if(guoqing16 == 6) then
					local guoqing18 = math.random(2)
						if(guoqing18 == 1) then
							local guoqing19 = math.random(3)
							if(guoqing19 == 2) then
								role.AddRoleItem(MapID, InstanceID, RoleID, 8220400,1, 6, 8, 420)
							else
								role.AddRoleItem(MapID, InstanceID, RoleID, 8220398+temp%2,1, 6, 8, 420)
							end
						else
							role.AddRoleItem(MapID, InstanceID, RoleID, 8220391+temp%7,1, 6, 8, 420)  --17	-18披风
						end
				else
					role.AddRoleItem(MapID, InstanceID, RoleID, 8220373+temp%18,1, 6, 8, 420)  --14-16披风
				end
			else
				role.AddRoleItem(MapID, InstanceID, RoleID, 8220346+temp%27,1, 6, 8, 420)  --11-13披风
			end
		else
			role.AddRoleItem(MapID, InstanceID, RoleID, 8220301+temp%45,1, 6, 8, 420)  --11以下披风
		end
		return 0
	end
	--上古神装图谱点化成碎片
	if DianJinID == 4800249 and (TypeID >= 4820192 and TypeID <= 4820227)then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4800248,25,-1, 8, 420)  --25个碎片
		return 0
	end
	--切割王者石
    local Item_ID_1 = {}
	Item_ID_1[3030421] = {3030031,3030039}
	Item_ID_1[3030422] = {3030032,3030040}
	Item_ID_1[3030423] = {3030033,3030041}
	Item_ID_1[3030424] = {3030034,3030042}
	Item_ID_1[3030425] = {3030035,3030043}
	Item_ID_1[3030426] = {3030036,3030044}
	if	Item_ID_1[TypeID] then
	    local FreeSize = role.GetBagFreeSize(RoleID)
        if	FreeSize > 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, Item_ID_1[TypeID][1],1, -1, 8, 420)
			role.AddRoleItem(MapID, InstanceID, RoleID, Item_ID_1[TypeID][2],1, -1, 8, 420)
			return 0
		elseif FreeSize > 0 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, Item_ID_1[TypeID][1],1, -1, 8, 420)
			return 0
		end
	end

    --雕琢宝石 成就
    if TypeID >= 3030001 and TypeID <= 3040000 and NewStoneTypeID ~= 3050087 then
		if role.GetTrialCompleteNum(RoleID, 4013) < 1 and role.GetTrialState(RoleID, 4013) >= 1 then
			role.SetTrialCompleteNum(RoleID, 4013, 1)
		end
		if role.GetTrialState(RoleID, 4013) == 1 and role.GetTrialCompleteNum(RoleID, 4013) >= Trail_maxnum[4013] then
			role.TrialComplete(RoleID, 4013)
		end
	end
	return NewStoneTypeID
end

function x_OnBrandEquip(MapID, InstanceID, RoleID, EquipID, ItemID, Sucess, Times)
    if Sucess == 1 then
		local Trail = {7006,8010,9027}
		for i = 1,3 do
			if role.GetTrialCompleteNum(RoleID,Trail[i]) < Times and role.GetTrialState(RoleID, Trail[i]) >= 1 then
				role.SetTrialCompleteNum(RoleID, Trail[i], Times)
			end
			if role.GetTrialState(RoleID, Trail[i]) == 1 and role.GetTrialCompleteNum(RoleID,Trail[i]) >= Trail_maxnum[Trail[i]] then
				role.TrialComplete(RoleID,Trail[i])
			end
		end
	end
end

--[[function x_OnUseBeGoldStone(ItemID, RoleID, MapID, InstanceID)

    if ItemID >= 8017009 and ItemID <= 8130814 then
	    moshen = {}
	    moshen[1]= {moshenid = 8017009,weimoshen = 8013007}
		moshen[2]= {moshenid = 8017010,weimoshen = 8013009}
		moshen[3]= {moshenid = 8027009,weimoshen = 8023007}
		moshen[4]= {moshenid = 8027010,weimoshen = 8023009}
		moshen[5]= {moshenid = 8037009,weimoshen = 8033007}
		moshen[6]= {moshenid = 8037010,weimoshen = 8033009}
		moshen[7]= {moshenid = 8047009,weimoshen = 8043007}
		moshen[8]= {moshenid = 8047010,weimoshen = 8043009}
		moshen[9]= {moshenid = 8050809,weimoshen = 8050307}
		moshen[10]= {moshenid = 8050810,weimoshen = 8050309}
		moshen[11]= {moshenid = 8050813,weimoshen = 8050407}
		moshen[12]= {moshenid = 8050814,weimoshen = 8050409}
		moshen[13]= {moshenid = 8051809,weimoshen = 8051307}
		moshen[14]= {moshenid = 8051810,weimoshen = 8051309}
		moshen[15]= {moshenid = 8051813,weimoshen = 8051407}
		moshen[16]= {moshenid = 8051814,weimoshen = 8051409}
		moshen[17]= {moshenid = 8060809,weimoshen = 8060307}
		moshen[18]= {moshenid = 8060810,weimoshen = 8060309}
		moshen[19]= {moshenid = 8060813,weimoshen = 8060407}
		moshen[20]= {moshenid = 8060814,weimoshen = 8060409}
		moshen[21]= {moshenid = 8061809,weimoshen = 8061307}
		moshen[22]= {moshenid = 8061810,weimoshen = 8061309}
		moshen[23]= {moshenid = 8061813,weimoshen = 8061407}
		moshen[24]= {moshenid = 8061814,weimoshen = 8061409}
		moshen[25]= {moshenid = 8070809,weimoshen = 8070307}
		moshen[26]= {moshenid = 8070810,weimoshen = 8070309}
		moshen[27]= {moshenid = 8070813,weimoshen = 8070407}
		moshen[28]= {moshenid = 8070814,weimoshen = 8070409}
		moshen[29]= {moshenid = 8071809,weimoshen = 8071307}
		moshen[30]= {moshenid = 8071810,weimoshen = 8071309}
		moshen[31]= {moshenid = 8071813,weimoshen = 8071407}
		moshen[32]= {moshenid = 8071814,weimoshen = 8071409}
		moshen[33]= {moshenid = 8080809,weimoshen = 8080307}
		moshen[34]= {moshenid = 8080810,weimoshen = 8080309}
		moshen[35]= {moshenid = 8080813,weimoshen = 8080407}
		moshen[36]= {moshenid = 8080814,weimoshen = 8080409}
		moshen[37]= {moshenid = 8081809,weimoshen = 8081307}
		moshen[38]= {moshenid = 8081810,weimoshen = 8081309}
		moshen[39]= {moshenid = 8081813,weimoshen = 8081407}
		moshen[40]= {moshenid = 8081814,weimoshen = 8081409}
		moshen[41]= {moshenid = 8090809,weimoshen = 8090307}
		moshen[42]= {moshenid = 8090810,weimoshen = 8090309}
		moshen[43]= {moshenid = 8090813,weimoshen = 8090407}
		moshen[44]= {moshenid = 8090814,weimoshen = 8090409}
		moshen[45]= {moshenid = 8091809,weimoshen = 8091307}
		moshen[46]= {moshenid = 8091810,weimoshen = 8091309}
		moshen[47]= {moshenid = 8091813,weimoshen = 8091407}
		moshen[48]= {moshenid = 8091814,weimoshen = 8091409}
		moshen[49]= {moshenid = 8101209,weimoshen = 8100307}
		moshen[50]= {moshenid = 8101210,weimoshen = 8100309}
		moshen[51]= {moshenid = 8101213,weimoshen = 8100407}
		moshen[52]= {moshenid = 8101214,weimoshen = 8100409}
		moshen[53]= {moshenid = 8101217,weimoshen = 8100507}
		moshen[54]= {moshenid = 8101218,weimoshen = 8100509}
		moshen[55]= {moshenid = 8101221,weimoshen = 8100607}
		moshen[56]= {moshenid = 8101222,weimoshen = 8100609}
		moshen[57]= {moshenid = 8110809,weimoshen = 8110307}
		moshen[58]= {moshenid = 8110810,weimoshen = 8110309}
		moshen[59]= {moshenid = 8111809,weimoshen = 8111307}
		moshen[60]= {moshenid = 8111810,weimoshen = 8111309}
		moshen[61]= {moshenid = 8121809,weimoshen = 8121307}
		moshen[62]= {moshenid = 8121810,weimoshen = 8121309}
		moshen[63]= {moshenid = 8122809,weimoshen = 8122307}
		moshen[64]= {moshenid = 8122810,weimoshen = 8122309}
		moshen[65]= {moshenid = 8130809,weimoshen = 8130307}
		moshen[66]= {moshenid = 8130810,weimoshen = 8130309}
		moshen[67]= {moshenid = 8130813,weimoshen = 8130407}
		moshen[68]= {moshenid = 8130814,weimoshen = 8130409}
		for i = 1, 68 do
		    if ItemID == moshen[i].moshenid then
				local date = tonumber(os.date("%j"))
				if moshen_BGS_Data ~= date then
				    moshen_BGS = 1
				else
				    moshen_BGS = moshen_BGS + 1
					if moshen_BGS >= 11 then
					    ItemID = moshen[i].weimoshen
					end
				end
				break
			end
		end
	end

	if ItemID == 4810012 then
        local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		role.AddRoleExp(MapID, InstanceID, RoleID, monster_exp[level]*10)
	elseif (ItemID >= 4090028 and ItemID <= 4090031) or ItemID == 4090022 then
        local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		role.AddRoleExp(MapID, InstanceID, RoleID, monster_exp[level]*10)
	end

	return ItemID
end]]

function x_OnChooseGrowAtt(RoleID,MapID,InstanceID, TypeID, color)
	--[[local tbl1 = {}
	tbl1[1100000] = "武器物理"
	tbl1[1200000] = "武器法术"
	tbl1[2100000] = "头裤腿物理"
	tbl1[2200000] = "头裤腿法术"
	tbl1[3100000] = "手鞋物理"
	tbl1[3200000] = "手鞋法术"
	tbl1[4100000] = "披风物理"
	tbl1[4200000] = "披风法术"
	tbl1[5100000] = "腰饰物理"
	tbl1[5200000] = "腰饰法术"
	tbl1[6100000] = "项链物理"
	tbl1[6200000] = "项链法术"
	tbl1[7100000] = "戒指物理"
	tbl1[7200000] = "戒指法术"
	local tbl2 = {"60级","70级","80蓝","80橙","特殊指定","8级","9级","10级"}]]


	local function GetEqupPosAndType(TypeID)  --通过表中记录的数据来获取该分配哪个段的属性
	    local type1,type2
		--------------------------------------
		if TypeID > 8000000 then
			local typenum = math.floor((TypeID - 8000000)/1000)
			if typenum < 30 then
				type1 = 1100000
			elseif typenum < 50 then
				type1 = 1200000
			elseif typenum < 80 then
				type1 = 2000000 + (typenum%10+1)*100000
			elseif typenum < 100 then
				type1 = 3000000 + (typenum%10+1)*100000
			elseif typenum < 110 then
				type1 = 4100000 + (math.random(2)-1)*100000
			elseif typenum < 120 then
				type1 = 5000000 + (typenum%10+1)*100000
			elseif typenum < 130 then
				type1 = 6000000 + typenum%10*100000
			elseif typenum < 140 then
				type1 = 7100000 + (math.random(2)-1)*100000
			end
			if TypeID%100 >=12 and TypeID%100 <= 17 and  type1 ~= nil then
				type2 = TypeID%100 - 13
				if type2 <= 0 then
				    type2 = 2
				end
			end
			if TypeID%100 >=18 and TypeID%100 <= 20 and  type1 ~= nil then
				type2 = TypeID%100 - 12
			end
		end
		--------------------------------------
	    --如果是特殊的指定的ID,则在此直接指定
		if TypeID == 8010012 or TypeID == 8020012 then
		    type2 = 5
		elseif TypeID == 8030012 or TypeID == 8040012 then
		    type2 = 5
		elseif TypeID == 8049003 or TypeID == 8039003 or TypeID == 8029003 or TypeID == 8019003 or TypeID == 8111916 or TypeID == 8111917 or TypeID == 8111918 or TypeID == 8111919 or TypeID == 8110916 or TypeID == 8110917 or TypeID == 8110918 or TypeID == 8110919 then
		    type2 = 5
		elseif TypeID == 8213207 or TypeID == 8212207 or TypeID == 8211207 or TypeID == 8210207 then
		    type2 = 5
		else
		    local tbl = {8050912,8050913,8051912,8051913,8060912,8060913,8061912,8061913,8070912,8070913,8071912,8071913,8080912,8080913,8081912,8081913,8090912,8090913,8091912,8091913,8101323,8101324,8101325,8101326,8110914,8110915,8111914,8111915,8121910,8121911,8122910,8122911,8130919,8130920,8130921,8130922}
			for i = 1,#tbl do
			    if TypeID < tbl[i] then
				    break
			    elseif TypeID == tbl[i] then
				    type2 = 5
				end
			end
			local tbl2 = {8019004,8019004,8050914,8060914,8070914,8080914,8090914,8101327,8110920,8110920,8121912,8130923,8029004,8029004,8050915,8060915,8070915,8080915,8090915,8101328,8110921,8110921,8121913,8130924,8049004,8049004,8051914,8061914,8071914,8081914,8091914,8101329,8111920,8111920,8122912,8130925,8039004,8039004,8051915,8061915,8071915,8081915,8091915,8101330,8111921,8111921,8122913,8130926}
		    for i = 1,#tbl2 do
			    if TypeID == tbl2[i] then
				    type2 = 5
				end
			end
		end

		if TypeID == 8101323 or TypeID == 8101324 then
            type1 = 4100000
		elseif TypeID == 8101325 or TypeID == 8101326 then
		    type1 = 4200000
		elseif TypeID == 8130919 or TypeID == 8130920 then
            type1 = 5100000
		elseif TypeID == 8130921 or TypeID == 8130922 then
		    type1 = 5200000
		end

		return type1,type2
	end

	local type1,type2 = GetEqupPosAndType(TypeID)

	local LevelNum = {}
	LevelNum[1100000] = {5,5,5,10,10,10,10,15,15,15}
	LevelNum[1200000] = {5,5,5,10,10,10,10,15,15,15}
	LevelNum[2100000] = {14,14,14,14,14,19,19,19,19,19}
	LevelNum[2200000] = {14,14,14,14,14,19,19,19,19,19}
	LevelNum[3100000] = {14,14,14,14,14,19,19,19,19,19}
	LevelNum[3200000] = {14,14,14,14,14,19,19,19,19,19}
	LevelNum[4100000] = {14,14,14,14,14,19,19,19,19,19}
	LevelNum[4200000] = {14,14,14,14,14,19,19,19,19,19}
	LevelNum[5100000] = {5,5,5,5,5,11,10,10,10,10}
	LevelNum[5200000] = {5,5,5,5,5,11,10,10,10,10}
	LevelNum[6100000] = {5,5,5,5,5,11,10,10,10,10}
	LevelNum[6200000] = {5,5,5,5,5,11,10,10,10,10}
	LevelNum[7100000] = {7,7,6,6,6,6,5,10,10,10}
	LevelNum[7200000] = {7,7,6,6,6,6,5,10,10,10}


    if type2 ~= nil and type2 == 5 then
	    local tbl2 = {8019004,8019004,8050914,8060914,8070914,8080914,8090914,8101327,8110920,8110920,8121912,8130923,8029004,8029004,8050915,8060915,8070915,8080915,8090915,8101328,8110921,8110921,8121913,8130924,8049004,8049004,8051914,8061914,8071914,8081914,8091914,8101329,8111920,8111920,8122912,8130925,8039004,8039004,8051915,8061915,8071915,8081915,8091915,8101330,8111921,8111921,8122913,8130926}
		for i = 1,#tbl2 do
			if TypeID == tbl2[i] then
				local tbl = {1101310,1101310,2101710,2101710,2101710,3101710,3101710,4101710,5100510,5100510,5100510,7100810,1101310,1101310,2101710,2101710,2101710,3101710,3101710,4101710,5100510,5100510,5100510,7100810,1201310,1201310,2101710,2101710,2101710,3101710,3101710,4101710,5100510,5100510,5100510,7100810,1201310,1201310,2101710,2101710,2101710,3101710,3101710,4101710,5100510,5100510,5100510,7100810}
				return tbl[i]
			end
		end
	    if TypeID == 8010012 or TypeID == 8020012 then
		    return 1
		elseif TypeID == 8030012 or TypeID == 8040012 then
		    return 2
		elseif TypeID == 8213207 or TypeID == 8212207 then
		    return 1201508
		elseif TypeID == 8211207 or TypeID == 8210207 then
		    return 1101508
		elseif TypeID == 8049003 or TypeID == 8039003 or TypeID == 8029003 or TypeID == 8019003 or TypeID == 8111916 or TypeID == 8111917 or TypeID == 8111918 or TypeID == 8111919 or TypeID == 8110916 or TypeID == 8110917 or TypeID == 8110918 or TypeID == 8110919 then
		    return type1 + math.random(LevelNum[type1][10])*100 + 10
		else
			local k = math.random(100)
			if k > 95 then
				return type1 + math.random(LevelNum[type1][10])*100 + 10
			elseif k > 80 then
				return type1 + math.random(LevelNum[type1][9])*100 + 9
			else
				return type1 + math.random(LevelNum[type1][8])*100 + 8
			end
		end
	elseif type2 ~= nil and type2 >= 6 and type2 <= 8 then
	    return type1 + math.random(LevelNum[type1][TypeID%100-10])*100 + TypeID%100-10
	elseif type2 ~= nil and type1 ~= nil then  --60级的只能在最弱的三级里面选,70级的可以在1-4级中选,蓝色的80级的会从1-8级中选,橙色的会在7-10级中选
		--[[此段为装备成长的主要生成脚本
		设每天普通掉落蓝装2000个,那么每天可以获得7级属性40个,8级属性20个,修正后第一天8级属性将会有40左右
		设每天掉落橙装20个,那么每天可以获得7级属性10个,8级属性10个
		所以,第一天或者第二天,将会出现一个9级属性。
		之后,9级属性将会每天产出2个左右
		10级属性每5产出2个
		]]

		local index = 1
		local level = 1
		if type2 == 1 then                --60级：10%为3级属性,20%为2级属性
		    local k = math.random(100)
			if k > 90 then
			    level = 3
			elseif k > 70 then
			    level = 2
			end
		elseif type2 == 2 then            --70级：10%为4级属性,20%为3级属性,30%为2级属性
		    local k = math.random(100)
			if k > 90 then
			    level = 4
			elseif k > 70 then
			    level = 3
			elseif k > 40 then
			    level = 2
			end
		elseif type2 == 3 then            --80级蓝色：100,120,210,340,100,100,20,10此为1-8级概率,单位为千分之一。如果8级属性的数量少于40,那么7级属性有50%几率变为8级
		    local k = math.random(1000)
			local tbl3 = {100,120,210,340,100,100,20,10}
			local q = 0
			for i = 1,8 do
				q = q + tbl3[i]
				if q >= k then
			       level = i
				   break
				end
			end
			if level == 7 and act.GetActScriptData(29, 1, 0) < 40 then
			    if math.random(2) == 1 then
				    level = 8
				end
			end
			if level == 8 then
				act.SetActScriptData(29, 1, 0, act.GetActScriptData(29, 1, 0)+1)
				act.SaveActScriptData(29)
			end
		elseif type2 == 4 then             --80级橙色：490,450,50,10此为7-10级概率,单位为千分之一。如果8级属性的数量多于9级的9倍,则有30%几率转变为9级,如果9级属性多于10级5倍,则有10%几率转化为10级
	        local k = math.random(1000)
			local tbl3 = {490,450,50,10}  --8级和9级10级的掉落比为45：5：1
			local q = 0
			for i = 1,4 do
				q = q + tbl3[i]
				if q >= k then
			       level = i + 6
				   break
				end
			end
			local num1 = act.GetActScriptData(29, 1, 0)
			local num2 = act.GetActScriptData(29, 1, 1)
			local num3 = act.GetActScriptData(29, 1, 2)

			if num1 == 0 then
			    num1 = 1
			end
			if num2 == 0 then
			    num2 = 1
			end
			if num3 == 0 then
			    num3 = 1
			end

			if num1/num2 > 9 then
			    if math.random(10) >= 8 then
				    level = 9
				end
			end

			if num2/num3 > 5 then
			    if math.random(10) >= 10 then
				    level = 10
				end
			end

			if level > 7 then
			    act.SetActScriptData(29, 1, level-8, act.GetActScriptData(29, 1, level-8)+1)
				act.SaveActScriptData(29)
			end
		end
        index = type1 + math.random(LevelNum[type1][level])*100 + level
		return index
	elseif type2 == nil and type1 ~= nil then
	    return type1 + 101
	else
	    return 1
	end
end


function x_OnGetAnewStart(RoleID, MapID, InstanceID, TypeID, color, TotalTimesGetStar, TotoleIMItemUsed, NowStarNum, GrayStarNum, GreenStarNum, BlueStarNum, PinkStarNum, ImItemID)
    --[[
	基础的星星几率为 30% 45% 23% 2%
	幸运道具有两种,一种可以将基础概率变为 10% 55% 28% 7%
	另一种可以直接将星星变为紫色,概率为 0% 0% 0% 100%

	当一件装备升星次数每达到10次,则绿色概率降低3%,分别加到后面两种上
	比如：一件已经升星40次的装备,它的基础概率会变成 30% 33% 31% 6% 用道具为 10% 43% 36% 11%
	最高可以变成 30% 21% 39% 10% 用道具为 10% 31% 44% 15%

	最后4颗星星将和总共升星次数以及消耗的IM道具总数相关,具体规则如下：
	当消耗的IM道具为Q(为0时按1计算),则获得紫星时,有 0.4^（11-第几颗星星）*2.5* floor(2500/Q^0.5) * 1% 的概率降为蓝星星（最大取值为100%）
	比如,一个装备成长消耗了625个IM道具,那么它的第10个星星使用道具的概率将为 10% 31% 59% 0%
	]]
	------------------------特殊定制------------------------------------
	local tbl2 = {8019004,8019004,8050914,8060914,8070914,8080914,8090914,8101327,8110920,8110920,8121912,8130923,8029004,8029004,8050915,8060915,8070915,8080915,8090915,8101328,8110921,8110921,8121913,8130924,8049004,8049004,8051914,8061914,8071914,8081914,8091914,8101329,8111920,8111920,8122912,8130925,8039004,8039004,8051915,8061915,8071915,8081915,8091915,8101330,8111921,8111921,8122913,8130926}
	for i = 1,#tbl2 do
		if TypeID == tbl2[i] then
			return 4
		end
	end
	if TypeID == 8213207 or TypeID == 8212207 or TypeID == 8211207 or TypeID == 8210207 then
        return 4
	end
----------------------------------------------------------------------------
	local gray,green,blue,pink = 20,55,23,2                 --前4次的概率经过特殊处理
	if NowStarNum <= 4 then
	    green = green - (5-NowStarNum)*10
		blue = blue + (5-NowStarNum)*6
		pink = pink + (5-NowStarNum)*4
	end
	if ImItemID ~= nil and ImItemID ~= 0 then
	    if ImItemID == 4820180 then
		    return 4
		end
		gray,green,blue,pink = 10,55,28,7
		if NowStarNum <= 4 then
			green = green - (5-NowStarNum)*10
			--blue = blue + (5-NowStarNum)*2
			pink = pink + (5-NowStarNum)*10
		end
	end


	if NowStarNum <= 6 then        --超过6级的不参与数量加成
		local p = 10

		if NowStarNum >= 6 then
			p = 30
		end

		local k = math.floor(TotalTimesGetStar/p)
		if k > 8 then
			k = 8
		end
		green = green - 3 * k
		blue = blue + 2 * k
		pink = pink + k
	end

	if TotoleIMItemUsed == 0 then
	    TotoleIMItemUsed = 1
	end

	local Q = math.random(100)
	if Q <= gray then
        return 1
    elseif Q <= gray+green then
	    return 2
	elseif Q <= gray+green+blue then
	    return 3
	else
	    if NowStarNum <= 6 then
		    return 4
		else
		    if 0.4 ^ (11 - NowStarNum) *2.5* math.floor(2500/TotoleIMItemUsed^0.5) >= math.random(100) then
				return 3
			else
			    return 4
			end
		end
	end

end


function x_OnSoulCrystal_GetZiZhi(TypeID)
   --资质最小不能小于10
    if TypeID == 8220401 or TypeID == 8220402 then
        return 50
	elseif TypeID == 8220403 or TypeID == 8220404 or TypeID == 8220521 or TypeID == 8220522 or TypeID == 8220523 or TypeID == 8220621 or TypeID == 8220622 or TypeID == 8220623 then
	    return 80
    elseif TypeID >= 8220501 and TypeID <= 8220620 then

        local index = 18
		--10以下15以下16以上的比例是100：10：1
		if TypeID%100 <= 10 then                 --10以下的火种
		    index = index + TypeID%100 * 2
		elseif TypeID%100 <= 15 then          --15以下的火种
			index = index + 20 + (TypeID%100 - 10) * 3
		else                                    --16以上的火种
		    index = 56 + (TypeID%100 -15)*4
		end

		if index < 56 then
			index = math.random(56-index) + index
		end

		if index >= 56 then
			for i = 1,(80-index) do
			    if math.random(10) > 4 then  --60%可以加一
				    index = index + 1
				else
				    break
			    end
			end
		end
		return index
   end
end

function x_OnSoulCrystal_GetSkill(TypeID, ZiZhi)
    local pct = {1000-math.floor(280*ZiZhi/100)-math.floor(20*ZiZhi/100),math.floor(280*ZiZhi/100),math.floor(20*ZiZhi/100)}
	local index = math.random(1000)
	if pct[1] >= index then
	    index = 1
	elseif pct[1] + pct[2] >= index then
	    index = 2
	else
		index = 3
	end
	if TypeID == 8220401 or (TypeID >= 8220501 and TypeID <= 8220599) then
		if index == 1 then
		    return 2600001 + (math.random(8) - 1) * 100
		elseif index == 2 then
		    return 2600801 + (math.random(13) - 1) * 100
		else
			return 2602101 + (math.random(4) - 1) * 100
		end 	
	else
	    if index == 1 then
		    return 2610001 + (math.random(8) - 1) * 100
		elseif index == 2 then
		    return 2610801 + (math.random(13) - 1) * 100
		else
			return 2612101 + (math.random(4) - 1) * 100
		end
	end
end

function x_OnSoulCrystal_GetAbility(MapID, InstanceID, TypeID, FirstTime, ZiZhi, Pct1, Pct2, RoleID, ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8) --RoleID有可能是-1或者0
    --[[
	难中易三中能力的出现比例是：    1：2：3
	]]
	local base = {}
	base[1] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33}
	base[2] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28}
	base[3] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
	if FirstTime == 1 then
		local tbl = {}
		local maxnum = 0
		if math.floor(ZiZhi/10) < 5 then
            maxnum = math.random(3)
        else
            maxnum = math.random(3,math.floor(ZiZhi/10)-2)
		end
		for i=1,maxnum do
		    local k = math.random(6)
		    if k>= 4 then
		        local q = math.random(#base[1])
				tbl[i] = 100000 + base[1][q] * 1000 + math.random(3) * 100 + 1
				table.remove(base[1],q)
			elseif k>= 2 then
			    local q = math.random(#base[2])
				tbl[i] = 200000 + base[2][q] * 1000 + math.random(3) * 100 + 1
				table.remove(base[2],q)
			else
			    local q = math.random(#base[3])
				tbl[i] = 300000 + base[3][q] * 1000 + math.random(3) * 100 + 1
				table.remove(base[3],q)
			end
		end
		return tbl[1],tbl[2],tbl[3],tbl[4],tbl[5],tbl[6],tbl[7],tbl[8]
	else
	    Pct1 = Pct1 + 2* math.floor(ZiZhi^0.5)        --开启新能力概率
	    Pct2 = Pct2 + ZiZhi                           --能力升级的概率
		if Pct2 > 50 then
		    Pct2 = Pct2
		end
		if Pct2 > 70 then
		    Pct2 = Pct2
		end
		if Pct1 > 100 then
		   Pct1 = 100
		end
		if Pct2 > 100 then
		   Pct2 = 100
		end

		local tbl = {ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8}
		local max = 0
		for i = 1,8 do
		    if tbl[i] ~= nil and tbl[i] ~= 0 then
			    if math.random(100) > 100 - Pct2 then
			        if tbl[i]%10 ~= 0 then
					    tbl[i] = tbl[i] + 1
					end
					local delete = math.floor(tbl[i]/1000)-math.floor(tbl[i]/100000)*100
					for k,v in pairs(base[math.floor(tbl[i]/100000)]) do
					    if v == delete then
						    table.remove(base[math.floor(tbl[i]/100000)],k)
							break
						end
					end
			    end
			else
			    max = i
			    break
			end
		end
		if max ~= 0 then
		    if math.random(100) > 100 - Pct1 then
				local k = math.random(6)
				if k>= 4 then
					local q = math.random(#base[1])
					tbl[max] = 100000 + base[1][q] * 1000 + math.random(3) * 100 + 1
				elseif k>= 2 then
					local q = math.random(#base[2])
					tbl[max] = 200000 + base[2][q] * 1000 + math.random(3) * 100 + 1
				else
					local q = math.random(#base[3])
					tbl[max] = 300000 + base[3][q] * 1000 + math.random(3) * 100 + 1
				end
			end
		end
		return tbl[1],tbl[2],tbl[3],tbl[4],tbl[5],tbl[6],tbl[7],tbl[8]
	end
end

function x_OnSoulCrystal_ResetAbility(MapID, InstanceID, TypeID, ZiZhi, RoleID, ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8)
    local a,b = role.GetRoleSoul(MapID,InstanceID,RoleID)
	local bool =true
	if a~= nil and b ~= nil then
		if (TypeID == 8220401 or (TypeID >= 8220501 and TypeID <= 8220599)) and a < 20 then
			bool = false
		elseif (TypeID == 8220402 or (TypeID >= 8220601 and TypeID <= 8220699)) and b < 20 then
			bool = false
		end
	end
	if role.GetRoleSilver(MapID, InstanceID, RoleID) < 200000 then
	    bool = false
	end
	if bool then
		if (TypeID == 8220401 or (TypeID >= 8220501 and TypeID <= 8220599))  then
			role.SetRoleSoul(MapID,InstanceID,RoleID,a-20,b)
		elseif (TypeID == 8220402 or (TypeID >= 8220601 and TypeID <= 8220699)) then
			role.SetRoleSoul(MapID,InstanceID,RoleID,a,b-20)
		end

		role.DecRoleSilver(MapID, InstanceID, RoleID, 200000, 452)

		local base = {}
		base[1] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33}
		base[2] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28}
		base[3] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}

		local tbl = {ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8}
		local strength = 0
		for i = 1,8 do
			if tbl[i] ~= nil and tbl[i] ~= 0 then
				local k = tbl[i]%10
				if k == 0 then
					k =10
				end
				strength = strength + k
			else
				break
			end
		end
		if strength > 80 then
		    strength = 80
		end
		--此下为出现的能力的数量跟新装备的强度的关系
		local ability_num = 0
		if strength < 8 then
			ability_num = math.random(1,strength)
		elseif strength <= 21 then
			ability_num = math.random(math.ceil(strength/10),6)
			if ability_num == 6 then
				if math.random(2) == 1 then
					ability_num = ability_num + 1
				end
			end
			if ability_num == 7 then
				if math.random(5) > 4 then
					ability_num = ability_num + 1
				end
			end
		else
			ability_num = math.random(math.ceil(strength/10),8)
		end

		local tbl_new = {}

		for i=1,ability_num do
			local k = math.random(6)
			if k>= 4 then
				local q = math.random(#base[1])
				tbl_new[i] = 100000 + base[1][q] * 1000 + math.random(3) * 100 + 1
				table.remove(base[1],q)
			elseif k>= 2 then
				local q = math.random(#base[2])
				tbl_new[i] = 200000 + base[2][q] * 1000 + math.random(3) * 100 + 1
				table.remove(base[2],q)
			else
				local q = math.random(#base[3])
				tbl_new[i] = 300000 + base[3][q] * 1000 + math.random(3) * 100 + 1
				table.remove(base[3],q)
			end
		end

		if strength - ability_num > 0 then
			for i = 1,strength-ability_num do
				local index = math.random(ability_num)
				if tbl_new[index] % 10 ~= 0 then
					tbl_new[index] = tbl_new[index] + 1
				else
					if math.random(2) == 1 then
						for i = 1,ability_num do
							if tbl_new[i] % 10 ~= 0 then
								tbl_new[i] = tbl_new[i] + 1
								break
							end
						end
					else
						for i = 0,ability_num-1 do
							if tbl_new[ability_num-i] % 10 ~= 0 then
								tbl_new[ability_num-i] = tbl_new[ability_num-i] + 1
								break
							end
						end
					end
				end
			end
		end

		return 0,tbl_new[1],tbl_new[2],tbl_new[3],tbl_new[4],tbl_new[5],tbl_new[6],tbl_new[7],tbl_new[8]
    else
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26,	605020)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 1,ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8
	end
end

function x_OnUseItemType109(MapID, InstanceID, RoleID, TypeID, ObjID, High32, Low32)
    if ObjID < 8000000 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26,	444501)     --该功能只能对装备使用
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	else
	    if (ObjID <= 8220699 and ObjID >= 8150101) or ObjID < 8140001 then  --是装备
			--每次添加不可强化装备需要修改该表,不可强化的装备都要填进来
			local tbl = {8213207,8212207,8211207,8210207,8019001,8019002,8019004,8019005,8029001,8029002,8029004,8029005,8039001,8039002,8039004,8039005,8049001,8049002,8049004,8049005,8050908,8050909,8050910,8050911,8050914,8050915,8051908,8051909,8051910,8051911,8051914,8051915,8060908,8060909,8060910,8060911,8060914,8060915,8061908,8061909,8061910,8061911,8061914,8061915,8070908,8070909,8070910,8070911,8070914,8070915,8071908,8071909,8071910,8071911,8071914,8071915,8080908,8080909,8080910,8080911,8080914,8080915,8081908,8081909,8081910,8081911,8081914,8081915,8090908,8090909,8090910,8090911,8090914,8090915,8091908,8091909,8091910,8091911,8091914,8091915,8101315,8101316,8101317,8101318,8101319,8101320,8101321,8101322,8101327,8101328,8101329,8101330,8110710,8110711,8110912,8110913,8110920,8110921,8111912,8111913,8111920,8111921,8112001,8112002,8112003,8121709,8121710,8121908,8121909,8121912,8121913,8122908,8122909,8122912,8122913,8130915,8130916,8130917,8130918,8130923,8130924,8130925,8130926,8220001,8220201}
			local bool = true
			for k,v in pairs(tbl) do
			   if v == ObjID then
			        bool = false
					break
			   end
			end
			if bool then
				if (TypeID >= 4820249 and TypeID <= 4820254) or TypeID == 4820258 or TypeID == 4820259 then
					local k = role.GetEquipStoneAddLevel(RoleID, High32, Low32)
					if k ~= nil then
						if TypeID == 4820249 or TypeID == 4820250 then
                            if k < 8 then
							    local p = {100,50,20,40,30,10,0,0} --到6级大概需要833个道具，卖80金
								local index = math.random(100)
								if 101 - index <= p[k+1] then
								    role. SetEquipStoneAddLevel (RoleID, High32, Low32, k+1)
								else
								    role. SetEquipStoneAddLevel (RoleID, High32, Low32, k-1)
								end
								return 1
							else
							    local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 26,	444502)     --该雕文只能对8级以下宝石增幅的装备使用
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
								return 0
							end
						elseif TypeID == 4820251 or TypeID == 4820252 then
                            if k < 8 then
							    role. SetEquipStoneAddLevel (RoleID, High32, Low32, k+1)
								return 1
							else
							    local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 26,	444502)     --该雕文只能对8级以下宝石增幅的装备使用
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
								return 0
							end
						elseif TypeID == 4820253 or TypeID == 4820254 then
							if k < 10 then
							    role. SetEquipStoneAddLevel (RoleID, High32, Low32, k+1)
								return 1
							else
							    local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 26,	444503)     --该雕文只能对10级以下宝石增幅的装备使用
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
								return 0
							end
						elseif TypeID == 4820258 or TypeID == 4820259 then
							if k < 9 then
							    role. SetEquipStoneAddLevel (RoleID, High32, Low32, k+1)
								return 1
							else
							    local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 26,	444507)     --该雕文只能对10级以下宝石增幅的装备使用
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
								return 0
							end
						end
					end
					return 0
				elseif TypeID >= 4820255 and TypeID <= 4820256 then
                    local k = role.GetEquipGrowAttID(RoleID, High32, Low32)
					if k ~= nil then
					    if k > 1000000 then
							local LevelNum = {}
							LevelNum[1100000] = {5,5,5,10,10,10,10,15,15,15}
							LevelNum[1200000] = {5,5,5,10,10,10,10,15,15,15}
							LevelNum[2100000] = {14,14,14,14,14,19,19,19,19,19}
							LevelNum[2200000] = {14,14,14,14,14,19,19,19,19,19}
							LevelNum[3100000] = {14,14,14,14,14,19,19,19,19,19}
							LevelNum[3200000] = {14,14,14,14,14,19,19,19,19,19}
							LevelNum[4100000] = {14,14,14,14,14,19,19,19,19,19}
							LevelNum[4200000] = {14,14,14,14,14,19,19,19,19,19}
							LevelNum[5100000] = {5,5,5,5,5,11,10,10,10,10}
							LevelNum[5200000] = {5,5,5,5,5,11,10,10,10,10}
							LevelNum[6100000] = {5,5,5,5,5,11,10,10,10,10}
							LevelNum[6200000] = {5,5,5,5,5,11,10,10,10,10}
							LevelNum[7100000] = {7,7,6,6,6,6,5,10,10,10}
							LevelNum[7200000] = {7,7,6,6,6,6,5,10,10,10}
							local level = k%10
							if(level == 0) then
								level = 10
							end
							local type1 = k - k%100000
							--表中漏掉了7100502，因此需要特殊处理---------------------------------
							local growid = type1 + math.random(LevelNum[type1][level])*100 + level
							if growid == 7100502 then
							    growid = 7100402
							end
							-----------------------------------------------------------------------
							role.SetEquipGrowAttID(RoleID, High32, Low32, growid)
						end
						return 1
					end
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26,	444504)     --只能对具有成长属性的装备使用此功能
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					return 0
				end
				return 0
			else
			    if (ObjID == 8213207 or ObjID == 8212207 or ObjID == 8211207 or ObjID == 8210207) and TypeID == 4800240 then
                    local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"])
					local times = role.GetRoleScriptData(RoleID, 1, RoleDataType["SDWI_Times"])
				    local baoshi = k%10
					local shengyin = math.floor((k%1000)/10)
					local chengzhang = math.floor((k%100000)/1000)
					local baoshi_zengfu = role.GetEquipStoneAddLevel(RoleID, High32, Low32)
					local qianghua = role.GetEquipStrengthLevel(RoleID,High32, Low32)
                    local gexp={24,48,72,96,120,144,192,240,312,528,768}
                    local tbl_percent = {}
					tbl_percent[40] = {2,19,7,5,67}
					tbl_percent[60] = {2,19,7,5,67}
					tbl_percent[80] = {0,0,0,0,100}
					tbl_percent[90] = {0,60,0,0,40}
					tbl_percent[120] = {3,36,3,3,55}
					tbl_percent[150] = {0,39,3,0,58}
					tbl_percent[180] = {3,32,0,6,59}
					tbl_percent[230] = {0,52,0,2,46}
					tbl_percent[300] = {1,44,1,1,53}
					tbl_percent[330] = {0,0,0,0,100}

					local tbl_max = {}
					tbl_max[40] = {1,2,3,2,8}
					tbl_max[60] = {2,4,6,4,15}
					tbl_max[80] = {2,4,6,4,16}
					tbl_max[90] = {0,4,0,0,14}
					tbl_max[120] = {3,3,7,5,14}
					tbl_max[150] = {0,5,8,0,15}
					tbl_max[180] = {4,5,0,7,16}
					tbl_max[230] = {0,5,0,8,17}
					tbl_max[300] = {5,7,9,9,19}
					tbl_max[330] = {5,7,9,9,20}

					local tbl_index = {40,60,80,90,120,150,180,230,300,330}

					local index = 0
					for i=1,10 do
					    if tbl_index[i] > times then
						    index = tbl_index[i]
							break
						end
					end
					if index ~= 0 then
					    local q = 101 - math.random(100)
						local index_ = 5
						local totle = 0
                        for i=1,5 do
                            totle = totle +	tbl_percent[index][i]
		                    if totle >= q then
							    index_ = i
								break
							end
						end
						--[[local level = 0
						local function GetSDWQlevel(n,baoshi,baoshi_zengfu,chengzhang,shengyin,qianghua)
							if n == 1 then
							    return baoshi
							elseif n == 2 then
							    return baoshi_zengfu
					        elseif n == 3 then
							    return chengzhang
							elseif n == 4 then
								return shengyin
							elseif n == 2 then
							    return qianghua
							end
						end]]
						--level = GetSDWQlevel(index_,baoshi,baoshi_zengfu,chengzhang,shengyin,qianghua)
						if index_ == 1 and baoshi >= tbl_max[index][index_] then
						    if chengzhang < tbl_max[index][3] then
							    index_ = 3
							elseif shengyin < tbl_max[index][4] then
							    index_ = 4
							else
							    index_ = 5
							end
						end

						if index_ == 3 and chengzhang >= tbl_max[index][index_] then
						    if baoshi < tbl_max[index][1] then
							    index_ = 1
							elseif shengyin < tbl_max[index][4] then
							    index_ = 4
							else
							    index_ = 5
							end
						end

						if index_ == 4 and shengyin >= tbl_max[index][index_] then
						    if chengzhang < tbl_max[index][3] then
							    index_ = 3
							elseif baoshi < tbl_max[index][1] then
							    index_ = 1
							else
							    index_ = 5
							end
						end

						if index_ == 1 then
						    if baoshi < tbl_max[index][index_] then
							    if ObjID == 8213207 or ObjID == 8212207 then
								    role.PutStoneOnEquip(RoleID,High32, Low32, 3030014)
								else
									role.PutStoneOnEquip(RoleID,High32, Low32, 3030006)
								end
							    role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+1)
								role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_Times"],times+1)
					        end
						elseif index_ == 2 then
						    if math.random(4) > 1 then
								if baoshi_zengfu < tbl_max[index][index_] then
									role. SetEquipStoneAddLevel (RoleID, High32, Low32, baoshi_zengfu+1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10)
								else
									role. SetEquipStoneAddLevel (RoleID, High32, Low32, baoshi_zengfu-1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k-10)
								end
							else
								if baoshi_zengfu > 0 then
									role. SetEquipStoneAddLevel (RoleID, High32, Low32, baoshi_zengfu-1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k-10)
								else
									role. SetEquipStoneAddLevel (RoleID, High32, Low32, baoshi_zengfu+1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10)
								end
							end
							role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_Times"],times+1)
						elseif index_ == 3 then
						    if chengzhang < tbl_max[index][index_] then
							    role.SetGrowEquipLevel(RoleID,High32, Low32,gexp[chengzhang+1])
								role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+1000)
								role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_Times"],times+1)
					        end
						elseif index_ == 4 then
						    if shengyin < tbl_max[index][index_] then
								if ObjID == 8213207 or ObjID == 8212207 then
								    role.SetEquipAdditionalAbility(RoleID,High32, Low32,3040023)
								else
									role.SetEquipAdditionalAbility(RoleID,High32, Low32,3040023)
								end
								role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10)
								role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_Times"],times+1)
					        end
						elseif index_ == 5 then
						    if math.random(5) > 1 then
								if qianghua < tbl_max[index][index_] then
									role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua+1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10000000)
								else
									role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua-1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k-10000000)
								end
							else
								if qianghua > 0 then
									role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua-1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k-10000000)
								else
									role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua+1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10000000)
								end
							end
							role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_Times"],times+1)
						end
					elseif times >= 330 then
					    role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_Times"],times+1)
					    local k_ = math.random(200)
						if k_ == 3 then
						    if chengzhang < 11 then
							    role.SetGrowEquipLevel(RoleID,High32, Low32,gexp[chengzhang+1])
								role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+1000)
							else
							    if qianghua < 17+math.random(3) then
									role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua+1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10000000)
								else
									role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua-1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k-10000000)
								end
					        end
						elseif k_ == 4 then
						    if shengyin < 10 then
							    role.SetEquipAdditionalAbility(RoleID,High32, Low32,3040023)
								role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10)
							else
							    if qianghua < 17+math.random(3) then
									role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua+1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10000000)
								else
									role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua-1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k-10000000)
								end
					        end
						elseif k_ == 5 then
						    if baoshi < 5 then
							    if ObjID == 8213207 or ObjID == 8212207 then
								    role.PutStoneOnEquip(RoleID,High32, Low32, 3030014)
								else
									role.PutStoneOnEquip(RoleID,High32, Low32, 3030006)
								end
							    role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+1)
							else
							    if qianghua < 17+math.random(3) then
									role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua+1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10000000)
								else
									role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua-1)
									--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k-10000000)
								end
					        end
						elseif k_ > 140 then
						    if baoshi_zengfu > 7 then
								role. SetEquipStoneAddLevel (RoleID, High32, Low32, baoshi_zengfu-1)
								--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k-10)
							else
								role. SetEquipStoneAddLevel (RoleID, High32, Low32, baoshi_zengfu+1)
								--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10)
							end
						else
						    if qianghua < 17+math.random(3) then
								role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua+1)
								--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k+10000000)
							else
								role.SetEquipStrengthLevel(RoleID,High32, Low32,qianghua-1)
								--role.SetRoleScriptData(RoleID, 1, RoleDataType["SDWI_State"],k-10000000)
							end
						end
					end

					return 1
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26,	444505)     --不可强化的装备无法使用此功能
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					return 0
				end
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26,	444506)     --只能对普通装备使用此功能
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	end
end

--注册函数

aux.RegisterRoleEvent(35, "x_RoleFAbaoLevelup")
aux.RegisterRoleEvent(55, "x_RoleLianQi")
aux.RegisterRoleEvent(48, "x_OnEquipConsolidateLevelUp")
aux.RegisterRoleEvent(49, "x_OnProduceEquip")
aux.RegisterRoleEvent(50, "x_OnEquipDigHole")
aux.RegisterRoleEvent(51, "x_OnCutStone")
aux.RegisterRoleEvent(53, "x_OnBrandEquip")
--aux.RegisterRoleEvent(56, "x_OnUseBeGoldStone")
aux.RegisterRoleEvent(80, "x_OnChooseGrowAtt")--装备成长
aux.RegisterRoleEvent(81, "x_OnGetAnewStart")
aux.RegisterWorldEvent(7, "x_OnSoulCrystal_GetZiZhi")
aux.RegisterWorldEvent(8, "x_OnSoulCrystal_GetSkill")
aux.RegisterWorldEvent(9, "x_OnSoulCrystal_GetAbility")
--aux.RegisterRoleEvent(82, "x_OnSoulCrystal_GetZiZhi")
--aux.RegisterRoleEvent(83, "x_OnSoulCrystal_GetSkill")
--aux.RegisterRoleEvent(84, "x_OnSoulCrystal_GetAbility")
aux.RegisterRoleEvent(86, "x_OnSoulCrystal_ResetAbility")
aux.RegisterRoleEvent(89, "x_OnUseItemType109")
