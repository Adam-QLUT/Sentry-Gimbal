#include "RM_Cilent_UI.h"
#include "UART_data_transmit.h"
#include "string.h"
#include "stdio.h"
#include "CRC8_CRC16.h"
// #include "cap_ctl.h"
#include "shoot.h"
#include "math.h"
#include "cmsis_os.h"
#include "referee_handle_pack.h"
// #include "Global_status.h"
// #include "CAN_receive&send.h"
// #include "IMU_updata.h"
// #include "DBUS_remote_control.h"

#define Root_2 1.4142136f

uint8_t UI_Seq, press_refrsh_flag;       // 包序号

Graph_Data G1, G2, G3, G4, G5, G6, G7;   // 瞄准线用UI
String_Data G8, G10, G11, G12, G13, G14, G15, G16, G17, G24, G25; // 指示状态用UI，字符

Graph_Data  G9, G18; //自瞄UI

Graph_Data G19, G20, G21, G22, G23; //车辆指向UI  //云台指向竖直向上UI


uint16_t Robot_ID = UI_Data_RobotID_RStandard1; // ID默认红1
uint16_t Cilent_ID = UI_Data_CilentID_RStandard1;

void UI_task_init(void)
{
   memset(&G1, 0, sizeof(G1));
   memset(&G2, 0, sizeof(G2));
   memset(&G3, 0, sizeof(G3));
   memset(&G4, 0, sizeof(G4));
   memset(&G5, 0, sizeof(G5));
   memset(&G6, 0, sizeof(G6));
   memset(&G7, 0, sizeof(G7));
   memset(&G8, 0, sizeof(G8));
   memset(&G9, 0, sizeof(G9));

   memset(&G12, 0, sizeof(G12));	
   memset(&G13, 0, sizeof(G13));
   memset(&G14, 0, sizeof(G14));
   memset(&G15, 0, sizeof(G15));
   memset(&G16, 0, sizeof(G16));
   memset(&G17, 0, sizeof(G17));
   memset(&G18, 0, sizeof(G18));
   memset(&G24, 0, sizeof(G24));
	 memset(&G25, 0, sizeof(G25));


//   memset(&G19, 0, sizeof(G19));
//   memset(&G20, 0, sizeof(G20));
//   memset(&G21, 0, sizeof(G21));
//   memset(&G22, 0, sizeof(G22));
//   memset(&G23, 0, sizeof(G23));

   infanty_hero_status_UI_add();
}

void infanty_hero_status_UI_add(void) // 手动按键添加UI
{
	char norm_arr[4] = "norm";
	char shoot_arr[5] = "shoot";
	char trigger_arr[5] = "trigg";
	char power_buff[10] = "speed_down";
	char super_cap_arr[4] = "CAP1";
	char ammo_arr[7] = "ammo_no";
	char anti_top[6] = "antiop";
	char cover[7]="covercl";
	char fly[7]="flyclos";

	Circle_Draw(&G1, "001", UI_Graph_ADD, 9, UI_Color_Main, 4, 960, 510, 12);
	Line_Draw(&G6, "006", UI_Graph_ADD, 9, UI_Color_Green, 2, 960, 540, 960, 433); // 绘制1-5m步兵的下坠弹道
	Char_Draw(&G8, "008", UI_Graph_ADD, 7, UI_Color_Orange, 35, 4, 5, 720, 310, &norm_arr[0]); //底盘模式切换 
	Circle_Draw(&G9, "009", UI_Graph_ADD, 7, UI_Color_Purplish_red, 4, 960, 540, 8);//自瞄圈
	Char_Draw(&G10, "010", UI_Graph_ADD, 7, UI_Color_Purplish_red, 24, 6, 4, 1040, 540,&anti_top[0]); 			 
	Char_Draw(&G13, "013", UI_Graph_ADD, 5, UI_Color_Orange, 24, 5, 4, 450, 740, &shoot_arr[0]);   //射击速度
	Char_Draw(&G14, "014", UI_Graph_ADD, 5, UI_Color_Orange, 24, 5, 4, 450, 700, &trigger_arr[0]); //拨弹单连发切换
	Char_Draw(&G15, "015", UI_Graph_ADD, 5, UI_Color_Orange, 24, 4, 4, 1000, 234, &super_cap_arr[0]); //电量剩余
	Char_Draw(&G16, "016", UI_Graph_ADD, 5, UI_Color_Orange, 24, 10, 4, 720, 234, &power_buff[0]);    // BUFF
	Char_Draw(&G25, "025", UI_Graph_ADD, 6, UI_Color_Orange, 24, 7, 4, 450, 660, &cover[0]);    // 弹舱盖状态
	//Char_Draw(&G24, "024", UI_Graph_ADD, 6, UI_Color_Orange, 24, 7, 4, 450, 625, &fly[0]);    // 飞坡模式

	
	//Char_Draw(&G17, "017", UI_Graph_ADD, 6, UI_Color_Orange, 24, 7, 4, 450, 660, &ammo_arr[0]);    // 剩余弹药量更新
	
	Rectangle_Draw(&G18, "018", UI_Graph_ADD, 5, UI_Color_Orange, 2, 670, 810, 1250, 270); //自瞄框	 
//	Line_Draw(&G19, "019", UI_Graph_ADD, 3, UI_Color_Green, 2, 898, 160, 1018, 160); //底盘状态
//	Line_Draw(&G20, "020", UI_Graph_ADD, 3, UI_Color_Pink, 2, 898, 40, 898, 160);
//	Line_Draw(&G21, "021", UI_Graph_ADD, 3, UI_Color_Pink, 2, 1018, 40, 898, 40);
//	Line_Draw(&G22, "022", UI_Graph_ADD, 3, UI_Color_Pink, 2, 1018, 160, 1018, 40);
//	Line_Draw(&G23, "023", UI_Graph_ADD, 4, UI_Color_Cyan, 2, 958, 100, 958, 185); //云台指向
																																																																																																																																																																										   
}

void press_refrsh(void) // 开局手动添加UI固定准星
{
	   UI_ReFresh(2, G1,G6);
	osDelay(10);
}

void CHASSIS_ReFresh(void) //底盘图示状态刷新
{
   UI_ReFresh(5, G19, G20, G21, G22, G23);
   osDelay(10);
}

void auto_refresh(void) //其他字符刷新
{                      
   Char_ReFresh(G8);
   osDelay(10);
   Char_ReFresh(G13);
   osDelay(10);
   Char_ReFresh(G14);
   osDelay(10);	
   Char_ReFresh(G15);
   osDelay(10);
   Char_ReFresh(G16);
   osDelay(10);
   Char_ReFresh(G17);
   osDelay(10);
   Char_ReFresh(G10);
   osDelay(10);
	 Char_ReFresh(G24);
}

void Auto_Frame(void) //自瞄框刷新
{
   UI_ReFresh(2, G9, G18);
   osDelay(10);
}



void anti( uint16_t status)
{
	char antiop[7] = "antiop";
	char anticl[7] = "anticl";

	if(status == 1)
	  Char_Draw(&G10, "010", UI_Graph_Change, 7, UI_Color_Green, 24, 7, 4, 1180, 540,&antiop[0]);
	else
	  Char_Draw(&G10, "010", UI_Graph_Change, 7, UI_Color_Orange, 24, 7, 4, 1180, 540,&anticl[0]);
}


void Base_sentry_Refresh(uint8_t robot_id,ext_game_robot_HP_t game_robot_HP_t) 
{
	 char sentry_arr[6] = "sentry";	
	 char base_arr[4] = "BASE";	
		uint8_t base_HP,last_base_HP;
		uint8_t sentry_HP,last_sentry_HP;	
	
	if(robot_id>100)
	{
			if(game_robot_HP_t.blue_base_HP<base_HP)
			{
				Char_Draw(&G11, "011", UI_Graph_Change, 5, UI_Color_Purplish_red, 24, 4, 4, 1425, 775, &base_arr[0]);    //基地		
				base_HP = game_robot_HP_t.blue_base_HP;				
			}
			else
			{ 
				Char_Draw(&G11, "011", UI_Graph_Change, 5, UI_Color_Orange, 24, 4, 4, 1425, 775, &base_arr[0]);    //基地	
				base_HP = game_robot_HP_t.blue_base_HP;						
			}

			
			if(game_robot_HP_t.blue_7_robot_HP<sentry_HP)
			{
				Char_Draw(&G10, "010", UI_Graph_Change, 5, UI_Color_Purplish_red, 24, 6, 4, 1425, 815, &sentry_arr[0]); //哨兵		
				sentry_HP = game_robot_HP_t.blue_7_robot_HP;				
			}
			else
			{ 
				Char_Draw(&G10, "010", UI_Graph_Change, 5, UI_Color_Orange, 24, 6, 4, 1425, 815, &sentry_arr[0]); //哨兵		
				sentry_HP = game_robot_HP_t.blue_7_robot_HP;						
			}
	}
	else
	{
			if(game_robot_HP_t.red_base_HP<base_HP)
			{
				Char_Draw(&G11, "011", UI_Graph_Change, 5, UI_Color_Purplish_red, 24, 4, 4, 1425, 775, &base_arr[0]);    //基地		
				base_HP = game_robot_HP_t.red_base_HP;				
			}
			else
			{ 
				Char_Draw(&G11, "011", UI_Graph_Change, 5, UI_Color_Orange, 24, 4, 4, 1425, 775, &base_arr[0]);    //基地	
				base_HP = game_robot_HP_t.red_base_HP;						
			}

			
			if(game_robot_HP_t.red_7_robot_HP<sentry_HP)
			{
				Char_Draw(&G10, "010", UI_Graph_Change, 5, UI_Color_Purplish_red, 24, 6, 4, 1425, 815, &sentry_arr[0]); //哨兵		
				sentry_HP = game_robot_HP_t.red_7_robot_HP;				
			}
			else
			{ 
				Char_Draw(&G10, "010", UI_Graph_Change, 5, UI_Color_Orange, 24, 6, 4, 1425, 815, &sentry_arr[0]); //哨兵		
				sentry_HP = game_robot_HP_t.red_7_robot_HP;						
			}
	}
	
}





void Chassis_Refresh(float angle) //底盘图示状态更新
{

   Line_Draw(&G19, "019", UI_Graph_Change, 3, UI_Color_Pink, 2, 958 + 60 * Root_2 * cosf(-degree2rad(angle + 186.5f)), 100 + 60 * Root_2 * sinf(-degree2rad(angle + 186.5f)), 958 + 60 * Root_2 * cosf(-degree2rad(angle + 96.5f)), 100 + 60 * Root_2 * sinf(-degree2rad(angle + 96.5f)));

   Line_Draw(&G20, "020", UI_Graph_Change, 3, UI_Color_Pink, 2, 958 + 60 * Root_2 * cosf(-degree2rad(angle + 276.5f)), 100 + 60 * Root_2 * sinf(-degree2rad(angle + 276.5f)), 958 + 60 * Root_2 * cosf(-degree2rad(angle + 186.5)), 100 + 60 * Root_2 * sinf(-degree2rad(angle + 186.5)));

   Line_Draw(&G21, "021", UI_Graph_Change, 3, UI_Color_Green, 2, 958 + 60 * Root_2 * cosf(-degree2rad(angle + 6.5f)), 100 + 60 * Root_2 * sinf(-degree2rad(angle + 6.5f)), 958 + 60 * Root_2 * cosf(-degree2rad(angle + 276.5f)), 100 + 60 * Root_2 * sinf(-degree2rad(angle + 276.5f)));

   Line_Draw(&G22, "022", UI_Graph_Change, 3, UI_Color_Pink, 2, 958 + 60 * Root_2 * cosf(-degree2rad(angle + 96.5f)), 100 + 60 * Root_2 * sinf(-degree2rad(angle + 96.5f)), 958 + 60 * Root_2 * cosf(-degree2rad(angle + 6.5f)), 100 + 60 * Root_2 * sinf(-degree2rad(angle + 6.5f)));

   Line_Draw(&G23, "023", UI_Graph_ADD, 4, UI_Color_Cyan, 2, 958, 100, 958, 185); //云台指向
}




void Auto_aim_circle_refresh(uint8_t status) 
{
   switch (status)
	 {
		 case 0:		 
		Circle_Draw(&G9, "009", UI_Graph_Change, 7, UI_Color_Purplish_red, 4, 960, 540, 8);			
			break;
		 
		 case 1:
		Circle_Draw(&G9, "009", UI_Graph_Change, 7, UI_Color_Orange, 4, 960, 540, 8);			 
			break;
		 
		 case 2:
		Circle_Draw(&G9, "009", UI_Graph_Change, 7, UI_Color_Green, 4, 960, 540, 8);
			break;
		 
			default:
		Circle_Draw(&G9, "009", UI_Graph_Change, 7, UI_Color_Purplish_red, 4, 960, 540, 8);			
			break;			
	 }	
}




void shoot_refresh(int16_t status) // 状态机更新，首先添加图层，默认时刻橙色，为0，检测转速和裁判系统之后改为15/18/30
{

   char shoot_arr2[7] = "SHOOT30";
   char shoot_arr3[7] = "SHOOT00";
   if (status > 5000) // 读到机器人升级，弹速变为30m/s
   {
      Char_Draw(&G13, "013", UI_Graph_Change, 5, UI_Color_Green, 24, 7, 4, 450, 740, &shoot_arr2[0]);
   }
   else
   {
      Char_Draw(&G13, "013", UI_Graph_Change, 5, UI_Color_Orange, 24, 7, 4, 450, 740, &shoot_arr3[0]);
   }
}

void trigger_refresh(int16_t status) //拨弹电机单连发更新
{
   char trigger_arr[5] = "LOWWW";
   char trigger_arr1[5] = "HIGHT";
   char trigger_arr3[5] = "zero0";

   switch (status)
   {
   case 0:
      Char_Draw(&G14, "014", UI_Graph_Change, 5, UI_Color_Green, 24, 5, 4, 450, 700, &trigger_arr[0]);
      break;

   case 1:
      Char_Draw(&G14, "014", UI_Graph_Change, 5, UI_Color_Green, 24, 5, 4, 450, 700, &trigger_arr1[0]);
      break;

   default:
      Char_Draw(&G14, "014", UI_Graph_Change, 5, UI_Color_Orange, 24, 5, 4, 450, 700, &trigger_arr3[0]);
      break;
   }
}

void chassis_status_refresh(uint8_t status) //底盘模式切换更新
{
   char normal_arr[4] = "NORM";
   char lean_arr[4] = "LEAN";
   char spin_arr[4] = "SPIN";
   char tank_arr[4] = "TANK";

   // switch (status)
   // {
   // case FLOW:
   //    Char_Draw(&G8, "008", UI_Graph_Change, 7,  UI_Color_Orange, 35, 4, 4, 720, 310, &normal_arr[0]);
   //    break;

   // case LEAN:
   //    Char_Draw(&G8, "008", UI_Graph_Change, 7,  UI_Color_Orange, 35, 4, 4, 720, 310, &lean_arr[0]);
   //    break;

   // case SPIN:
   //    Char_Draw(&G8, "008", UI_Graph_Change, 7, UI_Color_Green, 35, 4, 5, 720, 310, &spin_arr[0]);
   //    break;

   // case TANK:
   //    Char_Draw(&G8, "008", UI_Graph_Change, 7,  UI_Color_Orange, 35, 4, 4, 720, 310, &tank_arr[0]);
   //    break;

   // default:
   //    break;
   // }
}

void auto_aim_ui_update(bool_t yes_or_no) //自瞄更新
{
   if (yes_or_no)
      Rectangle_Draw(&G18, "018", UI_Graph_Change, 5, UI_Color_Green, 2, 670, 810, 1250, 270);
   else
      Rectangle_Draw(&G18, "018", UI_Graph_Change, 5, UI_Color_Orange, 2, 670, 810, 1250, 270);
}

void Super_POWER(float cap_voltage) //电量更新
{
   char Strff[6] = {0};
   float X_Value = cap_voltage;
   int Len = snprintf(Strff, 6, "%lf", X_Value);

   uint8_t flag, last_flag;
   if (cap_voltage > 8.0f)
   {
      if (cap_voltage > 12.0f)
      {
         flag = 1;
      }
      else
      {
         flag = 2;
      }
   }
   else
   {
      flag = 3;
   }
   if (last_flag != flag)
   {
      switch (flag)
      {
      case 1:
         Char_Draw(&G15, "015", UI_Graph_Change, 5, UI_Color_Green, 24, 6, 4, 1000, 234, &Strff[0]);
         break;

      case 2:
         Char_Draw(&G15, "015", UI_Graph_Change, 5, UI_Color_Yellow, 24, 6, 4, 1000, 234, &Strff[0]);
         break;

      case 3:
         Char_Draw(&G15, "015", UI_Graph_Change, 5, UI_Color_Orange, 24, 6, 4, 1000, 234, &Strff[0]); // 低于12v，显示危险状态
         break;

      default:
         break;
      }
   }
   last_flag = flag; //比一直给结构体赋值运行的快
}

void super_cap_speedup(bool_t yes_or_no) // BUFF更新
{
   char super_cap_up[10] = "SPEED_UPUP";
   char super_cap_down[10] = "SPEED_DOWN";
   if (yes_or_no)
      Char_Draw(&G16, "016", UI_Graph_Change, 5, UI_Color_Green, 24, 10, 4, 720, 234, &super_cap_up[0]);
   else
      Char_Draw(&G16, "016", UI_Graph_Change, 5, UI_Color_Orange, 24, 10, 4, 720, 234, &super_cap_down[0]);
}



void cover(uint16_t status) //弹舱盖更新
{
   char coverop[7] = "coverop";
	 char covercl[7] = "covercl";
   if (status)
      Char_Draw(&G17, "017", UI_Graph_Change, 5, UI_Color_Green, 24, 3, 4, 450, 660, &coverop[0]);
   else
      Char_Draw(&G17, "017", UI_Graph_Change, 5, UI_Color_Orange, 24, 3, 4, 450, 660, &covercl[0]);
}


void fly(uint16_t status) //飞坡模式
{
   char flyopen[7] = "flyopen";
	 char flyclos[7] = "covercl";
   if (status)
      Char_Draw(&G24, "024", UI_Graph_Change, 5, UI_Color_Green, 24, 7, 4, 450, 660, &flyopen[0]);
   else
      Char_Draw(&G24, "024", UI_Graph_Change, 5, UI_Color_Orange, 24, 7, 4, 450, 660, &flyclos[0]);
}
void ammo_update(uint16_t ammo_reserve)
{
	char ammo_arr_enough[7] = "ammo_ok";
	char ammo_arr_low[7] = "ammo_no";
	if(ammo_reserve>15)
		Char_Draw(&G17, "017", UI_Graph_Change, 5, UI_Color_Green, 24, 7, 4, 450, 660, &ammo_arr_enough[0]);
	else
		Char_Draw(&G17, "017", UI_Graph_Change, 5, UI_Color_Green, 24, 7, 4, 450, 660, &ammo_arr_low[0]);
}


void UI_id_check(void)
{
   const ext_robot_status_t *robot_state = get_robot_state_point(); // 获取机器人id
   switch (robot_state->robot_id)
   {
   case 1: // 红1
      Robot_ID = UI_Data_RobotID_RHero;
      Cilent_ID = UI_Data_CilentID_RHero;
      break;
   case 2: // 红2
      Robot_ID = UI_Data_RobotID_REngineer;
      Cilent_ID = UI_Data_CilentID_REngineer;
      break;
   case 3: // 红3
      Robot_ID = UI_Data_RobotID_RStandard1;
      Cilent_ID = UI_Data_CilentID_RStandard1;
      break;
   case 4: // 红4
      Robot_ID = UI_Data_RobotID_RStandard2;
      Cilent_ID = UI_Data_CilentID_RStandard2;
      break;
   case 5: // 红5
      Robot_ID = UI_Data_RobotID_RStandard3;
      Cilent_ID = UI_Data_CilentID_RStandard3;
      break;
   case 6: // 红6
      Robot_ID = UI_Data_RobotID_RAerial;
      Cilent_ID = UI_Data_CilentID_RAerial;
      break;
   case 101: // 蓝一
      Robot_ID = UI_Data_RobotID_BHero;
      Cilent_ID = UI_Data_CilentID_BHero;
      break;
   case 102: // 蓝二
      Robot_ID = UI_Data_RobotID_BEngineer;
      Cilent_ID = UI_Data_CilentID_BEngineer;
      break;
   case 103: // 蓝三
      Robot_ID = UI_Data_RobotID_BStandard1;
      Cilent_ID = UI_Data_CilentID_BStandard1;
      break;
   case 104: // 蓝四
      Robot_ID = UI_Data_RobotID_BStandard2;
      Cilent_ID = UI_Data_CilentID_BStandard2;
      break;
   case 105: // 蓝五
      Robot_ID = UI_Data_RobotID_BStandard3;
      Cilent_ID = UI_Data_CilentID_BStandard3;
      break;
   case 106: // 蓝六
      Robot_ID = UI_Data_RobotID_BAerial;
      Cilent_ID = UI_Data_CilentID_BAerial;
      break;
   }
}


/********************************************删除操作*************************************
**参数：Del_Operate  对应头文件删除操作
        Del_Layer    要删除的层 取值0-9
*****************************************************************************************/
// 没写好，没有清除ui的必要，所以别用了
void UI_Delete(uint8_t Del_Operate, uint8_t Del_Layer)
{
   uint8_t cache[100];
   uint8_t *framepoint;         // 读写指针
   uint16_t frametail = 0xFFFF; // CRC16校验值
   UI_Packhead framehead;
   UI_Data_Operate datahead;
   UI_Data_Delete del;

   framepoint = (uint8_t *)&framehead;

   framehead.SOF = UI_SOF;
   framehead.Data_Length = 8;
   framehead.Seq = UI_Seq;
   framehead.CRC8 = Get_CRC8_Check_Sum_UI(framepoint, 4, 0xFF);
   framehead.CMD_ID = UI_CMD_Robo_Exchange; // 填充包头数据

   datahead.Data_ID = UI_Data_ID_Del;
   datahead.Sender_ID = Robot_ID;
   datahead.Receiver_ID = Cilent_ID; // 填充操作数据

   del.Delete_Operate = Del_Operate;
   del.Layer = Del_Layer; // 控制信息

   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(framehead), frametail);
   framepoint = (uint8_t *)&datahead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(datahead), frametail);
   framepoint = (uint8_t *)&del;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(del), frametail); // CRC16校验值计算	
   memcpy(cache, &framehead, sizeof(framehead));
   UART_send_data(UART6_data, cache, sizeof(framehead));
   framepoint = (uint8_t *)&datahead;
   memcpy(cache, &datahead, sizeof(datahead));
   UART_send_data(UART6_data, cache, sizeof(datahead));
	memcpy(cache, &del, sizeof(del));
   UART_send_data(UART6_data, cache, sizeof(del));
	memcpy(cache, &frametail, sizeof(frametail));
   UART_send_data(UART6_data, cache, sizeof(frametail));

   UI_Seq++; // 包序号+1
}

/************************************************绘制直线*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Start_x、Start_x    开始坐标
        End_x、End_y   结束坐标
**********************************************************************************************************/

void Line_Draw(Graph_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t End_x, uint32_t End_y)
{
   int i;
   for (i = 0; i < 3 && imagename[i] != 0; i++)
      image->graphic_name[2 - i] = imagename[i];
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->end_x = End_x;
   image->end_y = End_y;
}

/************************************************绘制矩形*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Start_x、Start_x    开始坐标
        End_x、End_y   结束坐标（对顶角坐标）
**********************************************************************************************************/

void Rectangle_Draw(Graph_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t End_x, uint32_t End_y)
{
   int i;
   for (i = 0; i < 3 && imagename[i] != 0; i++)
      image->graphic_name[2 - i] = imagename[i];
   image->graphic_tpye = UI_Graph_Rectangle;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->end_x = End_x;
   image->end_y = End_y;
}

/************************************************绘制整圆*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Start_x、Start_x    圆心坐标
        Graph_Radius  图形半径
**********************************************************************************************************/

void Circle_Draw(Graph_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t Graph_Radius)
{
   int i;
   for (i = 0; i < 3 && imagename[i] != 0; i++)
      image->graphic_name[2 - i] = imagename[i];
   image->graphic_tpye = UI_Graph_Circle;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->radius = Graph_Radius;
}
/************************************************绘制圆弧*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Graph_StartAngle,Graph_EndAngle    开始，终止角度
        Start_y,Start_y    圆心坐标
        x_Length,y_Length   x,y方向上轴长，参考椭圆
**********************************************************************************************************/

void Arc_Draw(Graph_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_StartAngle, uint32_t Graph_EndAngle, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t x_Length, uint32_t y_Length)
{
   int i;

   for (i = 0; i < 3 && imagename[i] != 0; i++)
      image->graphic_name[2 - i] = imagename[i];
   image->graphic_tpye = UI_Graph_Arc;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->start_angle = Graph_StartAngle;
   image->end_angle = Graph_EndAngle;
   image->end_x = x_Length;
   image->end_y = y_Length;
}

/************************************************绘制字符型数据*************************************************
**参数：*image Graph_Data类型变量指针，用于存放图形数据
        imagename[3]   图片名称，用于标识更改
        Graph_Operate   图片操作，见头文件
        Graph_Layer    图层0-9
        Graph_Color    图形颜色
        Graph_Width    图形线宽
        Graph_Size     字号
        Graph_Digit    字符个数
        Start_x、Start_x    开始坐标
        *Char_Data          待发送字符串开始地址
**********************************************************************************************************/
void Char_Draw(String_Data *image, char imagename[3], uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color, uint32_t Graph_Size, uint32_t Graph_Digit, uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, char *Char_Data)
{
   int i;
   for (i = 0; i < 3 && imagename[i] != 0; i++)
      image->Graph_Control.graphic_name[2 - i] = imagename[i];
   image->Graph_Control.graphic_tpye = UI_Graph_Char;
   image->Graph_Control.operate_tpye = Graph_Operate;
   image->Graph_Control.layer = Graph_Layer;
   image->Graph_Control.color = Graph_Color;
   image->Graph_Control.width = Graph_Width;
   image->Graph_Control.start_x = Start_x;
   image->Graph_Control.start_y = Start_y;
   image->Graph_Control.start_angle = Graph_Size;
   image->Graph_Control.end_angle = Graph_Digit;

   for (i = 0; i < Graph_Digit; i++)
   {
      image->show_Data[i] = *Char_Data;
      Char_Data++;
   }
}

/************************************************UI推送函数（使更改生效）*********************************
**参数： cnt   图形个数
         ...   图形变量参数


Tips：：该函数只能推送1，2，5，7个图形，其他数目协议未涉及
**********************************************************************************************************/
int UI_ReFresh(int cnt, ...)
{
   uint8_t cache[200]; // 声明一个缓冲数组
   int i;
   Graph_Data imageData;
   uint8_t *framepoint;         // 读写指针
   uint16_t frametail = 0xFFFF; // CRC16校验值

   UI_Packhead framehead;
   UI_Data_Operate datahead;

   va_list ap;
   va_start(ap, cnt);
   memset(cache, 0, sizeof(cache));

   framepoint = (uint8_t *)&framehead;
   framehead.SOF = UI_SOF;
   framehead.Data_Length = 6 + cnt * 15;
   framehead.Seq = UI_Seq;
   framehead.CRC8 = Get_CRC8_Check_Sum_UI(framepoint, 4, 0xFF);
   framehead.CMD_ID = UI_CMD_Robo_Exchange; // 填充包头数据
   switch (cnt)
   {
   case 1:
      datahead.Data_ID = UI_Data_ID_Draw1;
      break;
   case 2:
      datahead.Data_ID = UI_Data_ID_Draw2;
      break;
   case 5:
      datahead.Data_ID = UI_Data_ID_Draw5;
      break;
   case 7:
      datahead.Data_ID = UI_Data_ID_Draw7;
      break;
   default:
      return (-1);
   }
   datahead.Sender_ID = Robot_ID;
   datahead.Receiver_ID = Cilent_ID; // 填充操作数据
   framepoint = (uint8_t *)&framehead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(framehead), frametail);
   framepoint = (uint8_t *)&datahead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(datahead), frametail); // CRC16校验值计算（部分）	
   memcpy(cache, &framehead, sizeof(framehead));                                // 首先填充帧头包
   memcpy(cache + sizeof(framehead), &datahead, sizeof(datahead));              // 其次填充数据帧头包
   for (i = 0; i < cnt; i++)                                                    // 使用遍历，把图形数据填充进来
   {
      imageData = va_arg(ap, Graph_Data);
      framepoint = (unsigned char *)&imageData;
      frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(imageData), frametail);
      memcpy(cache + sizeof(framehead) + sizeof(datahead) + i * sizeof(imageData), &imageData, sizeof(imageData)); // 循环，第一次填充偏移帧头包和数据帧头包，i=0，之后偏移i*添加的量大小
   }	 
   memcpy(cache + sizeof(framehead) + sizeof(datahead) + cnt * sizeof(imageData), &frametail, sizeof(frametail));
//   HAL_UART_Transmit_DMA(UART6_data.huart, cache, sizeof(framehead) + sizeof(datahead) + cnt * sizeof(imageData) + sizeof(frametail));
  UART_send_data(UART6_data, cache, sizeof(framehead) + sizeof(datahead) + cnt * sizeof(imageData) + sizeof(frametail));
   va_end(ap);
   UI_Seq++; // 包序号+1
   return 0;
}

/************************************************UI推送字符（使更改生效）*********************************
**参数： cnt   图形个数
         ...   图形变量参数
Tips：：该函数只能推送1，2，5，7个图形，其他数目协议未涉及
**********************************************************************************************************/
int Char_ReFresh(String_Data string_Data)
{
   uint8_t cache[200]; // 声明一个缓冲数组
   String_Data imageData;
   uint8_t *framepoint;         // 读写指针
   uint16_t frametail = 0xFFFF; // CRC16校验值

   UI_Packhead framehead;
   UI_Data_Operate datahead;
   imageData = string_Data;

   framepoint = (uint8_t *)&framehead;
   framehead.SOF = UI_SOF;
   framehead.Data_Length = 6 + 45;
   framehead.Seq = UI_Seq;
   framehead.CRC8 = Get_CRC8_Check_Sum_UI(framepoint, 4, 0xFF);
   framehead.CMD_ID = UI_CMD_Robo_Exchange; // 填充包头数据

   datahead.Data_ID = UI_Data_ID_DrawChar; // 有待争议

   datahead.Sender_ID = Robot_ID;
   datahead.Receiver_ID = Cilent_ID; // 填充操作数据

   framepoint = (uint8_t *)&framehead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(framehead), frametail);
   framepoint = (uint8_t *)&datahead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(datahead), frametail);
   framepoint = (uint8_t *)&imageData;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(imageData), frametail); // CRC16校验   //CRC16校验值计算（部分）

   memcpy(cache, &framehead, sizeof(framehead));
   memcpy(cache + sizeof(framehead), &datahead, sizeof(datahead));
   memcpy(cache + sizeof(framehead) + sizeof(datahead), &imageData, sizeof(imageData));
   memcpy(cache + sizeof(framehead) + sizeof(datahead) + sizeof(imageData), &frametail, sizeof(frametail));
   // append_CRC16_check_sum(cache,sizeof(framehead)+sizeof(datahead)+sizeof(imageData)+sizeof(frametail));
//   HAL_UART_Transmit_DMA(UART6_data.huart, cache, sizeof(framehead) + sizeof(datahead) + sizeof(imageData) + sizeof(frametail));
   UART_send_data(UART6_data, cache, sizeof(framehead) + sizeof(datahead) + sizeof(imageData) + sizeof(frametail));
   UI_Seq++;
   return 0;
}
/*****************************************************CRC8校验值计算**********************************************/
const uint8_t CRC8_INIT_UI = 0xff;
const uint8_t CRC8_TAB_UI[256] =
    {
        0x00,
        0x5e,
        0xbc,
        0xe2,
        0x61,
        0x3f,
        0xdd,
        0x83,
        0xc2,
        0x9c,
        0x7e,
        0x20,
        0xa3,
        0xfd,
        0x1f,
        0x41,
        0x9d,
        0xc3,
        0x21,
        0x7f,
        0xfc,
        0xa2,
        0x40,
        0x1e,
        0x5f,
        0x01,
        0xe3,
        0xbd,
        0x3e,
        0x60,
        0x82,
        0xdc,
        0x23,
        0x7d,
        0x9f,
        0xc1,
        0x42,
        0x1c,
        0xfe,
        0xa0,
        0xe1,
        0xbf,
        0x5d,
        0x03,
        0x80,
        0xde,
        0x3c,
        0x62,
        0xbe,
        0xe0,
        0x02,
        0x5c,
        0xdf,
        0x81,
        0x63,
        0x3d,
        0x7c,
        0x22,
        0xc0,
        0x9e,
        0x1d,
        0x43,
        0xa1,
        0xff,
        0x46,
        0x18,
        0xfa,
        0xa4,
        0x27,
        0x79,
        0x9b,
        0xc5,
        0x84,
        0xda,
        0x38,
        0x66,
        0xe5,
        0xbb,
        0x59,
        0x07,
        0xdb,
        0x85,
        0x67,
        0x39,
        0xba,
        0xe4,
        0x06,
        0x58,
        0x19,
        0x47,
        0xa5,
        0xfb,
        0x78,
        0x26,
        0xc4,
        0x9a,
        0x65,
        0x3b,
        0xd9,
        0x87,
        0x04,
        0x5a,
        0xb8,
        0xe6,
        0xa7,
        0xf9,
        0x1b,
        0x45,
        0xc6,
        0x98,
        0x7a,
        0x24,
        0xf8,
        0xa6,
        0x44,
        0x1a,
        0x99,
        0xc7,
        0x25,
        0x7b,
        0x3a,
        0x64,
        0x86,
        0xd8,
        0x5b,
        0x05,
        0xe7,
        0xb9,
        0x8c,
        0xd2,
        0x30,
        0x6e,
        0xed,
        0xb3,
        0x51,
        0x0f,
        0x4e,
        0x10,
        0xf2,
        0xac,
        0x2f,
        0x71,
        0x93,
        0xcd,
        0x11,
        0x4f,
        0xad,
        0xf3,
        0x70,
        0x2e,
        0xcc,
        0x92,
        0xd3,
        0x8d,
        0x6f,
        0x31,
        0xb2,
        0xec,
        0x0e,
        0x50,
        0xaf,
        0xf1,
        0x13,
        0x4d,
        0xce,
        0x90,
        0x72,
        0x2c,
        0x6d,
        0x33,
        0xd1,
        0x8f,
        0x0c,
        0x52,
        0xb0,
        0xee,
        0x32,
        0x6c,
        0x8e,
        0xd0,
        0x53,
        0x0d,
        0xef,
        0xb1,
        0xf0,
        0xae,
        0x4c,
        0x12,
        0x91,
        0xcf,
        0x2d,
        0x73,
        0xca,
        0x94,
        0x76,
        0x28,
        0xab,
        0xf5,
        0x17,
        0x49,
        0x08,
        0x56,
        0xb4,
        0xea,
        0x69,
        0x37,
        0xd5,
        0x8b,
        0x57,
        0x09,
        0xeb,
        0xb5,
        0x36,
        0x68,
        0x8a,
        0xd4,
        0x95,
        0xcb,
        0x29,
        0x77,
        0xf4,
        0xaa,
        0x48,
        0x16,
        0xe9,
        0xb7,
        0x55,
        0x0b,
        0x88,
        0xd6,
        0x34,
        0x6a,
        0x2b,
        0x75,
        0x97,
        0xc9,
        0x4a,
        0x14,
        0xf6,
        0xa8,
        0x74,
        0x2a,
        0xc8,
        0x96,
        0x15,
        0x4b,
        0xa9,
        0xf7,
        0xb6,
        0xe8,
        0x0a,
        0x54,
        0xd7,
        0x89,
        0x6b,
        0x35,
};
uint8_t Get_CRC8_Check_Sum_UI(uint8_t *pchMessage, unsigned int dwLength, uint8_t ucCRC8)
{
   uint8_t ucIndex;
   while (dwLength--)
   {
      ucIndex = ucCRC8 ^ (*pchMessage++);
      ucCRC8 = CRC8_TAB_UI[ucIndex];
   }
   return (ucCRC8);
}
uint16_t CRC_INIT_UI = 0xffff;
const uint16_t wCRC_Table_UI[256] =
    {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78};
/*
** Descriptions: CRC16 checksum function
** Input: Data to check,Stream length, initialized checksum
** Output: CRC checksum
*/
uint16_t Get_CRC16_Check_Sum_UI(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC)
{
   Uint8_t chData;
   if (pchMessage == NULL)
   {
      return 0xFFFF;
   }
   while (dwLength--)
   {
      chData = *pchMessage++;
      (wCRC) = ((uint16_t)(wCRC) >> 8) ^ wCRC_Table_UI[((uint16_t)(wCRC) ^ (uint16_t)(chData)) &
                                                       0x00ff];
   }
   return wCRC;
}

int sentry_send(void)
{
   uint8_t cache[50]; // 声明一个缓冲数组
   uint32_t imageData;
   uint8_t *framepoint;         // 读写指针
   uint16_t frametail = 0xFFFF; // CRC16校验值
	

   UI_Packhead framehead;
   UI_Data_Operate datahead;
   imageData |= (1 << 0);
//   imageData |= (1 << 1);

   framepoint = (uint8_t *)&framehead;
   framehead.SOF = UI_SOF;
   framehead.Data_Length = 6 + 4;
   framehead.Seq = UI_Seq;
   framehead.CRC8 = Get_CRC8_Check_Sum_UI(framepoint, 4, 0xFF);
   framehead.CMD_ID = UI_CMD_Robo_Exchange; // 填充包头数据

   datahead.Data_ID = 0x0120; 

//   datahead.Sender_ID = 7;//7,107
   datahead.Sender_ID = robot_status.robot_id;//7,107
   datahead.Receiver_ID = 0x8080; // 填充操作数据0x8080

   framepoint = (uint8_t *)&framehead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(framehead), frametail);
   framepoint = (uint8_t *)&datahead;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(datahead), frametail);
   framepoint = (uint8_t *)&imageData;
   frametail = Get_CRC16_Check_Sum_UI(framepoint, sizeof(imageData), frametail); // CRC16校验   //CRC16校验值计算（部分）

   memcpy(cache, &framehead, sizeof(framehead));
   memcpy(cache + sizeof(framehead), &datahead, sizeof(datahead));
   memcpy(cache + sizeof(framehead) + sizeof(datahead), &imageData, sizeof(imageData));
   memcpy(cache + sizeof(framehead) + sizeof(datahead) + sizeof(imageData), &frametail, sizeof(frametail));
   // append_CRC16_check_sum(cache,sizeof(framehead)+sizeof(datahead)+sizeof(imageData)+sizeof(frametail));
//   HAL_UART_Transmit_DMA(UART6_data.huart, cache, sizeof(framehead) + sizeof(datahead) + sizeof(imageData) + sizeof(frametail));
   UART_send_data(UART6_data, cache, sizeof(framehead) + sizeof(datahead) + sizeof(imageData) + sizeof(frametail));
   UI_Seq++;
   return 0;
}