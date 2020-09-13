
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010101);
	Quest.SetCompleteOptions(5010101);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n精湛的武艺源自勤勉不辍地锻炼！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20401)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20401,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    最近这里野猪泛滥，弄坏了不少东西。不过这幻林中野猪肝可是非常的美味呢，要是你没什么事情，就先去弄点野猪肝吧。");
        elseif( id == 20402)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20402,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    你有没有觉得空气中有很迷幻的味道，闻	多了你可要小心了。这都是那些蝶精散发出来的，就是为了让你无心战斗，陷入迷幻。");
        elseif( id == 20343)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20343,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    我们本来是打算去南边的北风岗，消灭那里的魔族，可还没到那里就被魔族袭击了，这件事只好交给天神来做了。");
        elseif( id == 20403)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20403,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    这里还有一只蝶王，可实在不好对付，不过刚才说过了，我们必须斩草除根的。"); 
	elseif( id == 20404)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20404,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    看来还需要再好好练练你的武艺啊，等你到了52级的时候，再来找我吧。"); 	
	elseif( id == 20405)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20405,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    这里有个叫做三鬼教的邪教，蛊惑了很多人。这个三鬼教可能与魔族有关，那些三鬼的信徒好像带有某种咒书，看看你能不能找到。"); 	
	elseif( id == 20406)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20406,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    三鬼教，他们种植了一些奇怪的植物，那些植物看着很危险。不仅如此，他们还饲养了一群魔灵，最好早点清理掉才是啊。"); 
	elseif( id == 20407)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20407,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    这附近有一个叫做仙淼洞的地方，里面好像是三鬼教的聚集地。现在以你的实力到洞中去会是十分危险的，还是先练练吧。"); 
	elseif( id == 20408)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20408,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    在仙淼洞里，随处可见三鬼教的信徒，一定要消灭它们。"); 
	elseif( id == 20409)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20409,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    将三鬼教的那些头领全部消灭，邪教还能那么猖狂？"); 
	elseif( id == 20410)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20410,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    到武岩城去看看吧，那里和三鬼教有密切的关系。不过你最好先将等级提升到58级再去，那里十分危险，巴泽尔就在城外埋伏着。"); 		   
	elseif( id == 30501)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30501,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    你听说过混沌魔域吗？魔域之门只会在每天的21：00-22：00打开，你可以通过斗战天城的恶魔之子进入混沌魔域。在里面你将有机会见到强大的恶魔以及许多稀有的宝物，有空的话你不妨去碰碰运气。");
	elseif( id == 30502)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30502,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    混沌魔域的大门只会在每天的21：00-22：00打开，你可以通过斗战天城的恶魔之子进入混沌魔域。在里面你将有机会见到强大的恶魔以及许多稀有的宝物，有空的话你不妨去碰碰运气。");
  elseif( id == 30503)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30503,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    混沌魔域的大门只会在每天的21：00-22：00打开，你可以通过斗战天城的恶魔之子进入混沌魔域。在里面你将有机会见到强大的恶魔以及许多稀有的宝物，有空的话你不妨去碰碰运气。");
	elseif( id == 20429)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20429,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    三鬼教那个邪教，让他们的高级信徒为他们传播教义，好壮大邪教，我们应当从中阻止。");
	elseif( id == 20431)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20431,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n    短时间内，突然增加了很多三鬼教的门徒，我需要你去消灭他们，避免三鬼教逐步壮大。");
	
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20400)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20400,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        哟呵呵，没想到武神是个女人吧，我的实力可不是假的。");
        elseif( id == 20401)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20401,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        干得不错，现在来说说正事吧。");   
        elseif( id == 20402)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20402,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        斩草还要除根。"); 
        elseif( id == 20403)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20403,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        这下空气清新了不少啊。"); 
	elseif( id == 20404)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20404,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        这还差不多。");  
	elseif( id == 20405)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20405,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        让我看看，这到底是什么东西。");  
	elseif( id == 20406)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20406,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        这些家伙到底想要做什么啊。");  
	elseif( id == 20407)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20407,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        那么准备好了，可以开始了。");  
	elseif( id == 20408)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20408,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        真是大开杀戒了吧。");  
	elseif( id == 20409)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20409,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        好的，这就是我们想要的效果，不过邪教不会就此善罢甘休的。");  
	elseif( id == 30501)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30501,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        你见到魔王了吗？如果没有的话，多去几次，总能碰到魔王现身的时候。那个魔王拥有的宝物可是让我也非常羡慕哦。"); 
	elseif( id == 30502)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30502,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        你见到魔王了吗？如果没有的话，多去几次，总能碰到魔王现身的时候。那个魔王拥有的宝物可是让我也非常羡慕哦。"); 
	elseif( id == 30503)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30503,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        你见到魔王了吗？如果没有的话，多去几次，总能碰到魔王现身的时候。那个魔王拥有的宝物可是让我也非常羡慕哦。"); 
	elseif( id == 20430)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20430,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        你要是早来就能减少她魅惑的人了，不过现在也不晚。"); 
       elseif( id == 20436)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20436,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        他的死去，必然让我们士气大增！"); 
       elseif( id == 30340)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30340,"<font=龙字体,0,19,20><color=0xfffcf157>武神贝蒂：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");

       end

end

Rbeidi = {}
Rbeidi["OnTalk"] = OnTalk
Rbeidi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rbeidi["OnCompleteQuestTalk"] = OnCompleteQuestTalk