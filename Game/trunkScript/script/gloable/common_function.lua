--得到队伍总人数
function GetTeamPlayerNum(MapID,InstanceID,RoleID)
	local PlayerNum = 0
	local PlayerTeamID = role.IsRoleHaveTeam(MapID,InstanceID,RoleID)
	if PlayerTeamID == nil or PlayerTeamID == -1 or PlayerTeamID == 4294967295 then
		PlayerNum = 1
	else
		local Player = {}
		Player[1], Player[2], Player[3], Player[4], Player[5], Player[6] = role.GetRoleTeamMemID(PlayerTeamID)
		for i=1, 6 do
			if Player[i] and Player[i]~= 4294967295 then
				PlayerNum = PlayerNum + 1
			end
		end
	end
	return PlayerNum
end

-- 表的深拷贝函数
function deepcopy(object)
    local lookup_table = {}
    local function _copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for index, value in pairs(object) do
            new_table[_copy(index)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end
