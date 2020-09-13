--Tooltip.GetEquipType(obj,proto)
--[[
	0,
	1,	// 消耗品
	2,	// 非消耗品
	3,	// 特殊物品
	4,	// 宠物
	5,	// 物品包
	6,	// 商城道具
	7,	// 武器
	8,	// 布甲
	9,	// 饰品
	10,	// 时装
	11,	// 铠甲
	12,	// 法宝
	13,	// 新手装
]]
local sex_table = {[0]="女性","男性"}

local att_table =
{
	[0]="体质#",
	[1]="武力#",
	[2]="真元#",
	[3]="法力#",
	[4]="技力#",
	[5]="身法#",
	[6]="<color=0xff00a2ff>#属性点数",
	[7]="<color=0xff00a2ff>#天资点数",
	[8]="<color=0xff00a2ff>#体力上限",
	[9]="<color=0xff00a2ff>#体力",
	[10]="<color=0xff00a2ff>#真气上限",
	[11]="<color=0xff00a2ff>#真气",
	[12]="<color=0xff00a2ff>#物理攻击",
	[13]="<color=0xff00a2ff>#物理防御",
	[14]="<color=0xff00a2ff>#法术攻击",
	[15]="<color=0xff00a2ff>#法术防御",
	[16]="<color=0xff00a2ff>#精准",
	[17]="<color=0xff00a2ff>#灵巧",
	[18]="<color=0xff00a2ff>#命中",
	[19]="<color=0xff00a2ff>#闪避",
	[20]="<color=0xff00a2ff>#神力",
	[21]="<color=0xff00a2ff>#神力上限",
	[22]="<color=0xff00a2ff>#体力恢复",
	[23]="<color=0xff00a2ff>#真气恢复",
	[24]="<color=0xff00a2ff>#怒气",
	[25]="<color=0xff00a2ff>#活力",
	[26]="<color=0xff00a2ff>#活力上限",
	[27]="<color=0xff00a2ff>#活力恢复",
	[28]="<color=0xff00a2ff>#移动速度",
	[29]="<color=0xff00a2ff>#跳跃能力",
	[30]="<color=0xff00a2ff>#游泳速度",
	[31]="<color=0xff00a2ff>#骑乘速度",
	[32]="<color=0xff00a2ff>#最小武器伤害",
	[33]="<color=0xff00a2ff>#最大武器伤害",
	[34]="<color=0xff00a2ff>#法器伤害",
	[35]="<color=0xff00a2ff>#护甲",
	[36]="<color=0xff00a2ff>#普通伤害减免",
	[37]="<color=0xff00a2ff>#出血伤害减免",
	[38]="<color=0xff00a2ff>#玄翎伤害减免",
	[39]="<color=0xff00a2ff>#神兵伤害减免",
	[40]="<color=0xff00a2ff>#毒性伤害减免",
	[41]="<color=0xff00a2ff>#仙音伤害减免",
	[42]="<color=0xff00a2ff>#乾坤伤害减免",
	[43]="<color=0xff00a2ff>#绝技伤害减免",
	[44]="<color=0xff00a2ff>#物理伤害减免",
	[45]="<color=0xff00a2ff>#法术伤害减免",
	[46]="<color=0xff00a2ff>#全部伤害减免",
	[47]="<color=0xff00a2ff>#额外伤害",
	[48]="<color=0xff00a2ff>#伤害吸收",
	[49]="<color=0xff00a2ff>#固定伤害抗性",
	[50]="<color=0xff00a2ff>#衰弱抗性",
	[51]="<color=0xff00a2ff>#控制效果抗性",
	[52]="<color=0xff00a2ff>#减速抗性",
	[53]="<color=0xff00a2ff>#灵力",
	[54]="<color=0xff00a2ff>#攻击失误率",
	[55]="<color=0xff00a2ff>#近战躲避",
	[56]="<color=0xff00a2ff>#远程躲避",
	[57]="<color=0xff00a2ff>#暴击",
	[58]="<color=0xff00a2ff>#暴击量",
	[59]="<color=0xff00a2ff>#格档",
	[60]="<color=0xff00a2ff>#灵感",
	[61]="<color=0xff00a2ff>#隐匿",
	[62]="<color=0xff00a2ff>#悟性",
	[63]="<color=0xff00a2ff>#士气",
	[64]="<color=0xff00a2ff>#内伤",
	[65]="<color=0xff00a2ff>#仇恨",
	[66]="<color=0xff00a2ff>#体型",
	[67]="<color=0xffff80c0>战斗经验加成#",
	[68]="<color=0xffff80c0>掉落金钱加成#",
	[69]="<color=0xffff80c0>掉宝几率加成#",
	[70]="幸运#",
	[71]="气质#",
	[72]="统御#",
	[73]="阅历#",
	[74]="道德#",
	[75]="修为#",
	[83]="<color=0xff00a2ff>#韧性",
	[84]="<color=0xff00a2ff>#神兵伤害加深",
	[85]="<color=0xff00a2ff>#玄翎伤害加深",
	[86]="<color=0xff00a2ff>#乾坤伤害加深",
	[87]="<color=0xff00a2ff>#仙音伤害加深",
	[88]="<color=0xff00a2ff>#物理伤害加深",
	[89]="<color=0xff00a2ff>#法术伤害加深",
	[90]="<color=0xff00a2ff>#全部伤害加深",
	[92]="<color=0xff00a2ff>#元素伤害",
	[109]="<color=0xff00a2ff>#法力燃烧抵抗",
	[110]="<color=0xff00a2ff>#韧性",
	[111]="<color=0xff00a2ff>#韧性强度",
	[112]="<color=0xff00a2ff>#控制效果抵抗",
	[113]="<color=0xff00a2ff>#控制效果加深",
	[114]="<color=0xff00a2ff>#固定伤害强化",
	[115]="<color=0xff00a2ff>#衰老效果强化",
	[116]="<color=0xff00a2ff>#减速效果强化",
	[117]="<color=0xff00a2ff>#物理反弹伤害",
	[118]="<color=0xff00a2ff>#物理反弹比例",
	[119]="<color=0xff00a2ff>#法术反弹伤害",
	[120]="<color=0xff00a2ff>#法术反弹比例",
	[121]="<color=0xff00a2ff>#物理反弹减免",
	[122]="<color=0xff00a2ff>#法术反弹减免",
	[123]="<color=0xff00a2ff>#法力燃烧",
	[124]="<color=0xff00a2ff>#每日信仰",
	[125]="<color=0xff00a2ff>#无视攻击",
	[126]="<color=0xff00a2ff>#无视防御",
	[127]="<color=0xff00a2ff>#神魂",
	[131]="<color=0xff00a2ff>#元素穿透",
	[132]="<color=0xff00a2ff>#元素抗性",

}

local att_table2 =
{
	[0]="体质#",
	[1]="武力#",
	[2]="真元#",
	[3]="法力#",
	[4]="技力#",
	[5]="身法#",
	[6]="#属性点数",
	[7]="#技能点数",
	[8]="#体力上限",
	[9]="#体力",
	[10]="#真气上限",
	[11]="#真气",
	[12]="#物理攻击",
	[13]="#物理防御",
	[14]="#法术攻击",
	[15]="#法术防御",
	[16]="#精准",
	[17]="#灵巧",
	[18]="#命中",
	[19]="#闪避",
	[20]="#神力",
	[21]="#神力上限",
	[22]="#体力恢复",
	[23]="#真气恢复",
	[24]="#怒气",
	[25]="#活力",
	[26]="#活力上限",
	[27]="#活力恢复",
	[28]="#移动速度",
	[29]="#跳跃能力",
	[30]="#游泳速度",
	[31]="#骑乘速度",
	[32]="#最小武器伤害",
	[33]="#最大武器伤害",
	[34]="#法器伤害",
	[35]="#护甲",
	[36]="#普通伤害减免",
	[37]="#出血伤害减免",
	[38]="#玄翎伤害减免",
	[39]="#神兵伤害减免",
	[40]="#毒性伤害减免",
	[41]="#仙音伤害减免",
	[42]="#乾坤伤害减免",
	[43]="#绝技伤害减免",
	[44]="#物理伤害减免",
	[45]="#法术伤害减免",
	[46]="#全部伤害减免",
	[47]="#额外伤害",
	[48]="#伤害吸收",
	[49]="#固定伤害抗性",
	[50]="#衰弱抗性",
	[51]="#控制效果抗性",
	[52]="#减速抗性",
	[53]="#灵力",
	[54]="#攻击失误率",
	[55]="#近战躲避",
	[56]="#远程躲避",
	[57]="#暴击",
	[58]="#暴击量",
	[59]="#格档",
	[60]="#灵感",
	[61]="#隐匿",
	[62]="#悟性",
	[63]="#士气",
	[64]="#内伤",
	[65]="#仇恨",
	[66]="#体型",
	[67]="战斗经验加成#",
	[68]="掉落金钱加成#",
	[69]="掉宝几率加成#",
	[70]="幸运#",
	[71]="气质#",
	[72]="统御#",
	[73]="阅历#",
	[74]="道德#",
	[75]="修为#",
	[83]="#韧性",
	[84]="#神兵伤害加深",
	[85]="#玄翎伤害加深",
	[86]="#乾坤伤害加深",
	[87]="#仙音伤害加深",
	[88]="#物理伤害加深",
	[89]="#法术伤害加深",
	[90]="#全部伤害加深",
	[92]="#元素伤害",
	[109]="#法力燃烧抵抗",
	[110]="#韧性",
	[111]="#韧性强度",
	[112]="#控制效果抵抗",
	[113]="#控制效果加深",
	[114]="#固定伤害强化",
	[115]="#衰老效果强化",
	[116]="#减速效果强化",
	[117]="#物理反弹伤害",
	[118]="#物理反弹比例",
	[119]="#法术反弹伤害",
	[120]="#法术反弹比例",
	[121]="#物理反弹减免",
	[122]="#法术反弹减免",
	[123]="#法力燃烧",
	[124]="#每日信仰",
	[125]="#无视攻击",
	[126]="#无视防御",
	[127]="#神魂",
	[131]="#元素穿透",
	[132]="#元素抗性",
}

local NUM_TEXT =
{
	[0]="",
	[1]="<color=0xffffff00>Ⅰ",
	[2]="<color=0xffffff00>Ⅱ",
	[3]="<color=0xffffff00>Ⅲ",
	[4]="<color=0xff05ff00>Ⅳ",
	[5]="<color=0xff05ff00>Ⅴ",
	[6]="<color=0xff00a2ff>Ⅵ",
	[7]="<color=0xff00a2ff>Ⅶ",
	[8]="<color=0xffff6000>Ⅷ",
	[9]="<color=0xffff6000>Ⅸ",
	[10]="<color=0xff9933ff>Ⅹ",
	[11]="<color=0xff9933ff>Ⅺ",
	[12]="<color=0xffff00ff>Ⅻ",
}

local wuxing_hall = {"<color=0xffff402d>火灵魂槽",	"<color=0xffd1b945>地灵魂槽",	"<color=0xff706a9b>暗灵魂槽",	"<color=0xffb3f3a9>仙灵魂槽",	"<color=0xffffffff>圣灵魂槽"}
local wuxing_qiehe_type = {"<color=0xffffbc00>三槽灵魂觉醒", "<color=0xffffbc00>四槽灵魂觉醒", "<color=0xffffbc00>五槽灵魂觉醒"}
local wuxing = {"火",	"地",	"暗",	"仙",	"圣"}

local quality_color =
{
	[0]="<color=0xffffffff>",
	[1]="<color=0xfffff717>",
	[2]="<color=0xff05ff00>",
	[3]="<color=0xff00a2ff>",
	[4]="<color=0xffff7800>",
	[5]="<color=0xff9932cd>",
	[6]="<color=0xffff80c0>",
	[255]="<color=0xff8d8d8d>"
}

local equip_pos = {"头防","妖精","上衣","下装","披风","手腕","足鞋","右手戒指","左手戒指","腰饰","项链","主武器","副武器"}

local spec_att_table =
{
	"简易 装备等级限制减5",
	"精简 装备等级限制减10",
	"无级别 装备无等级限制",
	"轻便 装备属性限制减少10%",
	"舒适 装备属性限制减少25%",
	"轻盈 装备属性限制减少50%",
	"隐凤 装备初始潜力值增加200",
	"隐凰 装备初始潜力值增加400",
	"飞凤 装备初始潜力值增加800",
	"鸣凰 装备初始潜力值增加1200",
	"卧龙 装备初始潜力值提高5%",
	"藏龙 装备初始潜力值提高10%",
	"伏龙 装备初始潜力值提高20%",
	"升龙 装备初始潜力值提高30%",
	"铭纹守护 装备在进行铭纹强化时不会失败",
	"烙印守护 装备在进行烙印强化时不会失败",
	"镌刻守护 装备在进行镌刻强化时不会失败",
	"龙附守护 装备在进行龙附强化时不会失败",
	"淬火守护 装备在进行淬火强化时不会失败",
	"开凿守护 装备在进行开凿强化时不会失败",
	"绝世 装备在进行任何强化（除炼器及熔铸）时都不会失败",
	"避邪 进行装备强化时成功率增加5%",
	"幸运 进行装备强化时完美率增加5%",
	"死亡守护 装备不会在玩家死亡时掉落"
}
local color_table = {[0]="赭色","菊兰","间紫","草绿","暗金","珊瑚色","苍紫","天蓝","紫罗兰","橄榄色","白色","墨青"}
local clan_table = {[0]="轩辕","神农","伏羲","三苗","九黎","月氏","女娲","共工"}
local reput_table = {[0]="仇视","嫌恶","冷漠","中立","友善","亲密","信赖","尊敬","崇拜","传奇"}

--时间换算，单位秒
local ONE_YEAR 	= 365*24*60*60
local ONE_MONTH	= 30*24*60*60
local ONE_DAY	 	= 24*60*60
local ONE_HOUR	 	= 60*60
local ONE_MINUTE 	= 60

--取整
local function GetInteger(x)
		return x-x%1
end
--取百分比(保留两位小数)
local function GetPercent(x)
		local y = x/100
		return y-y%0.01
end
--显示时间
local function GetTimeDisplay(n)
	if n > ONE_YEAR then
			return GetInteger(n/ONE_YEAR) .. "年以上"
	elseif n > ONE_MONTH then
			return GetInteger(n/ONE_MONTH) .. "月以上"
	elseif n > ONE_DAY then
			return GetInteger(n/ONE_DAY) .. "天以上"
	elseif n > ONE_HOUR then
			return GetInteger(n/ONE_HOUR) .. "小时以上"
	elseif n > ONE_MINUTE then
			return GetInteger(n/ONE_MINUTE) .. "分钟以上"
	else
			return "小于1分钟"
	end
end
--显示属性百分比数值
local function GetValueDisplayPct(value)

	return value > 0 and (" + " .. GetInteger(value/100) .. "%% ") or (" - " .. GetInteger(0-value/100) .. "%% ")

end
--显示属性数值
local function GetValueDisplay(type,value)

			if type == 28 or type == 29 or type == 30 or type == 31 or type == 36 or type == 54 or type == 55 or type == 56 or type == 58 or type == 59 or type == 67 or type == 68 or type == 69 then
					return GetValueDisplayPct(value)
			else
					return value > 0 and (" + " .. GetInteger(value) .. " ") or (" - " .. GetInteger(0 - value) .. " ")
			end
end
--显示属性数值
local function GetValueDisplay2(type,value)
			local bpercent = false
			if value > 100000 then
				bpercent = true
				value = value - 100000
			elseif value < -100000 then
				bpercent = true
				value = value + 100000
			end

			if type == 28 or type == 29 or type == 30 or type == 31 or type == 36 or type == 54 or type == 55 or type == 56 or type == 58 or type == 59 or type == 67 or type == 68 or type == 69 or bpercent then
					return GetValueDisplayPct(value)
			else
					return value >= 0 and (" + " .. GetInteger(value) .. " ") or (" - " .. GetInteger(0 - value) .. " ")
			end
end
--列表排序迭代
local function pairsByKeys(t,f)
	local a = {}
	for n in pairs(t) do a[#a + 1] = n end
	table.sort(a,f)
	local i = 0
	return function ()
		i = i + 1
		return a[i],t[a[i]]
	end
end
--属性顺序比较
local function compAtt(a,b)
	if a == 71 then return true end

	if b == 71 then return false end

	return a < b
end


function GetObjString(obj,proto)

	local ret = "<font=龙字体,0,20,20><color=0xffffffff>"

	--品级
	local quality = Tooltip.GetItemQuality(obj,proto)
	local qualcolor = quality_color[quality]
	if qualcolor then
			ret = ret .. qualcolor
	end

	--名称
	ret = ret .. Tooltip.GetItemName(obj)

	--魂晶
	local equippos = Tooltip.GetEquipPos(proto)
	if equippos == 13 then -- 根据位置判断是魂晶
		ret = ret .. "Lv" .. Tooltip.GetHunJingLevel(obj) .. " "
	end

	--强化次数
	local strengLevel = Tooltip.GetEquipStrengLevel(obj);
	if(strengLevel>0) then
			ret = ret .. "+" .. strengLevel;
	end

	--追加次数
	local BrandLevel = Tooltip.GetEquipBrandLevel(obj);
	if(BrandLevel>0) then
			ret = ret .. " 追 " .. BrandLevel;
	end

	--圣纹充能等级
	local chongnengLevel = Tooltip.GetShengWenChongLevel(obj);
	if chongnengLevel ~= nil and chongnengLevel>0 then
			ret = ret .. "+" .. chongnengLevel;
	end

	ret = ret .. "<font=龙字体,0,16,20><color=0xff05ff00>"
	--[[飞升状态
	if Tooltip.IsEquipSoaring(obj) == true then
			ret = ret .. "\\n" .. "已神铸"
	end]]

	--生产状态
	local ju,ming,rong = Tooltip.GetEquipProduceState(obj)
	if ju or ming or rong then
			ret = ret .. "\\n"
			--[[
			if ju > 0 then
					ret = ret .. "镌" .. ju
			end
			if ming > 0 then
					ret = ret .. "铭" .. ming
			end
			--]]
			if rong > 0 then
					ret = ret .. "熔" .. rong
			end
	end


	--类型
	ret = ret .. "\\n" .. Tooltip.GetEquipTypeDesc(obj,proto)

	--绑定
	if Tooltip.GetItemBindState(obj) == true then
			ret = ret .. "\\n" .. "已绑定"
			local roleName = Tooltip.GetItemBindRoleName(obj)
			if roleName ~= nil then
				ret = ret  .. "[" .. roleName .."]"
			end
	end

  --物品锁定(道具锁定)
	if Tooltip.GetItemLockState(obj) then
		local strResult = Tooltip.GetItemLockString(obj)
		if strResult == nil then
			ret = ret .. "\\n已锁"
		else
			ret = ret .. "\\n于 " .. strResult .. " 解锁"
		end
	end


	ret = ret .. "<color=0xffffbc00>"
	--性别限制
	local sex = Tooltip.GetItemSexLimit(proto)
	if sex ~= -1 then
		mysex = Tooltip.GetLocalPlayerSex()
		if mysex == sex then
			ret = ret .. "\\n" .. sex_table[sex] .. "限制"
		else
			ret = ret .. "\\n<color=0xffff402d>" .. sex_table[sex] .. "<color=0xffffbc00>限制"
		end

	end



	--等级限制
	local lvlimit = Tooltip.GetEquipLevelLimit(obj)
	if lvlimit > 1 then
		ret = ret .. "\\n  \\n等级要求 "

		local playerlv = Tooltip.GetLocalPlayerLevel()
		if playerlv < lvlimit then
			ret = ret .. "<color=0xffff402d>" .. lvlimit .. " <color=0xffffbc00>级"
		else
			ret = ret .. "<color=0xff05ff00>" .. lvlimit .. " <color=0xffffbc00>级"
		end
	end


	--神格限制
	local lvlimit = Tooltip.GetEquipGodHeadLimit(obj,0)
	if lvlimit~=nil and lvlimit > 1 then
		ret = ret .. "\\n神格要求 "

		local playerlv = Tooltip.GetLocalPlayerGodHead()
		if playerlv < lvlimit then
			ret = ret .. "<color=0xffff402d>" .. lvlimit .. " <color=0xffffbc00>级"
		else
			ret = ret .. "<color=0xff05ff00>" .. lvlimit .. " <color=0xffffbc00>级"
		end
	end

	--神铸等级及显示规则
	if quality >= 5 and Tooltip.IsHolyMan(proto) ~= 1 and Tooltip.IsHolyManEquip(proto) ~= 1 then --品级大于紫色，等级大于70
		local soar_level = Tooltip.Getequip_soar_level(obj)
		if soar_level >= 10 then
			ret = ret .. "\\n<color=0xffffbc00>神铸等级 <color=0xffffffff>" .. soar_level .. " <color=0xffffbc00>级(已满)"
		elseif soar_level <= 0 then
			if Tooltip.GetEquipType(obj,proto)~=12 and Tooltip.GetEquipType(obj,proto)~=10 then
			ret = ret .. "\\n<color=0xffffbc00>未神铸"
			end
		else
			ret = ret .. "\\n<color=0xffffbc00>神铸等级 <color=0xffffffff>" .. soar_level .. " <color=0xffffbc00>级"
		end
	end

	--属性限制
    if quality ~= 255 then
		for i=0,5 do
				local new = Tooltip.GetEquipAttributeLimit(0,proto,i)
				if new > 0 then
						local origin = Tooltip.GetLocalPlayerAttBidden(i) + Tooltip.GetLocalPlayerAttLevelup(i)
						if origin < new then
								strlimit = string.gsub(att_table[i], "#", " <color=0xffff402d>" .. new .. "<color=0xffffbc00>")
						else
								strlimit = string.gsub(att_table[i], "#", " " .. new)
						end
						ret = ret .. "\\n属性要求" .. strlimit
				end
		end
	end

	--魂晶
	local equippos = Tooltip.GetEquipPos(proto)
	if equippos == 13 then -- 根据位置判断是魂晶
		local hunjingIntelligence, hunjingability=Tooltip.GetHunJingSpec(obj)
		ret = ret .. "\\n" .. "资质 " .. "<color=0xff05ff00>" .. hunjingIntelligence
		ret = ret .. "\\n" .. "能力强度" .. "<color=0xff05ff00>" .. hunjingability

		if nil ~= Tooltip.GetHunJingSkillDesc(obj) and nil ~= Tooltip.GetHunJingSkillName(obj) then
			ret = ret .. "\\n \\n<color=0xff05ff00>" .. Tooltip.GetHunJingSkillName(obj) .. ":<color=0xffffffff>\\n" .. Tooltip.GetHunJingSkillDesc(obj)
		end

		if nil ~= Tooltip.GetHunJingAttDesc(obj) then
			ret = ret .. "\\n \\n<color=0xff05ff00>额外能力：<color=0xffffffff>\\n" .. Tooltip.GetHunJingAttDesc(obj)
		end
	end

	--氏族声望限制
	--[[
	local clan,cval = Tooltip.GetItemClanReputeLimit(proto)
	clan = clan - 20
	if clan >= 0 and cval > 0 then
		local lpclan = Tooltip.GetLocalPlayerRepution(clan)
		if lpclan >= cval then
			if quality ~= 255 then
					ret = ret .. "\\n需要" .. clan_table[clan] .. "氏族声望 " .. reput_table[cval]
			else
					ret = ret .. "\\n需要" .. clan_table[clan] .. "氏族声望<color=0xffff402d> " .. reput_table[cval] .. "<color=0xffffbc00>"
			end
		end
	end
	--]]

	--使用时限
	if Tooltip.IsItemInAmazingBag(obj) == false then
			local timelimit = Tooltip.GetItemTimeLimit(obj)
			if timelimit ~= nil and timelimit > 0 then
				ret = ret .. "\\n剩余 " .. GetTimeDisplay(timelimit) .. "到期"
			end
	end

	if quality == 255 then

			ret = ret .. "\\n" .. quality_color[255] .. "未鉴定" .. "<color=0xffffffff>"



                        --修正率及紫色鉴定几率

			local pct,pctex,Voilet1 = Tooltip.GetEquipQltyModPct(obj)


			if pct < 0 then
			-- 一级修正率小于零开始

			-- 鉴定几率
				local White,Yellow,Green,Blue,Orange,Voilet = Tooltip.GetEquipQltyPct(proto)



				if Orange~=nil and Orange >0  then
					local orate = Orange/10000*(1+pct/10000)
					orate = GetInteger(orate*100)
					if orate >0  then
                        ret = ret  .. "<color=0xffff8000>"
						ret = ret  .. "\\n橙色几率: 约" .. orate .. "%"
					end
				end

				if Blue~=nil and Blue   >0  then
					local brate = Blue/10000*(1+pct/10000)-Orange/10000*pct/10000
					brate = GetInteger(brate*100)
					if brate >0   then
                        ret = ret  .. "<color=0xff0088ff>"
						ret = ret  .. "\\n蓝色几率: 约" .. brate .. "%"
					end
				end

				if Green ~=nil and Green  >0  then
					local grate = Green/10000*(1+pct/10000)-Blue/10000*pct/10000
					grate = GetInteger(grate*100)
					if grate  >0  then
                        ret=  ret   .. "<color=0xff00ff00>"
						ret = ret  .. "\\n绿色几率: 约" .. grate .. "%"
					end
				end

				if Yellow~=nil and Yellow >0  then
					local yrate = Yellow/10000*(1+pct/10000)-Green/10000*pct/10000
					yrate = GetInteger(yrate*100)
					if yrate  >0  then
                        ret = ret  .. "<color=0xffffff00>"
						ret = ret  .. "\\n黄色几率: 约" .. yrate .."%"
					end
				end

				if White~=nil and White  >0  then
					local wrate = White/10000 - Yellow/10000*pct/10000
					wrate = GetInteger(wrate*100)
					if wrate  >0  then
                        ret = ret  .. "<color=0xffffffff>"
						ret = ret  .. "\\n白色几率: 约" .. wrate .."%"
					end
				end
                        -- 一级修正率小于零结束

			elseif pct >= 0 then

			-- 一级修正率大于并等于零开始
				if pctex == nil then
					pctex = 0
				end

                        -- 鉴定几率
				local White,Yellow,Green,Blue,Orange,Voilet = Tooltip.GetEquipQltyPct(proto)
				Voilet = Voilet1*100

				if voilet~=nil and Voilet > 0 then
					local vrate = GetInteger(Voilet/100)
					if vrate > 0 then
                        ret = ret  .. "<color=0xff5300a0>"
						ret = ret  .. "\\n紫色几率: 约" .. vrate .. "%"
					end
				end

				if Orange~=nil and Orange >0  then
					local orate = (1-Voilet/10000)*(Orange/10000+Blue/10000*pct/10000+Green/10000*(1-pct/10000)*pctex/10000)
					orate = GetInteger(orate*100)
					if orate >0  then
                        ret = ret  .. "<color=0xffff8000>"
						ret = ret  .. "\\n橙色几率: 约" .. orate .. "%"
					end
				end

				if Blue~=nil and Blue   >0  then
					local brate = (1-Voilet/10000)*(Blue/10000*(1-pct/10000)*(1-pctex/10000)+Green/10000*pct/10000+Yellow/10000*(1-pct/10000)*pctex/10000)
					brate = GetInteger(brate*100)
					if brate >0   then
                        ret = ret  .. "<color=0xff0088ff>"
						ret = ret  .. "\\n蓝色几率: 约" .. brate .. "%"
					end
				end

				if Green~=nil and Green  >0  then
					local grate = (1-Voilet/10000)*(Green/10000*(1-pct/10000)*(1-pctex/10000)+Yellow/10000*pct/10000+White/10000*(1-pct/10000)*pctex/10000)
					grate = GetInteger(grate*100)
					if grate  >0  then
                        ret=  ret   .. "<color=0xff00ff00>"
						ret = ret  .. "\\n绿色几率: 约" .. grate .. "%"
					end
				end

				if Yellow~=nil and Yellow >0  then
					local yrate = (1-Voilet/10000)*(Yellow/10000*(1-pct/10000)*(1-pctex/10000)+White/10000*pct/10000)
					yrate = GetInteger(yrate*100)
					if yrate  >0  then
                        ret = ret  .. "<color=0xffffff00>"
						ret = ret  .. "\\n黄色几率: 约" .. yrate .."%"
					end
				end

				if White~=nil and White  >0  then
					local wrate = (1-Voilet/10000)*(White/10000*(1-pct/10000)*(1-pctex/10000))
					wrate = GetInteger(wrate*100)
					if wrate  >0  then
                        ret = ret  .. "<color=0xffffffff>"
						ret = ret  .. "\\n白色几率: 约" .. wrate .."%"
					end
				end
                        -- 一级修正率大于并等于零结束
			end

			ret = ret .. "\\n <color=0xffffffff>"

			--最低伤害
			local minDmg,maxDmg =Tooltip.GetEquipWeaponDamage(proto)

			if minDmg > 0 then
				if maxDmg > 0 then
					ret = ret .. "\\n".."\\n最低伤害 " .. minDmg .." - " .. maxDmg
				end
			end

			--最低法术伤害
			--[[
			local minhun = Tooltip.GetEquipWeaponWuhun(proto)
				if minhun > 0 then
					ret = ret .. "\\n" .. "\\n最低法术伤害 " .. minhun
				end
			--]]

			--最低潜力
			--[[
			local  minpotval,maxpotval = Tooltip.GetEquipPotVal(proto)
			if maxpotval ~= nil then
				if minpotval > 0 then
					ret = ret .. "\\n" .. "\\n最低强化值 " .. minpotval .. "/" .. maxpotval
				end
			end
			ret = ret .. "\\n "
			--]]


			-- 筋骨
            local att = Tooltip.GetEquipProtoAtt(proto)
			if att ~= nil then
		        for k, v in pairsByKeys(att,compAtt) do
					if  att_table[k] then
						stratt = string.gsub(att_table[k], "#", GetValueDisplay(k, v))
						ret = ret .. "\\n" .. stratt
					end
				end
				ret = ret .. "\\n <color=0xffff0000>其他属性需鉴定后才可获得！"
			end

			--装备描述
			local dest = Tooltip.GetEquipDescEx(proto)
			if dest ~= nil then
				ret = ret .. "\\n <color=0xffffffff> "
				ret = ret .. "\\n "
				ret = ret .. dest
			end

      --装备署名，如果有生产者署名，覆盖之
			local bSignatrued = Tooltip.IsEquipSignatured(obj)
			if bSignatrued ~= nil and bSignatrued ~= false then
				local strPlayer = Tooltip.GetEquipSignaturePlayer(obj)
				local strSignatrueDesc = Tooltip.GetEquipSignature(obj)
				if strPlayer ~= nil then
					ret = ret .. "\\n<color=0xff06d401>".. strPlayer .. ":"
				end
				if( strSignatrueDesc ~= nil ) then
					ret = ret .. "\\n" .. strSignatrueDesc
				end
			else
				--生产者，如果装备用IM道具署名，生产者署名被覆盖
				local creator = Tooltip.GetItemCreator(obj)
				if creator ~= nil then
					ret = ret .. "\\n  \\n由" .. "<color=0xff06d401>" .. Tooltip.GetItemCreator(obj) .. "<color=0xffffffff>" .. "制造"
				end
			end

			return ret
	end

	--成长属性
	local isGrowEquip = Tooltip.GetIsGrowEquip(obj)
	if isGrowEquip ~= nil then
		local curEquipGrowExp, curEquipGrowMaxExp = Tooltip.GetEquipGrowExp(obj)	--获取装备经验
		local equipGrowStrength = Tooltip.GetGrowEquipStrength(obj)								--成长装备强度
		if curEquipGrowExp ~= nil and curEquipGrowMaxExp ~= nil and equipGrowStrength ~= nil then
			ret = ret .. "\\n<color=0xffffbc00>装备成长" .. NUM_TEXT[equipGrowStrength] .. "<color=0xffffbc00>：" .. curEquipGrowExp .. "/" .. curEquipGrowMaxExp
		end
		local equipGrowStarColor = Tooltip.GetEquipStarColor(obj)
		if equipGrowStarColor ~= nil then
			ret = ret .. "\\n<color=0xffffffff>"
			for i=0,9 do
				ret = ret .. equipGrowStarColor[i]
			end
		end
		local equipGrowAttType, equipGrowAttValue= Tooltip.GetEquipGrowAtt(obj)	--装备属性
		if equipGrowAttValue ~= nil and equipGrowAttType ~= nil then
			for i=0,4 do
				if equipGrowAttType[i] ~= nil and equipGrowAttValue[i] ~= nil then
					if att_table[equipGrowAttType[i]] then
						local stratt = string.gsub(att_table[equipGrowAttType[i]], "#", "")
						stratt = stratt .. equipGrowAttValue[i]
						ret = ret .. "\\n" .. stratt
					end
				end

			end
		end
	end

	ret = ret .. "<color=0xffffffff>"

	--武器伤害
	local min,max = Tooltip.GetEquipWeaponDamage(obj,proto)
	--法术伤害
	local hun = Tooltip.GetEquipWeaponWuhun(obj,proto)
	--护甲
	local armor = Tooltip.GetEquipArmorDefine(obj,proto)
	if (min > 0 and max > 0) or hun > 0 or armor > 0 then
		--空行
		ret = ret .. "\\n  "

		if min > 0 and max > 0 then
			ret = ret .. "\\n武器伤害 " .. min .. " - " .. max
		end

		if hun > 0 then
				ret = ret .. "\\n法器伤害 " .. hun
		end


		if armor > 0 then
				ret = ret .. "\\n防具护甲 " .. armor
		end

	end

	--崭新
	--[[
	--local new = Tooltip.GetEquipNewNess(obj,proto)
	--local equiptype = Tooltip.GetEquipType(obj,proto)
	--if 7 == equiptype then
		--if new < 0 then
			--new = 0
		--end
		--ret = ret .. "\\n耐久 " .. new
	--end
	--]]
	--潜力
	--[[
	--local new,currmax,max = Tooltip.GetEquipPotVal(obj,proto)
	--if max >= 0 and new ~= nil then
			--ret = ret .. "\\n强化值 " .. new .. "/" .. currmax .. "(上限:" .. max .. ")"
	--end

	--潜力可提升的次数
	local maxtime = Tooltip.GetEquipPotTime(obj,proto)
	if maxtime > 0 then
			ret = ret .. "\\n可提升强化值次数 " .. maxtime
	end
	--]]

    local nativeIntelligence, birthtime, attmods, halls, hallrewarded, hallrewardtypes, hallreward, linzhu, lizhurewardtypes1, lizhurewardtypes2, lizhurewardtypes3, lizhurewardtypes4, lizhurewardtypes5, lizhurewards1, lizhurewards2,lizhurewards3,lizhurewards4,lizhurewards5, wuxingReward, wuxingRewarded, skillnames=Tooltip.GetFaBaoSpec(obj)
	-- 法宝资质
	if nativeIntelligence ~= nil and nativeIntelligence>0 then
		ret = ret .. "\\n" .. "妖精资质 " .. "<color=0xff05ff00>" .. nativeIntelligence
	end

	-- 繁育次数
	if birthtime ~= nil then
		ret = ret .. "\\n" .."<color=0xffffffff>".. "妖精可繁育次数 " .. "<color=0xff05ff00>" .. birthtime
	end

	-- 法宝加属性
	if attmods ~= nil then
		ret = ret .. "\\n "
		for k, v in pairs(attmods) do
			if att_table[k] ~= nil then
				if k < 92 or k > 97 then
					ret = ret .. "\\n" .. string.gsub(att_table[k], "#", GetValueDisplay(k, v))
				else
					ret = ret .. "\\n" .. string.gsub("<color=0xff00a2ff>#元素伤害", "#", GetValueDisplay(k, v))
				end
			end
		end
	end

	--属性加成
 	local att = Tooltip.GetEquipAttribute(obj)
	ret = ret .. "<color=ffff7800>"
	if att ~= nil then
	    ret = ret .. "\\n  "
		for k, v in pairsByKeys(att,compAtt) do
			if  att_table[k] then
				stratt = string.gsub(att_table[k], "#", GetValueDisplay(k, v))
				ret = ret .. "\\n" .. stratt
			end
		end
	end

	--属性百分比加成
	local attpct = Tooltip.GetEquipAttributePct(obj)
	if attpct ~= nil then
		for k, v in pairs(att) do
			if  att_table[k] then
				stratt = string.gsub(att_table[k], "#", GetValueDisplayPct(k, v))
				ret = ret .. "\\n" .. stratt
			end
		end
	end

	-- 装备强化属性
	local equipConsolidateAtt = Tooltip.GetEqipConsolidateAtt(obj)
	if equipConsolidateAtt ~= nil then
		ret = ret .. "\\n  "
		for k, v in pairs(equipConsolidateAtt) do
			if att_table[k] ~= nil then
				ret = ret .. "\\n" .. string.gsub(att_table[k], "#", GetValueDisplay(k, v))
			end
		end
	end

	--熔铸属性加成
 	local attrong = Tooltip.GetCastingAttribute(obj)
	if attrong ~= nil then
		for k, v in pairsByKeys(attrong,compAtt) do
			if  att_table[k] then
				stratt = string.gsub(att_table[k], "#", GetValueDisplay(k, v))
				ret = ret .. "\\n" .. stratt
			end
		end
	end

	-- 圣灵属性相关
	local isShengLing = Tooltip.IsHolyMan(proto)
	if isShengLing ~= nil then
		if isShengLing == 1 then
			--表示是圣灵装备，开始显示圣灵属性
			local shengling1,shengling2,shengling3,shengling4,shengling5,shengling6,shengling7,shengling8,shengling9,shengling10,shengling11,shengling12,shengling13,shenglingequiptable = Tooltip.GetHolyManAtt(obj)
			ret = ret .. "\\n\\n\\n\\n" .. "\\n<color=0xffffbc00>" .. "吞噬等级  " .. "<color=0xff05ff00>" .. shengling2 .. "<color=0xffffbc00>/" .. shengling12
			
			ret = ret .. "\\n" .. "\\n" .. "<color=0xffffbc00>灵能  " .. "<color=0xff05ff00>"  .. shengling13 .. "<color=0xffffbc00>/" .. shengling11 
			if shengling4 ~= 0 then
				ret = ret .. "\\n\\n" .. "<color=0xff00a2ff>战力  " .. shengling4
			end
			if shengling5 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "防护  " .. shengling5
			end
			if shengling6 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "必杀  " .. shengling6
			end
			if shengling7 ~= 0 then 
				ret = ret .. "\\n<color=0xff00a2ff>" .. "必杀效果  " .. shengling7
			end
			if shengling8 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "狂暴  " .. shengling8
			end
			if shengling9 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "命中  " .. shengling9
			end
			if shengling10 ~= 0 then 
				ret = ret .. "\\n<color=0xff00a2ff>" .. "敏锐  " .. shengling10
			end

			ret = ret .. "\\n\\n\\n\\n\\n<color=0xffffbc00>刻印圣纹"
			if shenglingequiptable ~= nil then
				if shenglingequiptable[0] ~= nil then
					ret = ret .. "\\n" .. shenglingequiptable[0]
				end
				if shenglingequiptable[1] ~= nil then
					ret = ret .. "\\n" .. shenglingequiptable[1]
				end
				if shenglingequiptable[2] ~= nil then
					ret = ret .. "\\n" .. shenglingequiptable[2]
				end
				if shenglingequiptable[3] ~= nil then
					ret = ret .. "\\n" .. shenglingequiptable[3]
				end
				if shenglingequiptable[4] ~= nil then
					ret = ret .. "\\n" .. shenglingequiptable[4]
				end
				if shenglingequiptable[5] ~= nil then
					ret = ret .. "\\n" .. shenglingequiptable[5]
				end
			end

			--ret = ret .. "\\n" .. "默契  " .. shengling1
			--ret = ret .. "\\n" .. "吞噬妖精次数  " .. shengling2
			--ret = ret .. "\\n" .. "可装备数量  " .. shengling3
			--ret = ret .. "\\n" .. "圣灵伤害  " .. shengling4
			--ret = ret .. "\\n" .. "圣灵防御  " .. shengling5
			--ret = ret .. "\\n" .. "致命  " .. shengling6
			--ret = ret .. "\\n" .. "致命量  " .. shengling7
			--ret = ret .. "\\n" .. "伤害加深  " .. shengling8
			--ret = ret .. "\\n" .. "精准  " .. shengling9
			--ret = ret .. "\\n" .. "灵巧  " .. shengling10
			--ret = ret .. "\\n" .. "灵能  " .. shengling11

		end
	end

	-- 圣灵装备属性
	local isShengLingEquip = Tooltip.IsHolyManEquip(proto)
	if isShengLingEquip ~= nil then
		if isShengLingEquip == 1 then
			--开始显示圣灵装备属性
			local ShengLingEquip1,ShengLingEquip2,ShengLingEquip3,ShengLingEquip4,ShengLingEquip5,ShengLingEquip6,ShengLingEquip7,ShengLingEquip8,ShengLingEquip9 = Tooltip.GetHolyEquipAtt(obj)
			ret = ret .. "\\n\\n\\n\\n\\n<color=0xffffbc00>" .. "圣灵吞噬等级要求 " .. "<color=0xff05ff00>" .. ShengLingEquip9
			
			ret = ret .. "\\n<color=0xffffbc00>" .. "消耗灵能  " .. "<color=0xff05ff00>" .. ShengLingEquip1
			ret = ret .. "\\n\\n\\n\\n"
			if ShengLingEquip2 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "战力  +" .. ShengLingEquip2
			end
			if ShengLingEquip3 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "防护  +" .. ShengLingEquip3
			end
			if ShengLingEquip4 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "必杀  +" .. ShengLingEquip4
			end
			if ShengLingEquip5 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "必杀效果  +" .. ShengLingEquip5
			end
			if ShengLingEquip6 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "狂暴  +" .. ShengLingEquip6
			end
			if ShengLingEquip7 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "命中  +" .. ShengLingEquip7
			end
			if ShengLingEquip8 ~= 0 then
				ret = ret .. "\\n<color=0xff00a2ff>" .. "敏锐  +" .. ShengLingEquip8
			end
		end
	end

	-- 槽
	if halls ~= nil and hallrewarded~=nil and hallrewardtypes~=nil and hallreward~=nil then
		for k, v in pairs(halls) do
			if hallrewardtypes[k] ~= nill and hallreward[k] ~= nil then -- 奖励属性和奖励值不能为空
				if linzhu[k] ~= nil then
					ret = ret .. "\\n" .. "<color=0xffffbc00>" .. linzhu[k] .. " "
					-- 属性
					if k == 1 then
						if lizhurewardtypes1[1] ~= nil then
							ret = ret .. string.gsub(att_table[lizhurewardtypes1[1]], "#", GetValueDisplay(lizhurewardtypes1[1], lizhurewards1[1]))
						end
						if lizhurewardtypes1[2] ~= nil then
							ret = ret .. " " ..string.gsub(att_table[lizhurewardtypes1[2]], "#", GetValueDisplay(lizhurewardtypes1[2], lizhurewards1[2]))
						end
						if lizhurewardtypes1[3] ~= nil then
							ret = ret .. " " .. string.gsub(att_table[lizhurewardtypes1[3]], "#", GetValueDisplay(lizhurewardtypes1[3], lizhurewards1[3]))
						end
					end

					if k ==2 then
						if lizhurewardtypes2[1] ~= nil then
							ret = ret .. string.gsub(att_table[lizhurewardtypes2[1]], "#", GetValueDisplay(lizhurewardtypes2[1], lizhurewards2[1]))
						end
						if lizhurewardtypes2[2] ~= nil then
							ret = ret .. " " ..string.gsub(att_table[lizhurewardtypes2[2]], "#", GetValueDisplay(lizhurewardtypes2[2], lizhurewards2[2]))
						end
						if lizhurewardtypes2[3] ~= nil then
							ret = ret .. " " .. string.gsub(att_table[lizhurewardtypes2[3]], "#", GetValueDisplay(lizhurewardtypes2[3], lizhurewards2[3]))
						end
					end

					if k ==3 then
						if lizhurewardtypes3[1] ~= nil then
							ret = ret .. string.gsub(att_table[lizhurewardtypes3[1]], "#", GetValueDisplay(lizhurewardtypes3[1], lizhurewards3[1]))
						end
						if lizhurewardtypes3[2] ~= nil then
							ret = ret .. " " ..string.gsub(att_table[lizhurewardtypes3[2]], "#", GetValueDisplay(lizhurewardtypes3[2], lizhurewards3[2]))
						end
						if lizhurewardtypes3[3] ~= nil then
							ret = ret .. " " .. string.gsub(att_table[lizhurewardtypes3[3]], "#", GetValueDisplay(lizhurewardtypes3[3], lizhurewards3[3]))
						end
					end

					if k ==4 then
						if lizhurewardtypes4[1] ~= nil then
							ret = ret .. string.gsub(att_table[lizhurewardtypes4[1]], "#", GetValueDisplay(lizhurewardtypes4[1], lizhurewards4[1]))
						end
						if lizhurewardtypes4[2] ~= nil then
							ret = ret .. " " ..string.gsub(att_table[lizhurewardtypes4[2]], "#", GetValueDisplay(lizhurewardtypes4[2], lizhurewards4[2]))
						end
						if lizhurewardtypes4[3] ~= nil then
							ret = ret .. " " .. string.gsub(att_table[lizhurewardtypes4[3]], "#", GetValueDisplay(lizhurewardtypes4[3], lizhurewards4[3]))
						end
					end

					if k ==5 then
						if lizhurewardtypes5[1] ~= nil then
							ret = ret .. string.gsub(att_table[lizhurewardtypes5[1]], "#", GetValueDisplay(lizhurewardtypes5[1], lizhurewards5[1]))
						end
						if lizhurewardtypes5[2] ~= nil then
							ret = ret .. " " ..string.gsub(att_table[lizhurewardtypes5[2]], "#", GetValueDisplay(lizhurewardtypes5[2], lizhurewards5[2]))
						end
						if lizhurewardtypes5[3] ~= nil then
							ret = ret .. " " .. string.gsub(att_table[lizhurewardtypes5[3]], "#", GetValueDisplay(lizhurewardtypes5[3], lizhurewards5[3]))
						end
					end



					-- 属性结束
				end
				ret = ret .. "\\n" .. wuxing_hall[v+1] .. " " 	-- 槽名字
				if hallrewarded[k] then -- 激活
					ret = ret .. "<color=0xff05ff00>"
					ret = ret .. string.gsub(att_table[hallrewardtypes[k]], "#", GetValueDisplay(hallrewardtypes[k], hallreward[k]))
				else	-- 非激活
					ret = ret .. "<color=0xff8d8d8d>"
					ret = ret .. string.gsub(att_table2[hallrewardtypes[k]], "#", GetValueDisplay(hallrewardtypes[k], hallreward[k])).."[未觉醒]"
				end
			end
		end
	end
	-- 切合奖励
	if wuxingReward ~= nil then
		ret = ret .."\\n".."\\n" -- 空行
		for i=1,3 do
			if wuxingReward[i] ~= nil then
				ret = ret .."\\n".. wuxing_qiehe_type[i] .. " "
				if wuxingRewarded[i] then -- 激活
					ret = ret .."<color=0xff05ff00>"
				else	-- 非激活
					ret = ret .."<color=0xff8d8d8d>"
				end
				ret = ret .. wuxingReward[i]
			end
		end
	end

 	-- 妖精技能
	if skillnames ~= nil then
		ret = ret .."\\n".."\\n" -- 空行
		ret = ret .."\\n".. "<color=0xff05ff00>"
		for i=1,5 do
			if skillnames[i] ~= nil then
				ret = ret .."\\n".. skillnames[i]
			end
		end
	end

	--妖精资质上限提升，妖精潜能
	local isOpenYaoJingQianNeng = Tooltip.GetIsFaBaoQianNengOpen(obj)
	if isOpenYaoJingQianNeng ~= nil then
		if isOpenYaoJingQianNeng == 1 then
			ret = ret .. "\\n" .. "<color=0xff05ff00>潜能已开启"
		end
	end

	--宝石增幅
	local diamondLevel = Tooltip.GetDiamondAddLevel(obj)
	if diamondLevel ~= nil and diamondLevel ~= 0 then
			ret = ret .. "\\n<color=0xffffbc00>宝石增幅：提升" .. "<color=0xff05ff00>" ..diamondLevel .. "<color=0xffffbc00>" .. "级以下的宝石属性加成" .. "<color=0xff05ff00>" .. diamondLevel .. "0%" 
	end

	--宝石
	local gem
	local num,max = Tooltip.GetEquipHoldNum(obj)

	gem = "<font=龙字体,0,13,20>"
	if max > 0 then
			gem = gem .. "<color=0xffffffff>"
			gem = gem .. "\\n镶嵌孔数量 " .. max
			for i=1,max do
					local name,desc,qual = Tooltip.GetEquipHoldGem(obj,i-1)
					if name and desc and qual >=0 and qual <= 6 then
							gem = gem .. "\\n" .. quality_color[qual] .. name .. "<color=0xff05ff00>" .. " " .. desc
					end
			end
	end
	--判断是否有宝石属性
	if gem then
		ret = ret .. "\\n  " .. gem
	end

	--龙附
	--[[
	local long = "龙魂属性"
	for i=1,5 do
			local new = Tooltip.GetEquipLongfu(obj,i-1)
			if new > 0 then
					long = long .. "\\n " .. wuxing[i] .. "龙魂 " .. new
			end
	end
	--表龙魂能力
	local outer,name,desc = Tooltip.GetEquipLonghun(obj,false)
	if outer then
		local flag,pos, posR = Tooltip.GetEquipLonghunActive(obj,false)
		 if flag == true then
		 		 long = long .. quality_color[1]
		 else
		 		 long = long .. quality_color[255]
		 end
		 long = long .. "\\n[表]" .. name .. " " .. desc
		 if posR ~= -1 then
			long = long .. "\\n 左手需要 " .. equip_pos[pos+1]
			long = long .. "\\n 右手需要 " .. equip_pos[posR+1]
		 else
			long = long .. "\\n 需要 " .. equip_pos[pos+1]
		 end
		 long = long .. "\\n "
		 for i=1,5 do
				local new = Tooltip.GetEquipLonghunFive(outer,i-1)
				if new > 0 then
						long = long .. " " .. wuxing[i] .. new
				end
		end
	end
	--里龙魂能力
	local inner,name,desc = Tooltip.GetEquipLonghun(obj,true)
	if inner then
		local flag,pos1,pos2,pos1R,pos2R = Tooltip.GetEquipLonghunActive(obj,true)
		 if flag == true then
		 		 long = long .. quality_color[1]
		 else
		 		 long = long .. quality_color[255]
		 end
		 long = long .. "\\n[里]" .. name .. " " .. desc

		 if pos1R ~= -1 then
		 	long = long .. "\\n 左手需要 " .. equip_pos[pos1+1] .. " + " .. equip_pos[pos2+1]
			long = long .. "\\n 右手需要 " .. equip_pos[pos1R+1] .. " + " .. equip_pos[pos2R+1]
		 else
			 long = long .. "\\n 需要 " .. equip_pos[pos1+1] .. " + " .. equip_pos[pos2+1]
		 end
		 long = long .. "\\n "
		 for i=1,5 do
				local new = Tooltip.GetEquipLonghunFive(inner,i-1)
				if new > 0 then
						long = long .. " " .. wuxing[i] .. new
				end
		end
	end

	--判断是否有龙附属性
	if long ~= "龙魂属性" then
			ret = ret .. "\\n<color=0xffffffff>  \\n" .. long
			long = nil
	end
	--]]

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

	--特殊属性
	--[[
	local new = Tooltip.GetEquipSpecAttribute(obj)
	if new>0 and new<25 then
			ret = ret .. "\\n \\n<color=0xff05ff00>" .. spec_att_table[new]
	end
	--]]

	ret = ret .. "<font=龙字体,0,16,20><color=0xffffffff>"
	--颜色
	local color = Tooltip.GetEquipColor(obj)
	if color then
		ret = ret .. "\\n当前颜色: " .. color_table[color]
	end
	--描述
	ret = ret .. "\\n \\n" .. Tooltip.GetEquipDescEx(proto)

	ret = ret .. "<font=龙字体,0,16,20><color=0xffff0000>"
	--是否可吸魂
	if Tooltip.IsCanXiHun(proto) == true then
			ret = ret .. "\\n" .. "不可吸魂"
	end

	--是否可强化
	local nStrLv = Tooltip.GetEquipMaxStrengthenLevel(obj);
	if nStrLv == 0 then
			ret = ret .. "<font=龙字体,0,16,20><color=0xffff0000>"
			ret = ret .. "\\n" .. "不可强化"
	elseif nStrLv < 20 then
			ret = ret .. "\\n<color=0xffffbc00>最多可强化至 <color=0xff00ff00>" .. nStrLv .. " <color=0xffffbc00>级"
	end

	local bSignatrued = Tooltip.IsEquipSignatured(obj)
	if bSignatrued ~= nil and bSignatrued ~= false then
		local strPlayer = Tooltip.GetEquipSignaturePlayer(obj)
		local strSignatrueDesc = Tooltip.GetEquipSignature(obj)
		if strPlayer ~= nil then
			ret = ret .. "\\n<color=0xff06d401>".. strPlayer .. ":"
		end
		if( strSignatrueDesc ~= nil ) then
			ret = ret .. "\\n" .. strSignatrueDesc
		end
	else
		--生产者，如果装备用IM道具署名，生产者署名被覆盖
		local creator = Tooltip.GetItemCreator(obj)
		if creator ~= nil then
			ret = ret .. "\\n  \\n由" .. "<color=0xff06d401>" .. Tooltip.GetItemCreator(obj) .. "<color=0xffffffff>" .. "生产"
		end
	end

    local count = select(2,string.gsub(ret, "\\n", "\\n"))

	if count > 42 then
	    ret = string.gsub(ret, "<font=龙字体,0,16,20>", "<font=龙字体,0,13,20>")
		ret = string.gsub(ret, "<font=龙字体,0,20,20>", "<font=龙字体,0,16,20>")
	end

	return ret
end
