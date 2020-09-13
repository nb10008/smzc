
function GetObjString(obj,proto)
	--主动或者被动
	local ret = "<font=龙字体,0,20,20>"

	--名称
	ret = ret .. Tooltip.GetSkillName(proto)

	--状态
	ret = ret .. "<font=龙字体,0,16,20>"
	local bA = Tooltip.GetSkillIsLearn(proto)
	if bA == true then
			ret = ret .. "<color=0xff00ff00>   (已激活)<color=0xffffffff>"
	elseif bA == false then
			ret = ret .. "<color=0xffff0000>   (未激活)<color=0xffffffff>"
	end
			
	ret = ret .. "\\n<font=龙字体,0,16,20>"
	
	--主动，被动
	local use = Tooltip.GetSkillUseType(proto)
	if use == 1 then
			ret = ret .. "\\n<color=0xff05ff00>主动技能<color=0xffffffff>"
	elseif use == 2 then
			ret = ret .. "\\n<color=0xff00a2ff>被动技能<color=0xffffffff>"
	end
	
	--是否需要前置技能
	ret = ret .. "\\n" ..Tooltip.GetGESkillNeedSkillName(proto)

	--描述
	ret = ret .. "\\n<color=0xffffffff>" ..Tooltip.GetSkillDesc(proto)

	return ret
end
