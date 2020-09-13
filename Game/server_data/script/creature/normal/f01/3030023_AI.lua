---mapscriptdata[1] 记录进副本后是否已刷出怪物
--[2]记录老一是否已杀死
--[3]记录老二是否已杀死
--[4]记录老三是否已杀死
--[5]记录老四是否已杀死
--[6]记录是否刷出宝箱
--[11]记录是否刷出30级怪物
--[12]记录是否刷出30级宝箱

function s5055702_Cast(MapID, InstanceID, SkillID, OwnerID)

	if MapID==1266372430 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 10056)
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
		local position={{x=292,y=1,z=71},{x=311,y=1,z=62},{x=328,y=1,z=76},{x=312,y=1,z=99},{x=342,y=1,z=69}}
		
		local positionID=math.random(1,5)

		
		if TargetID~=-1 then

			
			role.RoleGotoNewMap(MapID, InstanceID, TargetID, MapID,  position[positionID].x, position[positionID].y, position[positionID].z)
			map.MapCreateCreature(MapID, InstanceID, (3030023+math.random(1,2)), position[positionID].x+2, position[positionID].y, position[positionID].z+2)
			unit.AddBuff(MapID, InstanceID, TargetID,5055702,OwnerID)
			unit.AddBuff(MapID, InstanceID, TargetID,5055603,OwnerID)  
		end
	end

end
aux.RegisterSkillEvent(5055702, 1, "s5055702_Cast")



function c3030023_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10050)
	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	map.OpenCloseDoor(MapID, InstanceID, 1110, nil)
	
end

aux.RegisterCreatureEvent(3030023, 2, "c3030023_OnEnterCombat")

function c3030023_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
end
aux.RegisterCreatureEvent(3030023, 3, "c3030023_OnLeaveCombat")


---死亡刷出老三
function c3030023_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10051)
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100100)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)


	local a = map.GetMapScriptData(MapID,InstanceID,1,12)
	if a == 0 then	
		map.MapCreateCreature(MapID, InstanceID, 6010049, 338, 50, 56)
		map.MapCreateCreature(MapID, InstanceID, 5010232, 348, 50, 66)
		map.SetMapScriptData(MapID,InstanceID,1,12,1)
	end

	

end

aux.RegisterCreatureEvent(3030023, 4, "c3030023_OnDie")



