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

function GetObjString(obj,proto)

	local ret = "<font=龙字体,0,20,20><color=0xffffffff>"
	--名称
	ret = ret .. Tooltip.GetSkillName(proto)

	ret = ret .. "<font=龙字体,0,16,20>"

	--技能等级
	local level = Tooltip.GetSkillLevel(proto)
	local maxlevel = Tooltip.GetSkillMaxLevel(proto)
	if maxlevel > 1 then
		ret = ret .. "[<color=0xff05ff00>" .. level .. "级<color=0xffffffff>]"
	end

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
	ret = ret .. "\\n"

	if use == 1 then
			--技能伤害
			local new = Tooltip.GetSkillDamage(obj,proto)
			if new > 0 then
					ret = ret .. "\\n技能威力<color=0xff00a2ff> " .. new .. "<color=0xffffffff>"
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
			ret = ret .. "\\n  "
	end

	--描述
	ret = ret .. "\\n" .. Tooltip.GetSkillDesc(proto)
	ret = ret .. "\\n\\n\\n\\n\\n\\n"
	ret = ret .. "该技能等级已达上限"
	return ret
end
