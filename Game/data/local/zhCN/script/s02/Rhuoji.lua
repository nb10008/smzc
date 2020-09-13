local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600018);
	Quest.SetCompleteOptions(5600018);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>火鸡领主：<color=0xffffffff><font=龙字体,0,17,20>\\n统领火鸡是我的责任。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30685)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30685,"<font=龙字体,0,19,20><color=0xfffcf157>火鸡领主：<color=0xffffffff><font=龙字体,0,17,20>\\n我就是火鸡领主，带领众火鸡站在新年大餐的顶端。那些干瘪的火腿总以为可以代替我们，那是不可能的！我希望你能代表火鸡，消灭它们！");
        elseif( id == 30686)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30686,"<font=龙字体,0,19,20><color=0xfffcf157>火鸡领主：<color=0xffffffff><font=龙字体,0,17,20>\\n我就是火鸡领主，带领众火鸡站在新年大餐的顶端。那些干瘪的火腿总以为可以代替我们，那是不可能的！我希望你能代表火鸡，消灭它们！");
        elseif( id == 30687)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30687,"<font=龙字体,0,19,20><color=0xfffcf157>火鸡领主：<color=0xffffffff><font=龙字体,0,17,20>\\n我就是火鸡领主，带领众火鸡站在新年大餐的顶端。那些干瘪的火腿总以为可以代替我们，那是不可能的！我希望你能代表火鸡，消灭它们！");
	elseif( id == 30688)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30688,"<font=龙字体,0,19,20><color=0xfffcf157>火鸡领主：<color=0xffffffff><font=龙字体,0,17,20>\\n我就是火鸡领主，带领众火鸡站在新年大餐的顶端。那些干瘪的火腿总以为可以代替我们，那是不可能的！我希望你能代表火鸡，消灭它们！");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30685)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30685,"<font=龙字体,0,19,20><color=0xfffcf157>火鸡领主：<color=0xffffffff><font=龙字体,0,17,20>\\n火鸡才是最好的新年大餐，火腿什么的根本不够格和我们在一个餐桌上，你说是吧。");
        elseif( id == 30686)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30686,"<font=龙字体,0,19,20><color=0xfffcf157>火鸡领主：<color=0xffffffff><font=龙字体,0,17,20>\\n火鸡才是最好的新年大餐，火腿什么的根本不够格和我们在一个餐桌上，你说是吧。");
	elseif( id == 30687)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30687,"<font=龙字体,0,19,20><color=0xfffcf157>火鸡领主：<color=0xffffffff><font=龙字体,0,17,20>\\n火鸡才是最好的新年大餐，火腿什么的根本不够格和我们在一个餐桌上，你说是吧。");
	 elseif( id == 30688)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30688,"<font=龙字体,0,19,20><color=0xfffcf157>火鸡领主：<color=0xffffffff><font=龙字体,0,17,20>\\n火鸡才是最好的新年大餐，火腿什么的根本不够格和我们在一个餐桌上，你说是吧。");
      
       end

end

Rhuoji = {}
Rhuoji["OnTalk"] = OnTalk
Rhuoji["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rhuoji["OnCompleteQuestTalk"] = OnCompleteQuestTalk