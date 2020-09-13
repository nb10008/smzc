--1、召唤次数
--2、召唤时间
--3、 记录日期
touming = 0 -- 初始透明NPCID为0
function Mrbobo_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local Num = role.GetRoleItemNum(RoleID, 4800073)--罗曼之星
	--local Act = act.GetActIsStart(38)
	local curhour = tonumber(os.date("%H"))
	local lTime =  map.GetMapScriptData(MapID, InstanceID, 1, 2)--使用时间
	local cTime = os.time()--当前时间
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)--召唤个数
	local s = os.difftime(cTime,lTime) -- 距离上一次召唤时间
	local CurTime = tonumber(os.date("%j"))--日期
	local CurTime1 = map.GetMapScriptData(MapID, InstanceID, 1, 3)
	local day = os.date("%A")--星期
	if CurTime ~= CurTime1 then
		a = 0
		map.SetMapScriptData(MapID, InstanceID, 1, 1, a)
	end

	--if Act ~= true then
	--	if TalkIndex == -1 then
	--		local MsgID = msg.BeginMsgEvent()
	--		msg.AddMsgEvent(MsgID, 20, 206566)		--“博博先生：\n  情人节妖精可在本次活动内进行召唤，时间为每天的18：00-24：00。每30分钟可召唤一次，需要交纳10个罗曼之星，交纳物品2分钟后，情人节妖精会出现在博博罗曼村的某个位置。能不能遇到可就要看你的运气了。”
	--		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
	--		msg.AddMsgEvent(MsgID, 21, -1)			--取消按钮
	--		msg.AddMsgEvent(MsgID, 1, 206552)		--“活动还没有开始”
	--		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	--	end
	--elseif Act == true then
	if Num < 10 then
		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206566)		--“博博先生：\n    ？”
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, -1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 206554)		--您身上的罗曼之星数量不足10个，无法召唤妖精。
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	else
		if a > 4 then
			if TalkIndex == -1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206566)		--“博博先生：\n    ？”
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21, -1)			--取消按钮
				msg.AddMsgEvent(MsgID, 1, 206555)		--今日召唤妖精的次数已达到5次
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
			if s < 3600 then
				if TalkIndex == -1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206566)		--“博博先生：\n    ？”
					msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21, -1)			--取消按钮
					--msg.AddMsgEvent(MsgID, 1, 206556)		--距离上一次召唤妖精还没有到达30分钟
					msg.AddMsgEvent(MsgID, 1, 206565)		--距离上一次召唤妖精还没有到达1小时
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
				if TalkIndex == -1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 206557)		--“博博先生：\n 你确定要召唤妖精么？”
					msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21, 4)			--取消按钮
					msg.AddMsgEvent(MsgID, 1, 100001)		--确定
					msg.AddMsgEvent(MsgID, 21, 5)			--取消按钮
					msg.AddMsgEvent(MsgID, 1, 100002)		--取消
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif TalkIndex == 4 then
					if Num < 10 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 206566)		--“博博先生：\n    ？”
						msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
						msg.AddMsgEvent(MsgID, 21, 6)			--取消按钮
						msg.AddMsgEvent(MsgID, 1, 206554)		--您身上的罗曼之星数量不足10个，无法召唤妖精。
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
						if touming == 0 then
							role.RemoveFromRole(MapID, InstanceID, RoleID, 4800073, 10, 101)---------罗曼之星
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 102, 300041)    --[<p1>]交纳了10个罗曼之星，2分钟之后契约妖精将在博博罗曼村中出现
							msg.AddMsgEvent(MsgID, 2, RoleID)
							msg.DispatchWorldMsgEvent(MsgID)
							touming = map.MapCreateCreature(MapID, InstanceID, 6030012, 260, 2999, 79)
							local CurTime = tonumber(os.date("%j"))--日期
							local cTime = os.time()
							map.SetMapScriptData(MapID, InstanceID, 1, 2, cTime)
							map.SetMapScriptData(MapID, InstanceID, 1, 3, CurTime)
						end
					end
				end
			end
		end
	end
end
aux.RegisterCreatureEvent(5600028, 7, "Mrbobo_OnTalk")


function Qyaojing_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	local b = math.random(1,3)
	if b == 1 then
		map.MapCreateCreature(MapID, InstanceID, 5600054, 260, 3130, 350)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300042)    --情人节的妖精灵魄已经出现在博博罗曼村<260,350>处
		msg.DispatchWorldMsgEvent(MsgID)
		map.SetMapScriptData(MapID, InstanceID, 1, 1, a+1)
		touming = 0
	elseif b == 2 then
		map.MapCreateCreature(MapID, InstanceID, 5600054, 128, 3117, 199)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300043)    --情人节的妖精灵魄已经出现在博博罗曼村<128,199>处
		msg.DispatchWorldMsgEvent(MsgID)
		map.SetMapScriptData(MapID, InstanceID, 1, 1, a+1)
		touming = 0
	elseif b == 3 then
		map.MapCreateCreature(MapID, InstanceID, 5600054, 390, 2981, 238)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 300044)    --情人节的妖精灵魄已经出现在博博罗曼村<390,238>处
		msg.DispatchWorldMsgEvent(MsgID)
		map.SetMapScriptData(MapID, InstanceID, 1, 1, a+1)
		touming = 0
	end

end
aux.RegisterCreatureEvent(6030012,13,"Qyaojing_OnDie")
