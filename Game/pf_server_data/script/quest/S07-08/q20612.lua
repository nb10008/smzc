--20612安魂花开
--任务采集安魂花[6010028],刷出2只花魔[1070090]
local x20612CreratureLiveFlag = 0

function c6010028_OnCheck(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    if x20612CreratureLiveFlag <= 1 then
      local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
      map.MapCreateCreature(MapID, InstanceID, 1070090, x-2, y, z-2)
      map.MapCreateCreature(MapID, InstanceID, 1070090, x+2, y, z+2)
      x20612CreratureLiveFlag = 1
    end
end

--注册
aux.RegisterCreatureEvent(6010028, 6, "c6010028_OnCheck")

--怪物消失或者死亡，将ScriptData索引 数值减1


function c1070090_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
        if x20612CreratureLiveFlag == 1 then
	   x20612CreratureLiveFlag  = 0
	end
end
aux.RegisterCreatureEvent(1070090,4,"c1070090_OnDie")
