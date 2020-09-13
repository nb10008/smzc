
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010096);
	Quest.SetCompleteOptions(5010096);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神女西亚：<color=0xffffffff>\\n你？我可以信赖吗？");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20331)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20331,"<font=龙字体,0,19,20><color=0xfffcf157>神女西亚：<color=0xffffffff>\\n    我来这里是要收集一些这里蝴蝶的粉末，你愿意帮我弄一些么。");
        elseif( id == 20332)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20332,"<font=龙字体,0,19,20><color=0xfffcf157>神女西亚：<color=0xffffffff>\\n    这种蝴蝶的粉末还不够，这里还有一种叫做幻灵蝶的蝴蝶，同样帮我收集一些吧");
        elseif( id == 20333)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20333,"<font=龙字体,0,19,20><color=0xfffcf157>神女西亚：<color=0xffffffff>\\n    让我看看下面的事情，昆虚古城，这是你要前往的地方，昆虚古城就在北边，不过要去那里还是先将等级提升至45级。");
	elseif( id == 20349)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20349,"<font=龙字体,0,19,20><color=0xfffcf157>神女西亚：<color=0xffffffff>\\n    这瑶池碧潭是北洲天雪唯一绿色的存在，我希望北洲天雪处处可以见到绿叶红花。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20330)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20330,"<font=龙字体,0,19,20><color=0xfffcf157>神女西亚：<color=0xffffffff>\\n        看来解决问题的人来了。");
        elseif( id == 20331)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20331,"<font=龙字体,0,19,20><color=0xfffcf157>神女西亚：<color=0xffffffff>\\n        有人能来帮忙真是太好了。");
        elseif( id == 20332)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20332,"<font=龙字体,0,19,20><color=0xfffcf157>神女西亚：<color=0xffffffff>\\n        真是有效率啊，我觉得你真的很有前途啊。");

       end

end

Rxiya = {}
Rxiya["OnTalk"] = OnTalk
Rxiya["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxiya["OnCompleteQuestTalk"] = OnCompleteQuestTalk