function R51001_OnComplete(MapID, InstanceID, QuestID, TargetID, NPCID)
	unit.AddBuff(MapID,InstanceID,TargetID,9800101,TargetID)
	role.OpenFramePage(TargetID, 1)
end

function R51005_OnComplete(MapID, InstanceID, QuestID, TargetID, NPCID)
	unit.AddBuff(MapID,InstanceID,TargetID,9800201,TargetID)
	role.OpenFramePage(TargetID, 1)
end

function R51006_OnComplete(MapID, InstanceID, QuestID, TargetID, NPCID)
	unit.AddBuff(MapID,InstanceID,TargetID,9800301,TargetID)
	role.OpenFramePage(TargetID, 1)
end
function R51007_OnComplete(MapID, InstanceID, QuestID, TargetID, NPCID)
	role.OpenFramePage(TargetID, 1)
end
aux.RegisterQuestEvent(51001, 1, "R51001_OnComplete")
aux.RegisterQuestEvent(51005, 1, "R51005_OnComplete")
aux.RegisterQuestEvent(51006, 1, "R51006_OnComplete")
aux.RegisterQuestEvent(51007, 1, "R51007_OnComplete")

function R51005_CheckAccept(MapID, InstanceID, QuestID, TargetID, NPCID)
	if role.IsRoleHaveBuff(MapID,InstanceID,TargetID,98001) == true then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 650001)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		return 0
	else
		return 1
	end
end

function R51006_CheckAccept(MapID, InstanceID, QuestID, TargetID, NPCID)
	if role.IsRoleHaveBuff(MapID,InstanceID,TargetID,98002) == true then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 650001)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		return 0
	else
		return 1
	end
end

function R51007_CheckAccept(MapID, InstanceID, QuestID, TargetID, NPCID)
	if role.IsRoleHaveBuff(MapID,InstanceID,TargetID,98003) == true then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 650001)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		return 0
	else
		return 1
	end
end

aux.RegisterQuestEvent(51005, 4, "R51005_CheckAccept")
aux.RegisterQuestEvent(51006, 4, "R51006_CheckAccept")
aux.RegisterQuestEvent(51007, 4, "R51007_CheckAccept")
