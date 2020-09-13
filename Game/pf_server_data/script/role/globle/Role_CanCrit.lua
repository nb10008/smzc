--用来衰减当玩家拥有星辰暴乱buff时的暴击率
function role_CanCrit(MapID, InstanceID, RoleID, TargetID)	
    if unit.IsPlayer(TargetID) then
        if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 10706) then
            local a,b = role.GetBuffLayer(RoleID, 1071901) 
            local c = math.random(1,100)
            local d = role.GetRoleAttValue(MapID, InstanceID, RoleID, 57) 
            local e = role.GetRoleAttValue(MapID, InstanceID, RoleID, 16)
            local f = role.GetRoleAttValue(MapID, InstanceID, TargetID, 17)
            local g = role.GetRoleAttValue(MapID, InstanceID, TargetID, 110)
            local crit = math.pow(e/150000, 0.5)*(1-(f-e)/5000)+(d-g)/1000 --计算暴击
            crit = (crit - 0.1*a)*100 --每层buff降10%暴击
            if c <= crit then
                return true
            else
                return false
            end
        else
            return true
        end
    else
        return true
    end
	
end
aux.RegisterRoleEvent(69, "role_CanCrit")

