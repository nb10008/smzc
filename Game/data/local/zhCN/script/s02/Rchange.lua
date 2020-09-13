local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600066);
	Quest.SetCompleteOptions(5600066);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>嫦娥：<color=0xffffffff><font=龙字体,0,17,20>\\n      中秋佳节倍思亲，这几年来我一个人孤零零的在这天宫之上，没想到偷偷离开天宫，来到这里，到处都弥漫着烤肉的气息，香气四溢。我的肚子好饿呀！谁能准备点食物给我~~");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30826)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30826,"<font=龙字体,0,19,20><color=0xfffcf157>嫦娥：<color=0xffffffff><font=龙字体,0,17,20>\\n      制作烤肉大餐需要50个猪肉、50个鸡翅与10株辣椒，你有足够的食材可以制作吗？");
        elseif( id == 30827)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30827,"<font=龙字体,0,19,20><color=0xfffcf157>嫦娥：<color=0xffffffff><font=龙字体,0,17,20>\\n      肚子饿的咕咕叫，这么香，真是要人命啦！你已经准备好丰盛的烤肉大餐要给我享用了吗？");
        elseif( id == 30828)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30828,"<font=龙字体,0,19,20><color=0xfffcf157>嫦娥：<color=0xffffffff><font=龙字体,0,17,20>\\n      Hot！ Hot！好热呀~吃辣吃的我都流汗了！这些辣椒还不够，你有准备好20个辣椒给我吗?");
        elseif( id == 30830)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30830,"<font=龙字体,0,19,20><color=0xfffcf157>嫦娥：<color=0xffffffff><font=龙字体,0,17,20>\\n      没想到烤肉大餐是如此的美味，不行，我绝对不会让我的行囊就这么空着回去，有多少食材快些拿来吧~辣椒就不要了，皮肤都受不了了啦。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30826)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30826,"<font=龙字体,0,19,20><color=0xfffcf157>嫦娥：<color=0xffffffff><font=龙字体,0,17,20>\\n      中秋佳节的烤肉大餐，这就奉上咯~");
        elseif( id == 30827)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30827,"<font=龙字体,0,19,20><color=0xfffcf157>嫦娥：<color=0xffffffff><font=龙字体,0,17,20>\\n      这~这~那就不好意思了哦~");
        elseif( id == 30828)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30828,"<font=龙字体,0,19,20><color=0xfffcf157>嫦娥：<color=0xffffffff><font=龙字体,0,17,20>\\n      20个辣椒这么快就到手了吗，很积极哦~");
        elseif( id == 30830)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30830,"<font=龙字体,0,19,20><color=0xfffcf157>嫦娥：<color=0xffffffff><font=龙字体,0,17,20>\\n      还不够哦~");
       end
end

Rchange = {}
Rchange["OnTalk"] = OnTalk
Rchange["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rchange["OnCompleteQuestTalk"] = OnCompleteQuestTalk