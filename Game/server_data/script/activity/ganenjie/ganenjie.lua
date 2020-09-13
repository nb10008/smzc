--第一位记录感恩使者ID
--第二位记录圣恩仙灵的ID
--3-5位记录各种礼包发放数量
--感恩节计时器消失

function ganenjie(curhour,curmin)  --

	if (curhour == 20 and curmin == 31) then
		local ganenshizheId = map.MapCreateColCreature(3424073512, -1, 5610082,956,23692,727, 1)
		local shengenxianlingId = map.MapCreateColCreature(3424073512, -1, 5610087, 1023,23692,782, 1)
--		map.MapCreateColCreature(3424073512, -1, 1094005, 1,0,1, 1)
		local MsgID1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID1, 102, 300071)--圣恩仙灵已经出现在斗战天城（1023,782）处，快去领取祈福仙令吧
		msg.DispatchBroadcast(MsgID1,-1,-1,-1)
		local MsgID2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID2, 102, 300072)--感恩使者已经出现在斗战天城（956,727）处，快去领取经验吧
		msg.DispatchBroadcast(MsgID2,-1,-1,-1)
		act.SetActScriptData(49, 1, 1, ganenshizheId)
		act.SetActScriptData(49, 1, 2, shengenxianlingId)
	end
	local ganenshizhe = act.GetActScriptData(49, 1, 1)
	local shengenxianling = act.GetActScriptData(49, 1, 2)
	if(curhour == 20 and curmin == 40) then
		create_box(curhour,curmin)
	elseif(curhour == 20 and curmin == 50) then
		if(ganenshizhe ~= nil and ganenshizhe ~= 0) then
			map.MapDeleteCreature(3424073512, -1, ganenshizhe)
			ganenshizhe = map.MapCreateColCreature(3424073512, -1, 5610083,956,23692,727, 1)
			act.SetActScriptData(49, 1, 1, ganenshizhe)
		end
		create_box(curhour,curmin)
	elseif(curhour == 21 and curmin == 00) then
		create_box(curhour,curmin)
	elseif(curhour == 21 and curmin == 10) then
		if(ganenshizhe ~= nil and ganenshizhe ~= 0) then
			map.MapDeleteCreature(3424073512, -1, ganenshizhe)
			ganenshizhe = map.MapCreateColCreature(3424073512, -1, 5610084,956,23692,727, 1)
			act.SetActScriptData(49, 1, 1, ganenshizhe)
			local MsgID2 = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID2, 102, 206913)--祈福结束的玩家可以去领取礼包了
			msg.DispatchBroadcast(MsgID2,-1,-1,-1)
		end
	elseif(curhour == 21 and curmin == 30) then
		if(ganenshizhe ~= nil and ganenshizhe ~= 0) then
			act.SetActScriptData(49, 1, 3, 0)
			act.SetActScriptData(49, 1, 4, 0)
			act.SetActScriptData(49, 1, 5, 0)
			map.MapDeleteCreature(3424073512, -1, ganenshizhe)
			ganenshizhe = map.MapCreateColCreature(3424073512, -1, 5610085,956,23692,727, 1)
			act.SetActScriptData(49, 1, 1, ganenshizhe)
		end
		if(shengenxianling ~= nil and shengenxianling ~= 0) then
			map.MapDeleteCreature(3424073512, -1, shengenxianling)
			act.SetActScriptData(49, 1, 2, 0)
		end
	elseif(curhour == 21 and curmin == 40) then
		create_box(curhour,curmin)
	elseif(curhour == 21 and curmin == 50) then
		if(ganenshizhe ~= nil and ganenshizhe ~= 0) then
			map.MapDeleteCreature(3424073512, -1, ganenshizhe)
			ganenshizhe = map.MapCreateColCreature(3424073512, -1, 5610086,956,23692,727, 1)
			act.SetActScriptData(49, 1, 1, ganenshizhe)
		end
		create_box(curhour,curmin)
	elseif(curhour == 22 and curmin == 00) then
		create_box(curhour,curmin)
	elseif(curhour == 22 and curmin ==10) then
		if(ganenshizhe ~= nil and ganenshizhe ~= 0) then
			map.MapDeleteCreature(3424073512, -1, ganenshizhe)
			act.SetActScriptData(49, 1, 1, 0)
		end
	end
end



function create_box(curhour,curmin)
	local tempTable = {}
	local Baoxiang = {}
	Baoxiang[1] = {x = 906, y = 23510, z = 816}
	Baoxiang[2] = {x = 841, y = 23510, z = 815}
	Baoxiang[3] = {x = 792, y = 23510, z = 836}
	Baoxiang[4] = {x = 729, y = 23510, z = 856}
	Baoxiang[5] = {x = 664, y = 23510, z = 828}
	Baoxiang[6] = {x = 969, y = 27095, z = 1007}
	Baoxiang[7] = {x = 905, y = 27095, z = 1017}
	Baoxiang[8] = {x = 897, y = 27095, z = 951}
	Baoxiang[9] = {x = 951, y = 27095, z = 965}
	Baoxiang[10] = {x = 935, y = 23510, z = 596}
	Baoxiang[11] = {x = 833, y = 23510, z = 600}
	Baoxiang[12] = {x = 791, y = 23510, z = 627}
	Baoxiang[13] = {x = 794, y = 23510, z = 677}
	Baoxiang[14] = {x = 795, y = 23510, z = 737}
	Baoxiang[15] = {x = 836, y = 23510, z = 669}
	Baoxiang[16] = {x = 865, y = 23510, z = 636}
	Baoxiang[17] = {x = 931, y = 23510, z = 674}
	Baoxiang[18] = {x = 990, y = 23510, z = 732}
	Baoxiang[19] = {x = 991, y = 23510, z = 803}
	Baoxiang[20] = {x = 954, y = 23510, z = 768}
	Baoxiang[21] = {x = 642, y = 22902, z = 534}
	Baoxiang[22] = {x = 687, y = 22902, z = 527}
	Baoxiang[23] = {x = 732, y = 22902, z = 536}
	Baoxiang[24] = {x = 735, y = 23510, z = 588}
	Baoxiang[25] = {x = 770, y = 22902, z = 547}
	Baoxiang[26] = {x = 817, y = 22902, z = 523}
	Baoxiang[27] = {x = 866, y = 22902, z = 543}
	Baoxiang[28] = {x = 918, y = 22902, z = 530}
	Baoxiang[29] = {x = 397, y = 22980, z = 489}
	Baoxiang[30] = {x = 463, y = 22980, z = 487}
	Baoxiang[31] = {x = 433, y = 22980, z = 447}
	Baoxiang[32] = {x = 398, y = 22980, z = 407}
	Baoxiang[33] = {x = 466, y = 22980, z = 406}
	Baoxiang[34] = {x = 436, y = 22902, z = 366}
	Baoxiang[35] = {x = 513, y = 22902, z = 364}
	Baoxiang[36] = {x = 572, y = 22902, z = 365}
	Baoxiang[37] = {x = 615, y = 22902, z = 323}
	Baoxiang[38] = {x = 614, y = 22902, z = 416}
	Baoxiang[39] = {x = 613, y = 22902, z = 461}
	Baoxiang[40] = {x = 615, y = 22902, z = 511}
	Baoxiang[41] = {x = 403, y = 23510, z = 706}
	Baoxiang[42] = {x = 442, y = 23510, z = 673}
	Baoxiang[43] = {x = 398, y = 23510, z = 641}
	Baoxiang[44] = {x = 439, y = 23510, z = 616}
	Baoxiang[45] = {x = 375, y = 23510, z = 589}
	Baoxiang[46] = {x = 354, y = 22902, z = 550}
	Baoxiang[47] = {x = 448, y = 23510, z = 586}
	Baoxiang[48] = {x = 389, y = 22902, z = 528}
	Baoxiang[49] = {x = 431, y = 22902, z = 545}
	Baoxiang[50] = {x = 465, y = 22902, z = 540}
	Baoxiang[51] = {x = 504, y = 22902, z = 529}
	Baoxiang[52] = {x = 536, y = 22902, z = 546}
	Baoxiang[53] = {x = 583, y = 22902, z = 533}
	Baoxiang[54] = {x = 317, y = 27106, z = 1040}
	Baoxiang[55] = {x = 256, y = 27106, z = 1014}
	Baoxiang[56] = {x = 270, y = 27106, z = 964}
	Baoxiang[57] = {x = 320, y = 27106, z = 942}
	Baoxiang[58] = {x = 553, y = 23510, z = 830}
	Baoxiang[59] = {x = 499, y = 23510, z = 846}
	Baoxiang[60] = {x = 434, y = 23510, z = 848}
	Baoxiang[61] = {x = 430, y = 23510, z = 792}
	Baoxiang[62] = {x = 375, y = 23510, z = 775}
	Baoxiang[63] = {x = 337, y = 23510, z = 811}
	Baoxiang[64] = {x = 312, y = 23510, z = 780}
	Baoxiang[65] = {x = 300, y = 23510, z = 721}
	Baoxiang[66] = {x = 347, y = 23510, z = 749}
	Baoxiang[67] = {x = 438, y = 23510, z = 740}
	for i=1,67 do
		tempTable[i] = i
	end
	for i=1,27 do
		table.remove(tempTable,math.random(#tempTable))
	end
	for k,v in pairs(tempTable) do
		map.MapCreateColCreature(3424073512, -1,6011025 , Baoxiang[v].x,Baoxiang[v].y,Baoxiang[v].z, 1)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 300073)--宝箱出现
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end
