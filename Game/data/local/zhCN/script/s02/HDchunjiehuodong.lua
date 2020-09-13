
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(6040022);
	Quest.SetCompleteOptions(6040022);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n喜迎新春人兴财旺年年好，乐接华年鸿运亨通步步高。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30867)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30867,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n今年的春节真是特别的热闹啊，可是现在不知怎么的，<N>熊猫村</N>的人手不够了，我也一直忙得脱不开身，麻烦你帮我前去给其他人拜个年。");
        elseif( id == 30868)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30868,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n<N>熊猫村</N>春节活动的年货还没采购，请你帮我去跟<D>罗梅达亚</D>说一声，还按照去年的年货表置办年货。");
        elseif( id == 30869)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30869,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n我这里需要赶制一些<I>春节时装</I>， 材料却不够用的了，麻烦你去帮我打些<A>狐狸</A>，取些<I>狐皮</I>给我。");
        elseif( id == 30870)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30870,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n我还有个不情之请，前几天<D>宠物店老板</D>在我这寄托了一只宠物被一群<A>迷蝶</A>吸引了去，麻烦你救出我的宠物。");
        elseif( id == 30871)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30871,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n前些日子<D>宠物店的老板</D>跑来跟我诉苦说他把他老婆的<I>定情信物</I>给弄丢了，麻烦你帮忙去找找，我会给你一个意外的奖励。");

	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30867)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30867,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n哦？这么快就拜完年了？好吧，这点小意思你就收下吧，就当我也给你拜个年吧。");
        elseif( id == 30868)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30868,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n已经跟<D>罗梅达亚</D>打过招呼了吗？太好了，今年大家又能过一个丰富的新年了。");
        elseif( id == 30869)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30869,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n华丽的衣服，每个人都爱上了这浓浓的新年的味道。多亏了有你帮忙，我才能顺利赶制这些<I>春节时装</I>。现在好了，我不用再为<I>春节时装</I>发愁了。");
        elseif( id == 30870)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30870,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n多谢你帮救回了这个小家伙，这下也可以在<D>宠物店老板</D>面前好交代了。 ");
        elseif( id == 30871)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30871,"<font=龙字体,0,19,20><color=0xfffcf157>春节使者米莲妮：<color=0xffffffff>\\n恭喜你，幸运的人，现在你可以进入<N>方舟</N>开始一段神奇之旅了。");

       end

end

HDchunjiehuodong = {}
HDchunjiehuodong["OnTalk"] = OnTalk
HDchunjiehuodong["OnAcceptQuestTalk"] = OnAcceptQuestTalk
HDchunjiehuodong["OnCompleteQuestTalk"] = OnCompleteQuestTalk