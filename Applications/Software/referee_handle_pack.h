//基于串口协议附录V1.5
#ifndef REFEREE_HANDLE_PACK_H
#define REFEREE_HANDLE_PACK_H


#include "main.h"

#include "protocol.h"
#define SEND_MAX_SIZE    128    //上传数据最大的长度
#define frameheader_len  5       //帧头长度
#define cmd_len          2       //命令码长度
#define crc_len          2       //CRC16校验

typedef struct//自定义结构体，需在函数中赋值，然后在 FREERTOS 里调用  后续可根据需要自行添加
{
	float Initial_SPEED;//弹丸初速度（单位：m/s）	
	uint8_t Launching_Frequency;//弹丸射速频率（单位：Hz）
	uint16_t Launching_SPEED_Limit;//发射机构射击初速度上限
	 uint16_t projectile_allowance_17mm;
	uint16_t Barrel_Heat;//发射机构的枪口热量	
	uint16_t CooLing_Value;//枪口热量每秒冷却值
	uint16_t Heat_Limit;//枪口热量上限

	uint16_t Chassis_Voltage;//chassis 口输出电压（单位：mV）
	uint16_t Chassis_Current;//chassis 口输出电流（单位：mA）
	uint16_t Buffer_Energy;//缓冲能量（单位：J）	
	float Chassis_Power;//底盘功率（单位：W）
	uint16_t Chassis_Power_Limit;//机器人底盘功率上限	
	float rest_time;
	
	//导航相关数据
	
	uint16_t remain_hp;                  //剩余血量
  uint8_t	game_type;                   //比赛类型
  uint8_t	game_progress;               //比赛阶段
  float	stage_remain_time;             //当前阶段剩余时间
  uint16_t	bullet_remaining_num_17mm; //剩余子弹数
  uint16_t	red_outpost_hp;            //红方前哨站血量
  uint16_t	red_base_hp;               //红方基地血量
  uint16_t	blue_outpost_hp;           //蓝方前哨站血量
  uint16_t	blue_base_hp;              //蓝方基地血量
  uint32_t rfid_status;                //蓝方RFID状态
	uint8_t    robot_id;                  //机器人ID
	
	uint16_t max_hp;

} REFEREE_DATA_t;
extern REFEREE_DATA_t REFEREE_DATA;

/********************
****game_type比赛类型1：RoboMaster 机甲大师超级对抗赛2：RoboMaster 机甲大师高校单项赛
										3：ICRA RoboMaster 高校人工智能挑战赛 4：RoboMaster 机甲大师高校联盟赛 3V3 对抗5：RoboMaster 机甲大师高校联盟赛步兵对抗
game_progress 当前比赛阶段0：未开始比赛1：准备阶段2：自检阶段3：5 秒倒计时4：比赛中 5：比赛结算中
stage_remain_time  当前阶段剩余时间 s
***/

__packed typedef struct  //0x0001  比赛状态数据11
{
 uint8_t game_type : 4;/**比赛类型**/
 uint8_t game_progress : 4;/**当前比赛阶段**/
 uint16_t stage_remain_time;/*当前阶段剩余时间*/
 uint64_t SyncTimeStamp;
}ext_game_status_t;


/***************
*****0：平局  1：红方胜利   2：蓝方胜利
***/
__packed typedef struct  //0x0002  比赛结果数据1
{
	uint8_t winner;/******/
}ext_game_result_t;

__packed typedef struct  //0x0003  机器人血量数据
{
 uint16_t red_1_robot_HP;
 uint16_t red_2_robot_HP;
 uint16_t red_3_robot_HP;
 uint16_t red_4_robot_HP;
 uint16_t red_5_robot_HP;
 uint16_t red_7_robot_HP;/*哨兵*/
 uint16_t red_outpost_HP;/**前哨站**/
 uint16_t red_base_HP;
 uint16_t blue_1_robot_HP;
 uint16_t blue_2_robot_HP;
 uint16_t blue_3_robot_HP;
 uint16_t blue_4_robot_HP;
 uint16_t blue_5_robot_HP;
 uint16_t blue_7_robot_HP;
 uint16_t blue_outpost_HP;
 uint16_t blue_base_HP;
}ext_game_robot_HP_t;/**血量**/



/**************
******0：未占领/未激活
1：已占领/已激活

bit 0-2：
? bit 0： 己方补给站前补血点的占领状态， 1 为已占领
? bit 1： 己方补给站内部补血点的占领状态， 1 为已占领
? bit 2： 己方补给区的占领状态， 1 为已占领（仅 RMUL 适用）

bit 3-5：己方能量机关状态
? bit 3： 己方能量机关激活点的占领状态， 1 为已占领
? bit 4：己方小能量机关的激活状态， 1 为已激活
? bit 5：己方大能量机关的激活状态， 1 为已激活

bit 6-11：己方高地占领状态
? bit 6-7： 己方环形高地的占领状态， 1 为被己方占领， 2 为被对方占领
? bit 8-9： 己方梯形高地的占领状态， 1 为被己方占领， 2 为被对方占领
? bit 10-11： 己方梯形高地的占领状态， 1 为被己方占领， 2 为被对方占领
bit 12-18： 己方基地虚拟护盾的剩余值百分比（四舍五入，保留整数）
bit 19-27：飞镖最后一次击中己方前哨站或基地的时间（0-420，开局默认为0）
bit 28-29： 飞镖最后一次击中己方前哨站或基地的具体目标，开局默认为 0，1 为击中前哨站， 2 为击中基地固定目标， 3 为击中基地随机目标
bit 30-31： 中心增益点的占领情况， 0 为未被占领， 1 为被己方占领， 2 为被对方占领， 3 为被双方占领。（仅 RMUL 适用）
*********************/
__packed typedef struct  //0x0101  场地事件数据
{
	uint32_t event_data;
}ext_event_data_t; 



/******************
0  保留 
***1 
补弹机器人 ID：
? 0：当前无机器人补弹
? 1：红方英雄机器人补弹
? 3/4/5：红方步兵机器人补弹
? 101：蓝方英雄机器人补弹
? 103/104/105：蓝方步兵机器人补弹
***2 
出弹口开闭状态：
? 0：关闭
? 1：弹丸准备中
? 2：弹丸释放
***3 
补弹数量：
? 50：50 颗弹丸
? 100：100 颗弹丸
? 150：150 颗弹丸
? 200：200 颗弹丸
***************************/
__packed typedef struct  //0x0102  补给站动作标识数据
{
 uint8_t reserved;
 uint8_t supply_robot_id;
 uint8_t supply_projectile_step;
 uint8_t supply_projectile_num;
}ext_supply_projectile_action_t;

/****
己方最后一次受到判罚的等级：
? 1： 双方黄牌
? 2： 黄牌
? 3：红牌
? 4：判负
**************/
//__packed typedef struct  //0x0104  裁判警告数据 
//{

// uint8_t level;
// uint8_t offending_robot_id;
// uint8_t count;
// 
//}ext_referee_warning_t;



/****************************
** 1 己方飞镖发射剩余时间，单位：秒
1 
bit 0-1：
最近一次己方飞镖击中的目标，开局默认为 0，1 为击中前哨站，2 为击中
基地固定目标，3 为击中基地随机目标
bit 2-4：
对方最近被击中的目标累计被击中计数，开局默认为 0，至多为 4
bit 5-6：
飞镖此时选定的击打目标，开局默认或未选定/选定前哨站时为 0，选中基
地固定目标 1，选中基地随机目标为 2
bit 7-15：保留
***/
//__packed typedef struct  //0x0105  飞镖发射时间数据  
//{
//	uint8_t dart_remaining_time;
//	uint16_t dart_info;
//}ext_dart_remaining_time_t;



/***************
0 1 本机器人 ID
1 1 机器人等级
2 2 机器人当前血量
4 2 机器人血量上限
6 2 机器人枪口热量每秒冷却值
8 2 机器人枪口热量上限
10 2 机器人底盘功率上限
12 1
电源管理模块的输出情况：
? bit 0： gimbal 口输出： 0 为无输出， 1 为 24V 输出
? bit 1： chassis 口输出： 0 为无输出， 1 为 24V 输出
? bit 2： shooter 口输出： 0 为无输出， 1 为 24V 输出
? bit 3-7： 保留
***************/
__packed typedef struct  //0x201  机器人性能体系数据  
{

 
	uint8_t robot_id;       //本机器人 ID
	uint8_t robot_level;    // 机器人等级
	uint16_t current_HP;    //机器人当前血量
	uint16_t maximum_HP;    //机器人血量上限
	uint16_t shooter_barrel_cooling_value;    //机器人枪口热量每秒冷却值
	uint16_t shooter_barrel_heat_limit;       //机器人枪口热量上限
	uint16_t chassis_power_limit;            //机器人底盘功率上限
	uint8_t power_management_gimbal_output : 1;//电源管理模块输出情况：
	uint8_t power_management_chassis_output : 1;
	uint8_t power_management_shooter_output : 1;

}ext_robot_status_t;


__packed typedef struct  //0x202  实时功率热量数据
{
  uint16_t reserved; 
  uint16_t reservedL; 
  float reservedK; 
  uint16_t buffer_energy; 
  uint16_t shooter_17mm_1_barrel_heat; //第1个17mm发射机构的射击热量
  uint16_t shooter_17mm_2_barrel_heat; //第2个17mm发射机构的射击热量 
  uint16_t shooter_42mm_barrel_heat; 

}ext_power_heat_data_t;



/*******************
0  本机器人位置 x 坐标，单位：m
4  本机器人位置 y 坐标，单位：m
8  本机器人测速模块朝向，单位：度。正北为 0 度
**************/
__packed typedef struct  //0x203  机器人位置数据
{
 float x;/*****/
 float y;/****/
 float angle;/*****/
}ext_robot_pos_t;



/*************
0  机器人回血增益（百分比，值为 10 意为每秒回复 10%最大血量）
1  机器人枪口冷却倍率（直接值，值为 5 意味着 5 倍冷却）
2  机器人防御增益（百分比，值为 50 意为 50%防御增益）
3  机器人攻击增益（百分比，值为 50 意为 50%攻击增益）
*****************/
__packed typedef struct  //0x0204  机器人增益数据
{
 uint8_t recovery_buff;//机器人回血增益（百分比，值为 10 意为每秒回复 10%最大血量）/******/
 uint8_t cooling_buff;//机器人枪口冷却倍率（直接值，值为 5 意味着 5 倍冷却）/******/
 uint8_t defence_buff;//机器人防御增益（百分比，值为 50 意为 50%防御增益）/******/
 uint16_t attack_buff;//机器人攻击增益（百分比，值为 50 意为 50%攻击增益）/*****/
}ext_buff_t;


__packed typedef struct  //0x0206  伤害状态数据
{
 uint8_t armor_id : 4;//当扣血原因为装甲模块或测速模块时，该 4bit 组成的数值为装甲模块或测速模块的 ID 编号；其他原因扣血时，该数值为 
 uint8_t HP_deduction_reason : 4;//血量变化类型
}ext_hurt_data_t;



__packed typedef struct  //0x0207  实时射击数据
{
 uint8_t bullet_type;//弹丸类型： 1：17mm 弹丸 2：42mm 弹丸
 uint8_t shooter_number;//1：第 1 个 17mm 发射机构 2：第 2 个 17mm 发射机构 3：42mm 发射机构
 uint8_t launching_frequency;//弹丸射速频率（单位：Hz）
 float initial_speed;//弹丸初速度（单位：m/s）
}ext_shoot_data_t;


/********
0  17mm 弹丸允许发弹量
2  42mm 弹丸允许发弹量
4  剩余金币数量
******/
__packed typedef struct  //0x0208  允许发弹量
{
 uint16_t projectile_allowance_17mm;/*****/
 uint16_t projectile_allowance_42mm;/****/
 uint16_t remaining_gold_coin;/****/
}ext_projectile_allowance_t;




/****************
bit 位值为 1/0 的含义：是否已检测到该增益点 RFID
? bit0：己方基地增益点
? bit1：己方环形高地增益点
? bit2：对方环形高地增益点
? bit3：己方 R3/B3 梯形高地增益点**
? bit4：对方 R3/B3 梯形高地增益点**
? bit5：己方 R4/B4 梯形高地增益点**
? bit6：对方 R4/B4 梯形高地增益点**
? bit7：己方能量机关激活点
? bit8：己方飞坡增益点（靠近己方一侧飞坡前）
? bit9：己方飞坡增益点（靠近己方一侧飞坡后）
? bit10：对方飞坡增益点（靠近对方一侧飞坡前）
? bit11：对方飞坡增益点（靠近对方一侧飞坡后）
? bit12：己方前哨站增益点
? bit13：己方补血点（检测到任一均视为激活）
? bit14：己方哨兵巡逻区**
? bit15：对方哨兵巡逻区**
? bit16：己方大资源岛增益点
? bit17：对方大资源岛增益点
? bit18：己方兑换区
? bit19-31：保留
*****************/
__packed typedef struct  //0x0209  机器人 RFID 状态
{
	uint32_t rfid_status;
}ext_rfid_status_t;




__packed typedef struct  //0x020A  飞镖选手端指令数据
{
 uint8_t dart_launch_opening_status;//当前飞镖发射站的状态： 1：关闭；2：正在开启或者关闭中；0：已经开
 uint8_t reserved;
 uint16_t target_change_time;//切换打击目标时的比赛剩余时间，单位：s，无未切换动作默认为 0
 uint16_t latest_launch_cmd_time;//最后一次操作手确定发射指令时的比赛剩余时间，单位：s，初始值为 0
}ext_dart_client_cmd_t;



/***************
0  己方英雄机器人位置 x 轴坐标，单位：m
4  己方英雄机器人位置 y 轴坐标，单位：m
8  己方工程机器人位置 x 轴坐标，单位：m
12  己方工程机器人位置 y 轴坐标，单位：m
16  己方 3 号步兵机器人位置 x 轴坐标，单位：m
20  己方 3 号步兵机器人位置 y 轴坐标，单位：m
24  己方 4 号步兵机器人位置 x 轴坐标，单位：m
28  己方 4 号步兵机器人位置 y 轴坐标，单位：m
32  己方 5 号步兵机器人位置 x 轴坐标，单位：m
36  己方 5 号步兵机器人位置 y 轴坐标，单位：m
*************/
__packed typedef struct  //0x020B  地面机器人位置数据
{
 float hero_x;
 float hero_y;
 float engineer_x;
 float engineer_y;
 float standard_3_x;
 float standard_3_y;
 float standard_4_x;
 float standard_4_y;
 float standard_5_x;
 float standard_5_y;
}ext_ground_robot_position_t;/*****/




/**bit0-10：哨兵已经成功兑换的发弹量值，开局为 0，在哨兵成功兑换一定数
量的发弹量后，该值将变为哨兵已经成功兑换的发弹量值。
bit 11-14：哨兵成功远程兑换发弹量的次数，开局为 0，在哨兵成功远程兑
换发弹量后，该值将变为哨兵已经成功远程兑换发弹量的次数。
bit 15-18：哨兵成功远程兑换血量的次数，开局为 0，在哨兵成功远程兑换
血量后，该值将变为哨兵已经成功远程兑换血量的次数。
bit 19-31：保留**/
__packed typedef struct//0x020D 哨兵决策指令
{
	uint32_t sentry_info;
	uint16_t sentry_info_2; 
}ext_sentry_info_t;



/*bit 0-1：雷达是否拥有触发双倍易伤的资格，开局为 0，数值为雷达拥有触发
双倍易伤的资格次数，至多为 2。
bit2：对手是否正在被触发双倍易伤
? 0：对手未被触发双倍易伤
? 1：对手正在被触发双倍易伤*/
/***/
__packed typedef struct// 0x020E 雷达是否具有双倍易伤资格
{
 uint8_t radar_info;
} ext_radar_info_t;



/**bit 0：哨兵机器人是否确认复活
? 0 表示哨兵机器人不想复活，即使此时哨兵复活读条已经
完成
? 1 表示哨兵机器人想复活，若复活读条完成将立即复活
bit 1：哨兵机器人是否想要兑换立即复活
? 0 表示哨兵机器人不想兑换立即复活；
? 1 表示哨兵机器人想兑换立即复活，若此时哨兵机器人符
合兑换立即复活的规则要求，则会立即消耗金币兑换哨兵
的立即复活
bit2-12：哨兵想要兑换的发弹量值，开局为 0，修改此值后，
哨兵在补血点即可兑换允许发弹量。
此值的变化需要单调递增，否则视为不合法。
示例：此值开局仅能为 0，此后哨兵可将其从 0 修改至 X，则消
耗 X 金币成功兑换 X 允许发弹量。此后哨兵可将其从 X 修改至
X+Y，以此类推。
bit 13-16：哨兵想要远程兑换发弹量的请求次数，开局为 0，
修改此值即可请求远程兑换发弹量。
此值的变化需要单调递增且每次仅能增加 1，否则视为不合
法。
示例：此值开局仅能为 0，此后哨兵可将其从 0 修改至 1，则消
耗金币远程兑换允许发弹量。此后哨兵可将其从 1 修改至 2，
以此类推。
bit 17-20：哨兵想要远程兑换血量的请求次数，开局为 0，修
改此值即可请求远程兑换血量。
此值的变化需要单调递增且每次仅能增加 1，否则视为不合
法。
示例：此值开局仅能为 0，此后哨兵可将其从 0 修改至 1，则消
耗金币远程兑换血量。此后哨兵可将其从 1 修改至 2，以此类
推。
**/
__packed typedef struct//0x0120  哨兵自主决策指令
{
	uint32_t sentry_cmd;
}ext_sentry_cmd_t;

extern ext_sentry_cmd_t sentry_cmd_t;

/**/
__packed typedef struct // 0x0121 雷达自主决策指令
{
	uint8_t radar_cmd;
} ext_radar_cmd_t;


__packed typedef struct  //0x0301  机器人交互数据
{
 uint16_t data_cmd_id;
 uint16_t sender_id;
 uint16_t receiver_id;
 uint8_t user_data[112];
	
}ext_robot_interaction_data_t;


__packed typedef struct//0x0303  命令码
{
	float target_position_x;//目标位置 x 轴坐标，单位 m 当发送目标机器人 ID 时，该值为 0
	float target_position_y;//目标位置 y 轴坐标，单位 m 当发送目标机器人 ID 时，该值为 0
	uint8_t cmd_keyboard;//云台手按下的键盘按键通用键值 无按键按下则为 0
	uint8_t target_robot_id;//对方机器人 ID 当发送坐标数据时，该值为 0
	uint16_t cmd_source;//信息来源 ID
}ext_map_command_t;



__packed typedef struct  //0x0305  选手端小地图接收雷达数据
{
	uint16_t target_robot_id;
	float target_position_x;
	float target_position_y;
}ext_map_robot_data_t;



/***/
__packed typedef struct  //0x0307  选手端小地图接收哨兵数据
{
	uint8_t intention;
	uint16_t start_position_x;
	uint16_t start_position_y;
	int8_t delta_x[49];
	int8_t delta_y[49];
	uint16_t sender_id;
}ext_map_sentry_data_t;



/***/
__packed typedef struct //0x0308  己方机器人可通过常规链路向己方任意选手端发送自定义的消息，该消息会在己方选手端特定位置显示。
{ 
uint16_t sender_id;
uint16_t receiver_id;
uint16_t user_data[30];
}ext_custom_info_t;



//__packed typedef struct  //0x0302  自定义控制器与机器人交互数据
//{
//uint8_t data[25];
//}ext_custom_robot_data_t;



//__packed typedef struct  //0x0304  键鼠遥控数据
//{
//int16_t mouse_x;
//int16_t mouse_y;
//int16_t mouse_z;
//uint8_t left_button_down;
//uint8_t right_button_down;
//uint16_t keyboard_value;
//uint16_t reserved;
//}ext_remote_control_t;



//__packed typedef struct  //0x0306  自定义控制器与选手端交互数据
//{
//uint16_t key_value;
// uint16_t x_position:12;
// uint16_t mouse_left:4;
// uint16_t y_position:12;
// uint16_t mouse_right:4;
// uint16_t reserved;
//}ext_custom_client_data_t;


///************************** 机器人间交互数据 ********************/
///* 交互数据接收信息： 0x0301 */
typedef __packed struct {
    uint16_t data_cmd_id;  //数据段的内容 ID
    uint16_t sender_ID;
    uint16_t receiver_ID;
} ext_student_interactive_header_data_t;



/* 发送数据结构体 */
typedef struct {
    uint16_t                              cmd_id;
    ext_student_interactive_header_data_t data_header;
    uint8_t                               data[120];
    uint16_t                              frame_tail;
}ext_referee_send_data_t;









typedef enum
{

    RED_HERO        = 1,
    RED_ENGINEER    = 2,
    RED_STANDARD_1  = 3,
    RED_STANDARD_2  = 4,
    RED_STANDARD_3  = 5,
    RED_AERIAL      = 6,
    RED_SENTRY      = 7,
    RED_DART        = 8,
    RED_RADAR       = 9,
    RED_OUTPOST     = 10,
    RED_LOCATION    = 11,

    BLUE_HERO       = 101,
    BLUE_ENGINEER   = 102,
    BLUE_STANDARD_1 = 103,
    BLUE_STANDARD_2 = 104,
    BLUE_STANDARD_3 = 105,
    BLUE_AERIAL     = 106,
    BLUE_SENTRY     = 107,
    BLUE_DART       = 108,
    BLUE_RADAR      = 109,
    BLUE_OUTPOST    = 110,
    BLUE_LOCATION   = 111,

}ext_robot_id_t;



typedef enum
{
    PROGRESS_UNSTART        = 0,
    PROGRESS_PREPARE        = 1,
    PROGRESS_SELFCHECK      = 2,
    PROGRESS_5sCOUNTDOWN    = 3,
    PROGRESS_BATTLE         = 4,
    PROGRESS_CALCULATING    = 5,
}ext_game_progress_t;



/* 

暂时注释

客户端 客户端自定义数据：cmd_id:0x0301。内容 ID:0x0100   0x0101  0x0102  0x0103  0x0104  0x0110
发送频率：上限 10Hz
*/

//typedef __packed struct  //0x0100  选手端删除图层
//{
//uint8_t delete_type;
//uint8_t layer;
//}ext_interaction_layer_delete_t;


//typedef __packed struct  //0x0101  选手端绘制一个图形
//{ 
//uint8_t figure_name[3]; 
//uint32_t operate_tpye:3; 
//uint32_t figure_tpye:3; 
//uint32_t layer:4; 
//uint32_t color:4; 
//uint32_t details_a:9;
//uint32_t details_b:9;
//uint32_t width:10; 
//uint32_t start_x:11; 
//uint32_t start_y:11; 
//uint32_t details_c:10; 
//uint32_t details_d:11;
//uint32_t details_e:11; 
//}ext_interaction_figure_1_t;


//typedef __packed struct  //0x0102  选手端绘制两个图形
//{
// interaction_figure_1_t interaction_figure[2];
//}ext_interaction_figure_2_t;


//typedef __packed struct  //0x0103  选手端绘制五个图形
//{
//interaction_figure_t interaction_figure[5];
//}ext_interaction_figure_5_t;


//typedef __packed struct  //0x0104  选手端绘制七个图形
//{
//interaction_figure_t interaction_figure[7];
//}ext_interaction_figure_7_t;


//typedef __packed struct  //0x0110  选手端绘制字符图形
//{
//graphic_data_struct_t grapic_data_struct;
//uint8_t data[30];
//}ext_client_custom_character_t;




extern ext_game_status_t game_status;//0x0001  比赛状态数据
extern ext_game_result_t game_result;//0x0002  比赛结果数据
extern ext_game_robot_HP_t game_robot_HP;//0x0003  机器人血量数据

extern ext_event_data_t event_data;//0x0101  场地事件数据

extern ext_supply_projectile_action_t supply_projectile_action;//0x0102  补给站动作标识数据

extern ext_robot_status_t robot_status;//0x201  机器人性能体系数据

extern ext_power_heat_data_t power_heat_data;//0x202  实时功率热量数据

extern ext_robot_pos_t robot_pos;//0x203  机器人位置数据

extern ext_buff_t robot_buff;//0x0204  机器人增益数据

extern ext_hurt_data_t hurt_data;//0x0206  伤害状态数据

extern ext_shoot_data_t shoot_data;//0x0207  实时射击数据

extern ext_projectile_allowance_t projectile_allowance;//0x0208  允许发弹量

extern ext_rfid_status_t rfid_status;//0x0209  机器人 RFID 状态

extern ext_dart_client_cmd_t dart_client_cmd;//0x020A  飞镖选手端指令数据

extern ext_ground_robot_position_t ground_robot_position;//0x020B  地面机器人位置数据

/*开始缺东西*/

extern ext_sentry_info_t sentry_data;//0x020D哨兵相关数据

extern ext_radar_info_t radar_data;//0x020E雷达相关数据

extern ext_robot_interaction_data_t robot_interaction_data;//0x0301  机器人交互数据

extern ext_map_command_t map_command;//0x0303  选手端小地图交互数据

extern ext_map_robot_data_t map_robot_data;//0x0305  选手端小地图接收雷达数据

extern ext_map_sentry_data_t map_sentry_data;//0x0307  选手端小地图接收哨兵数据

/*0x0308车间通讯相关*/
extern ext_custom_info_t exchange_data;//0x0308 己方机器人→己方选手端

void init_referee_struct_data(void);
void referee_data_solve(uint8_t *frame);


uint8_t get_robot_id(void);


void get_chassis_power_and_buffer(fp32 *power, uint16_t *buffer);
void get_chassis_VOL_CUR_POW_BUF(uint16_t *chassis_voltage,uint16_t *chassis_current,float *chassis_power, uint16_t *buffer_energy);
void get_chassis_power_limit(uint16_t *chassis_power_limit);


const ext_robot_status_t *get_robot_state_point(void);
const ext_power_heat_data_t *get_power_heat_data_point(void);
const ext_shoot_data_t *get_shoot_data_point(void);


void Required_Data();
void Cap_Required_Data();
void referee_data_send(); 
void SendSentryCMD(uint8_t sof,uint16_t cmd_id,uint16_t len);

#endif