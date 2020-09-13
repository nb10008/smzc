local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5800075);
	Quest.SetCompleteOptions(5800075);

	if (Quest.QuestCanComplete(40052) and (not Quest.QuestIsCompleted(40052))) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff>\\n第一:种植数量不能超过你的农场等级！  \\n当你种植的植物数量和你的农场等级相同时，就不能继续种植了。多多种植，去提升你的农场等级吧！  \\n第二:同一地点种植时需要间隔15分钟！  \\n如果你刚刚在一个种植点采摘了你的植物，那么，你需要间隔15分钟才可在这个点继续种植！你可以等待这个时间，或者重新找个新的种植点继续种植！  \\n第三:拉芙艾娜的礼物：爱之泉！  \\n作为妖精神女，拉芙艾娜拥有无可匹敌的生命力量，用她的礼物，可以瞬间催熟你的植株，达到快速种植的目的！  \\n第四:不劳而获！  \\n你看到了，对那些不及时采摘自己果实的人，你可以毫无心理负担地去偷摘他们的果实了，不过，每个植株只能偷摘一次吖！  \\n第五:果实枯萎！  \\n虽然是妖精植物，但是它们也是会枯萎的，特别是在果实成熟之后！请及时采摘你的果实！否则1小时后它们将会枯萎消失！  \\n第六:快速升级你的农场！  \\n如果想快点升级你的农场，以便种植更高级的植物的话；那么让我来告诉你诀窍！多搜集一些妖精山泉，用它们帮助你的朋友们浇水，你会获得更多的农场经验！");
	else
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff>\\n虽然说我是庄主，但是我的土地你也可以来种。");
	end
	
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40052)then
               if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+1,"介绍内容" );
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+2,"离开" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff>\\n下面我先给你讲一讲关于这妖精农场的事情吧。");
		elseif( step == QuestTalkBegin+2 )then
			Quest.CloseNPCTalk();
		elseif( step == QuestTalkBegin+1 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+3,"我已经知道了" );
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+4,"稍等，我再看看 ");
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>第一:种植数量不能超过你的农场等级！<color=0xffffffff>\\n当你种植的植物数量和你的农场等级相同时，就不能继续种植了。多多种植，去提升你的农场等级吧！");
		elseif( step == QuestTalkBegin+3 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+5,"我已经知道了" );
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+6,"稍等，我再看看 ");
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>第二:同一地点种植时需要间隔15分钟！<color=0xffffffff>\\n如果你刚刚在一个种植点采摘了你的植物，那么，你需要间隔15分钟才可在这个点继续种植！你可以等待这个时间，或者重新找个新的种植点继续种植！");
		elseif( step == QuestTalkBegin+5 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+7,"我已经知道了" );
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+8,"稍等，我再看看 ");
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>第三:拉芙艾娜的礼物：爱之泉！<color=0xffffffff>\\n作为妖精神女，拉芙艾娜拥有无可匹敌的生命力量，用她的礼物，可以瞬间催熟你的植株，达到快速种植的目的！");
		elseif( step == QuestTalkBegin+7 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+9,"我已经知道了" );
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+10,"稍等，我再看看" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>第四:不劳而获！<color=0xffffffff>\\n你看到了，对那些不及时采摘自己果实的人，你可以毫无心理负担地去偷摘他们的果实了，不过，每个植株只能偷摘一次吖！");
		elseif( step == QuestTalkBegin+9 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+11,"我已经知道了" );
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+12,"稍等，我再看看 ");
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>第五:果实枯萎！<color=0xffffffff>\\n虽然是妖精植物，但是它们也是会枯萎的，特别是在果实成熟之后！请及时采摘你的果实！否则1小时后它们将会枯萎消失！");
		elseif( step == QuestTalkBegin+11 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+13,"我已经知道了" );
			Quest.SetNPCTalkOption(EOT_AcceptQuestTalk,40052,QuestTalkBegin+14,"稍等，我再看看 ");
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>第六:快速升级你的农场！<color=0xffffffff>\\n如果想快点升级你的农场，以便种植更高级的植物的话；那么让我来告诉你诀窍！多搜集一些妖精山泉，用它们帮助你的朋友们浇水，你会获得更多的农场经验！");
		elseif( step == QuestTalkBegin+13 )then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(40052,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff>\\n介绍完毕，那么现在你先去买一个香果种吧，在妖精农场的食物加工台附近可以找到厨房管理员，他那里就有香果种")
		
		end
	elseif( id == 40053)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40053,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff>\\n拿着刚才的那个香果种，到那边的种植区去种下这颗香果种，成功种下之后再回来找我，我会告诉你接下来要做的事情。");
       elseif( id == 40054)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40054,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff>\\n浇水的话，首先就是要…………有水可浇。看到那边的井了吧，从那里打一桶水，然后到你种植的土坑进行浇水。");
	elseif( id == 40055)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40055,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff>\\n你可以选择是使用爱之泉还是慢慢等待你植物的果实成熟，不过不知道芙希希等得及等不及。将你种植的果实拿给我就行，那是制作火桃酱面包的必备材料之一。");
       elseif( id == 40056)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40056,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff>\\n重要的原材料已经有了，接下来看看火桃酱面包需要的材料。火桃酱面包：香果粉1个、火桃酱1个。香果粉的话，在食物加工台那里加工香果就可以了，然后到大锅那里进行制作就可以了。");
        end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	 if( id == 40051)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40051,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff><font=龙字体,0,17,20>\\n你说芙希希想吃火桃酱面包了，可以倒是可以，可最近农场很忙的，如果你需要我可以教你制作，这样也可以的吧。");
        elseif( id == 40052)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40052,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff><font=龙字体,0,17,20>\\n没错，就是这个香果种，接下来我们把它种到土地里吧。");
	 elseif( id == 40053)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40053,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff><font=龙字体,0,17,20>\\n做的很好，之后要做的就是浇水。");
	 elseif( id == 40054)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40054,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff><font=龙字体,0,17,20>\\n看来你已经掌握了如何浇水了，这个你拿着，这是受到祝福的爱之泉，可以让你的植物马上成熟结果的神奇之水。");
	elseif( id == 40055)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40055,"<font=龙字体,0,19,20><color=0xfffcf157>艾欧里奥：<color=0xffffffff><font=龙字体,0,17,20>\\n看来耕种也是件很有趣的事情吧，材料有了，接下来我们来做一份火桃酱面包吧。");
	end
end

Riolo = {}
Riolo["OnTalk"] = OnTalk
Riolo["OnScenarioTalk"] = OnScenarioTalk
Riolo["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Riolo["OnCompleteQuestTalk"] = OnCompleteQuestTalk