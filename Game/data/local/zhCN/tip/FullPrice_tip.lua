function GetObjString(obj,single,known)

	local ret = "<font=龙字体,0,16,20><color=0xffffffff>"

	ret = ret .. "总价  "

	--显示普通价格
	local gold,silver = Tooltip.GetPriceSilver(obj)

	if gold ~= nil or silver ~= nil then
			if gold > 999999 then
				ret = ret .. "大于 "
			end
			if gold > 0 then
					ret = ret .. gold .. " <pic=data\\ui\\bag\\l_icon-jin-s.bmp,0,0,16,16>"
			end
			if silver >= 0 then
					ret = ret .. silver .. " <pic=data\\ui\\bag\\l_icon-yin-s.bmp,0,0,16,16>"
			end
			--如果价格小于0则直接显示无法出售
			if gold < 0 or silver < 0 then
					ret = "<font=龙字体,0,16,20><color=0xffffffff>无法出售"
			end
	end

	return ret
end
