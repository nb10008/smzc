--原神出现：20507
--需求1：调查祭坛6010021，刷出怪物原神咒师1060056。

function x20507_OnInvest(MapID, InstanceID, questID, RoleID,CreatureTypeID)
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	   if m == nil then
		m = 0
	   end
	     if m < 3 then
	       local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	       map.MapCreateCreature(MapID, InstanceID, 1060056, x+2, y, z+2)
	       map.SetMapScriptData(MapID,InstanceID,1,1,m+1)
	     end
	     if map.GetMapScriptData(MapID,InstanceID,1,1) >= 3 then
	       local MsgID = msg.BeginMsgEvent()
	       msg.AddMsgEvent(MsgID, 13, 0)
	       msg.AddMsgEvent(MsgID, 1, 148)
	       msg.DispatchRoleMsgEvent(RoleID, MsgID)
	       bRet = 32
	end
end


function c1060056_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,a-1)

end

aux.RegisterQuestEvent(20507, 9, "x20507_OnInvest")
aux.RegisterCreatureEvent(1060056,4,"c1060056_OnDie")



