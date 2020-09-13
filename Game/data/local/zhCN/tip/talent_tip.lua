local sex_table = {"男性","女性"}
local type_table = {"普通攻击","剑技能","刀技能","神兵技能","玄翎技能","乾坤技能","阴柔技能","医毒技能","仙音技能","战技技能","奇术技能",
							"生产技能","公会技能","情侣技能","夫妻技能","兄弟技能","师徒技能","怪物技能","怪物技能","","","","","宠物技能"}
local pos_table = {"身前","身后"}
local dmg_table = {"徒手","出血","玄翎","神兵","毒性","仙音","乾坤","神性"}
local att_table =
{
	[0]="体力<color=0xff05ff00>#<color=0xffffffff>",
	[1]="武力<color=0xff05ff00>#<color=0xffffffff>",
	[2]="真元<color=0xff05ff00>#<color=0xffffffff>",
	[3]="法力<color=0xff05ff00>#<color=0xffffffff>",
	[4]="技力<color=0xff05ff00>#<color=0xffffffff>",
	[5]="身法<color=0xff05ff00>#<color=0xffffffff>"
}

local function GetValueDisplay(type,value)

			local ret
			if type == 28 or type == 29 or type == 30 or type == 31 or type == 36 or type == 54 or type == 55 or type == 56 or type == 58 or type == 59 or type == 67 or type == 68 or type == 69 then
					ret = value > 0 and (" + " .. value/100 .. "%% ") or (" - " .. (0 - value)/100 .. "%% ")
			else
					ret =  value > 0 and (" + " .. value .. " ") or (" - " .. 0 - value .. " ")
			end
			return ret
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

	local ret = "<font=龙字体,0,16,20><color=0xfffff717>"

	--提示
	local maxlevel = Tooltip.GetSkillMaxLevel(proto)
	local currlevel = Tooltip.GetSkillLevel(proto)
	if currlevel < maxlevel then
			ret = ret .. "点击可升级技能\\n"
	end
	--名称
	ret = ret .. "<font=龙字体,0,20,20><color=0xffffffff>"
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
		ret = ret .. "\\n" .. type_table[new]
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
	--空行
	ret = ret .. "\\n    "


	--前置技能
	local pre,level,bhave = Tooltip.GetSkillPreSkill(obj,proto,0)
	if pre and level then
			ret = ret .. "\\n升级前提 "
			if not bhave then
					ret = ret .. "\\n<color=0xffff402d>" .. pre .. "[" .. level .. "级]（未学会）" .. "<color=0xffffffff>"
			else
					ret = ret .. "\\n<color=0xff05ff00>" .. pre .. "[" .. level .. "级]" .. "<color=0xffffffff>"
			end
			for i=1, 9 do
				local pre2,level2,bhave2 = Tooltip.GetSkillPreSkill(obj,proto,i)
				if pre2 and level2 then
					if not bhave2 then
							ret = ret .. "\\n<color=0xffff402d>" .. pre2 .. "[" .. level2 .. "级]（未学会）" .. "<color=0xffffffff>"
					else
							ret = ret .. "\\n<color=0xff05ff00>" .. pre2 .. "[" .. level2 .. "级]" .. "<color=0xffffffff>"
					end
				end
			end
	end

	--等级限制
	local lvlimit = Tooltip.GetSkillLevelLimit(obj,proto)
	if lvlimit and lvlimit > 1 then
			ret = ret .. "\\n学习等级 "

			playerlv = Tooltip.GetLocalPlayerLevel()
			if playerlv < lvlimit then
					ret = ret .. "<color=0xffff402d>" .. lvlimit .. " <color=0xffffffff>级"
			else
					ret = ret .. "<color=0xff05ff00>" .. lvlimit .. " <color=0xffffffff>级"
			end
	end

	--天资限制
	local tal,et = Tooltip.GetSkillTalentLimit(obj,proto)
	if tal and tal > 0 and et then
			pt = Tooltip.GetLocalPlayerTalentBidden(et)
			ret = ret .. "\\n需要该系已投 "
			if tal > pt then
					ret = ret .. "<color=0xffff402d>" .. tal .. " <color=0xffffffff>点技能"
			else
					ret = ret .. "<color=0xff05ff00>" .. tal .. " <color=0xffffffff>点技能"
			end
	ret = ret .. "\\n    "
	end



	if use == 1 then
			--伤害类型
			local type = Tooltip.GetSkillDamageType(proto)
			if type > 0 and type < 9 then
					ret = ret .. "\\n<color=0xffffbc00>" .. dmg_table[type] .. "<color=0xffffffff>伤害"
			end
	end

	if use == 1 then
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

			ret = ret .. "\\n    "
	end

	--属性加成
 	local att = Tooltip.GetSkillAttribute(proto)
	if att ~= nil then
			for k, v in pairs(att) do
					if att_table[k] then
							stratt = string.gsub(att_table[k], "#", GetValueDisplay(k, v))
							ret = ret .. "\\n" .. stratt
					end
			end
	end

	if use == 2 then
			--ret = ret .. "\\n  "
	end

	--描述
	ret = ret .. "\\n" .. Tooltip.GetSkillDesc(proto)

	--下一级
	local next = Tooltip.GetSkillNextLevel(proto)
	if next ~= "" then
			ret = ret .. "\\n  \\n下一级\\n" .. next
	end


	return ret
end
