#pragma once

#define DataCollection_Max_Data_Num 100
#define DataCollection_Max_RoleData_Num 20
#define DataCollection_Index_Valid(index) (index>=0 && index< DataCollection_Max_Data_Num)
#define DataCollection_Role_Index_Valid(index) (index>=0 && index< DataCollection_Max_RoleData_Num)
#define DataCollection_Save_DB_Internal 5*60*10
