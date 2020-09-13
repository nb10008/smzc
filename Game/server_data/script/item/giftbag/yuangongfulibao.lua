--1¼¶Ô±¹¤¸£Àû°ü
function I4700038_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--Íæ¼ÒID£¬ Éú³ÉÎïÆ·TypeID£¬ÊýÁ¿£¬Æ·ÖÊ£¬Éú³ÉÄ£Ê½£¬logÓÃÍøÂçÏûÏ¢Ã¶¾Ù
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 19, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700039, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051607, 99, -1, 8, 420)  --³õ¼¶¼Ó¹¤ÐÄµÃ£¨°ó¶¨£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051607, 99, -1, 8, 420)  --³õ¼¶¼Ó¹¤ÐÄµÃ£¨°ó¶¨£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051608, 99, -1, 8, 420)  --³õ¼¶ÖýÔìÐÄµÃ£¨°ó¶¨£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051608, 99, -1, 8, 420)  --³õ¼¶ÖýÔìÐÄµÃ£¨°ó¶¨£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051609, 99, -1, 8, 420)  --³õ¼¶²Ã·ìÐÄµÃ£¨°ó¶¨£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3051609, 99, -1, 8, 420)  --³õ¼¶²Ã·ìÐÄµÃ£¨°ó¶¨£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050001, 100, -1, 8, 420)  --Ô­¿ó
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --¿óÔü
end

function I4700038_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700038, 1, "I4700038_GiftBag")
aux.RegisterItemEvent(4700038, 0, "I4700038_CanUseGiftBag")

--30¼¶Ô±¹¤¸£Àû°ü
function I4700039_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--Íæ¼ÒID£¬ Éú³ÉÎïÆ·TypeID£¬ÊýÁ¿£¬Æ·ÖÊ£¬Éú³ÉÄ£Ê½£¬logÓÃÍøÂçÏûÏ¢Ã¶¾Ù
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700040, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 2, -1, 8, 420)   --°ó¶¨+10
	role.AddRoleItem(MapID, InstanceID, TargetID, 4222004, 1, -1, 8, 420)	--ÐÜÃ¨·É³µ£¨´ô£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211403, 1, -1, 8, 420)	--Íú²Æ
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070008, 1, -1, 8, 420)   --ËæÉí²Ö¿â
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 30, -1, 8, 420)	--³¬¼¶¾­Ñé·û£¨°üÌì£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 999, -1, 8, 420)	--ÐüÉÍÁî
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070085, 1, -1, 8, 420)	--5000ÔùÈ¯
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)	--Õ½¶·Éñ·û[°üÖÜ]
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070022, 20, -1, 8, 420)	--°ó¶¨Ñý¾«ÃØÒ©
	role.AddRoleItem(MapID, InstanceID, TargetID, 4622106, 1, -1, 8, 420)   --ÆïÊõ6¼¶¼¼ÄÜÊé
	role.AddRoleItem(MapID, InstanceID, TargetID, 4622210, 1, -1, 8, 420)	--ÆïÊõ¾«Í¨6¼¶¼¼ÄÜÊé
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630001, 1, -1, 8, 420)	--ÐüÉÍÊ¹Õß
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630101, 1, -1, 8, 420)	--¿Õ¼äÐÐÕß
	role.AddRoleItem(MapID, InstanceID, TargetID, 4630201, 1, -1, 8, 420)	--ÉÌÈËÊØ»¤Õß
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 10, -1, 8, 420)  --¾­Ñé±¦Æ¿£¨°ó¶¨£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --¿óÔü
	role.AddRoleItem(MapID, InstanceID, TargetID, 4080079, 999, -1, 8, 420)  --±±ÖÞÖ®Ñ©
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 999, -1, 8, 420)  --ÐÅÑöÉñ·û£¨°ó¶¨£©

	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8160020, 1, 5, 8, 420)
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8170020, 1, 5, 8, 420)
	end
end

function I4700039_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 14) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700039, 1, "I4700039_GiftBag")
aux.RegisterItemEvent(4700039, 0, "I4700039_CanUseGiftBag")

--50¼¶Ô±¹¤¸£Àû°ü
function I4700040_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--Íæ¼ÒID£¬ Éú³ÉÎïÆ·TypeID£¬ÊýÁ¿£¬Æ·ÖÊ£¬Éú³ÉÄ£Ê½£¬logÓÃÍøÂçÏûÏ¢Ã¶¾Ù
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700041, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 8, -1, 8, 420) 	--°ó¶¨+10
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020118, 2, -1, 8, 420) 	--°ó¶¨+12
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 20, -1, 8, 420)	--¾«Á¶Ñý¾«ÃØÒ©
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 50, -1, 8, 420)  --¾­Ñé±¦Æ¿£¨°ó¶¨£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 999, -1, 8, 420)  --ÌìÉñ´ú±Ò
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 999, -1, 8, 420)  --»¶ÀÖ±Ò
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --¿óÔü
    local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8010021, 1, 5, 8, 420)  --´«³ÐÖ®ÈÐ
		role.AddRoleItem(MapID, InstanceID, TargetID, 8020021, 1, 5, 8, 420)	--´«³ÐÖ®¹­
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030121, 10, -1, 8, 420)	--°ó¶¨Èý¼¶Ì«ÑôÊ¯
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8030021, 1, 5, 8, 420)	--´«³ÐÖ®ÇÙ
		role.AddRoleItem(MapID, InstanceID, TargetID, 8040021, 1, 5, 8, 420)	--´«³ÐÖ®Æ÷
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030123, 10, -1, 8, 420)	--°ó¶¨Èý¼¶ÔÂ¹âÊ¯
	end
end

function I4700040_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700040, 1, "I4700040_GiftBag")
aux.RegisterItemEvent(4700040, 0, "I4700040_CanUseGiftBag")

--60¼¶Ô±¹¤¸£Àû°ü
function I4700041_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--Íæ¼ÒID£¬ Éú³ÉÎïÆ·TypeID£¬ÊýÁ¿£¬Æ·ÖÊ£¬Éú³ÉÄ£Ê½£¬logÓÃÍøÂçÏûÏ¢Ã¶¾Ù
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700042, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 30, -1, 8, 420)  --³¬¼¶¾­Ñé·û£¨°üÌì£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070085, 1, -1, 8, 420)	--5000ÔùÈ¯
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010020, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010025, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 50, -1, 8, 420)  --¾­Ñé±¦Æ¿£¨°ó¶¨
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800039, 20, -1, 8, 420)  --60Î±Ä§¶Ò»»È¯
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 999, -1, 8, 420)  --»¶ÀÖ±Ò
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820035, 10, -1, 8, 420)  --ÉÏ¹Åµä¼®
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --¿óÔü
	 local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8013007, 1, 5, 8, 420)  --Ä§ÉñÎäÆ÷
		role.AddRoleItem(MapID, InstanceID, TargetID, 8023007, 1, 5, 8, 420)
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8033007, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8043007, 1, 5, 8, 420)
	end
end

function I4700041_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700041, 1, "I4700041_GiftBag")
aux.RegisterItemEvent(4700041, 0, "I4700041_CanUseGiftBag")

--70¼¶Ô±¹¤¸£Àû°ü
function I4700042_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--Íæ¼ÒID£¬ Éú³ÉÎïÆ·TypeID£¬ÊýÁ¿£¬Æ·ÖÊ£¬Éú³ÉÄ£Ê½£¬logÓÃÍøÂçÏûÏ¢Ã¶¾Ù
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700043, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020121, 10, -1, 8, 420)  --°ó¶¨+15
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020061, 20, -1, 8, 420)	--ÖÐ¼¶Îü»êÊ¯
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 50, -1, 8, 420)  --¾­Ñé±¦Æ¿£¨°ó¶¨
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800131, 999, -1, 8, 420) --ÐÞÁ¶³¡ÃÅÆ±£¨°ó¶¨
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 999, -1, 8, 420)  --»¶ÀÖ±Ò
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820035, 30, -1, 8, 420)  --ÉÏ¹Åµä¼®
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --¿óÔü
--[[	 local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8013007, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8023007, 1, 5, 8, 420)
	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8033007, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8043007, 1, 5, 8, 420)
	end]]
end

function I4700042_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700042, 1, "I4700042_GiftBag")
aux.RegisterItemEvent(4700042, 0, "I4700042_CanUseGiftBag")

--80¼¶Ô±¹¤¸£Àû°ü
function I4700043_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--Íæ¼ÒID£¬ Éú³ÉÎïÆ·TypeID£¬ÊýÁ¿£¬Æ·ÖÊ£¬Éú³ÉÄ£Ê½£¬logÓÃÍøÂçÏûÏ¢Ã¶¾Ù
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020061, 20, -1, 8, 420)  --ÖÐ¼¶Îü»êÊ¯
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020016, 10, -1, 8, 420)  --¸ß¼¶Îü»êÊ¯
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020124, 5, -1, 8, 420)  --°ó¶¨+18
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020126, 2, -1, 8, 420)  --°ó¶¨+20
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 50, -1, 8, 420)  --¾«Á¶Ñý¾«ÃØÒ©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 999, -1, 8, 420)	--ÐüÉÍÁî[Ãâ·Ñ]
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600007, 100, -1, 8, 420)	--³¬¼¶¾­Ñé·û£¨°üÌì£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)	--Õ½¶·Éñ·û£¨°üÖÜ£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010021, 1, -1, 8, 420)	--³¬¼¶Ê¥Ò©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010026, 1, -1, 8, 420)	--³¬¼¶Ê¥Ë®
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 99, -1, 8, 420)  --¾­Ñé±¦Æ¿£¨°ó¶¨
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800041, 20, -1, 8, 420)  --80Î±Ä§¶Ò»»È
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 999, -1, 8, 420)  --»¶ÀÖ±Ò¯
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700230, 15, -1, 8, 420)   --9¼¶³É³¤×°±¸Ïä
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820255, 200, -1, 8, 420)  --³É³¤Ö®Êé£¨°ó¶¨£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820035, 50, -1, 8, 420)  --ÉÏ¹Åµä¼®
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060062, 2, -1, 8, 420)  --Ñý¾«Ç±ÄÜÃØÒ©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060058, 10, -1, 8, 420)  --ÎÈ¹ÌÑý¾«ÃØÒ©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060059, 20, -1, 8, 420)  --¸ßµÈÑý¾«ÃØÒ©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060061, 1, -1, 8, 420)  --ÖÁ×ðÃØÒ©£¨Ô±¹¤×¨ÓÃ£¬Î´¶ÔÍâµÀ¾ß£©
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050002, 999, -1, 8, 420)  --¿óÔü
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030115, 50, -1, 8, 420)  --4¼¶±¦Ê¯²ð³ýÆ÷
	local Vocation = role.GetRoleClass(TargetID)
	if Vocation == 1 or	Vocation == 2 or Vocation == 3 or Vocation == 6 or Vocation == 7 or Vocation == 8 or Vocation == 9	then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8013009, 1, 5, 8, 420)  --80Ä§ÉñÎäÆ÷
		role.AddRoleItem(MapID, InstanceID, TargetID, 8023009, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030306, 10, -1, 8, 420)--ÖËÈÈÌ«ÑôÊ¯[4¼¶]

	elseif Vocation == 4 or Vocation == 5 or Vocation == 10 or Vocation == 11 or Vocation == 12 or Vocation == 13 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 8033009, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 8043009, 1, 5, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030326, 10, -1, 8, 420)--ð¨½àÔÂ¹âÊ¯[4¼¶]
	end
end

function I4700043_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 34) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700043, 1, "I4700043_GiftBag")
aux.RegisterItemEvent(4700043, 0, "I4700043_CanUseGiftBag")

