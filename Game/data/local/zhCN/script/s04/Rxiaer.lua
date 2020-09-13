
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010079);
	Quest.SetCompleteOptions(5010079);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n我的绿洲，已经快要。。消失了。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20211)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20211,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n        普兰多谷中有一种叫做<A>科罗拉巨龙</A>的庞然大物，我需要一些大龙的龙骨来做些药剂，我想你能帮我。");
        elseif( id == 20212)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20212,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n        其实就是一种追踪用的药剂，找到了！这可是巨龙中的巨龙，名字叫做<color=0xffff6000>雷巴科巨龙<color=0xffffffff>，接下来就是把它的黑真角交给我就是了。");
        elseif( id == 20213)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20213,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n        黑真角，可以用来探测魔族。而且是强大的魔族，比如……七十二魔子。以你现在的能力，肯定会被轰成渣的，快去好好磨练一番。");
        elseif( id == 20214)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20214,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n        最后的峡谷，又叫做<N>熔岩之谷</N>，你到那里找<D>火神加尔</D>吧。");
	elseif( id == 20221)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20221,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n        翼神琼纳斯对这里有些担心，你去告诉她这里的情况，让她得以安心。");

	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20210)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20210,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n        欢迎你来到夏尔绿谷，别忘记了你不是来度假的。");
        elseif( id == 20211)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20211,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n        猜猜我会做出什么药剂呢？");
        elseif( id == 20212)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20212,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n        看来你很有精神啊，这么大的巨兽都被你征服了。");
        elseif( id == 20213)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20213,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n         现在准备进入最后的峡谷吧。");
	elseif( id == 20222)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20222,"<font=龙字体,0,19,20><color=0xfffcf157>夏尔：<color=0xffffffff>\\n         我会把这些龙心做成肥料来滋润绿谷的，让这里生机盎然。");
       end

end

Rxiaer = {}
Rxiaer["OnTalk"] = OnTalk
Rxiaer["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxiaer["OnCompleteQuestTalk"] = OnCompleteQuestTalk