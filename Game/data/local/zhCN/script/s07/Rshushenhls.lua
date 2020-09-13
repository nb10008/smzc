
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010164);
	Quest.SetCompleteOptions(5010164);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>树神哈伦斯：<color=0xffffffff>\\n绿意遍洒，生机勃发！这才是我最爱的世界！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20501)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20501,"<font=龙字体,0,19,20><color=0xfffcf157>树神哈伦斯：<color=0xffffffff>\\n魔族在附近洒下很多魔花的种子，看看那些<A>花妖</A>吧，居然有那么恐怖的嘴。能清理多少就是多少吧。");
        elseif( id == 20502)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20502,"<font=龙字体,0,19,20><color=0xfffcf157>树神哈伦斯：<color=0xffffffff>\\n魔族已经在这里布置了一些兵力，比如那些<A>丛林袭击者</A>之类的，尽快将它们消灭掉吧。");
        elseif( id == 20503)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20503,"<font=龙字体,0,19,20><color=0xfffcf157>树神哈伦斯：<color=0xffffffff>\\n这次来个大动作吧，把那些烦人的东西都清除掉，那些小喽啰是不够看了。来弄一些大家伙吧。");
	elseif( id == 20504)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20504,"<font=龙字体,0,19,20><color=0xfffcf157>树神哈伦斯：<color=0xffffffff>\\n附近有个叫做<N>古茶山</N>的地方，你准备一下，就到那里去吧。那里有个叫做<D>易罗萨</D>的人，他好像有什么秘密。");
	
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20500)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20500,"<font=龙字体,0,19,20><color=0xfffcf157>树神哈伦斯：<color=0xffffffff>\\n应该是斗神之王派你来的。");
        elseif( id == 20501)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20501,"<font=龙字体,0,19,20><color=0xfffcf157>树神哈伦斯：<color=0xffffffff>\\n这些应该可以成为漂亮的花吧，真是可惜了。");
	elseif( id == 20502)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20502,"<font=龙字体,0,19,20><color=0xfffcf157>树神哈伦斯：<color=0xffffffff>\\n接下来还有很多事要做呢。");
	elseif( id == 20503)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20503,"<font=龙字体,0,19,20><color=0xfffcf157>树神哈伦斯：<color=0xffffffff>\\n心情顿时愉快了很多啊，这些家伙确实该死。");
         elseif( id == 30343)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30343,"<font=龙字体,0,19,20><color=0xfffcf157>树神哈伦斯：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");

       end

end

Rshushenhls = {}
Rshushenhls["OnTalk"] = OnTalk
Rshushenhls["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rshushenhls["OnCompleteQuestTalk"] = OnCompleteQuestTalk