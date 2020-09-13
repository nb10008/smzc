--团购独享卡
function I4530167_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211102, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070088, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070081, 1, -1, 8, 420)
	--判断玩家性别
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700159, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700160, 1, -1, 8, 420)
	end
end
function I4530167_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530167, 1, "I4530167_GiftBag")
aux.RegisterItemEvent(4530167, 0, "I4530167_CanUseGiftBag")


--1级烈焰卡
function I4530168_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530171, 1, -1, 8, 420)
end
function I4530168_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530168, 1, "I4530168_GiftBag")
aux.RegisterItemEvent(4530168, 0, "I4530168_CanUseGiftBag")


--1级焚煞卡
function I4530169_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530173, 1, -1, 8, 420)
end
function I4530169_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4530169, 1, "I4530169_GiftBag")
aux.RegisterItemEvent(4530169, 0, "I4530169_CanUseGiftBag")


--1级弑灭卡
function I4530170_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530175, 1, -1, 8, 420)
end
function I4530170_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4530170, 1, "I4530170_GiftBag")
aux.RegisterItemEvent(4530170, 0, "I4530170_CanUseGiftBag")


--30级烈焰卡
function I4530171_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8160010, 1, 4, 8, 420)
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8170011, 1, 4, 8, 420)
    end
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030120, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030122, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010035, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530172, 1, -1, 8, 420)
end
function I4530171_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 16) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4530171, 1, "I4530171_GiftBag")
aux.RegisterItemEvent(4530171, 0, "I4530171_CanUseGiftBag")


--30级焚煞卡
function I4530173_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8160008, 1, 5, 8, 420)
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8170008, 1, 5, 8, 420)
    end
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700175, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700176, 1, -1, 8, 420)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030120, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030122, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010035, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020010, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530174, 1, -1, 8, 420)
end
function I4530173_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 19) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4530173, 1, "I4530173_GiftBag")
aux.RegisterItemEvent(4530173, 0, "I4530173_CanUseGiftBag")


--30级弑灭卡
function I4530175_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8160012, 1, 5, 8, 420)
    elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8170012, 1, 5, 8, 420)
    end
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700175, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700176, 1, -1, 8, 420)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030120, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030122, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040056, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040021, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010035, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020010, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530176, 1, -1, 8, 420)
end
function I4530175_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 20) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4530175, 1, "I4530175_GiftBag")
aux.RegisterItemEvent(4530175, 0, "I4530175_CanUseGiftBag")


--60级烈焰卡
function I4530172_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050017, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530180, 1, -1, 8, 420)
end
function I4530172_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4530172, 1, "I4530172_GiftBag")
aux.RegisterItemEvent(4530172, 0, "I4530172_CanUseGiftBag")


--60级焚煞卡
function I4530174_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650001, 1, -1, 8, 420)
	elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650101, 1, -1, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650201, 1, -1, 8, 420)
	elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650301, 1, -1, 8, 420)
    end
	role.AddRoleItem(MapID, InstanceID, TargetID, 8220001, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, 200, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 60, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020069, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300007, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090020, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090019, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040058, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070022, 1, -1, 8, 420)
end
function I4530174_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 19) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4530174, 1, "I4530174_GiftBag")
aux.RegisterItemEvent(4530174, 0, "I4530174_CanUseGiftBag")


--60级弑灭卡
function I4530176_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	if class==2 or class==6 or class==7 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650001, 1, -1, 8, 420)
	elseif class==3 or class==8 or class==9 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650101, 1, -1, 8, 420)
    elseif class==4 or class==10 or class==11 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650201, 1, -1, 8, 420)
	elseif class==5 or class==12 or class==13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4650301, 1, -1, 8, 420)
    end
	role.AddRoleItem(MapID, InstanceID, TargetID, 8220001, 1, 5, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820028, 300, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 80, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020069, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020080, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020081, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 8, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090020, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090019, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040058, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040023, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4221203, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211704, 1, -1, 8, 420)
end
function I4530176_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 24) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4530176, 1, "I4530176_GiftBag")
aux.RegisterItemEvent(4530176, 0, "I4530176_CanUseGiftBag")




--65级烈焰卡
function I4530180_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050017, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530181, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020010, 5, -1, 8, 420)
end
function I4530180_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4530180, 1, "I4530180_GiftBag")
aux.RegisterItemEvent(4530180, 0, "I4530180_CanUseGiftBag")




--70级烈焰卡
function I4530181_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020010, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020069, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530110, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090020, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090019, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090021, 1, -1, 8, 420)
end
function I4530181_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 14) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4530181, 1, "I4530181_GiftBag")
aux.RegisterItemEvent(4530181, 0, "I4530181_CanUseGiftBag")
