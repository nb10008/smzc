
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(6040023);
	Quest.SetCompleteOptions(6040023);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n春节到了，我代表熊猫村族人祝福诸位，春节愉快，合家欢乐。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30850)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30850,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n年轻人，你好。我是来自<N>熊猫村</N>的长老。我的族人都是热爱和平的，但是最近不少族人感染了未知的病毒，变得嗜血凶残。我请求你帮我清除这些可恨的族人。");
        elseif( id == 30851)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30851,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n年轻人，你好。我是来自<N>熊猫村</N>的长老。我的族人都是热爱和平的，但是最近不少族人感染了未知的病毒，变得嗜血凶残。我请求你帮我清除这些可恨的族人。");
        elseif( id == 30852)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30852,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n年轻人，你好。我是来自<N>熊猫村</N>的长老。我的族人都是热爱和平的，但是最近不少族人感染了未知的病毒，变得嗜血凶残。我请求你帮我清除这些可恨的族人。");
        elseif( id == 30853)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30853,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n年轻人，你好。我是来自<N>熊猫村</N>的长老。我的族人都是热爱和平的，但是最近不少族人感染了未知的病毒，变得嗜血凶残。我请求你帮我清除这些可恨的族人。");
	 elseif( id == 30872)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30872,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n诺亚方舟正在受着入侵者的侵害，勇士啊，请你快去拯救诺亚方舟。");
	 elseif( id == 30873)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30873,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n本次消灭的入侵者，要比之前的入侵者更加危险，但是我相信身为勇士的你，拥有足够的力量的。");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30850)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30850,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n咳咳，谢谢你，年轻人！在春节之际还麻烦你，真是不好意思，这些奖励请收下它们吧。");
        elseif( id == 30851)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30851,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n咳咳，谢谢你，年轻人！在春节之际还麻烦你，真是不好意思，这些奖励请收下它们吧。");
        elseif( id == 30852)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30852,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n咳咳，谢谢你，年轻人！在春节之际还麻烦你，真是不好意思，这些奖励请收下它们吧。");
        elseif( id == 30853)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30853,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n咳咳，谢谢你，年轻人！在春节之际还麻烦你，真是不好意思，这些奖励请收下它们吧。");
	elseif( id == 30872)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30872,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n这样方舟或许就安全了一些。");
	elseif( id == 30873)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30873,"<font=龙字体,0,19,20><color=0xfffcf157>熊猫长老：<color=0xffffffff>\\n你的力量是不容小看的。");
       end

end

HDxiongmaozhanglao = {}
HDxiongmaozhanglao["OnTalk"] = OnTalk
HDxiongmaozhanglao["OnAcceptQuestTalk"] = OnAcceptQuestTalk
HDxiongmaozhanglao["OnCompleteQuestTalk"] = OnCompleteQuestTalk