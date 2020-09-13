local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin,"一、百鬼炼狱说明" );
	Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin,"二、勇闯魔窟说明" );
	Quest.SetNPCTalkOption( EOT_ScenarioTalk,2,ScenarioTalkBegin,"三、魔族入侵说明" );
	Quest.SetAcceptOptions(5511017);			
	Quest.SetCompleteOptions(5511017);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>：\\n    关于天界的一些活动，来问我就可以了。");
end

local function OnScenarioTalk(id,step)
	if( id == 0 ) then
		if( step == ScenarioTalkBegin ) then
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,QuestTalkBegin+1,"活动时间" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,QuestTalkBegin+2,"具体内容" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+3, "返回" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+10000, "离开" );
	        Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n   你想要知道什么呢？");
		elseif( step == QuestTalkBegin+1 )then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>百鬼炼狱活动时间：<color=0xffffffff>\\n 每日<C>19：00-20：00</C>。");
		elseif( step == QuestTalkBegin+2 )then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>百鬼炼狱具体内容：<color=0xffffffff>\\n<C>40—100</C>级的玩家在活动开始后，在斗战天城NPC<D>“百鬼修炼人”</D>处可接取<P>“挑战百鬼炼狱”</P>的任务；玩家每日只能接取<C>1</C>次，但是每日最多可<C>9</C>次进出副本；玩家必须拥有<C>4</C>人以上的队伍才可进入百鬼炼狱。进入副本后，队伍内任意玩家与<D>“百鬼封印”</D>对话，可选择<I>“释放鬼怪”</I>，然后出现数个小怪和BOSS；队伍每次将出现的怪物杀死之后，会刷出新的怪物，可进行<C>10</C>次刷新，在一定的次数怪物等级提升，并且刷出BOSS。每个百鬼副本只有前<C>6</C>个玩家可以交付任务。");
                elseif( step == ScenarioTalkBegin+3 ) then --返回上级界面
			OnTalk();
		elseif( step == ScenarioTalkBegin+10000 ) then --关闭窗口
			Quest.CloseNPCTalk();
		end
	elseif( id == 1 ) then
		if( step == ScenarioTalkBegin ) then
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,QuestTalkBegin+1,"活动时间" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,QuestTalkBegin+2,"具体内容" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+3, "返回" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+10000, "离开" );
	        Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n   你想要知道什么呢？");
		elseif( step == QuestTalkBegin+1 )then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>勇闯魔窟活动时间：<color=0xffffffff>\\n每日<C>09：00、13：00、18：00、23：00</C>开始。共<C>4</C>次。每段活动持续时间为<C>1</C>小时。");
		elseif( step == QuestTalkBegin+2 )then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>勇闯魔窟具体内容:<color=0xffffffff>\\n活动开始后，在<N>奔雷裂谷</N>和<N>北洲天雪</N>会出现<D>“魔窟之门”</D>，<C>30</C>级以上的玩家必须以组队的形式参加，并且不得少于<C>3</C>人；玩家在组队的状态下，与<D>“魔窟之门”</D>对话就可以进入该活动。进入<D>“魔窟之门”</D>之后，与<D>魔窟使者</D>对话，可以接到任务；玩家在活动中击杀所有怪之后出现BOSS，将其击杀后可离开副本在斗战天城内的<D>“勇创魔窟嘉奖人”</D>处交付任务。");
                elseif( step == ScenarioTalkBegin+3 ) then --返回上级界面
			OnTalk();
		elseif( step == ScenarioTalkBegin+10000 ) then --关闭窗口
		end
	elseif( id == 2 ) then
		if( step == ScenarioTalkBegin ) then
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,2,QuestTalkBegin+1,"活动时间" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,2,QuestTalkBegin+2,"具体内容" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+3, "返回" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+10000, "离开" );
	        Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n   你想要知道什么呢？");
		elseif( step == QuestTalkBegin+1 )then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>魔族入侵活动时间：<color=0xffffffff>\\n每日<C>00：30、8：30、12：30、16：30、20：30</C>开始，共<C>5</C>次。每段活动持续时间为<C>1</C>小时。");
		elseif( step == QuestTalkBegin+2 )then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>魔族入侵具体内容:<color=0xffffffff>\\n活动开始的时候，会在除<N>天神塚</N>、<N>斗战天城</N>、<N>七花之海</N>之外的所有场景都会出现<A>“入侵魔物”</A>，玩家<C>3</C>人以上进行组队，与<A>“入侵魔物”</A>对话就可进行挑战魔物。挑战开始之后，会在周围出现魔物头目和精英，消灭之后完成该活动。。");
                elseif( step == ScenarioTalkBegin+3 ) then --返回上级界面
			OnTalk();
		elseif( step == ScenarioTalkBegin+10000 ) then --关闭窗口
		end
	end
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30601)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30601,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30602)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30602,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30603)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30603,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30604)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30604,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30605)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30605,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30606)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30606,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30607)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30607,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30608)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30608,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30609)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30609,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30610)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30610,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30611)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30611,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30612)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30612,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30613)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30613,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30614)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30614,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n找到魔窟的入口，进入魔窟消灭里面的恶魔。如果要压制住魔窟，就必须杀死魔窟首领才可以。");
	elseif( id == 30615)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30615,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30616)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30616,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30617)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30617,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30618)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30618,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30619)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30619,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30620)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30620,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30621)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30621,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30622)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30622,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30623)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30623,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30624)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30624,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30625)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30625,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30626)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30626,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n每天晚上，百鬼修炼人都会来到斗战天城，让大家进行百鬼修炼。你也去试试自己的实力吧，当然不用勉强，你能通过百年魔王的挑战我就很欣慰了，后面就看你自己了。");
	elseif( id == 30627)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30627,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30628)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30628,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30629)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30629,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30630)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30630,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30631)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30631,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30632)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30632,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30633)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30633,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30634)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30634,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30635)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30635,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30636)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30636,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30637)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30637,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30638)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30638,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30639)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30639,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30640)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30640,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30641)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30641,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30642)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30642,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	elseif( id == 30643)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30643,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔族还是经常会入侵人界与天界，魔族既然是有备而来，单枪匹马的话肯定要吃亏的。如果你遇到入侵的魔族，一定记住要与其他人组队才能阻止它们。");
	end
end



local function OnCompleteQuestTalk(id,step)
	if( id == 30601)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30601,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");
	elseif( id == 30602)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30602,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");
	elseif( id == 30603)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30603,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");	
	elseif( id == 30604)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30604,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");	
	elseif( id == 30605)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30605,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");	
	elseif( id == 30606)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30606,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");	
	elseif( id == 30607)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30607,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");	
	elseif( id == 30608)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30608,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");	
	elseif( id == 30609)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30609,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");	
	elseif( id == 30610)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30610,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");	
	elseif( id == 30611)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30611,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");
	elseif( id == 30612)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30612,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");	
	elseif( id == 30613)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30613,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");
	elseif( id == 30614)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30614,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n魔窟应该还会再次出现，你要做好准备呀。");
	elseif( id == 30615)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30615,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30616)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30616,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30617)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30617,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");
	elseif( id == 30618)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30618,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30619)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30619,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30620)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30620,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30621)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30621,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30622)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30622,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30623)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30623,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30624)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30624,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30625)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30625,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30626)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30626,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n看来你通过了这个试炼，以后还要继续努力。");	
	elseif( id == 30627)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30627,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30628)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30628,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30629)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30629,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30630)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30630,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30631)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30631,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30632)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30632,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30633)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30633,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30634)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30634,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30635)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30635,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30636)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30636,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30637)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30637,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30638)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30638,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30639)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30639,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30640)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30640,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30641)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30641,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30642)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30642,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	elseif( id == 30643)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30643,"<font=龙字体,0,19,20><color=0xfffcf157>天界活动指引人：<color=0xffffffff>\\n这次入侵算是平安无事，魔族还是不会善罢甘休的，你自己也要小心。");	
	end

end

Rtianjieshuoming = {}
Rtianjieshuoming["OnTalk"] = OnTalk
Rtianjieshuoming["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rtianjieshuoming["OnCompleteQuestTalk"] = OnCompleteQuestTalk
Rtianjieshuoming["OnScenarioTalk"] = OnScenarioTalk