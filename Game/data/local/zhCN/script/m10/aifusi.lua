
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010012);
	Quest.SetCompleteOptions(5010012);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>艾弗丝：<color=0xffffffff><font=龙字体,0,17,20>\\n现在选择武器并不会影响以后就职，所以不要过于纠结选择哪种武器。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20003)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20003,"<font=龙字体,0,19,20><color=0xfffcf157>艾弗丝：<color=0xffffffff><font=龙字体,0,17,20>\\n现在就开始进行战神候补的试炼吧！只有通过了这里的试炼，你才有资格进入<color=0xffababff>斗战天城<color=0xffffffff>，成为一名真正的天界勇士。\\n    先试试清理附近<color=0xffffe289>蓝魔<color=0xffffffff>，然后向<color=0xff00ddbb>索尔<color=0xffffffff>汇报。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20002)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20002,"<font=龙字体,0,19,20><color=0xfffcf157>艾弗丝：<color=0xffffffff><font=龙字体,0,17,20>\\n<color=0xffffff00>从下面的框中选中一件喜欢的武器<color=0xffffffff>，然后点击<color=0xffff00ff>完成<color=0xffffffff>按钮即可。\\n    长刃：物理近战，若喜欢日后可就职<color=0xff80ff00>神兵职业<color=0xffffffff>\\n    圣弓：物理远程，若喜欢日后可就职<color=0xff80ff00>玄翎职业<color=0xffffffff>\\n    宝器：法术远程，若喜欢日后可就职<color=0xff80ff00>乾坤职业<color=0xffffffff>\\n    魔琴：法术远程，若喜欢日后可就职<color=0xff80ff00>仙音职业<color=0xffffffff>");
       end

end

aifusi = {}
aifusi["OnTalk"] = OnTalk
aifusi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
aifusi["OnCompleteQuestTalk"] = OnCompleteQuestTalk