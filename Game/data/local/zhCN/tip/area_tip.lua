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
	--主动或者被动
	local use = Tooltip.GetAreaUseType(proto)
	local ret = "<font=龙字体,0,16,20><color=0xfffff717>"
	--提示
	--if use == 1 then
	--	ret = ret .. "可拖至快捷栏\\n"
	--end
		ret = ret .. "<font=龙字体,0,20,20><color=0xffffffff>"
	--名称
	local level = Tooltip.GetAreaSkillLevel(obj)
	ret = ret .. Tooltip.GetAreaName(proto)
	if level > 0 then
		ret = ret .. "[<color=0xff05ff00>" .. level .. "级<color=0xffffffff>]"
	end
		
	ret = ret .. "<font=龙字体,0,16,20>"
	--激活等级
	local nlevel = Tooltip.GetAreaActiveLevel(proto)
	if nlevel ~= nil and  nlevel > 0 then
		local playerlv = Tooltip.GetLocalPlayerLevel()
		if playerlv < nlevel then
			ret = ret .. "\\n \\n" .. "激活等级<color=0xffff402d> " .. nlevel .. "<color=0xffffffff>"
		else
			ret = ret .. "\\n \\n" .. "激活等级<color=0xff05ff00> " .. nlevel .. "<color=0xffffffff>"
		end
	end
	
	--激活神格
	local nnlevel = Tooltip.GetAreaGodHeadLevel(proto)
	if nnlevel ~= nil and nnlevel > 0 then
		local playerlv = Tooltip.GetLocalPlayerGodHead()
		if playerlv < nnlevel then
			ret = ret .. "\\n" .. "激活神格<color=0xffff402d> " .. nnlevel .. "<color=0xffffffff>"
		else
			ret = ret .. "\\n" .. "激活神格<color=0xff05ff00> " .. nnlevel .. "<color=0xffffffff>"
		end
	end
		
	--时间限制
	local time = Tooltip.GetAreaLeftTime(proto)
	if time > 0 then
		ret = ret .."\\n" .. "存在时间<color=0xff05ff00> "
			if time/(60*60*24) > 1 then
			 ret = ret .. time/(60*60*24) .. "天<color=0xffffffff>"
			end
			if (time/(60*60))%24 > 1 then
			 ret = ret .. (time/(60*60))%60 .. "小时<color=0xffffffff>"
			end
			if (time/60)%60 > 1 then
			 ret = ret .. (time/60)%60 .. "分钟<color=0xffffffff>"
			end
			if time%60 > 1 then
			 ret = ret .. time%60 .. "秒<color=0xffffffff>"
			end
	end
	
	--状态
	local nstate = Tooltip.GetAreaState(proto)
	
	--主动，被动
	local use = Tooltip.GetAreaUseType(proto)
	if use == 1 then
			ret = ret .. "\\n<color=0xff05ff00>主动<color=0xffffffff>"
	elseif use == 2 then
			ret = ret .. "\\n<color=0xff00a2ff>被动<color=0xffffffff>"
	end
	

	--被动和状态是2的情况下才进来
	if use ~= 2 then
		if nstate ~= 2 then
			--空行
			ret = ret .. "\\n    "
			--技能伤害
			local single,sum = Tooltip.GetAreaDamage(obj,proto)
			if single ~= nil and single > 100000 then
					ret = ret .. "\\n技能威力<color=0xff00a2ff> " .. GetInteger((single-100000)/100) .. "%" .. "<color=0xffffffff>" .. "武器数值"
			elseif sum ~= nil and sum > 0 then
					ret = ret .. "\\n技能威力<color=0xff00a2ff> " .. sum .. "<color=0xffffffff>"
			end
			--伤害目标数量
			local damageNum = Tooltip.GetAreaDamageNum(proto)
			if damageNum > 1 then
					ret = ret .. "\\n目标数量<color=0xff00a2ff> " .. damageNum .. " <color=0xffffffff>个"
			end
			--施放距离
			local new = Tooltip.GetAreaOPDistance(obj,proto)
			if new > 0 then
					ret = ret .. "\\n施放距离<color=0xff00a2ff> " .. GetInteger(new/100) .. " <color=0xffffffff>米"
			end
			--恢复时间
			local new = Tooltip.GetAreaCooldownTime(obj,proto)
			if new > 0 then
					ret = ret .. "\\n冷却时间<color=0xff00a2ff> " .. GetFloat(new/1000,0.1) .. " <color=0xffffffff>秒"
			end
		end
	end

	--技能消耗
	if use == 1 then
			for i=0,4 do
					local new = Tooltip.GetAreaCost(obj,proto,i)
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
	ret = ret .. "\\n \\n" .. Tooltip.GetAreaDesc(proto)

	--获得方式
	if nil ~= Tooltip.GetAreaGetDesc(obj,proto) then
		ret = ret .. "\\n \\n下一级获得方式：\\n" .. Tooltip.GetAreaGetDesc(obj,proto)
	end
	
	return ret
end



