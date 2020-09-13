--Ä§Éñ²ÄÁÏ¿¨
function I4530112_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	local item={}
	item[1]={ItemId=4090001,ItemNum=1};
	item[2]={ItemId=4820028,ItemNum=2};
	item[3]={ItemId=4820024,ItemNum=1};
	item[4]={ItemId=4820029,ItemNum=1};
    if level < 60 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, item[1].ItemId, item[1].ItemNum, -1, 8, 420)
	elseif level >= 60 and level < 80 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, item[2].ItemId, item[2].ItemNum, -1, 8, 420)
    elseif level >= 80 and level < 90 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, item[3].ItemId, item[3].ItemNum, -1, 8, 420)
	elseif level >= 90 then
	    role.AddRoleItem(MapID, InstanceID, TargetID, item[4].ItemId, item[4].ItemNum, -1, 8, 420)
	end

end

function I4530112_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local freeSize = role.GetBagFreeSize(TargetID)
    if(freeSize < 1) then
        bRet = 40
    end

    return bRet, bIgnore
end

aux.RegisterItemEvent(4530112, 1, "I4530112_GiftBag")
aux.RegisterItemEvent(4530112, 0, "I4530112_CanUseGiftBag")
