
--Apex2系统中的踢人时的错误号列表.

function AddErrNo()	

				AddErrAction( 61039, 1 );	
				AddErrAction( 61061, 1 );
				AddErrAction( 61099, 1 );
				
				--AddErrAction( 47200, 1 ); --驱动未启动
				AddErrAction( 63012, 1 );
				--AddErrAction( 61113, 1 );
				--AddErrAction( 61112, 1 );
				AddErrAction( 61096, 1 );
				AddErrAction( 61097, 1 );
				AddErrAction( 61086, 1 );
				AddErrAction( 61087, 1 );
				--AddErrAction( 61073, 1 );
				AddErrAction( 61084, 1 );
				--AddErrAction( 61090, 1 );
				AddErrAction( 49001, 1 );
				AddErrAction( 49002, 1 );
				AddErrAction( 49003, 1 );
				--AddErrAction( 63002, 1 );
				--AddErrAction( 63001, 1 );
	
			    AddErrActionRange( 41100, 41150, 1 );
			    AddErrActionRange( 41300, 41350, 1 );
			    AddErrActionRange( 41500, 41510, 1 );
				AddErrActionRange( 43500, 43600, 1 );
				AddErrActionRange( 47000, 47050, 1 );
				AddErrActionRange( 47500, 47530, 1 );
				AddErrActionRange( 47600, 47631, 1 );
				AddErrActionRange( 42500, 42600, 1 );
				
			    DelErrNo(41101);
			    DelErrNo(41302);
			    DelErrNo(47031);
			    DelErrNo(47032);
			    
				AddKillTimes_Every(1000);     
				--AddKillTimes_One(47200,10000);
				AddKillTimes_One(61087,10000);

				SetKillSpeed_Every(30);          --单位时间内设置每个错误号踢人的次数
				--SetKillSpeed_One(61039,4);			 --单位时间内单独设置错误号踢人的次数
				--SetKillSpeed_One(47200,100);
				SetKillSpeed_One(61087,100);
				SetKillSpeed_UnitTime(1*60*1000);--设置踢人的时间长度

				
				--AddKillTimes_One(61062,400);         --此错误号第三次就不踢了,这个次数按实际情况设定
				--AddDelayTime(120000,3);              --在2分钟内有错误号踢人次数大于等于3的都保护了,不踢
				--SetDelayKill( 0, 3 );                --第一个参数是延时开关，第二个参数是延时偏差
				--AddKillDelay( 61081, 10000);         --第一个参数错误号，第二个参数踢人延时时长，即在查到后直到10s才踢
end

AddErrNo();


