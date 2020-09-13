function OnTrigger(id)--触发器任务序列号

	if( id==1 ) then--观察变化
		if( Quest.QuestIsAccepted(20118) and (not Quest.QuestCanComplete(20118)) )then
			Quest.ShowScreenInfo("你站在花海之根的高处，感到了一股邪恶的力量就在附近。");
	elseif( id==2 ) then--踏雪峰观察变化
		if( Quest.QuestIsAccepted(20350) and (not Quest.QuestCanComplete(20350)) )then
			Quest.ShowScreenInfo("你在这踏雪峰之上，察看到了一些新的情况。");
		end
end