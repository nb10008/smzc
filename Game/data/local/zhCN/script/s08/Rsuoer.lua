
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010207);
	Quest.SetCompleteOptions(5010207);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>索尔：<color=0xffffffff>\\n想成为像我一样的勇者吗？嘿嘿！本勇者的背影是无法复制的！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20622)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20622,"<font=龙字体,0,19,20><color=0xfffcf157>索尔：<color=0xffffffff>\\n哦？天神也有好坏之分？这倒是第一次听说，好吧，姑且相信你，你来干什么？");
        elseif( id == 20623)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20623,"<font=龙字体,0,19,20><color=0xfffcf157>索尔：<color=0xffffffff>\\n东岭那里有很多原神教豢养的<A>僵尸</A>，幸好我及时发现，否则也不知又要有多少路人枉死东岭……");
        elseif( id == 20624)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20624,"<font=龙字体,0,19,20><color=0xfffcf157>索尔：<color=0xffffffff>\\n僵尸中的高手都集中在<N>平乐坡</N>，那里才是你要去的地方，记得把里面最厉害的僵尸<I>头颅</I>带回来！");
	elseif( id == 20625)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20625,"<font=龙字体,0,19,20><color=0xfffcf157>索尔：<color=0xffffffff>\\n<N>黄龙台</N>那边有我的好兄弟<D>亚雷斯</D>，如果见到他记得代我向他问好！");	
	
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20621)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20621,"<font=龙字体,0,19,20><color=0xfffcf157>索尔：<color=0xffffffff>\\n天神？！你是来帮原神教的吗？！");
        elseif( id == 20622)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20622,"<font=龙字体,0,19,20><color=0xfffcf157>索尔：<color=0xffffffff>\\n你帮我清除原神教的妖孽？多谢！");
	elseif( id == 20623)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20623,"<font=龙字体,0,19,20><color=0xfffcf157>索尔：<color=0xffffffff>\\n你看看，我还没说完你就跑了……真是心急的家伙！");
	elseif( id == 20624)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20624,"<font=龙字体,0,19,20><color=0xfffcf157>索尔：<color=0xffffffff>\\n不愧是天神啊！能顶我10个了！");
  elseif( id == 30347)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30347,"<font=龙字体,0,19,20><color=0xfffcf157>索尔：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");
      
	
       end

end

Rsuoer = {}
Rsuoer["OnTalk"] = OnTalk
Rsuoer["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rsuoer["OnCompleteQuestTalk"] = OnCompleteQuestTalk