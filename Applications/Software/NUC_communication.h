#ifndef NUC_COMMUNICATION_H
#define NUC_COMMUNICATION_H

#define DATA_SIZE 30
#define YAW_DATA_HIS 1
#define PITCH_DATA_HIS 0
#define ANGLE_TO_RAD 0.01745329251994329576923690768489f
#define RAD_TO_ANGLE 57.295779513082320876798154814105f
#define SCAN_PITCH_DEG_ACC 0.6f
#define SCAN_YAW_DEG_ACC 0.6f



extern float auto_yaw,auto_pitch,visual_yaw,trans_delay_data,raw_vision_data;

#include "referee_handle_pack.h"
#include "stdint.h"
typedef struct
{
	uint8_t header;
	uint8_t useless;
	uint8_t target_id;
	float yaw;
	float pitch;
	float pitch_speed;
	float yaw_speed;//����ĽǶ��ǡ�Ϊ��λ���յ����ٶ��ǻ���/s��
	uint8_t distance;
  uint8_t shoot;//������־λ
	uint8_t shoot_mode;//������־λ
	uint32_t his_time_data;
	char unknown[4];//ԭ����20����Ϊ16����Ϊʱ���ռ�����ĸ��ֽ�
	uint16_t checksum;
} __attribute__((packed)) NUC_data_t;
extern NUC_data_t fromNUC;


////��NUC��STM32����Ϣ��/����
//int decodeNUC(NUC_data_t *target, unsigned char tx_buff[], unsigned int len);
//int encodeNUC(NUC_data_t *target, unsigned char rx_buff[], unsigned int len);
//void board_to_nuc(uint8_t time);
//void Test_communcation_0128();

//float  auto_aim_judgement(float input);
//void vision_reset(void);
//void data_kalman_process(void);
//void auto_ctrl(void);

 //����
typedef __packed struct         
{
	//uint8_t header; //֡ͷ
	uint8_t game_type; //��������
	uint8_t game_progress; //�����׶�
	uint16_t remain_hp; //ʣ��Ѫ��
	uint16_t max_hp; //���Ѫ��
	
	uint16_t stage_remain_time;//��ǰ�׶�ʣ��ʱ��
	uint16_t bullet_remaining_num_17mm;
	uint16_t red_outpost_hp;
	uint16_t red_base_hp;
	uint16_t blue_outpost_hp;
	uint16_t blue_base_hp;
	uint32_t rfid_status;

  /* 自瞄传给导航 */
  uint8_t target_id;    //目标id
  float target_pose[3]; // 目标坐标
  float target_vx;      //目标x轴速度
  float target_vy;      //目标y轴速度
  float target_vz;      //目标z轴速度
  float target_theta;   //装甲板朝向角
  float target_omega;   //目标角速度
}STM32ROS_data_t;

extern STM32ROS_data_t stm32send_1,stm32send_6,stm32receive_1,stm32receive_6;
extern long long WaitTime;

 //
typedef struct __attribute__((packed))      
{
	uint8_t header; 
	uint8_t checksum;
	float x_speed;
	float y_speed;
	float rotate;
	float yaw_speed;
	uint8_t running_state;
}Navigation_data_t;

extern Navigation_data_t Navigation_send_1,Navigation_send_6,Navigation_receive_1,Navigation_receive_6;
void Navigation_send_message();


//auto control

typedef struct
{
    struct
    {
        uint8_t sof;
        uint8_t crc8;
    }__attribute__((packed)) FrameHeader; // 2
    struct
    {
        float curr_yaw;
        float curr_pitch;
        float curr_omega;
        uint8_t state;// state 0是打车 1是打前哨站 2是打小符 3是打符
        uint8_t autoaim; // autoaim那个0是不用自瞄 1是开自瞄
        uint8_t enemy_color;// 0为蓝色，1为红色
    }__attribute__((packed)) To_minipc_data; // 15
    struct
    {
        uint16_t crc16;
    }__attribute__((packed)) FrameTailer;//2
    uint8_t enter;                       //1
}__attribute__((packed)) STM32_data_t;

typedef struct  
{
    struct
    {
        uint8_t sof;
        uint8_t crc8;
    }__attribute__((packed)) FrameHeader; // 2
    struct
    {
      /* 自瞄传给电控 */
        float shoot_yaw;
        float shoot_pitch;
        uint8_t fire;      // 发弹信号
				uint8_t target_id;
			
    }__attribute__((packed)) from_minipc_data;    // 15
    struct
    {
        uint16_t crc16;
    }__attribute__((packed)) FrameTailer;
}__attribute__((packed)) MINIPC_data_t;


void STM32_to_MINIPC();
void decodeMINIPCdata(MINIPC_data_t *target, unsigned char buff[], unsigned int len);
void Auto_control();
void MINIPC_to_STM32();
void Scan();
void control_tran();
void turnbox();

extern MINIPC_data_t fromMINIPC;
extern STM32_data_t toMINIPC;

#endif

//end of file
