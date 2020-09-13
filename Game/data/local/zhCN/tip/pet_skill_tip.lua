function GetObjString(obj, proto)

	local ret = "<font=龙字体,0,20,20><color=0xffffffff>"
	--名称
	ret = ret .. Tooltip.GetPetSkillName(proto)

	--等级
	local level = Tooltip.GetPetSkillLevel(proto)
	if level > 1 then
		ret = ret .. "[<color=0xff05ff00>" .. level .. "级<color=0xffffffff>]"
	
	ret = ret .. "<font=龙字体,0,16,20>"
	--类型
	ret = ret .. "\\n" .. Tooltip.GetPetSkillType(proto)

	--描述
	ret = ret .. "\\n" .. Tooltip.GetPetSkillDesc(proto)

	end

	return ret
end


	