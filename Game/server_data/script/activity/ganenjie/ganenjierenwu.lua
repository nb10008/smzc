


--完成任务
function q30883_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	Quest_Exp = {}
	Quest_Exp[50]=243000
	Quest_Exp[51]=255000
	Quest_Exp[52]=265000
	Quest_Exp[53]=276000
	Quest_Exp[54]=289000
	Quest_Exp[55]=314000
	Quest_Exp[56]=327000
	Quest_Exp[57]=339000
	Quest_Exp[58]=352000
	Quest_Exp[59]=388000
	Quest_Exp[60]=507000
	Quest_Exp[61]=525000
	Quest_Exp[62]=544000
	Quest_Exp[63]=564000
	Quest_Exp[64]=582000
	Quest_Exp[65]=606000
	Quest_Exp[66]=627000
	Quest_Exp[67]=648000
	Quest_Exp[68]=670000
	Quest_Exp[69]=720000
	Quest_Exp[70]=1073000
	Quest_Exp[71]=1107000
	Quest_Exp[72]=1143000
	Quest_Exp[73]=1180000
	Quest_Exp[74]=1217000
	Quest_Exp[75]=1256000
	Quest_Exp[76]=1293000
	Quest_Exp[77]=1331000
	Quest_Exp[78]=1370000
	Quest_Exp[79]=2116000
	Quest_Exp[80]=2178000
	Quest_Exp[81]=2241000
	Quest_Exp[82]=2303000
	Quest_Exp[83]=2370000
	Quest_Exp[84]=2438000
	Quest_Exp[85]=2505000
	Quest_Exp[86]=2572000
	Quest_Exp[87]=2640000
	Quest_Exp[88]=2709000
	Quest_Exp[89]=2782000
	Quest_Exp[90]=3490000
	Quest_Exp[91]=3546000
	Quest_Exp[92]=3602000
	Quest_Exp[93]=3662000
	Quest_Exp[94]=3720000
	Quest_Exp[95]=3777000
	Quest_Exp[96]=3833000
	Quest_Exp[97]=3887000
	Quest_Exp[98]=3941000
	Quest_Exp[99]=3996000
	Quest_Exp[100]=4050000
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level >= 100 then
		level = 100
	end
	if(QuestID == 30883) then
		role.AddRoleExp(MapID, InstanceID, RoleID, Quest_Exp[level])
	elseif(QuestID == 30884) then
		role.AddRoleExp(MapID, InstanceID, RoleID, 1.2*Quest_Exp[level])
	elseif(QuestID == 30885) then
		role.AddRoleExp(MapID, InstanceID, RoleID, 1.5*Quest_Exp[level])
	elseif(QuestID == 30886) then
		role.AddRoleExp(MapID, InstanceID, RoleID, 1.8*Quest_Exp[level])
	elseif(QuestID == 30887) then
		role.AddRoleExp(MapID, InstanceID, RoleID, 2*Quest_Exp[level])
	end
end

aux.RegisterQuestEvent(30883, 1, "q30883_OnComplete")
aux.RegisterQuestEvent(30884, 1, "q30883_OnComplete")
aux.RegisterQuestEvent(30885, 1, "q30883_OnComplete")
aux.RegisterQuestEvent(30886, 1, "q30883_OnComplete")
aux.RegisterQuestEvent(30887, 1, "q30883_OnComplete")
