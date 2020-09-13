--
function I4820088_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local tbal = {}
	tbal[1] = {8010014,8020014,8030014,8040014,8050014,8051014,8060014,8061014,8070014,8071014,8080014,8081014,8090014,8091014,8100014,8110014,8111014,8121014,8122014,8130014}
    tbal[2] = {8010015,8020015,8030015,8040015,8050015,8051015,8060015,8061015,8070015,8071015,8080015,8081015,8090015,8091015,8100015,8110015,8111015,8121015,8122015,8130015}
    tbal[3] = {8010016,8020016,8030016,8040016,8050016,8051016,8060016,8061016,8070016,8071016,8080016,8081016,8090016,8091016,8100016,8110016,8111016,8121016,8122016,8130016}
    tbal[4] = {8010017,8020017,8030017,8040017,8050017,8051017,8060017,8061017,8070017,8071017,8080017,8081017,8090017,8091017,8100017,8110017,8111017,8121017,8122017,8130017}

	if TypeID - 4820087 == 1 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, tbal[1][math.random(#tbal[1])], 1, 3, 8, 420)
	elseif TypeID - 4820087 == 2 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, tbal[2][math.random(#tbal[2])], 1, 3, 8, 420)
	elseif TypeID - 4820087 == 3 then
	    if math.random(100) ==57 then
	        role.AddRoleItem(MapID, InstanceID, TargetID, tbal[4][math.random(#tbal[4])], 1, 4, 8, 420)
	    else
	        role.AddRoleItem(MapID, InstanceID, TargetID, tbal[3][math.random(#tbal[3])], 1, 3, 8, 420)
	    end
	end
end


function I4820088_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4820088, 1, "I4820088_GiftBag")
aux.RegisterItemEvent(4820088, 0, "I4820088_CanUseGiftBag")
aux.RegisterItemEvent(4820089, 1, "I4820088_GiftBag")
aux.RegisterItemEvent(4820089, 0, "I4820088_CanUseGiftBag")
aux.RegisterItemEvent(4820090, 1, "I4820088_GiftBag")
aux.RegisterItemEvent(4820090, 0, "I4820088_CanUseGiftBag")
