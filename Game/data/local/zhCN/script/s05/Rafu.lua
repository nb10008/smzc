
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010093);
	Quest.SetCompleteOptions(5010093);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>阿鲁<color=0xffffffff>\\n这福临镇，地处偏僻，魔族应该看不上这里吧。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20320)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20320,"<font=龙字体,0,19,20><color=0xfffcf157>阿鲁<color=0xffffffff>\\n    如今福临镇周围被魔族控制，那些野狼也被魔族同化了，在镇子周围作乱。");
        elseif( id == 20321)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20321,"<font=龙字体,0,19,20><color=0xfffcf157>阿鲁<color=0xffffffff>\\n    附近的魔族越来越多了，要想办法要保证这里的安全啊。有些魔狼变成了可怕的狼人，这可真是让人感到不安啊。");
        elseif( id == 20322)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20322,"<font=龙字体,0,19,20><color=0xfffcf157>阿鲁<color=0xffffffff>\\n    其实福临镇地处偏僻，魔族应该不会总是来这里作恶的。七彩之海谢拉希望等级提升至<C>40</C>级之后去找她一下。");
 
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20319)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20319,"<font=龙字体,0,19,20><color=0xfffcf157>阿鲁<color=0xffffffff>\\n        果然有天神下凡了，真是太棒了。");
        elseif( id == 20320)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20320,"<font=龙字体,0,19,20><color=0xfffcf157>阿鲁<color=0xffffffff>\\n        这人界真是不太平啊。");
        elseif( id == 20321)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20321,"<font=龙字体,0,19,20><color=0xfffcf157>阿鲁<color=0xffffffff>\\n        这下应该很安全了吧。");


       end

end

Rafu = {}
Rafu["OnTalk"] = OnTalk
Rafu["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rafu["OnCompleteQuestTalk"] = OnCompleteQuestTalk