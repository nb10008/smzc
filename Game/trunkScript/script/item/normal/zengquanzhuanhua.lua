function I4820234_Use(MapID, InstanceID, TypeID, TargetID)
	local num5000 = role.GetRoleItemNum(TargetID, 3070085)   --5000µãÔùÈ¯ÅĞ¶Ï
	if(num5000 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 5000*num5000)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3070085, num5000, 101)
	end

	local num3000 = role.GetRoleItemNum(TargetID, 3070084)	 --3000µãÔùÈ¯ÅĞ¶Ï
	if(num3000 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 3000*num3000)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3070084, num3000, 101)
	end

	local num2000 = role.GetRoleItemNum(TargetID, 3070083)	 --2000µãÔùÈ¯ÅĞ¶Ï
	if(num2000 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 2000*num2000)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3070083, num2000, 101)
	end

	local num1500 = role.GetRoleItemNum(TargetID, 3070082)  --1500µãÔùÈ¯ÅĞ¶Ï
	if(num1500 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 1500*num1500)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3070082, num1500, 101)
	end

	local num700 = role.GetRoleItemNum(TargetID, 3070081)  --700µãÔùÈ¯ÅĞ¶Ï
	if(num700 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 700*num700)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3070081, num700, 101)
	end

	local num500 = role.GetRoleItemNum(TargetID, 3070080)  --500µãÔùÈ¯ÅĞ¶Ï
	if(num500 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 500*num500)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3070080, num500, 101)
	end

	local num1000 = role.GetRoleItemNum(TargetID, 3090030)  --1000µãÔùÈ¯ÅĞ¶Ï
	if(num1000 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 1000*num1000)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3090030, num1000, 101)
	end

	local num100 = role.GetRoleItemNum(TargetID, 3090029)  --100µãÔùÈ¯ÅĞ¶Ï
	if(num100 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 100*num100)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3090029, num100, 101)
	end

	local num80 = role.GetRoleItemNum(TargetID, 3090101)  --80µãÔùÈ¯ÅĞ¶Ï
	if(num80 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 80*num80)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3090101, num80, 101)
	end

	local num10 = role.GetRoleItemNum(TargetID, 3090105)  --10µãÔùÈ¯ÅĞ¶Ï
	if(num10 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 10*num10)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3090105, num10, 101)
	end

	local num5 = role.GetRoleItemNum(TargetID, 3090104)  --5µãÔùÈ¯ÅĞ¶Ï
	if(num5 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 5*num5)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3090104, num5, 101)
	end

	local num1 = role.GetRoleItemNum(TargetID, 3090103)  --5µãÔùÈ¯ÅĞ¶Ï
	if(num1 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 1*num1)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3090103, num1, 101)
	end

	local num300 = role.GetRoleItemNum(TargetID, 3090002)  --300µãÔùÈ¯ÅĞ¶Ï
	if(num300 >= 1) then
		role.AddRoleGiftPoints(MapID, InstanceID, TargetID, 300*num300)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 3090002, num300, 101)
	end

end

function I4820234_CanUse(MapID, InstanceID, TypeID, TargetID)
	local num5000 = role.GetRoleItemNum(TargetID, 3070085)   --5000µãÔùÈ¯ÅĞ¶Ï
	local num3000 = role.GetRoleItemNum(TargetID, 3070084)	 --3000µãÔùÈ¯ÅĞ¶Ï
	local num2000 = role.GetRoleItemNum(TargetID, 3070083)	 --2000µãÔùÈ¯ÅĞ¶Ï
	local num1500 = role.GetRoleItemNum(TargetID, 3070082)  --1500µãÔùÈ¯ÅĞ¶Ï
	local num700 = role.GetRoleItemNum(TargetID, 3070081)  --700µãÔùÈ¯ÅĞ¶Ï
	local num500 = role.GetRoleItemNum(TargetID, 3070080)  --500µãÔùÈ¯ÅĞ¶Ï
	local num1000 = role.GetRoleItemNum(TargetID, 3090030)  --1000µãÔùÈ¯ÅĞ¶Ï
	local num100 = role.GetRoleItemNum(TargetID, 3090029)  --100µãÔùÈ¯ÅĞ¶Ï
	local num80 = role.GetRoleItemNum(TargetID, 3090101)  --80µãÔùÈ¯ÅĞ¶Ï
	local num10 = role.GetRoleItemNum(TargetID, 3090105)  --10µãÔùÈ¯ÅĞ¶Ï
	local num5 = role.GetRoleItemNum(TargetID, 3090104)  --5µãÔùÈ¯ÅĞ¶Ï
	local num1 = role.GetRoleItemNum(TargetID, 3090103)  --5µãÔùÈ¯ÅĞ¶Ï
	local num300 = role.GetRoleItemNum(TargetID, 3090002)  --300µãÔùÈ¯ÅĞ¶Ï

	local sum = num5000 + num3000 + num2000 + num1500 + num700 + num500
				+ num1000 + num100 + num80 + num10 + num5 + num1 + num300
	if sum ==nil or sum <= 0 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4820234, 1, "I4820234_Use")
aux.RegisterItemEvent(4820234, 0, "I4820234_CanUse")
