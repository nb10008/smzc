--圣魔君临天下卡
function I5700001_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530202, 1, -1, 8, 420)

end

function I5700001_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(5700001, 1, "I5700001_OnUse")
aux.RegisterItemEvent(5700001, 0, "I5700001_CanUse")


--圣魔君临天下卡1级
function I4530202_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220903, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211504, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530203, 1, -1, 8, 420)
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700153, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700154, 1, -1, 8, 420)
	end

end

function I4530202_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530202, 1, "I4530202_OnUse")
aux.RegisterItemEvent(4530202, 0, "I4530202_CanUse")


--圣魔君临天下卡10级
function I4530203_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530204, 1, -1, 8, 420)

end

function I4530203_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530203, 1, "I4530203_OnUse")
aux.RegisterItemEvent(4530203, 0, "I4530203_CanUse")


--圣魔君临天下卡20级
function I4530204_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 100, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020114, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070055, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820046, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820047, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070085, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530205, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 5000000, 101)

end

function I4530204_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530204, 1, "I4530204_OnUse")
aux.RegisterItemEvent(4530204, 0, "I4530204_CanUse")


--圣魔君临天下卡30级
function I4530205_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030128, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020061, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020072, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 10, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)
	if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8160008, 1, 5, 8, 420)
	elseif class==4 or class==5 or class==10 or class==11   or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8170008, 1, 5, 8, 420)
	end

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530206, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 5000000, 101)

end

function I4530205_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530205, 1, "I4530205_OnUse")
aux.RegisterItemEvent(4530205, 0, "I4530205_CanUse")





--圣魔君临天下卡40级
function I4530206_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040058, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3040023, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030115, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020072, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 5, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530207, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 15000000, 101)

end

function I4530206_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530206, 1, "I4530206_OnUse")
aux.RegisterItemEvent(4530206, 0, "I4530206_CanUse")




--圣魔君临天下卡50级
function I4530207_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030140, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030141, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020118, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020072, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 500, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530208, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 15000000, 101)
	local class = role.GetRoleClass(TargetID)

    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8017007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101207, 1, 4, 8, 420)

        role.AddRoleItem(MapID, InstanceID, TargetID, 8110807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8121807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130807, 1, 4, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8027007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101211, 1, 4, 8, 420)

        role.AddRoleItem(MapID, InstanceID, TargetID, 8110807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8121807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130807, 1, 4, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8047007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101215, 1, 4, 8, 420)

        role.AddRoleItem(MapID, InstanceID, TargetID, 8111807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8122807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130811, 1, 4, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8037007, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091811, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8101219, 1, 4, 8, 420)

        role.AddRoleItem(MapID, InstanceID, TargetID, 8111807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8122807, 1, 4, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130811, 1, 4, 8, 420)
    end
end

function I4530207_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 17) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530207, 1, "I4530207_OnUse")
aux.RegisterItemEvent(4530207, 0, "I4530207_CanUse")



--圣魔君临天下卡60级
function I4530208_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020122, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020072, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 500, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070072, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 100, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 10, -1, 8, 420)


	role.AddRoleItem(MapID, InstanceID, TargetID, 4530209, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 20000000, 101)

end

function I4530208_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530208, 1, "I4530208_OnUse")
aux.RegisterItemEvent(4530208, 0, "I4530208_CanUse")






--圣魔君临天下卡70级
function I4530209_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020072, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 500, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070072, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 200, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700238, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700214, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530210, 1, -1, 8, 420)
	role.AddRoleSilver(MapID, InstanceID, TargetID, 50000000, 101)

end

function I4530209_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530209, 1, "I4530209_OnUse")
aux.RegisterItemEvent(4530209, 0, "I4530209_CanUse")




--圣魔君临天下卡80级
function I4530210_OnUse(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020125, 1, -1, 8, 420)
	local class = role.GetRoleClass(TargetID)

    if class==2 or class==6 or class==7 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8013009, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8100309, 1, 5, 8, 420)

        role.AddRoleItem(MapID, InstanceID, TargetID, 8110309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8121309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130309, 1, 5, 8, 420)
    elseif class==3 or class==8 or class==9 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8023009, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8050409, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8060409, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8070409, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8080409, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8090409, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8100409, 1, 5, 8, 420)

        role.AddRoleItem(MapID, InstanceID, TargetID, 8110309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8121309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130309, 1, 5, 8, 420)
    elseif class==4 or class==10 or class==11 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8043009, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8100509, 1, 5, 8, 420)

        role.AddRoleItem(MapID, InstanceID, TargetID, 8111309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8122309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130409, 1, 5, 8, 420)
    elseif class==5 or class==12 or class==13 then
        role.AddRoleItem(MapID, InstanceID, TargetID, 8033009, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8051409, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8061409, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8071409, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8081409, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8091409, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8100609, 1, 5, 8, 420)

        role.AddRoleItem(MapID, InstanceID, TargetID, 8111309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8122309, 1, 5, 8, 420)
        role.AddRoleItem(MapID, InstanceID, TargetID, 8130409, 1, 5, 8, 420)
    end
	role.AddRoleSilver(MapID, InstanceID, TargetID, 50000000, 101)

end

function I4530210_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4530210, 1, "I4530210_OnUse")
aux.RegisterItemEvent(4530210, 0, "I4530210_CanUse")
