
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5516012);
	Quest.SetCompleteOptions(5516012);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>美神艾丝美拉达：<color=0xffffffff><font=龙字体,0,17,20>\\n美丽是天神赐你的最大奖励哦。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30115)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30115,"<font=龙字体,0,19,20><color=0xfffcf157>美神艾丝美拉达：<color=0xffffffff><font=龙字体,0,17,20>\\n我爱浪漫，我也爱一种叫做罗曼之星的东西，你可以在博博罗曼村找到它们。");
        elseif( id == 30116)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30116,"<font=龙字体,0,19,20><color=0xfffcf157>美神艾丝美拉达：<color=0xffffffff><font=龙字体,0,17,20>\\n我想用家族花带来将天城好好装扮一下，完成家族任务就会获得家族花带哦。");
        elseif( id == 30117)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30117,"<font=龙字体,0,19,20><color=0xfffcf157>美神艾丝美拉达：<color=0xffffffff><font=龙字体,0,17,20>\\n制作更加漂亮的衣服一定会用到七彩的魔法布，我可以用一些魔法布和金丝制作这种布料。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30115)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30115,"<font=龙字体,0,19,20><color=0xfffcf157>美神艾丝美拉达：<color=0xffffffff><font=龙字体,0,17,20>\\n这个大陆上一定都留有你浪漫的足迹。");
	elseif( id == 30116)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30116,"<font=龙字体,0,19,20><color=0xfffcf157>美神艾丝美拉达：<color=0xffffffff><font=龙字体,0,17,20>\\n你们的家族一定会变得越来越强大。");
        elseif( id == 30117)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30117,"<font=龙字体,0,19,20><color=0xfffcf157>美神艾丝美拉达：<color=0xffffffff><font=龙字体,0,17,20>\\n这就是你所需要的七彩魔法布啦。");
       end


end

Raisimeilada = {}
Raisimeilada["OnTalk"] = OnTalk
Raisimeilada["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Raisimeilada["OnCompleteQuestTalk"] = OnCompleteQuestTalk