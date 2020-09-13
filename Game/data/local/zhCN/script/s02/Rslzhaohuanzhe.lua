
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(6040030);
	Quest.SetCompleteOptions(6040030);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n<T>圣灵之魂</T>已经降临到三界，快来领取属于你的<T>圣灵之魂</T>吧！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 51001)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(51001,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n<T>圣灵之魂</T>已经来到我们的世界！\\n<T>圣灵之魂</T>不同于神、人、魔，是存在于异世界的神秘物种。他们拥有强大的战斗力，足以为三界带来翻天覆地的改变。");
	elseif( id == 51002)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(51002,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n在三界，<T>圣灵</T>只有通过吸收<I>妖精</I>的精华才能获得能力的成长。妖精的<A>颜色</A>和<A>等级</A>都会影响<T>圣灵</T>成长的幅度。\\n如果没有妖精可以吞噬，<I>妖精魂核</I>也是具有相同效果的。");
	elseif( id == 51003)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(51003,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n给圣灵装备圣纹是增强<T>圣灵</T>的另一个途径，<I>圣纹</I>是刻印在<T>圣灵</T>身上的一种特殊符文。");
	elseif( id == 51004)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(51004,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n我身边的<color=0xff00ddbb>圣纹制作师<color=0xffffffff>可以帮助玩家快速的合成圣纹装备，请尝试一下<D>圣纹合成</D>吧！");
	elseif( id == 51005)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(51005,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n<T>圣灵之魂</T>已经来到我们的世界！\\n当<color=0xffff00ff>圣灵降临II<color=0xffffffff>状态消失时，你就可以领取第二只<T>圣灵之魂</T>了！");
	elseif( id == 51006)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(51006,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n<T>圣灵之魂</T>已经来到我们的世界！\\n当<color=0xffff00ff>圣灵降临III<color=0xffffffff>状态消失时，你就可以领取第三只<T>圣灵之魂</T>了！");
	elseif( id == 51007)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(51007,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n<T>圣灵之魂</T>已经来到我们的世界！\\n当<color=0xffff00ff>圣灵降临IV<color=0xffffffff>状态消失时，你就可以领取第四只<T>圣灵之魂</T>了！");
	elseif( id == 51008)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(51008,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n领取<I>圣灵技能召唤书</I>，然后使用它，就会学会<T>圣灵召唤</T>技能。");	
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 51001)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(51001,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n只要将<T>圣灵之魂<T>佩戴在身上，使用圣灵召唤技能，即可召唤出圣灵，每次召唤会消耗1000点真气。\\n请选择一个你喜欢的<T>圣灵之魂</T>吧：\\n<D>圣灵·楠杜尔</D>：强大的战斗伤害\\n<D>圣灵·科拉迪</D>：出众的防护水平\\n<D>圣灵·泰西斯</D>：高超的必杀技巧\\n<D>圣灵·多莉斯</D>：均衡的能力属性");
	elseif( id == 51002)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(51002,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n打开角色属性界面的<T>圣灵</T>分页点击<M>吞噬</M>按钮即可打开<M>妖精吞噬</M>界面。\\n不同的<T>圣灵</T>拥有不同的<M>吞噬等级</M>上限，能力成长的强弱取决于被吞噬的妖精的<A>颜色</A>和<A>等级</A>。\\n在妖精吞噬过程中，可以使用<color=0xff05ff00>妖精魂核<color=0xffffffff>代替妖精被圣灵吞噬。使用此魂核进行妖精吞噬的效果，等同于吞噬一只紫色妖精。妖精魂核<color=0xffff0000>不可用于妖精熔炼、妖精融合等<color=0xffffffff>\\n只有等级在<C>40</C>级以上，非粉色妖精，才可以被<T>圣灵</T>吞噬\\n\\n<color=0xffff0000>吞噬过程不可回退，请谨慎选择要吞噬的妖精！<color=0xffffffff>");
	elseif( id == 51003)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(51003,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n在<T>圣灵</T>分页点击<M>刻印</M>按钮即可打开<M>圣纹刻印</M>界面。\\n旁边的<A>圣纹制作师</A>处可以将<I>封印的圣纹回路</I>加工成可使用的<D>圣纹</D>。");
	elseif( id == 51004)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(51004,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n制作出来的圣纹处于<color=0xffffff00>未鉴定<color=0xffffffff>状态，需要<color=0xff00a2ff>圣灵之光<color=0xffffffff>进行鉴定，圣灵之光可以通过<color=0xff05ff00>萃取圣纹装备<color=0xffffffff>或<color=0xff05ff00>商城购买<color=0xffffffff>的方式获得。\\n已经刻印到<T>圣灵</T>身上的<D>圣纹</D>需要<I>洗印符文</I>拆除下来，当拥有<I>洗印符文</I>后，在<T>圣灵</T>分页对应的<D>圣纹</D>下方点击<M>洗</M>按钮，即可完成洗印操作。\\n\n圣纹制作完成之后，打开角色界面选择<T>圣灵</T>，然后将制作好的<I>圣纹</I>直接拖至<T>圣纹槽</T>中即可装备圣纹。");
	elseif( id == 51005)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(51005,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n只要将<T>圣灵之魂<T>佩戴在身上，使用圣灵召唤技能，即可召唤出圣灵，每次召唤会消耗1000点真气。\\n请选择一个你喜欢的<T>圣灵之魂</T>吧：\\n<D>圣灵·楠杜尔</D>：强大的战斗伤害\\n<D>圣灵·科拉迪</D>：出众的防护水平\\n<D>圣灵·泰西斯</D>：高超的必杀技巧\\n<D>圣灵·多莉斯</D>：均衡的能力属性");
	elseif( id == 51006)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(51006,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n只要将<T>圣灵之魂<T>佩戴在身上，使用圣灵召唤技能，即可召唤出圣灵，每次召唤会消耗1000点真气。\\n请选择一个你喜欢的<T>圣灵之魂</T>吧：\\n<D>圣灵·楠杜尔</D>：强大的战斗伤害\\n<D>圣灵·科拉迪</D>：出众的防护水平\\n<D>圣灵·泰西斯</D>：高超的必杀技巧\\n<D>圣灵·多莉斯</D>：均衡的能力属性");
	elseif( id == 51007)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(51007,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\\n只要将<T>圣灵之魂<T>佩戴在身上，使用圣灵召唤技能，即可召唤出圣灵，每次召唤会消耗1000点真气。\\n请选择一个你喜欢的<T>圣灵之魂</T>吧：\\n<D>圣灵·楠杜尔</D>：强大的战斗伤害\\n<D>圣灵·科拉迪</D>：出众的防护水平\\n<D>圣灵·泰西斯</D>：高超的必杀技巧\\n<D>圣灵·多莉斯</D>：均衡的能力属性");
	elseif( id == 51008)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(51008,"<font=龙字体,0,19,20><color=0xfffcf157>圣灵召唤者：<color=0xffffffff><font=龙字体,0,17,20>\\n快拿着这本珍贵的技能书吧。要记得尽快学习<T>圣灵召唤</T>技能，");
	end
end

Rslzhaohuanzhe = {}
Rslzhaohuanzhe["OnTalk"] = OnTalk
Rslzhaohuanzhe["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rslzhaohuanzhe["OnCompleteQuestTalk"] = OnCompleteQuestTalk