local spec_cost_table = {
		[2] 	= "<pic=data\\ui\\bag\\l_icon-bao-s.bmp,0,0,14,14>",
		[10] 	= "公会贡献",
		[11] 	= "公会功勋",
		[13] 	= "武技",
		[40] 	= "物品兑换",
		[41] 	= "神魂",
		[42] 	= "魔魂",
		[120]	=	"轩辕",
		[121]	=	"神农",
		[122]	=	"伏羲",
		[123]	=	"三苗",
		[124]	=	"九黎",
		[125]	=	"月氏",
		[126]	=	"女娲",
		[127]	=	"共工"
}
local repute_level_table = {
		[0]		= "仇视",
		[1]		= "嫌恶",
		[2]		= "冷漠",
		[3]		= "中立",
		[4]		= "友善",
		[5]		= "亲密",
		[6]		= "信赖",
		[7]		= "尊敬",
		[8]		= "崇拜",
		[9]		= "传奇"
}

local repute_level_table_hunjing = {
		[1]		= "友善",
		[2]		= "尊敬",
		[3]		= "崇敬",
		[4]		= "崇拜",
}

local wuxun_level_table = {
		[1]		= "0",
		[2]		= "200",
		[3]		= "400",
		[4]		= "500",
		[5]		= "600",
		[6]		= "700",
		[7]		= "800",
		[8]		= "900",
		[9]		= "1000",
		[10]	= "1100"
}


function GetObjString(obj,single,known)

	local ret = "<font=龙字体,0,16,20><color=0xffffffff>"

	local istax = Tooltip.IsTax(obj)

	if (istax == true) then
		if single == 1 then
			ret = ret .. "基础单价  "
		else
			ret = ret .. "基础价格  "
		end
	else
		if single == 1 then
			ret = ret .. "购买单价  "
		else
			ret = ret .. "出售价格  "
		end
	end

	--显示普通价格
	local gold,silver = Tooltip.GetPriceSilver(obj)
	if gold ~= nil or silver ~= nil then
			if gold > 0 then
					ret = ret .. gold .. " <pic=data\\ui\\bag\\l_icon-jin-s.bmp,0,0,16,16>"
			end
			if silver > 0 then
					ret = ret .. silver .. " <pic=data\\ui\\bag\\l_icon-yin-s.bmp,0,0,16,16>"
			end
			--如果价格小于0则直接显示无法出售
			if gold < 0 or silver < 0 then
					ret = "<font=龙字体,0,16,20><color=0xffff402d>无法出售<color=0xffffffff>"
			end
			--价格为零则需要判断是否显示
			if gold == 0 and silver == 0 then
					if single ~= 1 then
							ret = ret .. "0 <pic=data\\ui\\bag\\l_icon-yin-s.bmp,0,0,16,16>"
					end
			end
	end

	if (istax == true) then
		ret = ret .. "\\n税收  "
		local taxgold, taxsilver = Tooltip.GetPriceSilverByTax(obj)
		if taxgold ~= nil or taxsilver ~= nil then
			if taxgold > 0 then
				ret = ret .. taxgold .. " <pic=data\\ui\\bag\\l_icon-jin-s.bmp,0,0,16,16>"
			end
			if taxsilver >= 0 then
				ret = ret .. taxsilver .. " <pic=data\\ui\\bag\\l_icon-yin-s.bmp,0,0,16,16>"
			end
		end
		ret = ret .. "\\n <font=龙字体,0,16,20><color=0xff05ff00>（税收部分将自动缴纳至本城的统治帮派）"
	end


	--显示特殊消耗
	local etype,number,level,item = Tooltip.GetPriceSpecCost(obj)

	if etype ~= nil then
			--如果特殊消耗是声望
			if etype >= 120 and etype <= 127 then
						if level > 0 then
								local cost_level = repute_level_table[level]
								if cost_level ~= nil then
										ret = ret .. "\\n需要  " .. spec_cost_table[etype] .. "氏族声望——" .. cost_level
								end
						end
						if number > 0 then
								if gold ~= 0 or silver ~= 0 then
										ret = ret .. "\\n      "
								end
								ret = ret .. number .. spec_cost_table[etype] .. "氏族贡献"
						end
			--如果是物品兑换
			elseif etype == 40 then
					if item ~= nil and number > 0 then
							ret = ret .. "\\n需要  " .. item .. "×" .. number
					end
			elseif etype == 13 then
					if level > 1 then
						ret = ret .. "\\n需要"..wuxun_level_table[level].."点以上荣誉才能购买"
					end
					if number > 0 then
						if gold ~= 0 or silver ~= 0 then
										ret = ret .. "\\n"
						end
						ret = ret .. "需要斗技点数  "..number .. "点"
					end
			elseif etype == 41 then
					if level > 0 then
						ret = ret .. "\\n需要天神声望 "..repute_level_table_hunjing[level]
					end
					if number > 0 then
						ret = ret .. "\\n需要神魂  " ..number
					end
			elseif etype == 42 then
					if level > 0 then
						ret = ret .. "\\n需要魔族声望 "..repute_level_table_hunjing[level]
					end
					if number > 0 then
						ret = ret .. "\\n需要魔魂  " ..number
					end
			elseif etype == 43 then
					if level > 0 then
						ret = ret .. "\\n需要<color=0xff00ffff> ".. number .." <color=0xffffffff>战场金币" 
					end
			else
					local spec_name = spec_cost_table[etype]
					if number > 0 and spec_name ~= nil then
							if gold ~= 0 or silver ~= 0 then
										ret = ret .. "\\n      "
							end
							ret = ret .. "\\n需要  " .. number .. spec_name
					end
			end
	end

	return ret
end

