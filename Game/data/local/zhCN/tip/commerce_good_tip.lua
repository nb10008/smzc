function GetObjString(obj, proto)

	local ret = "<font=龙字体,0,20,20><color=0xff05ff00>"
	--名称
	ret = ret .. Tooltip.GetCommerceName(proto)

	ret = ret .. "<font=龙字体,0,16,20><color=0xff05ff00>"
	--类型
	ret = ret .. "\\n" .. Tooltip.GetCommerceType(proto)
	
	ret = ret .. "<font=龙字体,0,16,20><color=0xffffffff>"
	--描述
	ret = ret .."\\n"
	ret = ret .."\\n"
	ret = ret .."\\n"
	ret = ret .."\\n"
	ret = ret .."\\n"
	ret = ret .. "\\n" .. Tooltip.GetCommerceDesc(proto)
	
	return ret
end


	