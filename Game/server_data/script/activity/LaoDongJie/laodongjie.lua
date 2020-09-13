xiedai_num = {}
xiedai_num[3424071976] = 0
xiedai_num[3424071720] = 0
xiedai_num[3424072488] = 0
landuo_num = {}
landuo_num[3424071976] = 0
landuo_num[3424071720] = 0
landuo_num[3424072488] = 0

laodongjie_pos = {}
laodongjie_pos[3424071976] = {}
laodongjie_pos[3424071976][1] = {x = 702,y = 14090,z = 916}
laodongjie_pos[3424071976][2] = {x = 700,y = 14071,z = 933}
laodongjie_pos[3424071976][3] = {x = 698,y = 14086,z = 949}
laodongjie_pos[3424071976][4] = {x = 689,y = 14124,z = 953}
laodongjie_pos[3424071976][5] = {x = 684,y = 14113,z = 948}
laodongjie_pos[3424071976][6] = {x = 685,y = 14102,z = 937}
laodongjie_pos[3424071976][7] = {x = 678,y = 14089,z = 928}
laodongjie_pos[3424071976][8] = {x = 671,y = 14095,z = 936}
laodongjie_pos[3424071976][9] = {x = 671,y = 14095,z = 948}
laodongjie_pos[3424071976][10] = {x = 673,y = 14100,z = 963}
laodongjie_pos[3424071720] = {}
laodongjie_pos[3424071720][1] = {x = 2602,y = 7400,z = 2707}
laodongjie_pos[3424071720][2] = {x = 2600,y = 7400,z = 2695}
laodongjie_pos[3424071720][3] = {x = 2598,y = 7400,z = 2680}
laodongjie_pos[3424071720][4] = {x = 2609,y = 7400,z = 2678}
laodongjie_pos[3424071720][5] = {x = 2626,y = 7400,z = 2675}
laodongjie_pos[3424071720][6] = {x = 2635,y = 7397,z = 2680}
laodongjie_pos[3424071720][7] = {x = 2633,y = 7400,z = 2696}
laodongjie_pos[3424071720][8] = {x = 2622,y = 7400,z = 2698}
laodongjie_pos[3424071720][9] = {x = 2620,y = 7400,z = 2708}
laodongjie_pos[3424071720][10] = {x = 2635,y = 7400,z = 2706}
laodongjie_pos[3424072488] = {}
laodongjie_pos[3424072488][1] = {x = 1071,y = 3800,z = 1363}
laodongjie_pos[3424072488][2] = {x = 1065,y = 3800,z = 1373}
laodongjie_pos[3424072488][3] = {x = 1059,y = 3800,z = 1383}
laodongjie_pos[3424072488][4] = {x = 1043,y = 3796,z = 1384}
laodongjie_pos[3424072488][5] = {x = 1050,y = 3800,z = 1374}
laodongjie_pos[3424072488][6] = {x = 1059,y = 3800,z = 1364}
laodongjie_pos[3424072488][7] = {x = 1054,y = 3800,z = 1353}
laodongjie_pos[3424072488][8] = {x = 1046,y = 3800,z = 1357}
laodongjie_pos[3424072488][9] = {x = 1038,y = 3800,z = 1364}
laodongjie_pos[3424072488][10] = {x = 1031,y = 3796,z = 1378}

--杀掉特殊小怪再刷新的
function c1100009_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    local k1 = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
	local k2 = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)
	if TargetTypeID == 1100009 then
	    if xiedai_num[k1] ~= nil then
			xiedai_num[k1] = xiedai_num[k1] + 1
		end
	else
	    if landuo_num[k1] ~= nil then
			landuo_num[k1] = landuo_num[k1] + 1
		end
	end
	if k1 ~= 0 and k2 ~= 0 and ((TargetTypeID == 1100009 and xiedai_num[k1] and xiedai_num[k1] <= 70) or (TargetTypeID == 1100010 and landuo_num[k1] and landuo_num[k1] <= 70)) then
		local ID = map.MapCreateColCreature(k1, -1, TargetTypeID, laodongjie_pos[k1][k2].x, laodongjie_pos[k1][k2].y, laodongjie_pos[k1][k2].z, 1)
		cre.SetCreatureScriptData(k1, -1, ID, 1, 0, k1)
		cre.SetCreatureScriptData(k1, -1, ID, 1, 1, k2)
	end
end


aux.RegisterCreatureEvent(1100009, 4, "c1100009_OnDie")
aux.RegisterCreatureEvent(1100010, 4, "c1100009_OnDie")
