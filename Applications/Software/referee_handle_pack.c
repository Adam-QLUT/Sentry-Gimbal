//基于串口协议附录V1.6(已更新)


#include "referee_handle_pack.h"
#include "string.h"
#include "stdio.h"
#include "CRC8_CRC16.h"
#include "protocol.h"
// #include "cap_ctl.h"
#include "global_status.h"
#include "usart.h" 

REFEREE_DATA_t REFEREE_DATA;
ext_sentry_cmd_t sentry_cmd_t;
frame_header_struct_t referee_receive_header;
frame_header_struct_t referee_send_header;

ext_game_status_t game_status;//0x0001  比赛状态数据
ext_game_result_t game_result;//0x0002  比赛结果数据
ext_game_robot_HP_t game_robot_HP;//0x0003  机器人血量数据

ext_event_data_t event_data;//0x0101  场地事件数据

//ext_referee_warning_t referee_warning;//0x0104  裁判警告数据

//ext_dart_info_t dart_info_t;//0x0105  飞镖发射相关数据 

ext_robot_status_t robot_status;//0x201  机器人性能体系数据

ext_power_heat_data_t power_heat_data;//0x202  实时功率热量数据

ext_robot_pos_t robot_pos;//0x203  机器人位置数据

ext_buff_t robot_buff;//0x0204  机器人增益数据

ext_hurt_data_t hurt_data;//0x0206  伤害状态数据

ext_shoot_data_t shoot_data;//0x0207  实时射击数据

ext_projectile_allowance_t projectile_allowance;//0x0208  允许发弹量

ext_rfid_status_t rfid_status;//0x0209  机器人 RFID 状态

ext_dart_client_cmd_t dart_client_cmd;//0x020A  飞镖选手端指令数据

ext_ground_robot_position_t ground_robot_position;//0x020B  地面机器人位置数据

//ext_radar_mark_data_t radar_mark_data;//0x020C  雷达标记进度数据

ext_sentry_info_t sentry_info_t;//0x020D  哨兵自主决策信息同步

ext_radar_info_t radar_info_t;//0x020E   雷达自主决策信息同步

ext_robot_interaction_data_t robot_interaction_data;//0x0301  机器人交互数据

//ext_ext_client_custom_character_t ext_client_custom_character_t;//0x0110   

//ext_sentry_cmd_t sentry_cmd_t;// 0x0120  哨兵自主决策指令

//ext_radar_cmd_t radar_cmd_t;// 0x0121 雷达自主决策指令

ext_map_command_t map_command;//0x0303  选手端小地图交互数据

ext_map_robot_data_t map_robot_data;//0x0305  选手端小地图接收雷达数据

ext_map_sentry_data_t map_sentry_data;//0x0307  选手端小地图接收哨兵数据

//ext_custom_robot_data_t custom_robot_data;//0x0302  自定义控制器与机器人交互数据

//ext_remote_control_t remote_control;//0x0304  键鼠遥控数据//

//ext_custom_client_data_t custom_client_data;//0x0306  自定义控制器与选手端交互数据

ext_custom_info_t custom_info_t;// 0x0308  己方机器人可通过常规链路向己方任意选手端发送自定义的消息，该消息会在己方选手端特定位置显示。

//ext_robot_custom_data_t robot_custom_data_t;//  0x0309  自定义控制器接收机器人数据


void init_referee_struct_data(void)
{
    memset(&referee_receive_header, 0, sizeof(frame_header_struct_t));
    memset(&referee_send_header, 0, sizeof(frame_header_struct_t));

    memset(&game_status, 0, sizeof(ext_game_status_t));
    memset(&game_result, 0, sizeof(ext_game_result_t));
    memset(&game_robot_HP, 0, sizeof(ext_game_robot_HP_t));

    memset(&event_data, 0, sizeof(ext_event_data_t));

//    memset(&referee_warning, 0, sizeof(ext_referee_warning_t));

//    memset(&dart_info_t, 0, sizeof(ext_dart_info_t));

    memset(&robot_status, 0, sizeof(ext_robot_status_t));

    memset(&power_heat_data, 0, sizeof(ext_power_heat_data_t));

    memset(&robot_pos, 0, sizeof(ext_robot_pos_t));

    memset(&hurt_data, 0, sizeof(ext_hurt_data_t));

    memset(&shoot_data, 0, sizeof(ext_shoot_data_t));

    memset(&projectile_allowance, 0, sizeof(ext_projectile_allowance_t));

    memset(&rfid_status, 0, sizeof(ext_rfid_status_t));    

    memset(&dart_client_cmd, 0, sizeof(ext_dart_client_cmd_t));   

    memset(&ground_robot_position, 0, sizeof(ext_ground_robot_position_t));           

//    memset(&radar_mark_data, 0, sizeof(ext_radar_mark_data_t));  

    memset(&sentry_info_t, 0, sizeof(ext_sentry_info_t));
		
		memset(&radar_info_t, 0, sizeof(ext_radar_info_t));

    memset(&robot_interaction_data, 0, sizeof(ext_robot_interaction_data_t));
 
    memset(&map_command, 0, sizeof(ext_map_command_t));

    memset(&map_robot_data, 0, sizeof(ext_map_robot_data_t));

//    memset(&custom_robot_data, 0, sizeof(ext_custom_robot_data_t));

//    memset(&remote_control, 0, sizeof(ext_remote_control_t));

//    memset(&custom_client_data, 0, sizeof(ext_custom_client_data_t));
		
		memset(&custom_info_t, 0, sizeof(ext_custom_info_t));
		
	//	memset(&robot_custom_data_t, 0, sizeof(ext_robot_custom_data_t));

}

void referee_data_solve(uint8_t *frame)
{
    uint16_t cmd_id = 0;
    uint8_t index = 0;

    memcpy(&referee_receive_header, frame, sizeof(frame_header_struct_t));

    index += sizeof(frame_header_struct_t);

    memcpy(&cmd_id, frame + index, sizeof(uint16_t));
    index += sizeof(uint16_t);

    switch (cmd_id)
    {
        case GAME_STATE_CMD_ID:
        {
            memcpy(&game_status, frame + index, sizeof(ext_game_status_t));
        }
        break;
        case GAME_RESULT_CMD_ID:
        {
            memcpy(&game_result, frame + index, sizeof(ext_game_result_t));
        }
        break;
        case GAME_ROBOT_HP_CMD_ID:
        {
            memcpy(&game_robot_HP, frame + index, sizeof(ext_game_robot_HP_t));
        }
        break;
        case FIELD_EVENTS_CMD_ID:
        {
            memcpy(&event_data, frame + index, sizeof(ext_event_data_t));
        }
        break;
//        case REFEREE_WARNING_CMD_ID:
//        {
//            memcpy(&referee_warning, frame + index, sizeof(ext_referee_warning_t));
//        }
        break;
//        case DART_LAUNCH_TIME_CMD_ID:
//        {
////            memcpy(&dart_info_t, frame + index, sizeof(ext_dart_info_t));
//        }
//        break;
        case ROBOT_STATE_CMD_ID:
        {
            memcpy(&robot_status, frame + index, sizeof(ext_robot_status_t));
        }
        break;
        case POWER_HEAT_DATA_CMD_ID:
        {
            memcpy(&power_heat_data, frame + index, sizeof(ext_power_heat_data_t));
        }
        break;
        case ROBOT_POS_CMD_ID:
        {
            memcpy(&robot_pos, frame + index, sizeof(ext_robot_pos_t));
        }
        break;
        case BUFF_MUSK_CMD_ID:
        {
            memcpy(&robot_buff, frame + index, sizeof(ext_buff_t));
        }
        break;
        case ROBOT_HURT_CMD_ID:
        {
            memcpy(&hurt_data, frame + index, sizeof(ext_hurt_data_t));
        }
        break;
        case SHOOT_DATA_CMD_ID:
        {
            memcpy(&shoot_data, frame + index, sizeof(ext_shoot_data_t));
        }
        break;
        case BULLET_REMAINING_CMD_ID:
        {
            memcpy(&projectile_allowance, frame + index, sizeof(ext_projectile_allowance_t));
        }
        break;
        case ROBOT_RFID_STATE_CMD_ID:
        {
            memcpy(&rfid_status, frame + index, sizeof(ext_rfid_status_t));
        }
        break;
        case DART_PLAYER_COMMAND_CMD_ID:
        {
            memcpy(&dart_client_cmd, frame + index, sizeof(ext_dart_client_cmd_t));
        }
        break;
        case GROUND_ROBOT_POSITION_CMD_ID:
        {
            memcpy(&ground_robot_position, frame + index, sizeof(ext_ground_robot_position_t));
        }
        break;
//        case RADAR_MARKS_PROGRESS_CMD_ID:
//        {
//            memcpy(&radar_mark_data, frame + index, sizeof(ext_radar_mark_data_t));
//        }
//        break;
//				case SENTRY_INFO_CMD_ID:
        {
            memcpy(&sentry_info_t, frame + index, sizeof(ext_sentry_info_t));
        }
        break;
//				case RADAR_INFO_CMD_ID:
        {
            memcpy(&radar_info_t, frame + index, sizeof(ext_radar_info_t));
        }
        break;
        case ROBOT_INTERACTION_CMD_ID:
        {
            memcpy(&robot_interaction_data, frame + index, sizeof(ext_robot_interaction_data_t));
        }
        break;
//        case CUSTOM_CONTROLLER_AND_BOT_INTERACTION_CMD_ID:
//        {
//            memcpy(&custom_robot_data, frame + index, sizeof(ext_custom_robot_data_t));
//        }
//        break;
        case PLAYER_SIDE_MINIMAP_INTERACTION_CMD_ID:
        {
            memcpy(&map_command, frame + index, sizeof(ext_map_command_t));
        }
        break;
//        case KEYBOARD_AND_MOUSE_REMOTE_CONTROL_CMD_ID:
//        {
//            memcpy(&remote_control, frame + index, sizeof(ext_remote_control_t));
//        }
//        break;
        case PLAYER_MINIMAP_RECEIVES_RADAR_CMD_ID:
        {
            memcpy(&map_robot_data, frame + index, sizeof(ext_map_robot_data_t));
        }
        break;
//        case INTERACTION_DATA_BETWEEN_CONTROLLER_AND_PLAYER_CMD_ID:
//        {
//            memcpy(&custom_client_data, frame + index, sizeof(ext_custom_client_data_t));
//        }
//        break;
//				case CUSTOM_INFO_CMD_ID:
 //       {
  //          memcpy(&custom_info_t, frame + index, sizeof(ext_custom_info_t));
   //     }
     //   break;
		//		case ROBOT_CUSTOM_DATA_CMD_ID:
        //{
//            memcpy(&robot_custom_data_t, frame + index, sizeof(ext_robot_custom_data_t));
        //}
        break;
        case PLAYER_MINIMAP_RECEIVES_SENTRY_CMD_ID:
        {
            memcpy(&map_sentry_data, frame + index, sizeof(ext_map_sentry_data_t));
        }
        break;
        default:
        {
            break;
        }
    }
}


/*-----本赛季删除了许多可从电管读取的数据，各兵种如需读取其他数据自行添加-----*/


//从裁判系统获取车辆ID
uint8_t get_robot_id(void)
{
    return robot_status.robot_id;
}



//从裁判系统获取车辆底盘缓冲能量
//void get_chassis_VOL_CUR_POW_BUF(uint16_t *buffer_energy)
//{

//    *buffer_energy   = power_heat_data.buffer_energy; 
//}


void get_chassis_power_limit(uint16_t *chassis_power_limit)
{
    *chassis_power_limit = robot_status.chassis_power_limit;
}


const ext_robot_status_t *get_robot_state_point(void)
{
    return &robot_status;
}

const ext_power_heat_data_t *get_power_heat_data_point(void)
{
    return &power_heat_data;
}


const ext_shoot_data_t *get_shoot_data_point(void)
{
    return &shoot_data;
}

void Required_Data() //后续可根据需要自行添加
{
		REFEREE_DATA.Initial_SPEED = shoot_data.initial_speed; //弹丸初速
//		REFEREE_DATA.Launching_Frequency = shoot_data.launching_frequency;
//		REFEREE_DATA.projectile_allowance_17mm =projectile_allowance.projectile_allowance_17mm;
//	
		REFEREE_DATA.Barrel_Heat = power_heat_data.shooter_17mm_1_barrel_heat;//当前热量	
//		REFEREE_DATA.CooLing_Value = robot_status.shooter_barrel_cooling_value;		
//		REFEREE_DATA.Heat_Limit =  robot_status.shooter_barrel_heat_limit;	
//		REFEREE_DATA.Chassis_Voltage =  power_heat_data.chassis_voltage; 
//		REFEREE_DATA.Chassis_Current =  power_heat_data.chassis_current;  
//		REFEREE_DATA.Buffer_Energy = power_heat_data.buffer_energy;
//		REFEREE_DATA.Chassis_Power = power_heat_data.chassis_power; 
		REFEREE_DATA.Chassis_Power_Limit = robot_status.chassis_power_limit; 
//		REFEREE_DATA.rest_time =	game_status.stage_remain_time;
//	  //导航相关
	  REFEREE_DATA.game_progress=game_status.game_progress;//比赛阶段
	  REFEREE_DATA.game_type=game_status.game_type;//比赛类型
	  REFEREE_DATA.blue_base_hp=game_robot_HP.blue_base_HP;
  	  REFEREE_DATA.blue_outpost_hp=game_robot_HP.blue_outpost_HP;
	  REFEREE_DATA.red_base_hp=game_robot_HP.red_base_HP;
	  REFEREE_DATA.red_outpost_hp=game_robot_HP.red_outpost_HP;
	  REFEREE_DATA.bullet_remaining_num_17mm=projectile_allowance.projectile_allowance_17mm;//剩余发弹量
	  REFEREE_DATA.remain_hp=robot_status.current_HP;//机器人当前血量
	  REFEREE_DATA.stage_remain_time=game_status.stage_remain_time;/*当前阶段剩余时间*/
	  REFEREE_DATA.rfid_status = rfid_status.rfid_status;//rfid状态
		REFEREE_DATA.max_hp=robot_status.maximum_HP;
	
}


void Cap_Required_Data() 
{
	// 	// 以下获取底盘功率和缓冲能量		
    // cap.cache_energy = REFEREE_DATA.Buffer_Energy;//获取缓冲能量
    // if (REFEREE_DATA.Chassis_Power == 0)
    //   cap.set_max_power = 44;
    // else
    //   cap.set_max_power = REFEREE_DATA.Chassis_Power_Limit -1;
    
}

extern CAN_HandleTypeDef hcan1;
void referee_data_send()
{
    uint8_t can_send_data [8];
    static CAN_TxHeaderTypeDef tx_message;
    uint32_t send_mail_box;

    tx_message.StdId = 0x303;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;

    float_to_bytes(REFEREE_DATA.Chassis_Power_Limit,&can_send_data[0]);  // 将x的字节存入can_data[0]~can_data[3]

    HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
}

unsigned char Instruction_buffer_Send[19];
uint8_t seq = 0;
 void SendSentryCMD(uint8_t sof,uint16_t cmd_id,uint16_t len)
 {
   uint16_t frame_length = frameheader_len + cmd_len + len + crc_len + subcontent_len + send_len + receive_len;   //数据帧长度    
   uint16_t subcontent_id = 0x0120;
   uint16_t sender_id = robot_status.robot_id;
   uint16_t reciiver_id =0x8080;
   uint32_t content_data =1;

   // 清空发送缓冲区，确保数据干净
     memset(Instruction_buffer_Send,0,19);  //存储数据的数组清零
    
     /*****帧头打包*****/
     Instruction_buffer_Send[0] = sof;//数据帧起始字节
     memcpy(&Instruction_buffer_Send[1],&len,2);//数据帧中data的长度
     Instruction_buffer_Send[3] = seq;//包序号
     append_CRC8_check_sum(Instruction_buffer_Send,frameheader_len);  //帧头校验CRC8

     /*****命令码打包*****/
     memcpy(&Instruction_buffer_Send[frameheader_len],&cmd_id, cmd_len);
    
     /*****数据打包*****/
     memcpy(&Instruction_buffer_Send[frameheader_len+cmd_len],&subcontent_id, subcontent_len);
     memcpy(&Instruction_buffer_Send[frameheader_len+cmd_len+subcontent_len],&sender_id, send_len);
     memcpy(&Instruction_buffer_Send[frameheader_len+cmd_len+subcontent_len+send_len],&reciiver_id, receive_len);
     memcpy(&Instruction_buffer_Send[frameheader_len+cmd_len+subcontent_len+send_len+receive_len],&content_data, len);

     /* CRC16校验 */
     append_CRC16_check_sum(Instruction_buffer_Send,frame_length);  //一帧数据校验CRC16

     if (seq == 0xff) seq=0;
   else seq++;
  
   //    // 使用DMA发送数据
     HAL_UART_Transmit_DMA(&huart6,Instruction_buffer_Send,19);
 }


//void SendSentryCMD(uint8_t sof,uint16_t cmd_id, uint32_t *p_data, uint16_t len)
//{
//  //完整帧长度：帧头，命令码，待发送数据长度，CRC校验长度
//  uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //数据帧长度    
//  
//  // 清空发送缓冲区，确保数据干净
//    memset(Instruction_buffer_Send,0,19);  //存储数据的数组清零
//    
//    /*****帧头打包*****/
//    Instruction_buffer_Send[0] = sof;//数据帧起始字节
//    memcpy(&Instruction_buffer_Send[1],&len,2);//数据帧中data的长度
////    memcpy(&Instruction_buffer_Send[1],(uint8_t*)&len,2);
//    Instruction_buffer_Send[3] = seq;//包序号
//    append_CRC8_check_sum(Instruction_buffer_Send,frameheader_len);  //帧头校验CRC8

//    /*****命令码打包*****/
//    memcpy(&Instruction_buffer_Send[5],&cmd_id, 2);
//    
//    /*****数据打包*****/
//    memcpy(&Instruction_buffer_Send[frameheader_len+cmd_len],p_data,len);
//    append_CRC16_check_sum(Instruction_buffer_Send,frame_length);  //一帧数据校验CRC16

//    if (seq == 0xff) seq=0;
//  else seq++;
//  
//  //    // 使用DMA发送数据
//    HAL_UART_Transmit_DMA(&huart6,Instruction_buffer_Send,13);
//}