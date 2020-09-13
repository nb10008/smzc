local sex_table = {"男性","女性"}
local type_table = {"普通攻击","剑技能","刀技能","神兵技能","玄翎技能","乾坤技能","阴柔技能","医毒技能","仙音技能","战技技能","奇术技能",
							"生产技能","公会技能","情侣技能","夫妻技能","兄弟技能","师徒技能","怪物技能","怪物技能","","","","","宠物技能"}
local pos_table = {"身前","身后"}
local dmg_table = {"徒手","出血","玄翎","神兵","毒性","仙音","乾坤","神性"}
local cost_table = {[0]="体力","真气","怒气","神力","活力"}

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
	--主动或者被动
	local use = Tooltip.GetSkillUseType(proto)
	local ret = "<font=龙字体,0,16,20>"
	--提示
	if use == 1 then
	    local bool = false
	    for i=0,4 do
			local new = Tooltip.GetSkillCost(obj,proto,i)
			if new > 0 and i == 3 then
				bool = true
			end
		end
		if bool then
		    ret = ret .. "<color=0xffff0000>不可拖至快捷栏\\n<font=龙字体,0,20,20><color=0xffffffff>"
		else
			ret = ret .. "<color=0xfffff717>可拖至快捷栏\\n<font=龙字体,0,20,20><color=0xffffffff>"
		end
	end

	--技能消耗
	if use == 1 then

	end

	--名称
	ret = ret .. Tooltip.GetSkillName(proto)

	--技能等级
	local level = Tooltip.GetSkillLevel(proto)
	local maxlevel = Tooltip.GetSkillMaxLevel(proto)
	if maxlevel > 1 then
		ret = ret .. "[<color=0xff05ff00>" .. level .. "级<color=0xffffffff>]"
	end

	ret = ret .. "<font=龙字体,0,16,20>"
	--类型
	local new = Tooltip.GetSkillType(proto)
	if new > 1 and new < 25 then
		ret = ret .. "\\n<color=0xff05ff00>" .. type_table[new] .."<color=0xffffffff>"
	elseif new == 1 then
		return ret
	end

	--主动，被动
	local use = Tooltip.GetSkillUseType(proto)
	if use == 1 then
			ret = ret .. "\\n<color=0xff05ff00>主动<color=0xffffffff>"
	elseif use == 2 then
			ret = ret .. "\\n<color=0xff00a2ff>被动<color=0xffffffff>"
	end

	--主动技能才显示
	if use == 1 then
		--性别限制
		local sex = Tooltip.GetSkillSexLimit(proto)
		if sex == 1 or sex == 2 then
			local lpsex = Tooltip.GetLocalPlayerSex()
			if (lpsex + sex) == 2 then
				ret = ret .. "\\n<color=0xff05ff00>" .. sex_table[sex] .. "<color=0xffffffff>限制"
			else
				ret = ret .. "\\n<color=0xffff402d>" .. sex_table[sex] .. "<color=0xffffffff>限制"
			end
		end


		--身前，身后
		local new = Tooltip.GetSkillPosLimit(proto)
		if new > 0 and new < 3 then
				ret = ret .. "\\n目标<color=0xff05ff00>" .. pos_table[new] .. "<color=0xffffffff>发动"
		end

		--骑战
		--bmount = Tooltip.GetSkillMountLimit(proto)
		--if bmount then
				--ret = ret .. "\\n骑战<color=0xff05ff00>可<color=0xffffffff>施放"
		--else
				--ret = ret .. "\\n骑战<color=0xff05ff00>不可<color=0xffffffff>施放"
		--end

	end

	if use == 1 then
			--伤害类型
			local type = Tooltip.GetSkillDamageType(proto)
			if type > 0 and type < 9 then
					ret = ret .. "\\n<color=0xffffbc00>" .. dmg_table[type] .. "<color=0xffffffff>伤害"
			end
	end

	if use == 1 then
		--空行
		ret = ret .. "\\n    "
			--技能伤害
			local single,sum = Tooltip.GetSkillDamage(obj,proto)
			if single ~= nil and single > 100000 then
					ret = ret .. "\\n技能威力<color=0xff00a2ff> " .. GetInteger((single-100000)/100) .. "%" .. "<color=0xffffffff>" .. "武器数值"
			elseif sum ~= nil and sum > 0 then
					ret = ret .. "\\n技能威力<color=0xff00a2ff> " .. sum .. "<color=0xffffffff>"
			end
			--伤害目标数量
			local damageNum = Tooltip.GetSkillDamageNum(proto)
			if damageNum > 1 then
					ret = ret .. "\\n目标数量<color=0xff00a2ff> " .. damageNum .. " <color=0xffffffff>个"
			end
			--施放距离
			local new = Tooltip.GetSkillOPDistance(obj,proto)
			if new > 0 then
					ret = ret .. "\\n施放距离<color=0xff00a2ff> " .. GetInteger(new/100) .. " <color=0xffffffff>米"
			end
			--起手时间
			local new = Tooltip.GetSkillPrepareTime(obj,proto)
			if new > 0 then
					ret = ret .. "\\n起手时间<color=0xff00a2ff> " .. GetFloat(new/1000,0.1) .. " <color=0xffffffff>秒"
			end
			--恢复时间
			local new = Tooltip.GetSkillCooldownTime(obj,proto)
			if new > 0 then
					ret = ret .. "\\n冷却时间<color=0xff00a2ff> " .. GetFloat(new/1000,0.1) .. " <color=0xffffffff>秒"
			end
	end

	--技能消耗
	if use == 1 then
			for i=0,4 do
					local new = Tooltip.GetSkillCost(obj,proto,i)
					if new > 0 then
						local lpcost = Tooltip.GetLocalPlayerCostAttCurValue(i)
						if lpcost >= new then
							ret = ret .. "\\n" .. cost_table[i] .. "消耗<color=0xff00a2ff> " .. GetValueDisplay(new) .. "<color=0xffffffff>"
						else
							ret = ret .. "\\n" .. cost_table[i] .. "消耗<color=0xffff402d> " .. GetValueDisplay(new) .. "（" .. cost_table[i] .. "不足）<color=0xffffffff>"
						end
					end
			end
	end

	if use == 2 then
			--ret = ret .. "\\n  "
	end

	--描述
	ret = ret .. "\\n \\n" .. Tooltip.GetSkillDesc(proto)

	--下一级
	--[[local next = Tooltip.GetSkillNextLevel(proto)
	if next ~= "" then
			ret = ret .. "\\n  \\n下一级\\n" .. next
	end]]


	return ret
end



