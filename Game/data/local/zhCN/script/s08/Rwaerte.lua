
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010210);
	Quest.SetCompleteOptions(5010210);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n人间变成这般模样到底是谁的错？");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20649)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20649,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n<color=0xffff0000>毒血<color=0xffffffff>想作为第三方势力存活于神魔之间，呵呵，替魔族问候他吧！");
        elseif( id == 20650)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20650,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n你看！<A>狂风贵族</A>和<A>狂风影武</A>必然会来寻仇，先下手为强吧！");
        elseif( id == 20651)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20651,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n虽然原神教是被天神利用的可怜人，但我们也不应该放过他们的罪行！希望他们下辈子能做好人！");
	elseif( id == 20652)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20652,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n把他们的头领<color=0xffff0000>护法天伤<color=0xffffffff>和<color=0xffff0000>神使彻天左<color=0xffffffff>消灭后，再来找我吧！");	
	elseif( id == 20653)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20653,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n这是这些罪人的<I>灵魂结晶</I>，把它丢到冥山周围的<M>神像</M>里，为他们净化吧……");
        elseif( id == 20654)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20654,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n看来我已经无法假装下去，作为人类，我真的十分痛恨天神和魔族，不过你很特别，如果这个世界是你的，你还会让天神和魔族不断侵扰下去吗？");
	elseif( id == 20655)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20655,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n对了！<color=0xffff0000>彻天左<color=0xffffffff>和<color=0xffff0000>彻天右<color=0xffffffff>必须同时死亡，否则另一个又会活过来，看来你又要大展拳脚了！");
	elseif( id == 20656)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20656,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n如果您下定决心的话，就请与斗神之王玛斯摊牌吧，虽然这很危险，但我相信，以您的实力，必可超越他！");
        elseif( id == 20658)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20658,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n大人！原来我们都被骗了，其实神族和魔族只是假意互斗，他们是想灭绝人类，重新瓜分这个人间啊！");
	elseif( id == 20659)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20659,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n大人！您真的要去与地狱魔君决斗？那先请您测试一下自己的实力，如果不能达成小人所说的目标，还是不要去做无谓的牺牲……");	
	elseif( id == 20660)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20660,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n大人有所不知！<color=0xffff0000>地狱魔君<color=0xffffffff>虽然强悍，但也仅是魔王的手下，地狱魔君的死一定会惊动魔王，到时假如魔王与神王联手……大人！灭杀地狱魔君之行，还请三思啊！");  
		
		
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20648)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20648,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n只是为了生存！神也争，魔也争！我为鱼肉，人为刀俎！还能怎么办？只好投靠一方先保住性命，我们都不想像蛮人那样被不明不白地干掉……");
        elseif( id == 20649)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20649,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n一名天神有这样酷似魔族的敏捷身手，难得！");
	elseif( id == 20650)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20650,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n这下我就放心了！");
	elseif( id == 20651)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20651,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n还不够！他们的头领也不该存在于世间。");
        elseif( id == 20652)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20652,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n这下就齐了……");
	elseif( id == 20653)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20653,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n尘归尘、土归土，但愿你们可以往生极乐……");
	elseif( id == 20654)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20654,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n谁说它不是你的？只要你能扫清面前的障碍，它！就是你的，只不过这个障碍大了些！");
	elseif( id == 20655)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20655,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n……你是我们人类的希望，真诚地恳请您统治这个世界，让贪婪的天神和邪恶的魔族全都化作飞灰吧！");
	elseif( id == 20657)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20657,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n大人！您还好吧？为了我们人类，害的您与同族反目……哎……");
	elseif( id == 20658)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20658,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n看看冥山吧，上面除了神王在人间的代言人神使彻天右，更有魔王的亲信地狱魔君存在。");
	elseif( id == 20659)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20659,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n好……好强！但是……");
	elseif( id == 20660)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20660,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n大人听到魔王的怒吼了吗？看来魔王降临人间的日子……不远了……");
	elseif( id == 30348)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30348,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");
  elseif( id == 30349)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30349,"<font=龙字体,0,19,20><color=0xfffcf157>瓦尔特：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");

       end

end

Rwaerte = {}
Rwaerte["OnTalk"] = OnTalk
Rwaerte["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rwaerte["OnCompleteQuestTalk"] = OnCompleteQuestTalk