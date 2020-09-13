
--ĞÂÄêÇ®´ü
function I4800085_OnUse(MapID, InstanceID, TypeID, TargetID)
	local k = math.random(100)
	local gold = 0
    if k <=85 then
	    gold = 10000
	elseif k <= 99 then
	    gold = 100000
	elseif math.random(100) <= 99 then
	    gold = 1000000
    else
	    gold = 10000000
    end

	role.AddRoleSilver(MapID, InstanceID, TargetID, gold, 101)

end


aux.RegisterItemEvent(4800085, 1, "I4800085_OnUse")



