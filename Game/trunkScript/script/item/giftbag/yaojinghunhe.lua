
--弱效魂核
function I4101001_hunhe(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k = math.random(1,9)  --9种魂石
	local i = math.random(1,3)  --3个等级
	local j = math.random(3,5)  --3-5个
	if k == 1 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100004, j, -1, 8, 420)
	elseif k == 1 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100005, j, -1, 8, 420)
	elseif k == 1 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100006, j, -1, 8, 420)
	elseif k == 2 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100014, j, -1, 8, 420)
	elseif k == 2 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100015, j, -1, 8, 420)
	elseif k == 2 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100016, j, -1, 8, 420)
	elseif k == 3 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100024, j, -1, 8, 420)
	elseif k == 3 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100025, j, -1, 8, 420)
	elseif k == 3 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100026, j, -1, 8, 420)
	elseif k == 4 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100034, j, -1, 8, 420)
	elseif k == 4 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100035, j, -1, 8, 420)
	elseif k == 4 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100036, j, -1, 8, 420)
	elseif k == 5 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100044, j, -1, 8, 420)
	elseif k == 5 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100045, j, -1, 8, 420)
	elseif k == 5 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100046, j, -1, 8, 420)
	elseif k == 6 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100054, j, -1, 8, 420)
	elseif k == 6 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100055, j, -1, 8, 420)
	elseif k == 6 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100056, j, -1, 8, 420)
	elseif k == 7 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100064, j, -1, 8, 420)
	elseif k == 7 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100065, j, -1, 8, 420)
	elseif k == 7 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100066, j, -1, 8, 420)
	elseif k == 8 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100074, j, -1, 8, 420)
	elseif k == 8 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100075, j, -1, 8, 420)
	elseif k == 8 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100076, j, -1, 8, 420)
	elseif k == 9 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100084, j, -1, 8, 420)
	elseif k == 9 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100085, j, -1, 8, 420)
	elseif k == 9 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100086, j, -1, 8, 420)
	end
end

function I4101001_CanUsehunhe(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4101001, 1, "I4101001_hunhe")
aux.RegisterItemEvent(4101001, 0, "I4101001_CanUsehunhe")



--妖之魂核
function I4101002_hunhe(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k = math.random(1,9)  --9种魂石
	local i = math.random(1,3)  --3个等级
	local j = math.random(3,5)  --3-5个
	if k == 1 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100005, j, -1, 8, 420)
	elseif k == 1 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100006, j, -1, 8, 420)
	elseif k == 1 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100007, j, -1, 8, 420)
	elseif k == 2 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100015, j, -1, 8, 420)
	elseif k == 2 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100016, j, -1, 8, 420)
	elseif k == 2 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100017, j, -1, 8, 420)
	elseif k == 3 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100025, j, -1, 8, 420)
	elseif k == 3 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100026, j, -1, 8, 420)
	elseif k == 3 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100027, j, -1, 8, 420)
	elseif k == 4 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100035, j, -1, 8, 420)
	elseif k == 4 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100036, j, -1, 8, 420)
	elseif k == 4 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100037, j, -1, 8, 420)
	elseif k == 5 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100045, j, -1, 8, 420)
	elseif k == 5 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100046, j, -1, 8, 420)
	elseif k == 5 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100047, j, -1, 8, 420)
	elseif k == 6 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100055, j, -1, 8, 420)
	elseif k == 6 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100056, j, -1, 8, 420)
	elseif k == 6 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100057, j, -1, 8, 420)
	elseif k == 7 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100065, j, -1, 8, 420)
	elseif k == 7 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100066, j, -1, 8, 420)
	elseif k == 7 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100067, j, -1, 8, 420)
	elseif k == 8 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100075, j, -1, 8, 420)
	elseif k == 8 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100076, j, -1, 8, 420)
	elseif k == 8 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100077, j, -1, 8, 420)
	elseif k == 9 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100085, j, -1, 8, 420)
	elseif k == 9 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100086, j, -1, 8, 420)
	elseif k == 9 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100087, j, -1, 8, 420)
	end
end

function I4101002_CanUsehunhe(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4101002, 1, "I4101002_hunhe")
aux.RegisterItemEvent(4101002, 0, "I4101002_CanUsehunhe")


--强效魂核
function I4101003_hunhe(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k = math.random(1,9)  --9种魂石
	local i = math.random(1,3)  --3个等级
	local j = math.random(3,5)  --3-5个
	if k == 1 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100006, j, -1, 8, 420)
	elseif k == 1 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100007, j, -1, 8, 420)
	elseif k == 1 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100008, j, -1, 8, 420)
	elseif k == 2 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100016, j, -1, 8, 420)
	elseif k == 2 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100017, j, -1, 8, 420)
	elseif k == 2 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100018, j, -1, 8, 420)
	elseif k == 3 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100026, j, -1, 8, 420)
	elseif k == 3 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100027, j, -1, 8, 420)
	elseif k == 3 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100028, j, -1, 8, 420)
	elseif k == 4 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100036, j, -1, 8, 420)
	elseif k == 4 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100037, j, -1, 8, 420)
	elseif k == 4 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100038, j, -1, 8, 420)
	elseif k == 5 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100046, j, -1, 8, 420)
	elseif k == 5 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100047, j, -1, 8, 420)
	elseif k == 5 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100048, j, -1, 8, 420)
	elseif k == 6 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100056, j, -1, 8, 420)
	elseif k == 6 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100057, j, -1, 8, 420)
	elseif k == 6 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100058, j, -1, 8, 420)
	elseif k == 7 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100066, j, -1, 8, 420)
	elseif k == 7 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100067, j, -1, 8, 420)
	elseif k == 7 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100068, j, -1, 8, 420)
	elseif k == 8 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100076, j, -1, 8, 420)
	elseif k == 8 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100077, j, -1, 8, 420)
	elseif k == 8 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100078, j, -1, 8, 420)
	elseif k == 9 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100086, j, -1, 8, 420)
	elseif k == 9 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100087, j, -1, 8, 420)
	elseif k == 9 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100088, j, -1, 8, 420)
	end
end

function I4101003_CanUsehunhe(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4101003, 1, "I4101003_hunhe")
aux.RegisterItemEvent(4101003, 0, "I4101003_CanUsehunhe")




--极效魂核
function I4101004_hunhe(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k = math.random(1,9)  --9种魂石
	local i = math.random(1,3)  --3个等级
	local j = math.random(3,5)  --3-5个
	if k == 1 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100008, j, -1, 8, 420)
	elseif k == 1 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100009, j, -1, 8, 420)
	elseif k == 1 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100010, j, -1, 8, 420)
	elseif k == 2 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100018, j, -1, 8, 420)
	elseif k == 2 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100019, j, -1, 8, 420)
	elseif k == 2 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100020, j, -1, 8, 420)
	elseif k == 3 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100028, j, -1, 8, 420)
	elseif k == 3 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100029, j, -1, 8, 420)
	elseif k == 3 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100030, j, -1, 8, 420)
	elseif k == 4 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100038, j, -1, 8, 420)
	elseif k == 4 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100039, j, -1, 8, 420)
	elseif k == 4 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100040, j, -1, 8, 420)
	elseif k == 5 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100048, j, -1, 8, 420)
	elseif k == 5 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100049, j, -1, 8, 420)
	elseif k == 5 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100050, j, -1, 8, 420)
	elseif k == 6 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100058, j, -1, 8, 420)
	elseif k == 6 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100059, j, -1, 8, 420)
	elseif k == 6 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100060, j, -1, 8, 420)
	elseif k == 7 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100068, j, -1, 8, 420)
	elseif k == 7 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100069, j, -1, 8, 420)
	elseif k == 7 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100070, j, -1, 8, 420)
	elseif k == 8 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100078, j, -1, 8, 420)
	elseif k == 8 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100079, j, -1, 8, 420)
	elseif k == 8 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100080, j, -1, 8, 420)
	elseif k == 9 and i == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100088, j, -1, 8, 420)
	elseif k == 9 and i == 2 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100089, j, -1, 8, 420)
	elseif k == 9 and i == 3 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3100090, j, -1, 8, 420)
	end
end

function I4101004_CanUsehunhe(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4101004, 1, "I4101004_hunhe")
aux.RegisterItemEvent(4101004, 0, "I4101004_CanUsehunhe")






