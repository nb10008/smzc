--幽怨妖精之魂
--完成任务
function q30305_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["YYYJCollect"], role.GetRoleScriptData(RoleID, 1, RoleDataType["YYYJCollect"])+1)
	CheckFaith(MapID, InstanceID, 150, RoleID)
	--role.AddRoleItem(MapID, InstanceID, TargetID, 4810005, 10, 0, 0, 104)
end

function q30305_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local CurTime = tonumber(os.date("%j"))
	local LastTime = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["YYYJCollect"])/10)
	if CurTime ~= LastTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["YYYJCollect"], CurTime*10)
	end
	local Num = role.GetRoleScriptData(RoleID, 1, RoleDataType["YYYJCollect"])%10
	if Num >= 2 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440001)			--2次
		msg.AddMsgEvent(MsgID, 9, 2)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	else
		return 1
	end
end

aux.RegisterQuestEvent(30322, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30322, 4, "q30305_CheckAccept")
aux.RegisterQuestEvent(30323, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30323, 4, "q30305_CheckAccept")
aux.RegisterQuestEvent(30324, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30324, 4, "q30305_CheckAccept")
aux.RegisterQuestEvent(30325, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30325, 4, "q30305_CheckAccept")
aux.RegisterQuestEvent(30326, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30326, 4, "q30305_CheckAccept")
aux.RegisterQuestEvent(30327, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30327, 4, "q30305_CheckAccept")
aux.RegisterQuestEvent(30328, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30328, 4, "q30305_CheckAccept")
aux.RegisterQuestEvent(30329, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30329, 4, "q30305_CheckAccept")
aux.RegisterQuestEvent(30330, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30330, 4, "q30305_CheckAccept")
aux.RegisterQuestEvent(30331, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30331, 4, "q30305_CheckAccept")
aux.RegisterQuestEvent(30332, 1, "q30305_OnComplete")
aux.RegisterQuestEvent(30332, 4, "q30305_CheckAccept")
