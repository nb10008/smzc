
function s02_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 1154 then
		local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3000521423, 41300/50, 6102, 40700/50)--41300  6102  40700
	end
end
aux.RegisterMapEvent("s02",6,"s02_OnEnterTrigger")

--玩家死亡
function s02_RoleDie(MapID, InstanceID, RoleID, KillerID)
	if unit.IsPlayer(RoleID) and unit.IsPlayer(KillerID) then
		role.SetTrialCompleteNum(KillerID, 2016, role.GetTrialCompleteNum(KillerID,2016) + 1)
		if role.GetTrialState(KillerID, 2016) == 1 and role.GetTrialCompleteNum(KillerID,2016) >= Trail_maxnum[2016] then
			role.TrialComplete(KillerID,2016)
		end
	end
end

aux.RegisterMapEvent("s02", 5, "s02_RoleDie")

--加载m02时，若全局变量WanShengJie_Open = 1,则刷出万圣节NPC
function s02_OnInit(MapID,InstanceID,CreatureID,CreatureTypeID)
	--[[if WanShengJie_Open == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 5600002, 700, 23510, 765, 1, "")
	end]]
	if LaoDongJie_Open == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 5600057, 700, 23510, 765, 1, "")
	end
    if GetFaith_Open == 1 then
	    map.MapCreateColCreature(MapID, InstanceID, 5610015, 633, 23094, 651, 1, "")
	end
	if XinYangZhiNu_Open == 1 then
	    map.MapCreateColCreature(MapID, InstanceID, 5610045, 587, 23510, 821, 1, "")
	end
	if CangJingGe_Open == 1 then
	    map.MapCreateColCreature(MapID, InstanceID, 5511032, 640, 23510, 822, 1, "")
	end
	if JBZDY_Open == 1 then
	    map.MapCreateColCreature(MapID, InstanceID, 6011030, 371, 23510, 732, 1, "")
	end
	local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curyear = tonumber(os.date("%Y"))
	if curyear == 2011 and curmon == 1 and curday >= 11 and curday <= 18 and LaBaZhou_Open == 1 then
	    LaBaZhou_NPC = map.MapCreateColCreature(MapID, InstanceID, 5600101, 692, 23510, 599, 1, "")
	end
	if guoqingshizhe_id == nil or guoqingshizhe_id == 0  then
		if( curyear== 2011 and curmon==10 and curday < 18) then
			guoqingshizhe_id = map.MapCreateCreature(3424073512, -1, 5610062, 505, 23510, 759)
		end
	end
	if guoqingshangdian_id == nil or guoqingshangdian_id == 0  then
		if( curyear== 2011 and curmon==10 and curday < 18) then
			guoqingshangdian_id = map.MapCreateCreature(3424073512, -1, 5610064, 500, 23510, 765)
		end
	end
	--勇士回归
	if BravemanBack == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 5600078, 565, 23510, 854, 1, "")
	end

	--欢乐侍者
	if huanleshizhe == 1 then
		map.MapCreateCreature(3424073512, -1, 5610065, 402, 23692, 753)
	end
	--欢乐币兑换人
	if huanlebiwupin == 1 then
		map.MapCreateCreature(3424073512, -1, 5610066, 395, 23692, 753)
		map.MapCreateCreature(3424073512, -1, 5610067, 402, 23692, 760)
	end
	if huanlebijinqian == 1 then
		map.MapCreateCreature(3424073512, -1, 5610093, 408, 23692, 747)
	end
	--单身主义者（光棍节NPC）
	if (curyear == 2011 and curmon == 11 and curday >= 8 and curday <=14) then
		map.MapCreateColCreature(MapID, InstanceID, 5600079, 505, 23510, 759, 1, "")
	end
	--贝尔蒂（爱之光NPC）
	if (happiness == 1)then
		map.MapCreateColCreature(MapID, InstanceID, 5610094, 543,23510,831, 1, "")
	end
	--幸运之神
	if lucky == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 5610098, 543,23510,831, 1, "")
	--龙年使者
		--map.MapCreateColCreature(MapID, InstanceID, 5610099, 542,23510,817, 1, "")
	end
	if curyear == 2012 and ((curmon == 2 and curday >=28) or (curmon == 3 and curday <=5 ) )then
	--跨服PK预选人
		map.MapCreateColCreature(MapID, InstanceID, 5610105, 295,23510,737,1, "")
	end
	if (curyear == 2012 and (curmon == 1 and curday >=17) or (curmon == 2 and curday <=20 ) )then  --新春商人
		map.MapCreateColCreature(MapID, InstanceID, 5610105, 295,23510,737, 1, "")
	end
	--特权使者
	if (tequanshizhe == 1)then
		map.MapCreateColCreature(MapID, InstanceID, 5610110, 650,23510,876, 1, "")
	end
	--宝石兑换上
	if (Baoshiduihuan == 1)then
		map.MapCreateColCreature(MapID, InstanceID, 6040036, 744,23510,716, 1, "")--744 716
	end
	--通灵世界刷新传送门和脚本创建地图
	map.MapCreateColCreature(MapID, InstanceID, 5610069, 892, 23809, 748, 1, "")
	map.CreateInstance(3390518568, 1)
	map.CreateInstance(52648950, 1)
	map.CreateInstance(52648182, 1)
	map.CreateInstance(52648438, 1)
	map.CreateInstance(52649974, 1)
	map.CreateInstance(52649206, 1)
	map.CreateInstance(52649462, 1)
	map.CreateInstance(S59_MapID, 9999)
	--阳光大使
	if yangguangdashi == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 5610089, 569, 22980, 432, 1, "")
	end

	--s神魔造物主
	if shenmozaowuzhu == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 5011017, 471,22980,298, 1, "")
	end

	--s妖精秘药兑换商
	if yaojingmiyaoduihuan == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 5610115, 518,23510,618, 1, "")-- pos_x="25942" pos_y="23510" pos_z="30905"/>
	end

end
aux.RegisterMapEvent("s02", 0, "s02_OnInit")

--神器任务攻击所属
function Shenqi_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	if unit.IsPlayer(SrcID) then
		local roleID = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
		local creID = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
		if roleID ~= nil and roleID == SrcID then
			return false, true, false, true
		elseif creID ~= nil and (creID >= 3040380 and creID <= 3040383) then
			return true, false, false, true
		else
			return false, false, false, false
		end
	end
end
aux.RegisterMapEvent("s02", 16, "Shenqi_FriendEnemy")
