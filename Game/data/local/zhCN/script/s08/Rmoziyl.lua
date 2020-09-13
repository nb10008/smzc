
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010206);
	Quest.SetCompleteOptions(5010206);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n别看我是女孩子就小看我哦？人家也是七十二魔子之一呢！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20612)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20612,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n先帮我摘来10朵安魂花，对了，采花时会有花魔现身，记得消灭它们，免得后患无穷。");
        elseif( id == 20613)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20613,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n这是<I>安魂花</I>制成的<I>安魂迷香</I>，帮我从<N>天台废墟</N>带回<C>3</C>名原神教<I>刺客的灵魂</I>，我要严加拷问！");
        elseif( id == 20614)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20614,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n作为对原神教的惩戒，<C>100</C>名<A>原神传播者</A>，<C>100</C>名<A>原神教刺客</A>，<C>50</C>名<color=0xffff6000>原神精英刺客<color=0xffffffff>，<C>1</C>名<color=0xffff0000>护法雷芒<color=0xffffffff>，这是他们要为此付出的代价！");
	elseif( id == 20615)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20615,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n你可知道那是什么？");	
	elseif( id == 20616)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20616,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n原神教一定是要把冥王之心送至冥山复活冥王，该死！冥王代表着毁灭的力量，如果他被复活，不光人类，连魔族都要死光，这群恶毒的天神！");
        elseif( id == 20617)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20617,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n目标！日月山！那山上居住的<A>狂风刀手</A>和<A>狂风喽啰</A>非常凶悍，你要多加小心！");
	elseif( id == 20618)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20618,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n你的实力又有提升，好！去<N>云蒙麓</N>除掉<color=0xffff0000>风斩<color=0xffffffff>和他的手下，谋夺圣物的行动中他们肯定有份。");
	elseif( id == 20619)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20619,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n怎么？这就累了？好吧，请你喝酒！来，先去找些<I>木柴</I>。");
        elseif( id == 20620)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20620,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n接下来，把这份野味烤了！");
	elseif( id == 20621)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20621,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n往南走吧！那边有个<D>人类的勇士</D>！");	
	       
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20611)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20611,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n你真的值得信赖吗？好吧，我承认正好缺个帮手，就你了！");
        elseif( id == 20612)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20612,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n接下来就看你的本事了。");
	elseif( id == 20613)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20613,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n……真不敢相信，原神教中竟然有魔族的<I>圣物</I>！");
	elseif( id == 20614)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20614,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n恩？<I>圣物</I>呢？你没有看到？");
        elseif( id == 20615)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20615,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n<I>冥王之心</I>！是冥王之心！");
	elseif( id == 20616)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20616,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n先别慌！我们先从周围找起！");
	elseif( id == 20617)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20617,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n恩！看来他们没有见到圣物！");
	elseif( id == 20618)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20618,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n真在他身上？还好还好！呼！");
	elseif( id == 20619)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20619,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n恩！很好！");
	elseif( id == 20620)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20620,"<font=龙字体,0,19,20><color=0xfffcf157>幽露：<color=0xffffffff>\\n呵呵！别客气，吃吧！");
	
       end

end

Rmoziyl = {}
Rmoziyl["OnTalk"] = OnTalk
Rmoziyl["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rmoziyl["OnCompleteQuestTalk"] = OnCompleteQuestTalk