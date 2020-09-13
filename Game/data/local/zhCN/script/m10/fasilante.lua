
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010017);
	Quest.SetCompleteOptions(5010017);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>法斯兰特：<color=0xffffffff><font=龙字体,0,17,20>\\n看来是新来的天兵，在这里好好接受训练吧。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20010)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20010,"<font=龙字体,0,19,20><color=0xfffcf157>法斯兰特：<color=0xffffffff><font=龙字体,0,17,20>\\n    请再回去找<color=0xff00ddbb>统帅伊万夫<color=0xffffffff>，他会有新的任务布置给你。");
        elseif( id == 20017)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20017,"<font=龙字体,0,19,20><color=0xfffcf157>法斯兰特：<color=0xffffffff><font=龙字体,0,17,20>\\n    当你的等级达到<A>10</A>级，便去找<D>统帅伊万夫</D>吧，他会指引你前往我们的主城——<N>斗战天城</N>，在那里，你会真正的开始你的战神之旅。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20009)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20009,"<font=龙字体,0,19,20><color=0xfffcf157>法斯兰特：<color=0xffffffff><font=龙字体,0,17,20>\\n    装备分为白、<color=0xffffff00>黄<color=0xffffffff>、<color=0xff05ff00>绿<color=0xffffffff>、<color=0xff00a2ff>蓝<color=0xffffffff>、<color=0xffffae00>橙<color=0xffffffff>、<color=0xff9933ff>紫<color=0xffffffff>等品质，白色最低，越好的装备对自己的实力增强越多，你可千万要记住。");
       end

end

fasilante = {}
fasilante["OnTalk"] = OnTalk
fasilante["OnAcceptQuestTalk"] = OnAcceptQuestTalk
fasilante["OnCompleteQuestTalk"] = OnCompleteQuestTalk