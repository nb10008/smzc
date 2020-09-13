--8010022	÷∆≤√÷Æ’»
function I8010022_Get(MapID, InstanceID, TypeID, TargetID, GetId, High, Low)
	local baoshiID = 0 
	local qianghuaID = 0 
	if TypeID == 8010022 or TypeID == 8020022 or TypeID == 8010023 or TypeID == 8020023 then
		baoshiID = 3030001
	elseif TypeID == 8030022 or TypeID == 8030023 or TypeID == 8040022 or TypeID == 8040023 then
		baoshiID = 3030009
	elseif TypeID == 8010024 or TypeID == 8010025 or TypeID == 8010026 or TypeID == 8020024 or TypeID == 8020025 or TypeID == 8020026 then
		baoshiID = 3030002
	elseif TypeID == 8030024 or TypeID == 8030025 or TypeID == 8030026 or TypeID == 8040024 or TypeID == 8040025 or TypeID == 8040026 then
		baoshiID = 3030010
	elseif TypeID == 8060022 then
		baoshiID = 3030029
	elseif TypeID == 8061022 then
		baoshiID = 3030037
	elseif TypeID == 8070022 or TypeID == 8060024 then
		baoshiID = 3030029
	elseif TypeID == 8071022 or TypeID == 8061024 then
		baoshiID = 3030037
	end
	if TypeID == 8010022 or TypeID == 8020022 or TypeID == 8030022 or TypeID == 8040022	then
		qianghuaID =2
	elseif TypeID == 8010023 or TypeID == 8020023 or TypeID == 8030023 or TypeID == 8040023 or TypeID == 8010024 or TypeID == 8020024 or TypeID == 8030024 or TypeID == 8040024 then
		qianghuaID = 4 
	elseif TypeID == 8060022 or TypeID == 8061022 or TypeID == 8070022 or TypeID == 8071022	then
		qianghuaID = 5
	elseif TypeID == 8010025 or TypeID == 8020025 or TypeID == 8030025 or TypeID == 8040025 or TypeID == 8060024 or TypeID == 8061024 then
		qianghuaID = 8
	elseif TypeID == 8010026 or TypeID == 8020026 or TypeID == 8030026 or TypeID == 8040026	then
		qianghuaID = 9
	end
	role.SetEquipStrengthLevel(TargetID,High,Low, qianghuaID)--«ø20
	role.SetGrowEquipLevel(TargetID,High,Low,3074)--≥…≥§ Ù–‘3074
	for i=1,5 do
		role.DigHoleOnEquip(TargetID,High,Low)--ø™ø◊
		role.PutStoneOnEquip(TargetID,High,Low,baoshiID)--œ‚±¶ Ø
	end
	if  TypeID == 8010025 or TypeID == 8020025 or TypeID == 8030025 or TypeID == 8040025 then
		for i=1,5 do
			role.SetEquipAdditionalAbility(TargetID,High,Low,3040021)--œ‚±¶◊∑º”…À∫¶
		end
	elseif TypeID == 8010026 or TypeID == 8020026 or TypeID == 8030026 or TypeID == 8040026	then
		for i=1,8 do
			role.SetEquipAdditionalAbility(TargetID,High,Low,3040021)--œ‚±¶◊∑º”…À∫¶
		end
	elseif TypeID == 8060024 or TypeID == 8061024 then
		for i=1,5 do
			role.SetEquipAdditionalAbility(TargetID,High,Low,3040056)--œ‚±¶◊∑º”…À∫¶
		end
	end

end
aux.RegisterItemEvent(8010022, 8, "I8010022_Get")
aux.RegisterItemEvent(8010023, 8, "I8010022_Get")
aux.RegisterItemEvent(8010024, 8, "I8010022_Get")
aux.RegisterItemEvent(8010025, 8, "I8010022_Get")
aux.RegisterItemEvent(8010026, 8, "I8010022_Get")
aux.RegisterItemEvent(8020022, 8, "I8010022_Get")
aux.RegisterItemEvent(8020023, 8, "I8010022_Get")
aux.RegisterItemEvent(8020024, 8, "I8010022_Get")
aux.RegisterItemEvent(8020025, 8, "I8010022_Get")
aux.RegisterItemEvent(8020026, 8, "I8010022_Get")
aux.RegisterItemEvent(8030022, 8, "I8010022_Get")
aux.RegisterItemEvent(8030023, 8, "I8010022_Get")
aux.RegisterItemEvent(8030024, 8, "I8010022_Get")
aux.RegisterItemEvent(8030025, 8, "I8010022_Get")
aux.RegisterItemEvent(8030026, 8, "I8010022_Get")
aux.RegisterItemEvent(8040022, 8, "I8010022_Get")
aux.RegisterItemEvent(8040023, 8, "I8010022_Get")
aux.RegisterItemEvent(8040024, 8, "I8010022_Get")
aux.RegisterItemEvent(8040025, 8, "I8010022_Get")
aux.RegisterItemEvent(8040026, 8, "I8010022_Get")
aux.RegisterItemEvent(8060022, 8, "I8010022_Get")
aux.RegisterItemEvent(8070022, 8, "I8010022_Get")
aux.RegisterItemEvent(8060024, 8, "I8010022_Get")
aux.RegisterItemEvent(8061022, 8, "I8010022_Get")
aux.RegisterItemEvent(8071022, 8, "I8010022_Get")
aux.RegisterItemEvent(8061024, 8, "I8010022_Get")
