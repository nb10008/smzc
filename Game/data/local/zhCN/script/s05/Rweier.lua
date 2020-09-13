
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010095);
	Quest.SetCompleteOptions(5010095);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n想拿我当猎物的家伙！终将成为我的猎物！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20327)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20327,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n    这里出现了好多残忍的猎人，他们好像只是为了乐趣在这里猎杀我们，你能不能帮助我们将这些家伙赶走。");
        elseif( id == 20328)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20328,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n    在这些猎杀者中，有一些是非常熟练的高级猎手，这才是我最担心的。");
        elseif( id == 20329)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20329,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n    这里除了这些猎杀者，还有就是魔族。没错，魔族控制了雪地的守护者，这里只好靠我们自己了，那些没用的守护者也是弱者，我们不需要弱者。");
        elseif( id == 20330)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20330,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n    谢谢你的帮助，当你的等级提升至44级，就可以前往北边的瑶池碧潭了，神女西亚就在瑶池碧潭。");
        elseif( id == 20346)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20346,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n    既然雪山上所有都被魔族控制了，那么北洲王者也一定没有幸免，所以我们需要尽快消灭它，以免后患。还有去看看别的地方是否也有变化。");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20326)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20326,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n        嗷~~你就是河神说的那个人吧，快来帮帮我们吧。");
        elseif( id == 20327)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20327,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n        真是太感谢你了，看来河神推荐的人果然没错。");
        elseif( id == 20328)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20328,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n        他们是罪有应得，就应该是这个下场。");
        elseif( id == 20329)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20329,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n        难道因为个头大，就被叫做守护者了？太可笑了。");
       elseif( id == 20346)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20346,"<font=龙字体,0,19,20><color=0xfffcf157>巨熊威尔：<color=0xffffffff>\\n        做的不错，值得信赖。");
       end

end

Rweier = {}
Rweier["OnTalk"] = OnTalk
Rweier["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rweier["OnCompleteQuestTalk"] = OnCompleteQuestTalk