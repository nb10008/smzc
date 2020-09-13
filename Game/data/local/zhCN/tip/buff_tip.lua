local type_table = {[2]="固定伤害",[3]="衰弱",[4]="控制",[5]="治疗",[6]="强化",[7]="减速",[8]="食品",[9]="药品",[15]="坐骑",[16]="宠物"}

local ONE_DAY	 	= 24*60*60
local ONE_HOUR	 	= 60*60
local ONE_MINUTE 	= 60

local function GetTimeValue(time)
                if time <= 0 then
                          time = 0
                end               
                 

		if time > ONE_DAY then
			return	string.format("%d天",time/ONE_DAY)
		elseif time > ONE_HOUR then
		  	return string.format("%d小时",time/ONE_HOUR)
		elseif time > ONE_MINUTE then
		  	return string.format("%d分钟",time/ONE_MINUTE)
		else
		  	return string.format("%d秒",time)
		end
end

function GetObjString(obj,proto)

	local ret = "<font=龙字体,0,20,20><color=0xffffffff>"
	--名称
	ret = ret .. Tooltip.GetBuffName(proto)

	ret = ret .. "<font=龙字体,0,16,20>"
	--类型
	local type = type_table[Tooltip.GetBuffType(proto)];
	if type then
			ret = ret .. "\\n<color=0xffff7800>" .. type .. "<color=0xffff7800>类状态"
	end

	--描述
	ret = ret .. "\\n \\n" .. Tooltip.GetBuffDesc(proto)

	--剩余时间
  local remain = Tooltip.GetBuffRemainTime(obj)
  if remain == 0 then
			ret = ret .. "\\n<color=0xff05ff00>永久<color=0xffffffff>"
	else
			ret = ret .. "\\n剩余<color=0xff05ff00>" .. GetTimeValue(remain)
	end

	return ret
end

