ganenshizhe = 0
function ganenjshizhe(curhour,curmin)
	if (curhour == 20 and curmin == 31) then
		ganenshizhe = map.MapCreateColCreature(3424073512, -1, 5610082,956,23692,727, 1)
		local MsgID2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID2, 102, 300072)--感恩使者已经出现在斗战天城（956,727）处，快去领取经验吧
		msg.DispatchBroadcast(MsgID2,-1,-1,-1)
	end
	if(curhour == 20 and curmin == 50) then
		if(ganenshizhe ~= nil and ganenshizhe ~= 0) then
			map.MapDeleteCreature(3424073512, -1, ganenshizhe)
			ganenshizhe = map.MapCreateColCreature(3424073512, -1, 5610083,956,23692,727, 1)
		end
	elseif(curhour == 21 and curmin == 10) then
		if(ganenshizhe ~= nil and ganenshizhe ~= 0) then
			map.MapDeleteCreature(3424073512, -1, ganenshizhe)
			ganenshizhe = map.MapCreateColCreature(3424073512, -1, 5610084,956,23692,727, 1)
		end
	elseif(curhour == 21 and curmin == 30) then
		if(ganenshizhe ~= nil and ganenshizhe ~= 0) then
			map.MapDeleteCreature(3424073512, -1, ganenshizhe)
			ganenshizhe = map.MapCreateColCreature(3424073512, -1, 5610085,956,23692,727, 1)
		end
	elseif(curhour == 21 and curmin == 50) then
		if(ganenshizhe ~= nil and ganenshizhe ~= 0) then
			map.MapDeleteCreature(3424073512, -1, ganenshizhe)
			ganenshizhe = map.MapCreateColCreature(3424073512, -1, 5610086,956,23692,727, 1)
		end
	elseif(curhour == 22 and curmin ==10) then
		if(ganenshizhe ~= nil and ganenshizhe ~= 0) then
			map.MapDeleteCreature(3424073512, -1, ganenshizhe)
		end
	end
end
