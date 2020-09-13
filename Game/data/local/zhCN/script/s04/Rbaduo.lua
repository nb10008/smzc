
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010200);
	Quest.SetCompleteOptions(5010200);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>爱吃魔石的巴多：<color=0xffffffff><font=龙字体,0,17,20>\\n金色魔石，可以增加物理攻击\\n黑色魔石，可以增加法术攻击\\n红色魔石，可以恢复体力\\n蓝色魔石，可以回复真气\\n这些都是我的最爱~！");

end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 50007)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50007,"<font=龙字体,0,19,20><color=0xfffcf157>爱吃魔石的巴多：<color=0xffffffff><font=龙字体,0,17,20>\\n你好，我叫<D>巴多</D>，是个石头人。不要看我很硕大，其实我很虚弱，我只有不断地吃<I>魔石</I>才能够维持生命，<I>魔石</I>分为很多种，但都是通过用<T>小魔锤</T>砸碎<I>魔核</I>获得的。\\n这里的魔物基本都有几率掉落<I>小魔锤</I>，<T>但是只有石头质地的魔物才会掉落魔核</T>。\\n你能否帮我去搞到一些<I>魔石</I>？\\n \\n<T>这是个日常任务，每日只可完成一次</T>");
	elseif( id == 50027)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50027,"<font=龙字体,0,19,20><color=0xfffcf157>爱吃魔石的巴多：<color=0xffffffff><font=龙字体,0,17,20>\\n你。。。你知道友谊是什么东西吗？<D>巴多</D>也想知道，非常想知道。<D>巴多</D>想看看友谊是什么样子的，就。。。。就靠你了。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 50007)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50007,"<font=龙字体,0,19,20><color=0xfffcf157>爱吃魔石的巴多：<color=0xffffffff><font=龙字体,0,17,20>\\n恢复活力了！谢谢你。\\n如果你有<I>魔核碎片</I>，可以找到<D>魔核碎片收集人</D>，他那里有<I>石宝宝宠物</I>可以兑换哦！\\n我这里还有些<I>小魔锤</I>和<I>魔核碎片</I>，记得明天你可以一样来找我。");
	elseif( id == 50027)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50027,"<font=龙字体,0,19,20><color=0xfffcf157>爱吃魔石的巴多：<color=0xffffffff><font=龙字体,0,17,20>\\n这个就是友谊？<D>巴多</D>想要自己的友谊，不过还是谢谢你。");
       end

end

Rbaduo = {}
Rbaduo["OnTalk"] = OnTalk
Rbaduo["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rbaduo["OnCompleteQuestTalk"] = OnCompleteQuestTalk