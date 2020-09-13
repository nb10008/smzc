local sex_table = {"男性","女性"}
local type_table = {"普通攻击","剑技能","刀技能","神兵技能","玄翎技能","乾坤技能","阴柔技能","医毒技能","仙音技能","战技技能","奇术技能",
							"生产技能","公会技能","情侣技能","夫妻技能","兄弟技能","师徒技能","怪物技能","怪物技能","","","","","宠物技能"}
local pos_table = {"身前","身后"}
local dmg_table = {"徒手","出血","玄翎","神兵","毒性","仙音","乾坤","绝技"}
local cost_table = {[0]="体力","真气","怒气","耐力","活力"}

local function GetValueDisplay(value)

		if value < 0 then
				value = 0 - value
		end

		if value > 100000 then
					return (value - 100000)/100 .. "%%"
		else
					return value
		end
		return value
end

--取整
local function GetInteger(x)
		return x-x%1
end

--取小数点后几位
local function GetFloat(x,n)
		return x-x%n
end

function GetObjString(obj,proto)
	local ret = "<font=龙字体,0,20,20><color=0xffffffff>"
	local active = Tooltip.GetPulseActiveState(proto)
	
	--判断是否激活，然后显示名称
	if active >0 then
		ret = ret .. "<color=0xff05ff00>" .. Tooltip.GetPulseName(proto) .. "<color=0xffffffff>"
		else
		ret = ret .. Tooltip.GetPulseName(proto) .. "<color=0xffff0000>" .. "未开启" .. "<color=0xffffffff>"
	end
	
	--显示当前等级和最高等级
	local ncurlevel,maxlevel = Tooltip.GetPulseLevel(obj,proto)
	if active > 0 and ncurlevel ~= nil and maxlevel > 0 then
		ret = ret .. "<color=0xff05ff00>" .. ncurlevel .. "/" .. maxlevel .. "<color=0xffffffff>"
	end
		
	ret = ret .. "<font=龙字体,0,16,20>"
	--等级限制
	local nlevel = Tooltip.GetPulseLevelLimit(proto)
	if nlevel ~= nil and  nlevel > 0 then
		local playerlv = Tooltip.GetLocalPlayerLevel()
		if playerlv < nlevel then
			ret = ret .. "\\n \\n" .. "需要等级<color=0xffff402d> " .. nlevel .. "<color=0xffffffff>"
		else
			ret = ret .. "\\n \\n" .. "需要等级<color=0xff05ff00> " .. nlevel .. "<color=0xffffffff>"
		end
	end

	
	--神格神格
	local nnlevel = Tooltip.GetPulseGodLevelLimit(proto)
	if nnlevel ~= nil and nnlevel > 0 then
		local playerlv = Tooltip.GetLocalPlayerGodHead()
		if playerlv < nnlevel then
			ret = ret .. "\\n" .. "需要神格<color=0xffff402d> " .. nnlevel .. "<color=0xffffffff>"
		else
			ret = ret .. "\\n" .. "需要神格<color=0xff05ff00> " .. nnlevel .. "<color=0xffffffff>"
		end
	end
		

	--描述
	ret = ret .. "\\n \\n<color=0xff05ff00>当前能力说明：<color=0xffffffff>\\n" .. Tooltip.GetPulseDesc(proto)

	--获得方式
	if nil ~= Tooltip.GetPulseNextDesc(proto) then
		ret = ret .. "\\n \\n<color=0xff05ff00>下一级能力说明：<color=0xffffffff>\\n" .. Tooltip.GetPulseNextDesc(proto)
	end
	
	return ret
end



