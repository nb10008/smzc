

local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetCompleteOptions(3610209);		
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n能见到老夫是你们的荣幸！");
end 


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
     if( id == 20264)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20264,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！"); 
     elseif( id == 20299)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20299,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！"); 
     elseif( id == 20300)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20300,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     elseif( id == 20301)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20301,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     elseif( id == 20302)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20302,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     elseif( id == 20303)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20303,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     elseif( id == 20304)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20304,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     elseif( id == 20305)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20305,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     elseif( id == 20306)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20306,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     elseif( id == 20307)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20307,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     elseif( id == 20308)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20308,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     elseif( id == 20309)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20309,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     elseif( id == 20310)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(20310,"<font=龙字体,0,19,20><color=0xffffe289>罗刹王：<color=0xfffff7e0><font=龙字体,0,17,20>\\n        既然你完成了百斩修罗的考验，那么老夫就给你点小小的奖赏吧！");
     end
end

Rluoshawang = {}
Rluoshawang["OnTalk"] = OnTalk
Rluoshawang["OnCompleteQuestTalk"] = OnCompleteQuestTalk