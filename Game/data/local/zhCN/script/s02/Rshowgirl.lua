
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610050);
	Quest.SetCompleteOptions(5610050);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐：<color=0xffffffff>\\n看姐的腿风骚不？");
end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30792)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30792,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐:<color=0xffffffff>\\n又快到了一年一度，多少“宅男”、“怨女”们翘首期待的CHINA-JOY（以下简称CJ） ，姐作为会场中的一道风景，今年的衣服还没准备好。这可怎么弄！如果你能帮我搞到我想要的衣服款式。我自然会感谢你的，至于感谢的内容嘛，你懂的！去年我的衣服貌似是位帅锅帮我搞定的，你先去找他吧，他一般是不露面的，想要见他你得先完成5次师门任务，看一下你的诚意哦。你可以在师门任务使者那里接取师门任务，完成后来找我！");
     elseif( id == 30794)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30794,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐:<color=0xffffffff>\\n咦，小子，这么快就回来了，难道已经做好了吗？喔，还没，大哥需要我的身段尺寸是吗。那好吧，不过姐这里没有尺子，你得先帮姐去搞一把尺子。西洲幻林的三鬼门徒和三鬼信徒身上都可以得到一种叫做“皮筋”的物品，20个“皮筋”就可以做皮尺了，有了皮尺姐就可以告诉你什么是完美的曲线身材。");
		elseif( id == 30796)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30796,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐:<color=0xffffffff>\\n好美的华服！为了感谢你，你可以直接从姐这里选取2把伪魔武器。");
		elseif( id == 30815)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30815,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐:<color=0xffffffff>\\n嗨，小子，又见面了，上次给姐做的不错，不过也许、可能、大概，还真的再麻烦你一次，据可靠消息称，今年砖家叫兽们有一个新的规定，据说是不能露大腿！这可怎么弄，我滴新衣服不能穿了，你得帮我让张三大哥给改一下，要符合规定！");
		elseif( id == 30819)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30819,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐:<color=0xffffffff>\\n好美的华服！为了感谢你，你可以直接从姐这里选取2把伪魔武器。");

				end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30792)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30792,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐：<color=0xffffffff>\\n嗯，你的诚意我知道了。你帮我捎个口信给那位帅锅吧！他叫张三，见了面你就知道他帅不帅了！");
		elseif( id == 30794)then
			Quest.CloseNPCTalk();
		    Quest.CompleteQuestTalk(30794,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐：<color=0xffffffff>\\n嗯，干的很好，那咱们开始进行量身呗！此处省略一千字。好了，姐的身材是什么尺码你也知道了，快去找大哥交差吧，等你归来呦！");
		elseif( id == 30796)then
			if( not Quest.QuestCanComplete(30796) ) then
				if( step == QuestTalkBegin )then
					Quest.ClearNPCTalkOptions();
					Quest.SetNPCTalkOption( EOT_CompleteQuest,30796,QuestTalkBegin+1,"太迷人了！" );
					Quest.SetNPCTalkOption( EOT_CompleteQuest,30796,QuestTalkBegin+2,"这也能算衣服？" );
					Quest.SetNPCTalkOption( EOT_CompleteQuest,30796,QuestTalkBegin+3,"…………" );
					Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐：<color=0xffffffff>\\n姐穿上这件新衣服，是不是风情万种啊？");
				else
					Quest.UpdateQuest(30796);
				end
			else
			    Quest.CloseNPCTalk();
	            Quest.CompleteQuestTalk(30796,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐：<color=0xffffffff>\\n这是给你的奖励！");
			end
		elseif( id == 30819)then
			if( not Quest.QuestCanComplete(30819) ) then
				if( step == QuestTalkBegin )then
					Quest.ClearNPCTalkOptions();
					Quest.SetNPCTalkOption( EOT_CompleteQuest,30819,QuestTalkBegin+1,"太迷人了！" );
					Quest.SetNPCTalkOption( EOT_CompleteQuest,30819,QuestTalkBegin+2,"这次总算像件衣服了。" );
					Quest.SetNPCTalkOption( EOT_CompleteQuest,30819,QuestTalkBegin+3,"…………" );
					Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐：<color=0xffffffff>\\n姐穿上这件新衣服，是不是风情万种啊？");
				else
					Quest.UpdateQuest(30819);
				end
			else
			    Quest.CloseNPCTalk();
	            Quest.CompleteQuestTalk(30819,"<font=龙字体,0,19,20><color=0xfffcf157>showgirl小姐：<color=0xffffffff>\\n这是给你的奖励！");
			end

	   end
end

Rshowgirl = {}
Rshowgirl["OnTalk"] = OnTalk
Rshowgirl["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rshowgirl["OnCompleteQuestTalk"] = OnCompleteQuestTalk
