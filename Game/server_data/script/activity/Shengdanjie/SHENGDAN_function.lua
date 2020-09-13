--在斗战天城刷新NPC新年宴会管家，新年宴会供应商于14:00、17:00、21:00、刷新生存时间为2小时
--圣诞节活动期间（2010-12-14至2011-1-14）  圣诞树
--每日的12：30、16：30、20：30会开始“6030001”

Xinniandayan_start = 0
sheng_dan_shu = 0 --记录永久圣诞树
shengdan_yanhuishang = 0
shengdan_shu = 0

function yuandan_OnTimerMin(actID)
	--local curmon = tonumber(os.date("%m"))
	--local curday = tonumber(os.date("%d"))
	local curmin = tonumber(os.date("%M"))
	local curhour = tonumber(os.date("%H"))
	--local curweekyear = tonumber(os.date("%Y"))

--刷新圣诞树
	if sheng_dan_shu == 0 then
		sheng_dan_shu = map.MapCreateCreature(3424073512, -1, 5600019, 616, 22900, 534)-- 28163   23510   29803
	end

    if curmin == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300019)    --圣诞树出现在斗战天城的的<563，596>，30级以上的玩家可以到圣诞树处领取圣诞礼包和圣诞祝福
		msg.DispatchWorldMsgEvent(MsgID)
	end

--神秘圣诞树刷新
	if ((curhour == 12 or curhour == 16 or curhour == 20) and curmin >= 30) or (curhour == 13 or curhour == 17 or curhour == 21) or ((curhour == 14 or curhour == 18 or curhour == 22) and curmin < 30) then
		if shengdan_shu == 0 then
				local a = math.random(1,3)
				if a == 1 then
					shengdanshu = map.MapCreateColCreature(3424073256, -1, 6030001, 894, 2872, 1237)--七花之海s03  44741   2873   61871
					unit.AddBuff(3424073256, -1, shengdanshu, 6936601, shengdanshu)
					unit.AddBuff(3424073256, -1, shengdanshu, 6936801, shengdanshu)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 300020)    --神秘圣诞树已经出现在七花之海的<894, 1237>处，快拿手中的圣诞袜去取得树圣诞礼物吧！6030001可取1000次礼品，当取完以后，还会有各种特殊事件！神秘圣诞树只存在2小时，大家请抓紧！
					msg.DispatchWorldMsgEvent(MsgID)
				elseif a == 2 then
					shengdanshu1 = map.MapCreateColCreature(3424071976, -1, 6030001, 646, 14111, 1027)--奔雷裂谷s04   32337   14170  50737
					unit.AddBuff(3424071976, -1, shengdanshu1, 6936601, shengdanshu1)
					unit.AddBuff(3424071976, -1, shengdanshu1, 6936801, shengdanshu1)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 300021)    --神秘圣诞树已经出现在奔雷裂谷的<646, 1027>处，快拿手中的圣诞袜去取得树圣诞礼物吧！6030001可取1000次礼品，当取完以后，还会有各种特殊事件！神秘圣诞树只存在2小时，大家请抓紧！
					msg.DispatchWorldMsgEvent(MsgID)
				elseif a == 3 then
					shengdanshu2 = map.MapCreateColCreature(3424071720, -1, 6030001, 1201, 10476, 1236)--北洲天雪s05    60055   10475   61821
					unit.AddBuff(3424071720, -1, shengdanshu2, 6936601, shengdanshu2)
					unit.AddBuff(3424071720, -1, shengdanshu2, 6936801, shengdanshu2)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 300022)    --神秘圣诞树已经出现在北洲天雪的<1201, 1236>处，快拿手中的圣诞袜去取得树圣诞礼物吧！6030001可取1000次礼品，当取完以后，还会有各种特殊事件！神秘圣诞树只存在2小时，大家请抓紧！
					msg.DispatchWorldMsgEvent(MsgID)
				end
			shengdan_shu = 1
			end
	elseif (curhour == 14 or curhour == 18 or curhour == 22) and curmin == 30 then
	    if shengdan_shu ~= 0 then
			map.MapDeleteCreature(shengdan_shu_Map, -1, shengdan_shu)
			shengdan_shu = 0
		end
	elseif (curhour == 12 or curhour == 16 or curhour == 20) and curmin == 25 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300023)    --神秘圣诞树即将出现在七花之海、奔雷裂谷或者北洲天雪，快拿手中的圣诞袜去取得树圣诞礼物吧！神秘圣诞树可取1000次礼品，当取完以后，还会有各种特殊事件！神秘圣诞树只存在2小时，大家请抓紧！
		msg.DispatchWorldMsgEvent(MsgID)
	else
	    shengdan_shu = 0
	end
end

--新年
aux.RegisterActEvent(34, 4, "yuandan_OnTimerMin")




function Xinnian_OnTimerMin(actID)

	local curmin = tonumber(os.date("%M"))
	local curhour = tonumber(os.date("%H"))




--刷新NPC 新年大宴会
	if (curhour == 14 or curhour == 17 or curhour == 21) and curmin < 31 then
		if Xinniandayan_start == 0 then

			Xinniandayan_start = 1
			for q=0,14 do                       --容错性修改
				act.SetActScriptData(36, 1, q, 0)
				act.SaveActScriptData(36)
			end
		end
		--local CompNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"])
		--role.SetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"], 0)
        --shengdan_yanhuishang = map.MapCreateCreature(3424073512, -1, 5600015, 531, 23510, 621)-- 26583  23510  31082


	else
	    Xinniandayan_start = 0
	    if (curhour ==13 or curhour == 16 or curhour == 20 ) and curmin == 55 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300018)    --新年宴会供应商即将出现在斗战天城的<531，621>，只要交纳新年大餐就可获得各种奖励，交付最多的前三位玩家还有机会获得特殊大礼！
			msg.DispatchWorldMsgEvent(MsgID)
		elseif (curhour == 14 or curhour == 17 or curhour == 21) and curmin == 31 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 300028)    --新年宴会已经收集到了足够的食物，请大家准备好新年大餐等待下个时间段的新年宴会吧！
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end

end
aux.RegisterActEvent(36, 4, "Xinnian_OnTimerMin")

--[[新年活动结束
function Xinniandayan_OnEnd(actID)
	local CompNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"])
	role.SetRoleScriptData(RoleID, 1, RoleDataType["Xinnianshipin"], 0)

end
aux.RegisterActEvent(38, 3, "Xinniandayan_OnEnd")
aux.RegisterActEvent(39, 3, "Xinniandayan_OnEnd")
aux.RegisterActEvent(40, 3, "Xinniandayan_OnEnd")]]

--圣诞活动结束
function shengdan_OnEnd(actID)
	map.MapDeleteCreature(3424073512, -1, sheng_dan_shu)--删除永久圣诞树
end
aux.RegisterActEvent(34, 3, "shengdan_OnEnd")

--圣诞活动结束
function yuandan_OnEnd(actID)
    map.MapDeleteCreature(3424073512, -1, shengdan_yanhuishang)
	map.MapDeleteCreature(3424073512, -1, shixian_hecheng)--删除永久圣诞树
	for q=0,14 do                       --容错性修改
		act.SetActScriptData(36, 1, q, 0)
		act.SaveActScriptData(36)
	end
end
aux.RegisterActEvent(36, 3, "yuandan_OnEnd")

