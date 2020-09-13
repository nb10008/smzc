function c6040033_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650002)--¸ÅÊö
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)
		msg.AddMsgEvent(MsgID, 1, 650003)--Ñý¾«ÍÌÊÉ
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 650004)--¿ÌÓ¡/Ï´Ó¡Ê¥ÎÆ
		msg.AddMsgEvent(MsgID, 21, 6)
		msg.AddMsgEvent(MsgID, 1, 650005)--Ê¥ÎÆ³äÄÜ
		msg.AddMsgEvent(MsgID, 21, 7)
		msg.AddMsgEvent(MsgID, 1, 650006)--Ê¥ÎÆ»½ÐÑ/ÝÍÈ¡
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650009)--¹Ø±Õ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650011)--Ñý¾«ÍÌÊÉËµÃ÷
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 650010)--·µ»Ø
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650009)--¹Ø±Õ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650012)--¿ÌÓ¡/Ï´Ó¡Ê¥ÎÆËµÃ÷
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 650010)--·µ»Ø
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650009)--¹Ø±Õ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650013)--Ê¥ÎÆ³äÄÜËµÃ÷
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 650010)--·µ»Ø
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650009)--¹Ø±Õ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 7 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 650014)--Ê¥ÎÆ»½ÐÑ/ÝÍÈ¡ËµÃ÷
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, -1)
		msg.AddMsgEvent(MsgID, 1, 650010)--·µ»Ø
		msg.AddMsgEvent(MsgID, 21, 10)
		msg.AddMsgEvent(MsgID, 1, 650009)--¹Ø±Õ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end

aux.RegisterCreatureEvent(6040033, 7, "c6040033_On_Talk")
