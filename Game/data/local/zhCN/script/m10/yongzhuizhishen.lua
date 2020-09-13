
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010011);
	Quest.SetCompleteOptions(5010011);
	Quest.NPCTalk(0,0,"<font=龙字体,0,21,20><color=0xfffcf157>永坠之神<color=0xffffffff><font=龙字体,0,18,20>\\nNPC头上的<T>叹号</T>和<T>问号</T>图标表示有任务可以接取或完成。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20002)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20002,"<font=龙字体,0,19,20><color=0xfffcf157>永坠之神<color=0xffffffff><font=龙字体,0,17,20>\\n接下来请前往<color=0xff00ddbb>艾弗丝<color=0xffffffff>处领取一件适合自己的<color=0xff9933ff>武器<color=0xffffffff>。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20001)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20001,"<font=龙字体,0,19,20><color=0xfffcf157>永坠之神<color=0xffffffff><font=龙字体,0,17,20>\\n点击下面的<color=0xffff00ff>完成<color=0xffffffff>按钮，就可以交付任务了。");
       end

end

yongzhuizhishen = {}
yongzhuizhishen["OnTalk"] = OnTalk
yongzhuizhishen["OnAcceptQuestTalk"] = OnAcceptQuestTalk
yongzhuizhishen["OnCompleteQuestTalk"] = OnCompleteQuestTalk