Family_zhaoji = {}

function I4800127_Use(MapID, InstanceID, TypeID, TargetID)
	local FamilyID = role.GetRoleFamilyID(MapID, InstanceID,TargetID)
	if FamilyID ~= nil then
	    local MemberID = {}
	    MemberID[1],MemberID[2],MemberID[3],MemberID[4],MemberID[5],MemberID[6],MemberID[7],MemberID[8],MemberID[9],MemberID[10],MemberID[11],MemberID[12] = family.GetFamilyAllMemberID(FamilyID)
	    for k,v in pairs(MemberID) do
		    if v ~= TargetID then
			    local Map, Instance = role.GetRoleMapID(v)
				if Map ~= nil then
					if not role.IsRoleHaveBuff(Map, Instance, v, 60016) and not unit.IsDead(Map, Instance, v) then
					    msg.SendRoleSwitchMsg(2, Map, Instance, v, 1, 13)
						local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
						Family_zhaoji[v] = {MapID, InstanceID, x, y ,z, 2}
					end
				end
			end
		end
	end
end

function I4800127_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FamilyID = role.GetRoleFamilyID(MapID, InstanceID,TargetID)
	if FamilyID == nil then
		--提示玩家背包空间不足
		bRet = 32
	end
	local bool = false
	if MapID ~= nil then
		for k,v in pairs(Skill_NormalCRC) do
			if MapID == v then
				bool = true
				break
			end
		end
	end
	if not bool then
		bRet = 32
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800127, 1, "I4800127_Use")
aux.RegisterItemEvent(4800127, 0, "I4800127_CanUse")


function Family_OnClick(MapID, InstanceID, RoleID, TOMsgID, ToMapID, x, y, z)

	if Family_zhaoji[RoleID] ~= nil and not unit.IsDead(MapID, InstanceID, RoleID) then

		if role.IsRoleInStatus(RoleID, 1024) then
			role.CancelRoleStallState(MapID, InstanceID, RoleID)
		end
		unit.AddBuff(MapID, InstanceID, RoleID, 6001601, RoleID)
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, Family_zhaoji[RoleID][1], Family_zhaoji[RoleID][3], Family_zhaoji[RoleID][4], Family_zhaoji[RoleID][5])
	    Family_zhaoji[RoleID] = nil

	end

end

aux.RegisterActEvent(13, 8, "Family_OnClick")


function I4800126_Use(MapID, InstanceID, TypeID, TargetID)
	local FamilyID = role.GetRoleFamilyID(MapID, InstanceID,TargetID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if FamilyID ~= nil then
	    local MemberID = {}
	    MemberID[1],MemberID[2],MemberID[3],MemberID[4],MemberID[5],MemberID[6],MemberID[7],MemberID[8],MemberID[9],MemberID[10],MemberID[11],MemberID[12] = family.GetFamilyAllMemberID(FamilyID)
	    for k,v in pairs(MemberID) do
			local Map, Instance = role.GetRoleMapID(v)
			if Map ~= nil and MapID == Map then
				local x1, y1 ,z1 = unit.GetPosition(Map, Instance, v)
				if (x1-x)^2+(z1-z)^2 <= 22500 then
					local BUFF = {}
					BUFF[0] = 2000101
					BUFF[10] = 2000101
					BUFF[20] = 2000201
					BUFF[30] = 2000201
					BUFF[40] = 2000301
					BUFF[50] = 2000301
					BUFF[60] = 2000401
					BUFF[70] = 2000501
					BUFF[80] = 2000601
					BUFF[90] = 2000701
					BUFF[100] = 2000801
					local rolelevel = math.floor(role.GetRoleLevel(Map, Instance, v)/10)*10
					if rolelevel > 100 then
						rolelevel = 100
					end
					unit.AddBuff(Map, Instance, v, BUFF[rolelevel], v)
			    end
			end
		end
	end
end

function I4800126_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FamilyID = role.GetRoleFamilyID(MapID, InstanceID,TargetID)
	if FamilyID == nil then
		--提示玩家背包空间不足
		bRet = 32
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800126, 1, "I4800126_Use")
aux.RegisterItemEvent(4800126, 0, "I4800126_CanUse")
