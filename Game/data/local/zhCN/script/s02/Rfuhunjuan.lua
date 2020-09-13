local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5511029);
	Quest.SetCompleteOptions(5511029);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n更多奖励，更多兑换。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
       if( id == 30644)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30644,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n你找到<I>+5附魂卷</I>和<A>50金</A>就可以换取<I>附魂石+5</I>了。");
	elseif( id == 30653)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30653,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n你使用<C>1</C>个<I>四等材料兑换卷</I>和<C>50</C><A>金	</A>，可以兑换<C>3</C>个<I>四等精铁</I>。");
	elseif( id == 30654)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30654,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n你使用<C>1</C>个<I>四等材料兑换卷</I>和<C>100</C><A>金</A>，可以兑换<C>3</C>个<I>四等玄铁</I>。");
	elseif( id == 30655)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30655,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n你使用<C>1</C>个<I>宝石兑换卷</I>和<C>300</C><A>金游戏币</A>，可以兑换<C>1</C>个<I>太阳石</I>。");
	elseif( id == 30656)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30656,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n你使用<C>1</C>个<I>宝石兑换卷</I>和<C>300</C><A>金游戏币</A>，可以兑换<C>1</C>个<I>月光石</I>。");
	elseif( id == 30657)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30657,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n你使用<C>1</C>个<I>玄晶石碎片卷</I>和<C>20</C><A>金游戏币</A>，可以兑换<C>1</C>个<I>玄晶石碎片</I>。");
	elseif( id == 30689)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30689,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n你使用<C>1</C>个<I>魔光碎片卷</I>和<C>20</C><A>金游戏币</A>，可以兑换<C>5</C>个<I>魔光碎片</I>。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30644)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30644,QuestTalkBegin+1,"确定！" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30644,QuestTalkBegin+2,"再考虑一下" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n请注意：完成此任务需要<I>+5附魂卷</I>和<A>50金</A>。确定吗？");

		elseif( step == QuestTalkBegin+1 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30644,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n看来你已经做好了决定，那么拿走附魂石吧。");
		elseif( step == QuestTalkBegin+2 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30644,QuestTalkBegin,"离开" );
			Quest.NPCTalk(0,0,"还没准备好");
		end
	elseif( id == 30653)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30653,QuestTalkBegin+1,"确定！" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30653,QuestTalkBegin+2,"再考虑一下" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n完成此任务需要<C>1</C>个<I>四等材料兑换卷</I>和<C>50</C><A>金</A>，你确定要进行兑换吗？");

		elseif( step == QuestTalkBegin+1 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30653,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n这是你的四等精铁。");
		elseif( step == QuestTalkBegin+2 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30653,QuestTalkBegin,"离开" );
			Quest.NPCTalk(0,0,"还没准备好");
		end
	elseif( id == 30654)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30654,QuestTalkBegin+1,"确定！" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30654,QuestTalkBegin+2,"再考虑一下" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n完成此任务需要<C>1</C>个<I>四等材料兑换卷</I>和<C>100</C><A>金</A>，你确定要进行兑换吗？");

		elseif( step == QuestTalkBegin+1 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30654,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n这是你的四等精铁。");
		elseif( step == QuestTalkBegin+2 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30654,QuestTalkBegin,"离开" );
			Quest.NPCTalk(0,0,"还没准备好");
		end
	elseif( id == 30655)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30655,QuestTalkBegin+1,"确定！" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30655,QuestTalkBegin+2,"再考虑一下" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n完成此任务需要<C>1</C>个<I>宝石兑换卷</I>和<C>300</C><A>金</A>，你确定要进行兑换吗？");

		elseif( step == QuestTalkBegin+1 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30655,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n拿好你的太阳石，欢迎下次再来。");
		elseif( step == QuestTalkBegin+2 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30655,QuestTalkBegin,"离开" );
			Quest.NPCTalk(0,0,"还没准备好");
		end
	elseif( id == 30656)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30656,QuestTalkBegin+1,"确定！" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30656,QuestTalkBegin+2,"再考虑一下" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n完成此任务需要<C>1</C>个<I>宝石兑换卷</I>和<C>300</C><A>金</A>，你确定要进行兑换吗？");

		elseif( step == QuestTalkBegin+1 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30656,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n拿好你的月光石，欢迎下次再来。");
		elseif( step == QuestTalkBegin+2 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30656,QuestTalkBegin,"离开" );
			Quest.NPCTalk(0,0,"还没准备好");
		end
	elseif( id == 30657)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30657,QuestTalkBegin+1,"确定！" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30657,QuestTalkBegin+2,"再考虑一下" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n完成此任务需要<C>1</C>个<I>玄晶石碎片卷</I>和<C>20</C><A>金</A>，你确定要进行兑换吗？");

		elseif( step == QuestTalkBegin+1 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30657,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n拿好你的玄晶石碎片。");
		elseif( step == QuestTalkBegin+2 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30657,QuestTalkBegin,"离开" );
			Quest.NPCTalk(0,0,"还没准备好");
		end
	elseif( id == 30689)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30689,QuestTalkBegin+1,"确定！" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30689,QuestTalkBegin+2,"再考虑一下" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n完成此任务需要<C>1</C>个<I>魔光碎片卷</I>和<C>20</C><A>金</A>，你确定要进行兑换吗？");

		elseif( step == QuestTalkBegin+1 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30689,"<font=龙字体,0,19,20><color=0xfffcf157>在线奖励兑换师：<color=0xffffffff>\\n拿好你的魔光碎片。");
		elseif( step == QuestTalkBegin+2 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30689,QuestTalkBegin,"离开" );
			Quest.NPCTalk(0,0,"还没准备好");
		end
       end

end

Rfuhunjuan = {}
Rfuhunjuan["OnTalk"] = OnTalk
Rfuhunjuan["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rfuhunjuan["OnCompleteQuestTalk"] = OnCompleteQuestTalk