local sex_table = {[0]="女性","男性"}
local quality_color = {[0]="<color=0xffffffff>",[1]="<color=0xfffff717>",[2]="<color=0xff05ff00>",[3]="<color=0xff00a2ff>",[4]="<color=0xffff7800>",[5]="<color=0xff9932cd>",[6]="<color=0xffff80c0>",[255]="<color=0xff8d8d8d>"}
local atta_table = {[0]="体质",[1]="武力",[2]="真元",[3]="法力",[4]="技力",[5]="身法"}
local clan_table = {[0]="轩辕","神农","伏羲","三苗","九黎","月氏","女娲","共工"}
local reput_table = {[0]="仇视","嫌恶","冷漠","中立","友善","亲密","信赖","尊敬","崇拜","传奇"}
local ShengLingAtt = {[0]="战力","防护","必杀", "必杀效果", "狂暴", "命中", "敏锐"}
local att_table =
{
	[0]="<color=0xffff80c0>体质",
	[1]="<color=0xffff80c0>武力",
	[2]="<color=0xffff80c0>真元",
	[3]="<color=0xffff80c0>法力",
	[4]="<color=0xffff80c0>技力",
	[5]="<color=0xffff80c0>身法",
	[6]="<color=0xff00a2ff>属性点数",
	[7]="<color=0xff00a2ff>天资点数",
	[8]="<color=0xff00a2ff>体力上限",
	[9]="<color=0xff00a2ff>体力",
	[10]="<color=0xff00a2ff>真气上限",
	[11]="<color=0xff00a2ff>真气",
	[12]="<color=0xff00a2ff>物理攻击",
	[13]="<color=0xff00a2ff>物理防御",
	[14]="<color=0xff00a2ff>法术攻击",
	[15]="<color=0xff00a2ff>法术防御",
	[16]="<color=0xff00a2ff>精准",
	[17]="<color=0xff00a2ff>灵巧",
	[18]="<color=0xff00a2ff>命中",
	[19]="<color=0xff00a2ff>闪避",
	[20]="<color=0xff00a2ff>神力",
	[21]="<color=0xff00a2ff>神力上限",
	[22]="<color=0xff00a2ff>体力恢复",
	[23]="<color=0xff00a2ff>真气恢复",
	[24]="<color=0xff00a2ff>怒气",
	[25]="<color=0xff00a2ff>活力",
	[26]="<color=0xff00a2ff>活力上限",
	[27]="<color=0xff00a2ff>活力恢复",
	[28]="<color=0xff00a2ff>移动速度",
	[29]="<color=0xff00a2ff>跳跃能力",
	[30]="<color=0xff00a2ff>游泳速度",
	[31]="<color=0xff00a2ff>骑乘速度",
	[32]="<color=0xff00a2ff>最小武器伤害",
	[33]="<color=0xff00a2ff>最大武器伤害",
	[34]="<color=0xff00a2ff>法器伤害",
	[35]="<color=0xff00a2ff>护甲",
	[36]="<color=0xff00a2ff>普通伤害减免",
	[37]="<color=0xff00a2ff>出血伤害减免",
	[38]="<color=0xff00a2ff>玄翎伤害减免",
	[39]="<color=0xff00a2ff>神兵伤害减免",
	[40]="<color=0xff00a2ff>毒性伤害减免",
	[41]="<color=0xff00a2ff>仙音伤害减免",
	[42]="<color=0xff00a2ff>乾坤伤害减免",
	[43]="<color=0xff00a2ff>绝技伤害减免",
	[44]="<color=0xff00a2ff>物理伤害减免",
	[45]="<color=0xff00a2ff>法术伤害减免",
	[46]="<color=0xff00a2ff>全部伤害减免",
	[47]="<color=0xff00a2ff>额外伤害",
	[48]="<color=0xff00a2ff>伤害吸收",
	[49]="<color=0xff00a2ff>固定伤害抗性",
	[50]="<color=0xff00a2ff>衰弱抗性",
	[51]="<color=0xff00a2ff>控制效果抗性",
	[52]="<color=0xff00a2ff>减速抗性",
	[53]="<color=0xff00a2ff>灵力",
	[54]="<color=0xff00a2ff>攻击失误率",
	[55]="<color=0xff00a2ff>近战躲避",
	[56]="<color=0xff00a2ff>远程躲避",
	[57]="<color=0xff00a2ff>暴击",
	[58]="<color=0xff00a2ff>暴击量",
	[59]="<color=0xff00a2ff>格档",
	[60]="<color=0xff00a2ff>灵感",
	[61]="<color=0xff00a2ff>隐匿",
	[62]="<color=0xff00a2ff>悟性",
	[63]="<color=0xff00a2ff>士气",
	[64]="<color=0xff00a2ff>内伤",
	[65]="<color=0xff00a2ff>仇恨",
	[66]="<color=0xff00a2ff>体型",
	[67]="<color=0xffff80c0>战斗经验加成",
	[68]="<color=0xffff80c0>掉落金钱加成",
	[69]="<color=0xffff80c0>掉宝几率加成",
	[70]="<color=0xffff80c0>幸运",
	[71]="<color=0xffff80c0>气质",
	[72]="<color=0xffff80c0>统御",
	[73]="<color=0xffff80c0>阅历",
	[74]="<color=0xffff80c0>道德",
	[75]="<color=0xffff80c0>修为",
	[83]="<color=0xff00a2ff>韧性",
	[84]="<color=0xff00a2ff>神兵伤害加深",
	[85]="<color=0xff00a2ff>玄翎伤害加深",
	[86]="<color=0xff00a2ff>乾坤伤害加深",
	[87]="<color=0xff00a2ff>仙音伤害加深",
	[88]="<color=0xff00a2ff>物理伤害加深",
	[89]="<color=0xff00a2ff>法术伤害加深",
	[90]="<color=0xff00a2ff>全部伤害加深",
	[92]="<color=0xff00a2ff>元素伤害",
	[109]="<color=0xff00a2ff>法力燃烧抵抗",
	[110]="<color=0xff00a2ff>韧性",
	[111]="<color=0xff00a2ff>韧性强度",
	[112]="<color=0xff00a2ff>控制效果抵抗",
	[113]="<color=0xff00a2ff>控制效果加深",
	[114]="<color=0xff00a2ff>固定伤害强化",
	[115]="<color=0xff00a2ff>衰老效果强化",
	[116]="<color=0xff00a2ff>减速效果强化",
	[117]="<color=0xff00a2ff>物理反弹伤害",
	[118]="<color=0xff00a2ff>物理反弹比例",
	[119]="<color=0xff00a2ff>法术反弹伤害",
	[120]="<color=0xff00a2ff>法术反弹比例",
	[121]="<color=0xff00a2ff>物理反弹减免",
	[122]="<color=0xff00a2ff>法术反弹减免",
	[123]="<color=0xff00a2ff>法力燃烧",
	[124]="<color=0xff00a2ff>每日信仰",
	[125]="<color=0xff00a2ff>无视攻击",
	[126]="<color=0xff00a2ff>无视防御",
	[127]="<color=0xff00a2ff>神魂",
	[131]="<color=0xff00a2ff>元素穿透",
	[132]="<color=0xff00a2ff>元素抵抗",

}

--时间换算，单位秒
local ONE_YEAR 	= 365*24*60*60
local ONE_MONTH	= 30*24*60*60
local ONE_DAY	 	= 24*60*60
local ONE_HOUR	 	= 60*60
local ONE_MINUTE 	= 60

--取小数点后几位
local function GetFloat(x,n)
		return x-x%n
end

--取整
local function GetInteger(x)
		return x-x%1
end
--取百分比(保留两位小数)
local function GetPercent(x)
		local y = x/100
		return y-y%0.01
end
local function GetTimeDisplayLite(n)
	if n > ONE_YEAR then
			return GetInteger(n/ONE_YEAR) .. "年"
	elseif n > ONE_MONTH then
			return GetInteger(n/ONE_MONTH) .. "月"
	elseif n > ONE_DAY then
			return GetInteger(n/ONE_DAY) .. "天"
	elseif n > ONE_HOUR then
			return GetInteger(n/ONE_HOUR) .. "小时以上"
	elseif n > ONE_MINUTE then
			return GetInteger(n/ONE_MINUTE) .. "分钟以上"
	else
			return "小于1分钟"
	end
end

function GetObjString(proto,quality,nameid)

	local ret = "<font=龙字体,0,20,20><color=0xffffffff>"

	--品级
	local qualcolor = quality_color[quality]
	if qualcolor then
			ret = ret .. qualcolor
	end

	--名称
	ret = ret .. Tooltip.GetItemNameEx(proto,nameid)

	ret = ret .. "<font=龙字体,0,16,20><color=0xff05ff00>"

	--类型
	ret = ret .. "\\n" .. Tooltip.GetItemType(proto)
	--绑定
	if Tooltip.GetItemBindType(proto) == 1 then

			ret = ret .. "\\n" .. "获得绑定"

	end

	ret = ret .. "<color=0xffffbc00>"
	--性别限制
	local sex = Tooltip.GetItemSexLimit(proto)
	if sex ~= -1 then

			ret = ret .. "\\n" .. sex_table[sex] .. "限制"

	end

	--等级限制
	local lvlimit = Tooltip.GetItemLevelLimit(proto)
	if lvlimit > 1 then

			ret = ret .. "\\n  \\n等级要求 "

			local playerlv = Tooltip.GetLocalPlayerLevel()
			if playerlv < lvlimit then
					ret = ret .. "<color=0xffff402d>" .. lvlimit .. " <color=0xffffbc00>级"
			else
					ret = ret .. "<color=0xff05ff00>" .. lvlimit .. " <color=0xffffbc00>级"
			end
	end

	--妖精特殊判断，添加妖精所带属性
	local fabaoAtt1,fabaoAtt2,fabaoAtt3=Tooltip.GetFabaoRoleAtt(proto)
	if fabaoAtt1 ~= nil then
		ret = ret .. "\\n" .. att_table[fabaoAtt1]
	end
	if fabaoAtt2 ~= nil then
		ret = ret .. "\\n" .. att_table[fabaoAtt2]
	end
	if fabaoAtt3 ~= nil then
		ret = ret .. "\\n" .. att_table[fabaoAtt3]
	end

	--神格限制
	local lvlimit = Tooltip.GetEquipGodHeadLimit(0,proto)
	if lvlimit > 1 then
		ret = ret .. "\\n神格要求 "

		local playerlv = Tooltip.GetLocalPlayerGodHead()
		if playerlv > lvlimit then
			ret = ret .. "<color=0xffff402d>" .. lvlimit .. " <color=0xffffbc00>级"
		else
			ret = ret .. "<color=0xff05ff00>" .. lvlimit .. " <color=0xffffbc00>级"
		end
	end
	
	--属性限制
	for i=0,5 do
			local new = Tooltip.GetEquipAttributeLimit(0,proto,i)
			if new > 0 then
					local origin = Tooltip.GetLocalPlayerAttBidden(i) + Tooltip.GetLocalPlayerAttLevelup(i)
					if origin < new then
							ret = ret .. "\\n属性要求" .. atta_table[i] .. " <color=0xffff402d>" .. new .. "<color=0xffffbc00>"
					else
							ret = ret .. "\\n属性要求" .. atta_table[i] .. " " .. new
					end
			end
	end
	
	--氏族声望限制
	local clan,cval = Tooltip.GetItemClanReputeLimit(proto)
	clan = clan - 20
	if clan >= 0 and cval > 0 then
		local lpclan = Tooltip.GetLocalPlayerRepution(clan)
		if lpclan >= cval then
			ret = ret .. "\\n需要" .. clan_table[clan] .. "氏族声望 " .. reput_table[cval]
		else
			ret = ret .. "\\n需要" .. clan_table[clan] .. "氏族声望<color=0xffff402d> " .. reput_table[cval] .. "<color=0xffffbc00>"
		end
	end

	--描述
	--ret = ret .. "\\n<color=0xffffffff>" .. Tooltip.GetItemDesc(proto)
	ret = ret .. "\\n<color=0xffffffff>" .. Tooltip.GetEquipDescEx(proto)
	
	if quality == 255 then
			--ret = ret .. "\\n" .. quality_color[255] .. "未鉴定"
			return ret
	end

	ret = ret .. "\\n  "
	ret = ret .. "<color=0xffffffff>"

	--获得装备相关属性影响系数
	local fweapon,farmor,fpotval = Tooltip.GetEquipQltyEffect(quality)

	--武器伤害
	local min,max = Tooltip.GetEquipWeaponDamage(0,proto)
	if min > 0 and max > 0 then
			ret = ret .. "\\n武器伤害 " .. GetInteger(min*fweapon) .. " - " .. GetInteger(max*fweapon)
	end
	--法术伤害
	local hun = Tooltip.GetEquipWeaponWuhun(0,proto)
	if hun > 0 then
			ret = ret .. "\\n法器伤害 " .. GetInteger(hun*fweapon)
	end
	--护甲
	local armor = Tooltip.GetEquipArmorDefine(0,proto)
	if armor > 0 then
			ret = ret .. "\\n防具护甲 " .. GetInteger(armor*farmor)
	end

	--潜力
	--local new,maxv,fabaoflag = Tooltip.GetShopEquipPotVal(0,proto)
	--if fabaoflag == 12 then
			--ret = ret .. "\\n强化值 约" .. new
	--elseif new > 0 and new ~= nil then
			--ret = ret .. "\\n强化值 约" .. GetInteger(new*fpotval)
	--end

	--气质
	local att = Tooltip.GetEquipProtoAtt(proto)
	if att then
		for k,v in pairs(att) do
			if k == 71 then
				ret = ret .. "\\n气质 " .. v
			end
		end

	end

	--圣灵属性相关
	local isShengLing = Tooltip.IsHolyMan(proto)
	if isShengLing ~= nil then
		if isShengLing == 1 then
			--开始显示圣灵装备属性
			local ShengLing1,ShengLing2,ShengLing3,ShengLing4,ShengLing5,ShengLing6,ShengLing7,ShengLing8 = Tooltip.GetGoodsHolyManAtt(proto)
			if ShengLing1~= nil then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "战力  +" .. ShengLing1
			end
			if ShengLing2~= nil then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "防护  +" .. ShengLing2
			end
			if ShengLing3~= nil then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "灵能  +" .. ShengLing3
			end
			if ShengLing4~= nil then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "必杀  +" .. ShengLing4
			end
			if ShengLing5~= nil then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "必杀效果  +" .. ShengLing5
			end
			if ShengLing6~= nil then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "狂暴  +" .. ShengLing6
			end
			if ShengLing7~= nil then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "命中  +" .. ShengLing7
			end
			if ShengLing8~= nil then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "敏锐  +" .. ShengLing8
			end

		end
	end

	local isShengLingEquip = Tooltip.IsHolyManEquip(proto)
	if isShengLingEquip ~= nil then
		if isShengLingEquip == 1 then
			local ShengWen1,ShengWen2,ShengWen3,ShengWen4,ShengWen5,ShengWen6,ShengWen7,ShengWen8,ShengWen9,ShengWen10,ShengWen11,ShengWen12,ShengWen13,ShengWen14 = Tooltip.GetGoodsHolyEquipAtt(proto)
			if ShengWen1 ~= nil and ShengWen2 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. ShengLingAtt[ShengWen1] .. "  +" .. ShengWen2
			end
			if ShengWen3 ~= nil and ShengWen4 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. ShengLingAtt[ShengWen3] .. "  +" .. ShengWen4
			end
			if ShengWen5 ~= nil and ShengWen6 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. ShengLingAtt[ShengWen5] .. "  +" .. ShengWen6
			end
			if ShengWen7 ~= nil and ShengWen8 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. ShengLingAtt[ShengWen7] .. "  +" .. ShengWen8
			end
			if ShengWen9 ~= nil and ShengWen10 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. ShengLingAtt[ShengWen9] .. "  +" .. ShengWen10
			end
			if ShengWen11 ~= nil and ShengWen12 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. ShengLingAtt[ShengWen11] .. "  +" .. ShengWen12
			end
			if ShengWen13 ~= nil and ShengWen14 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. ShengLingAtt[ShengWen13] .. "  +" .. ShengWen14
			end

		end
	end

	--套装属性
	local suit = "\\n \\n<color=0xff9932cd>套装属性<color=0xffffffff>"
	for i=0,2 do
			local suitid,new,num,maxnum = Tooltip.GetEquipSuitName(proto,quality,i)
			if new ~= nil then
					suit = suit .. "\\n<color=0xff05ff00>" .. new .. "<color=0xffffbc00>  " .. num .. "/" .. maxnum
					if num >= 1 then
							for i=0,4 do
									quat,att = Tooltip.GetEquipSuitAttribute(suitid,i)
									if quat ~= nil and att ~= nil then
											if quat <= num then
												suit = suit .. "\\n" .. quat .. "件 " .. att
												--break
											else
												suit = suit .. "\\n<color=0xff8d8d8d>" .. quat .. "件 " .. att

											end
									end
							end
					end
			end
	end
	if suit ~= "\\n \\n<color=0xff9932cd>套装属性<color=0xffffffff>"	then
			ret = ret .. "\\n<color=0xffffffff>" .. suit
	end

	ret = ret .. "\\n<color=0xffffffff>"
	--剩余时间
	local totalTimeLimit = Tooltip.GetItemProtoAllTimeLimit(proto)
	if totalTimeLimit ~= nil and totalTimeLimit > 0 then
			ret = ret .. "\\n剩余 " .. GetTimeDisplayLite(totalTimeLimit) .. "到期"
	end

	return ret
end

