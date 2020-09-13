
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010028);
	Quest.SetCompleteOptions(5010028);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>仓库说明人：<color=0xffffffff>\\n使用仓库可直接与<D>仓库管理员</D>对话。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20021)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20021,"<font=龙字体,0,19,20><color=0xfffcf157>仓库说明人：<color=0xffffffff>\\n到城市里好好看看吧，尤其是<N>商业区</N>、<N>强化区</N>、<N>摆摊区</N>，这些地方肯定有你需要的东西。\\n哦，对了！仓库只有在城市或是据点里才有，但是你也可以使用<I>随身仓库</I>，随身仓库可以随时随地打开仓库，相当方便。");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20020)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20020,"<font=龙字体,0,19,20><color=0xfffcf157>仓库说明人：<color=0xffffffff>\\n与我身旁的<D>仓库管理员</D>对话便可打开仓库，仓库可用于存放物品和金钱。\\n若背包和仓库的空间不够使用，可通过扩充来增加空间，扩充分别需要道具<color=0xffffff00>附加背包<color=0xffffffff>和<color=0xffffff00>附加仓库<color=0xffffffff>，可以在商城购买获得。\\n另外请注意，同一个帐号下的角色是共用仓库的，因此可以通过仓库完成角色间的物品转移，但是绑定的物品是无法转移的。");
       end

end

Rcangkuzhiyin = {}
Rcangkuzhiyin["OnTalk"] = OnTalk
Rcangkuzhiyin["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rcangkuzhiyin["OnCompleteQuestTalk"] = OnCompleteQuestTalk