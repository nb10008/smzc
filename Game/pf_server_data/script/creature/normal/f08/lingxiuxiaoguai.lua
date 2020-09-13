---mapscriptdata[0] 记录进副本后是否已刷出怪物
---mapscriptdata[1] 记录第一个boss的id

---mapscriptdata[2] 记录第几波怪物
---mapscriptdata[3] 记录boss1是否被杀死




function lingxiuxiaoguai_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	---回复生命吧

		
	unit.AddBuff(MapID, InstanceID,RoleID,5934901,RoleID)
		
	

end
aux.RegisterCreatureEvent(3040246, 4, "lingxiuxiaoguai_OnDie")
