
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010029);
	Quest.SetCompleteOptions(5010029);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n我是伟大的斗神之王，一切都在我的掌控之下。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20023)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20023,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    接下来的试炼会更加艰苦，只有勇者才可通过！你接下来的任务是前往<N>七花之海</N>，一个魔幻之地。那里遭受了魔物的入侵，现在就需要你协助那里的战士击退魔物。\\n    在出发之前，<D>神使迪妮莎</D>会给你一些实用的建议，你先去找她，她就在一旁。");
        elseif( id == 20200)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20200,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    到<N>奔雷裂谷</N>去找<D>翼神琼纳斯</D>吧，那里有新的考验在等着你。");
        elseif( id == 20300)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20300,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    七花之海和奔雷裂谷的事情充分说明，你是一个值得信赖的助手。如今在人界，那些魔族更是猖狂，我希望你以我斗神的名义，下凡到人界去。讨伐危害人界的魔族，一定记住，你是以斗神的名义下凡到人界的。");
        elseif( id == 20400)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20400,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    北洲天雪已经平安无事，接下来就前往西洲幻林吧，那里的麻烦事也不少。武神贝蒂正在西洲幻林，你可以现在就去找她了。");
        elseif( id == 41000)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41000,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    你终于到达20级了，接下来是个很重要的环节，就是转职。首先对职业有个简单的了解吧，城中有四个武器之灵，分别与他们对话可以了解职业的特点。");
	elseif( id == 20500)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20500,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    接下来就前往<N>南洲迷沼</N>吧，那里也需要你去解决很多问题，魔族好像比我们先一步到达了那里，快去吧。");
	elseif( id == 20600)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20600,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    哦？魔族是这样告诉你的吗？而你就这样相信了？下位者，看来你需要了解真相，东洲冥山的智慧之神<D>奥克山</D>会告诉你一切。");
	elseif( id == 20630)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20630,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    你问的这些，我可以告诉你！但是时机未到！你只要记得你是天神就可以了！去<N>肃界沙漠</N>看看，那里有你想找的人！");
	elseif( id == 20657)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20657,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    还想回去看看那个崇拜你的人类？可以！不过先祈祷他身上的神咒没有发作吧，若能拿到<I>解咒之药</I>他尚有一线生机，否则，赐他……死！");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20022)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20022,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    神选的勇士，欢迎你来到这里，你作为战神候补虽已完成了第一次考验，但仍未达到要求，等你达到<C>20</C>级的时候，才有资格成为一名真正的战神。\\n    但是，能够通过初次考验已经值得嘉奖，你从我这里挑选一把武器吧！");
        elseif( id == 20116)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20116,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    看来你磨练的不错，在七花之海表现的很优异。");
        elseif( id == 20218)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20218,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    看来你的修炼进行的还不错啊。");
        elseif( id == 20344)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20344,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    看来你这次确实完成的非常好啊。");
	elseif( id == 20526)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20526,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    恩？！下位者！请注意你的言辞！");
	elseif( id == 20629)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20629,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    你这回又要质问我什么？");
	elseif( id == 20656)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20656,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    下位者！你在亵渎上位者的权威，现在的你还没有资格与我并驾齐驱，不过我很期待可以亲手灭却你这叛逆的灵魂！");
	elseif( id == 20428)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20428,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    看来西洲幻林的事你解决的还不错啊。");
	elseif( id == 41017)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41017,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    看来进行的很顺利呀，那么接下来是新的任务。");
	elseif( id == 41018)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41018,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    看来进行的很顺利呀，那么接下来是新的任务。");
	elseif( id == 41019)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41019,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    看来进行的很顺利呀，那么接下来是新的任务。");
	elseif( id == 41020)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41020,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    看来进行的很顺利呀，那么接下来是新的任务。");
	elseif( id == 30867)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30867,QuestTalkBegin+1,"确定" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30867,QuestTalkBegin+2,"返回" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>斗神之王玛斯：<color=0xffffffff>\\n    是春节使者米莲妮让你来给我拜年？哈哈，想不到这个米莲妮倒是挺有心的。好吧，替我谢谢她，她的心意我收下了。");	        
		elseif( step == QuestTalkBegin+1 )then
			Quest.UpdateQuest(30867);
		elseif( step == QuestTalkBegin+2 )then
			Quest.CloseNPCTalk();
		end
	end
	

end

Rmasi = {}
Rmasi["OnTalk"] = OnTalk
Rmasi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rmasi["OnCompleteQuestTalk"] = OnCompleteQuestTalk