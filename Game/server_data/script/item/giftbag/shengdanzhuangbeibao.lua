--Ê¥µ®Àñ°ü
function I4800243_OnUse(MapID, InstanceID, TypeID, TargetID)

	local class = role.GetRoleClass(TargetID)
	local high,low = -1,-1
	if class==2 or class==6 or class==7 then
        high,low = role.AddRoleItem(MapID, InstanceID, TargetID, 8210207, 1, 6, 8, 420)
	elseif class==4 or class==10 or class==11 then
        high,low = role.AddRoleItem(MapID, InstanceID, TargetID, 8213207, 1, 6, 8, 420)
	elseif class==3  or class==8 or class==9 then
        high,low = role.AddRoleItem(MapID, InstanceID, TargetID, 8211207, 1, 6, 8, 420)
	elseif class==5 or class==12 or class==13 then
        high,low = role.AddRoleItem(MapID, InstanceID, TargetID, 8212207, 1, 6, 8, 420)
	end
	if high ~= -1 then
		for i=1,5 do
			role.DigHoleOnEquip(TargetID,high,low)
		end
	end
    role.SetRoleScriptData(TargetID, 1, RoleDataType["SDWI_State"], 0)
	role.SetRoleScriptData(TargetID, 1, RoleDataType["SDWI_Times"], 0)
end

function I4800243_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	local class = role.GetRoleClass(TargetID)
	if class==1 then
	    bRet = 32
	end
	if(FreeSize < 1) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4800243, 1, "I4800243_OnUse")
aux.RegisterItemEvent(4800243, 0, "I4800243_CanUse")
