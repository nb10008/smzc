local sex_table = {[0] = "女性", [1] = "男性"}
local quality_color = {[0]="<color=0xffffffff>",[1]="<color=0xfffff717>",[2]="<color=0xff05ff00>",[3]="<color=0xff00a2ff>",[4]="<color=0xffff7800>",[5]="<color=0xff9932cd>",[6]="<color=0xffff80c0>",[255]="<color=0xff8d8d8d>"}
local usetype_table =
{
	"右键可使用",
	"右键点击，再对背包中装备进行使用",
	"右键点击，再对背包中时装进行使用",
	"将该物品放置于行囊中，或强化界面的辅料格中使用",
	"将该物品放置于时装合成界面的辅料格中使用"
}
local equip_pos = {"武器","头防","上衣","下装","手腕","足鞋","披风","妖精","项链","戒指","腰饰","魂晶"}
local clan_table = {[0]="轩辕","神农","伏羲","三苗","九黎","月氏","女娲","共工"}
local reput_table = {[0]="仇视","嫌恶","冷漠","中立","友善","亲密","信赖","尊敬","崇拜","传奇"}

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

function GetObjString(proto,known,nameid)

	local ret = "<font=龙字体,0,20,20><color=0xffffffff>"

	local quality = Tooltip.GetItemQuality(0,proto)
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
	--是否是技能书
	local skill = Tooltip.GetItemLearnSkill(proto)
	if skill then
		--职业限制

		--前置技能
		local pre,level,bhave = Tooltip.GetSkillPreSkill(0,skill,0)
		if pre and level then
				ret = ret .. "\\n技能要求 "
				if not bhave then
						ret = ret .. "\\n<color=0xffff402d>" .. pre .. " " .. level .. "级" .. "<color=0xffffffff>"
				else
						ret = ret .. "\\n" ..pre .. " " .. level .. "级"
				end
				for i=1, 9 do
					local pre2,level2,bhave2 = Tooltip.GetSkillPreSkill(obj,proto,i)
					if pre2 and level2 then
						if not bhave2 then
							ret = ret .. "\\n<color=0xffff402d>" .. pre2 .. " " .. level2 .. "级" .. "<color=0xffffffff>"
						else
								ret = ret .. "\\n" ..pre2 .. " " .. level2 .. "级"
						end
					end
				end
		end
		--是否已学习
		local flag = Tooltip.GetSkillIsLearn(skill)
		if flag then
			ret = ret .. "\\n<color=0xffff402d>已学习"
		end
	end
	--空行
	ret = ret .. "\\n    "
	ret = ret .. "<color=0xffffffff>"
	--使用次数
	local maxusetimes = Tooltip.GetItemMaxUseTimes(proto)
	if maxusetimes > 1 then

			ret = ret .. "\\n使用次数 " .. maxusetimes .. "/" .. maxusetimes

	end

	--强化类道具
	local potval = Tooltip.GetItemCostPotVal(proto)
	if potval~=nil and potval > 0 then
			ret = ret .. "\\n消耗" .. potval .. "点装备强化值"

			ret = ret .. "\\n强化部位 ".."<color=0xff05ff00>"
			for i=0,11 do
					by = Tooltip.GetItemHoldEquipPos(proto,i)
					if by~=nil and by == 1 then
							j = i+1
							ret = ret .. equip_pos[j].. " "
					end
			end
			ret = ret .. "\\n ".."<color=0xffffffff>"
	end

	--道具镶嵌
	if Tooltip.ShowGoodsUsePart(proto) == true then
			ret = ret .. "\\n可镶嵌部位 ".."<color=0xff05ff00>"
			for i=0,11 do
					by = Tooltip.GetItemHoldEquipPos(proto,i)
					if by~=nil and by == 1 then
							j = i+1
							ret = ret .. equip_pos[j].. " "
					end
			end
			ret = ret .."<color=0xffffffff>"
	end

	--道具镶嵌等级
	local level = Tooltip.ShowGoodsUseLevel(proto)
	if level ~= nil and level > 0 then
			ret = ret .. "\\n可镶嵌装备最低等级<color=0xfffff717> " .. level .."<color=0xffffffff>"
	end

	--起手时间
	local new = Tooltip.GetItemPreTime(proto)
	if new ~= nil and new > 0 then
		ret = ret .. "\\n \\n起手时间<color=0xff05ff00> " .. new / 1000 .. "<color=0xffffffff> 秒"
	end

	--恢复时间
	local cdTime = Tooltip.GetItemCoolDownTime(proto)
	if cdTime > 0 then
		ret = ret .. "\\n冷却时间<color=0xff00a2ff> " .. GetFloat(cdTime/1000,0.1) .. "<color=0xffffffff> 秒"
	end

		--妖精秘药药效
	local power = Tooltip.GetProtoFairyPower(proto)
	if power > 0 then
		ret = ret .. "\\n秘药药效<color=0xfffff717> " .. power .."<color=0xffffffff> "
	end

	--描述
	ret = ret .. "\\n \\n" .. Tooltip.GetItemDesc(proto)

	--使用描述
	local usetype = Tooltip.GetItemUseType(proto)
	if usetype then
			local usetypedesc = usetype_table[usetype]
			if usetypedesc then
					ret = ret .. "\\n \\n" .. usetypedesc
			end
	end

	--剩余时间
	local totalTimeLimit = Tooltip.GetItemProtoAllTimeLimit(proto)
	if totalTimeLimit ~= nil and totalTimeLimit > 0 then
			ret = ret .. "\\n剩余 " .. GetTimeDisplayLite(totalTimeLimit) .. "到期"
	end

	return ret
end

