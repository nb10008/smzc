
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010203);
	Quest.SetCompleteOptions(5010203);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>魔子左德：<color=0xffffffff>\\n大家都叫我不死的左德，嘿嘿！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20517)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20517,"<font=龙字体,0,19,20><color=0xfffcf157>魔子左德：<color=0xffffffff>\\n哦？摩纳特竟把这么重要的宝贝让你带来，这两种宝贝既能提升魔族的力量，同时也能让魔族中毒。好吧，先让我看看你的本事！目标！那群家伙！");
        elseif( id == 20518)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20518,"<font=龙字体,0,19,20><color=0xfffcf157>魔子左德：<color=0xffffffff>\\n这样吧！如果你在附近的<color=0xffff6000>变异蛮使<color=0xffffffff>身上还能找到<I>天神印记</I>，我就相信你们的推断。");
        elseif( id == 20519)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20519,"<font=龙字体,0,19,20><color=0xfffcf157>魔子左德：<color=0xffffffff>\\n老子去清理些小杂鱼！那<color=0xffff0000>摄魂女皇<color=0xffffffff>就任你处置了！哈哈哈！");
	elseif( id == 20520)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20520,"<font=龙字体,0,19,20><color=0xfffcf157>魔子左德：<color=0xffffffff>\\n我那兄弟早就怀疑原神教，他已经到了泪海沼泽，去问问他有啥收获吧！嘿！他可不好说话！<C>78</C>级拿着<C>20</C>颗<I>精英撕裂者的心脏</I>去证明你的实力吧！");
	
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20516)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20516,"<font=龙字体,0,19,20><color=0xfffcf157>魔子左德：<color=0xffffffff>\\n恩？天界的小家伙！想试试老子的拳头吗？");
        elseif( id == 20517)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20517,"<font=龙字体,0,19,20><color=0xfffcf157>魔子左德：<color=0xffffffff>\\n好！小家伙很对我的胃口！唔！你刚才说原神教借用天神的物品操纵野人与我魔族为敌？恩，有趣的推论！");
	elseif( id == 20518)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20518,"<font=龙字体,0,19,20><color=0xfffcf157>魔子左德：<color=0xffffffff>\\n哈哈！这些日子可把老子闷坏了，好！看老子杀个痛快！！");
	elseif( id == 20519)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20519,"<font=龙字体,0,19,20><color=0xfffcf157>魔子左德：<color=0xffffffff>\\n小家伙！老子可等你很久了！怎样？");
  elseif( id == 30345)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30345,"<font=龙字体,0,19,20><color=0xfffcf157>魔子左德：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");

       end

end

Rmozizd = {}
Rmozizd["OnTalk"] = OnTalk
Rmozizd["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rmozizd["OnCompleteQuestTalk"] = OnCompleteQuestTalk