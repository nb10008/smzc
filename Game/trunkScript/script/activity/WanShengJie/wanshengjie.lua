
function WanShengJie_OnStart(actID)
	local k = act.GetActScriptData(30, 1, 20)
	local curyear = tonumber(os.date("%Y"))
	if k == 0 or k == nil or k ~= curyear then
	    for i=0, 19 do
		    act.SetActScriptData(30, 1, i, 0)
		end
		act.SetActScriptData(30, 1, 20, curyear)
		act.SaveActScriptData(30)
	end
end

--注册该活动的相关事件
--aux.RegisterActEvent(30, 2, "WanShengJie_OnStart")

--与幻瑶老师5600002对话
function n5600002_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206201) -- "万圣节大使：\n    哈哈，你们这帮小家伙最近是不是收集了很多糖纸啊？在11月8日24：00之前，将最多糖纸交给我的10名玩家将会的道具奖励：\n  1名为永久制万圣节时装一套，永久制猎魔蓝龙一个；\n    第2至5名为永久制万圣节时装一套，30天制猎魔蓝龙一个；\n    第6至10名为永久制万圣节时装一套，7天制猎魔蓝龙一个。\n    你已经交给我的糖纸数是：XXX张"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["TangZhi"]))
		msg.AddMsgEvent(MsgID, 21, 5) -- 确定按钮
		msg.AddMsgEvent(MsgID, 1, 206202) -- “糖纸数量排行榜（前十名）”
		msg.AddMsgEvent(MsgID, 21, 4)   --取消按钮
		msg.AddMsgEvent(MsgID, 1, 206203)   --"交付糖纸"
		if act.GetActIsStart(30) ~= true then
		    msg.AddMsgEvent(MsgID, 21, 6)   --取消按钮
			msg.AddMsgEvent(MsgID, 1, 206219)   --"领取奖励"
		end
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
    elseif TalkIndex == 4 then
	    if act.GetActIsStart(30) ~= true then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206204) -- "万圣节大使：\n    万圣节糖纸提交的时间是11月9日之前，你已经错过了时间！"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		else
			local a = role.GetRoleItemNum(RoleID, 4040006)
			if a > 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4040006, 1, 420)
				HuoDeTangZhi(RoleID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206201) -- "万圣节大使：\n    哈哈，你们这帮小家伙最近是不是收集了很多糖纸啊？在11月9日之前，将最多糖纸交给我的10名玩家将会获得意外的惊喜哦！"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 9, role.GetRoleScriptData(RoleID, 1, RoleDataType["TangZhi"]))
				msg.AddMsgEvent(MsgID, 21, 5) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 206202) -- “糖纸数量排行榜（前十名）”
				msg.AddMsgEvent(MsgID, 21, 4)   --取消按钮
				msg.AddMsgEvent(MsgID, 1, 206203)   --"交付糖纸"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 206205) -- "万圣节大使：\n    你没有糖纸，难道过万圣节你从来不和朋友分享糖果吗？"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
				msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			end
		end
    elseif TalkIndex == 5 then
	    local k = nil
		for i = 0, 9 do
		    if act.GetActScriptData(30, 1, i) ~= nil and act.GetActScriptData(30, 1, i) > 0 then
			    k = i
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206206) -- "万圣节大使：\n    快点把糖纸给我，你就是第一个获得糖纸的幸运儿了！"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 0 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206207) -- "万圣节大使：\n    哈哈，上交糖纸最多的人是：：\n    糖纸数：xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206208) -- "万圣节大使：\n    哈哈，上交糖纸最多的人是：：\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206209) -- "万圣节大使：\n    哈哈，上交糖纸最多的人是：：\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 3 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206210) -- "万圣节大使：\n    哈哈，上交糖纸最多的人是：：\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 4 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206211) -- "万圣节大使：\n    哈哈，上交糖纸最多的人是：：\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 5 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206212) -- "万圣节大使：\n    哈哈，上交糖纸最多的人是：：\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 5))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 6 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206213) -- "万圣节大使：\n    哈哈，上交糖纸最多的人是：：\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 5))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 16))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 6))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 7 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206214) -- "万圣节大使：\n    哈哈，上交糖纸最多的人是：：\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 5))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 16))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 6))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 17))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 7))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 8 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206215) -- "万圣节大使：\n    哈哈，上交糖纸最多的人是：：\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 5))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 16))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 6))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 17))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 7))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 18))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 8))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		elseif k == 9 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206216) -- "万圣节大使：\n    哈哈，上交糖纸最多的人是：：\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx\n    糖纸数：xxx  xxxxxxx
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 10))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 0))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 11))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 1))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 12))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 2))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 13))
            msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 3))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 14))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 4))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 15))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 5))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 16))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 6))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 17))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 7))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 18))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 8))
			msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(30, 1, 19))
			msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(30, 1, 9))
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		end
    elseif TalkIndex == 6 then
	    local Rank = 0
        for q=0,9 do
			if act.GetActScriptData(30, 1, q) == RoleID then
				Rank = q + 1
			end
		end
        if Rank==0 then
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206220) -- "万圣节大使：\n    前十名里面没有你的名字哦，我没有什么东西可以给你。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
        elseif act.GetActScriptData(30, 1, Rank+21) > 0 then
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206221) -- "万圣节大使：\n    你已经领取过奖励了，想重复领取和不行哦。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
        elseif role.GetBagFreeSize(RoleID) < 2 then
        	local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206222) -- "万圣节大使：\n    快去清理一下背包吧，不然是装不下奖品的哦。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 100001) -- “确定”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		else
		    local sex = role.GetRoleSex(RoleID)
			if sex == 1 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 3090032, 1, -1, 8, 420)
			elseif sex == 0 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 3090034, 1, -1, 8, 420)
			end
            if	Rank == 1 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4220604, 1, -1, 8, 420)
			elseif Rank <= 5 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4220603, 1, -1, 8, 420)
			elseif Rank <= 10 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4220602, 1, -1, 8, 420)
			end
			act.SetActScriptData(30, 1, Rank+21, 1)
			act.SaveActScriptData(30)
		end
	end
end

aux.RegisterCreatureEvent(5600002, 7, "n5600002_OnTalk")

function I4040001_OnUse(MapID, InstanceID, TypeID, RoleID)
    local i = math.random(100)
	if i <= 40 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4040002, 1, 1, 8, 420)
	elseif i <= 80 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4040003, 1, 1, 8, 420)
	elseif i <= 95 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4040004, 1, 1, 8, 420)
	else
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4040005, 1, 1, 8, 420)
	end
end

function I4040001_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4040001, 1, "I4040001_OnUse")
aux.RegisterItemEvent(4040001, 0, "I4040001_CanUse")


function I4040004_OnUse(MapID, InstanceID, TypeID, RoleID)

	role.AddRoleItem(MapID, InstanceID, RoleID, 8180001, 1, 3, 8, 420)

end

function I4040004_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4040004, 1, "I4040004_OnUse")
aux.RegisterItemEvent(4040004, 0, "I4040004_CanUse")

--纯情蔷薇
function I4040002_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
    local ItemID = 0
	local k = math.random(100)
	if k <= 80 then
	    ItemID = 4040003
	elseif k <= 90 then
	    ItemID = 4040004
	else
	    ItemID = 4040005
	end
    local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, ItemID, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4040006, 1, -1, 8, 420)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 206217)          --您收到了来自XXX的XXXX！
    msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 4, ItemID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)     --发送消息
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 0)
	msg.AddMsgEvent(MsgID, 1, 206217)          --您收到了来自XXX的XXXX！
    msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 4, ItemID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)     --发送消息
end

function I4040002_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2402)   --目标玩家行囊的空间不足
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		bRet = 32
	end
	local FreeSize1 = role.GetBagFreeSize(RoleID)
	if(FreeSize1 < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4040002, 1, "I4040002_OnUse")
aux.RegisterItemEvent(4040002, 0, "I4040002_CanUse")
