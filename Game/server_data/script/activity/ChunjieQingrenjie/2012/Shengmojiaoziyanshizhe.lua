function n6040019_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444444) -- "春风吹，战鼓擂，整个三界谁怕谁。大家好，我是活动使者，欢迎大家在这辞旧迎新的时刻参加圣魔饺子宴。祝大家新年新气象，经验多多，奖励多多，金钱多多。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 7)
			msg.AddMsgEvent(MsgID, 1, 444445) -- “我想了解活动内容是神马。”
			msg.AddMsgEvent(MsgID, 21, 8)
			msg.AddMsgEvent(MsgID, 1, 444446)   --"我想了解活动什么时候开始？"
			msg.AddMsgEvent(MsgID, 21, 9)
			msg.AddMsgEvent(MsgID, 1, 444447)   --"我想知道活动奖励给力不？
			msg.AddMsgEvent(MsgID, 21, 10)
			msg.AddMsgEvent(MsgID, 1, 444448)   --"使者你好，使者再见！"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	elseif TalkIndex == 7 then
		local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444449) -- "整个过程很容易，只要你收集饺子交给<D>沸腾的神锅</D>就可以了：<I>水晶饺子</I>可以从<M>新春熊猫</M>身上以及<N>诺亚方舟</N>副本收集。<I>高汤饺子</I>可以从<D>善财使者</D>那里得到。至于其他的途径，就暂时保密啦。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 444450) -- “好的，我知道了。”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	elseif TalkIndex == 8 then
		local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444451) -- "在春节期间每天的<S>【0:00】【6:00】【12:00】【18:00】</S>将开启圣魔饺子宴活动，在这些时刻之前希望诸位能够尽力帮助我收集散落在三界大陆的饺子，交与<D>沸腾的神锅</D>。而每天的【5：00—5：59】【11：00—11：59】【17：00—17：59】【23：00—23：59】时间段则是活动特殊奖励时间，排名前三名的玩家可以找<D>沸腾的神锅</D>领取特殊奖励，千万不要错过。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 444450) -- “好的，我知道了。”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	elseif TalkIndex == 9 then
		local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444452)--"必须给力!在活动时间在，每次上交5个饺子可以获得一定的奖励（可重复）。而在每次活动特殊奖励时间，根据上交饺子数量的不同（分为三档：大于等于500个；大于等于1000个；大于等于3000个），我将给予全服斗神档次不同的奖励！友情提示：排名前三名的玩家可以找<D>沸腾的神锅</D>领取特殊奖励，千万不要错过。"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- 确定按钮
			msg.AddMsgEvent(MsgID, 1, 444450) -- “好的，我知道了。”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	end
end
aux.RegisterCreatureEvent(6040019, 7, "n6040019_OnTalk")
