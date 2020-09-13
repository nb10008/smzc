
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600058);
	Quest.SetCompleteOptions(5600058);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>爱神丘比特<color=0xffffffff><font=龙字体,0,17,20>\\n        你必须达到<C>50</C>级才能结婚，同时你的配偶必须是同一个家族中的成员。\\n        丘比特之箭是爱情的象征，你和你的爱人需要带着它到<P>博博罗曼村</P>（每周二、四、六、日的晚上18:00-24:00开放）找<M>阿芙罗狄</M>举行婚礼。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30376)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30376,"<font=龙字体,0,19,20><color=0xfffcf157>爱神丘比特：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你收集到足够的卡片之后，我就给你丘比特之箭。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30376)then
				      Quest.CloseNPCTalk();
				      Quest.CompleteQuestTalk(30376,"<font=龙字体,0,19,20><color=0xfffcf157>爱神丘比特：<color=0xffffffff><font=龙字体,0,17,20>\\n        拿着这充满爱意的箭去追求你们的幸福吧。");
      	elseif( id == 30802)then
					if( step == QuestTalkBegin )then
						Quest.ClearNPCTalkOptions();
						Quest.SetNPCTalkOption( EOT_CompleteQuest,30802,QuestTalkBegin+1,"确定" );
						Quest.SetNPCTalkOption( EOT_CompleteQuest,30802,QuestTalkBegin+2,"返回" );
						Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>爱神丘比特：<color=0xffffffff>\\n        飕、飕，让我射你一下，就会有一位有缘人出现。");	        
					elseif( step == QuestTalkBegin+1 )then
						Quest.UpdateQuest(30802);
					elseif( step == QuestTalkBegin+2 )then
						Quest.CloseNPCTalk();
					end
				end
end

Rqiubite = {}
Rqiubite["OnTalk"] = OnTalk
Rqiubite["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rqiubite["OnCompleteQuestTalk"] = OnCompleteQuestTalk