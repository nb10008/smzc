
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010082);
	Quest.SetCompleteOptions(5010082);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>强化师萨维：<color=0xffffffff><font=龙字体,0,17,20>\\n当你获得了装备以后，品质越高越有进行强化的价值.");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40018)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40018,"<font=龙字体,0,19,20><color=0xfffcf157>强化师萨维：<color=0xffffffff><font=龙字体,0,17,20>\\n当你获得了<I>装备</I>以后，便可进行<T>强化</T>。<C>品质</C>越高越有强化的价值。我简要地向你介绍下<T>装备强化</T>：\\n    1、不同的装备强化后<T>所增加的属性不同</T>\\n    2、强化只有在<D>装备强化师</D>处进行\\n    3、每次强化需要消耗<I>强化石</I>，\\n    4、强化是有<T>成功几率</T>的\\n首先我们需要的是<I>强化石</I>，你去收集<C>15</C>颗<I>石怪残骸</I>，将<A>小石怪</A>击杀可以获得，我自会帮你炼化出强化石。");
        elseif( id == 40019)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40019,"<font=龙字体,0,19,20><color=0xfffcf157>强化师萨维：<color=0xffffffff><font=龙字体,0,17,20>\\n获得<I>强化石</I>后便可以找<D>装备强化师</D>进行强化，接下来是强化的注意事项：\\n  1、强化后，装备的强化等级<T>显示于装备名称之后</T>\\n  2、<I>强化石</I>可将装备强化至<A>+15</A>，之后需要<I>强化水晶</I>\\n  3、任何装备的<T>第一次强化</T><C>100%</C><T>成功</T>\\n  4、强化失败，且未使用<I>保底符</I>，则<T>装备会被摧毁</T>\\n  5、强化失败，且使用了<I>保底符</I>，则<T>强化等级</T><A>-1</A>\\n  6、<I>幸运符</I>可以<T>提升强化的成功几率</T>\\n以上这些信息请牢记。如果你有<C>10</C>份<I>结晶</I>，我可以奖励一些<I>幸运符</I>给你。");
        elseif( id == 40020)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40020,"<font=龙字体,0,19,20><color=0xfffcf157>强化师萨维：<color=0xffffffff><font=龙字体,0,17,20>\\n当<T>武器强化到+2</T>以后，便会出现<D>绚丽光晕</D>，强化等级增加，颜色也会变化。不过记住，强化等级越高，越应该使用<I>保底符</I>，光赌运气是不成的。\\n如果<I>强化石</I>和<I>幸运符</I>不够用了，用<C>20</C>份<I>结晶</I>可以去和<D>罗梅达亚</D>交换，他居住在<N>斗战天城</N>内！");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40018)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40018,"<font=龙字体,0,19,20><color=0xfffcf157>强化师萨维：<color=0xffffffff><font=龙字体,0,17,20>\\n稍等下，这就合成出<I>强化石</I>，一般来说，<T>任务奖励给予的强化石</T>都有<T>限制</T>，只能对某些等级下的装备进行强化，请及时使用。\\n而击杀几乎所有的怪物都有可能获得<I>通用的强化石</I>，这些<I>强化石</I>是没有装备等级对应的限制。");
        elseif( id == 40019)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40019,"<font=龙字体,0,19,20><color=0xfffcf157>强化师萨维：<color=0xffffffff><font=龙字体,0,17,20>\\n这些<I>新手幸运符</I>请收下吧！请注意，<I>新手幸运符</I>只对<A>+1</A>至<A>+5</A>的装备比较有效，<A>+5</A>之后的提升成功率效果会逐渐变弱。");

        
       end


end

Rweisa = {}
Rweisa["OnTalk"] = OnTalk
Rweisa["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rweisa["OnCompleteQuestTalk"] = OnCompleteQuestTalk