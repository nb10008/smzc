delete from fabao,equip,item using item inner join equip inner join fabao 
where item.serialnum = equip.serialnum and equip.serialnum = fabao.serialnum 
and item.typeid >= 8600001 and item.typeid <= 8600031;


delete from equip,item using item inner join equip  
where item.serialnum = equip.serialnum 
and item.typeid >= 8600001 and item.typeid <= 8600031;

