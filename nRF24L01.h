#ifndef __NRF24L01_H__
#define __NRF24L01_H__


#ifdef __SDCC_mcs51 //使用SDCC作为51编译器
#include "mcs51/8051.h"
#define CE              P2_7
#define CSN             P2_6
#define SCK             P2_5
#define MOSI            P2_4
#define MISO            P2_3
#define IRQ             P2_2

#define CE_Write(a)     (CE=a)
#define CSN_Write(a)     (CSN=a)
#define SCK_Write(a)     (SCK=a)
#define MOSI_Write(a)     (MOSI=a)
#define MISO_Write(a)     (MISO=a)
#define IRQ_Write(a)     (IRQ=a)

#define CE_Read()       (CE)
#define CSN_Read()       (CSN)
#define SCK_Read()       (SCK)
#define MOSI_Read()       (MOSI)
#define MISO_Read()       (MISO)
#define IRQ_Read()       (IRQ)

#define Soft_SPI_Init()   
`
#else 
#error not supported	
#endif



//定义数据设置
#define uchar unsigned char
#define uint  unsigned int


// NRF24L01寄存器操作命令
#define READ_REG        0x00  //读配置寄存器,低5位为寄存器地址
#define WRITE_REG       0x20  //写配置寄存器,低5位为寄存器地址
#define RD_RX_PLOAD     0x61  //读RX有效数据,1~32字节
#define WR_TX_PLOAD     0xA0  //写TX有效数据,1~32字节
#define FLUSH_TX        0xE1  //清除TX FIFO寄存器.发射模式下用
#define FLUSH_RX        0xE2  //清除RX FIFO寄存器.接收模式下用
#define REUSE_TX_PL     0xE3  //重新使用上一包数据,CE为高,数据包被不断发送.
#define NOP             0xFF  //空操作,可以用来读状态寄存器

// NRF24L01寄存器地址
#define CONFIG          0x00  //配置寄存器地址
#define EN_AA           0x01  //使能自动应答功能
#define EN_RXADDR       0x02  //接收地址允许
#define SETUP_AW        0x03  //设置地址宽度(所有数据通道)
#define SETUP_RETR      0x04  //建立自动重发
#define RF_CH           0x05  //RF通道
#define RF_SETUP        0x06  //RF寄存器
#define STATUS          0x07  //状态寄存器
#define OBSERVE_TX      0x08  // 发送检测寄存器
#define CD              0x09  // 载波检测寄存器
#define RX_ADDR_P0      0x0A  // 数据通道0接收地址
#define RX_ADDR_P1      0x0B  // 数据通道1接收地址
#define RX_ADDR_P2      0x0C  // 数据通道2接收地址
#define RX_ADDR_P3      0x0D  // 数据通道3接收地址
#define RX_ADDR_P4      0x0E  // 数据通道4接收地址
#define RX_ADDR_P5      0x0F  // 数据通道5接收地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收数据通道0有效数据宽度(1~32字节)
#define RX_PW_P1        0x12  // 接收数据通道1有效数据宽度(1~32字节)
#define RX_PW_P2        0x13  // 接收数据通道2有效数据宽度(1~32字节)
#define RX_PW_P3        0x14  // 接收数据通道3有效数据宽度(1~32字节)
#define RX_PW_P4        0x15  // 接收数据通道4有效数据宽度(1~32字节)
#define RX_PW_P5        0x16  // 接收数据通道5有效数据宽度(1~32字节)
#define FIFO_STATUS     0x17  // FIFO状态寄存器

// STATUS寄存器bit位定义
#define MAX_TX          0x10  //达到最大发送次数中断
#define TX_OK           0x20  //TX发送完成中断
#define RX_OK           0x40  //接收到数据中断

// 24L01发送接收数据宽度定义
#define TX_ADR_WIDTH    5     //5字节地址宽度
#define RX_ADR_WIDTH    5     //5字节地址宽度
#define TX_PLOAD_WIDTH  32    //32字节有效数据宽度
#define RX_PLOAD_WIDTH  32    //32字节有效数据宽度



// 发送地址
extern const unsigned char TX_ADDRESS[TX_ADR_WIDTH];
// 接收地址
extern const unsigned char RX_ADDRESS[RX_ADR_WIDTH];


extern unsigned char nRF24L01_read_while_write(unsigned char data);
extern unsigned char nRF24L01_write_to_addr(unsigned char address, unsigned char data);
extern unsigned char nRF24L01_read_from_addr(unsigned char address);
extern unsigned char nRF24L01_read_from_addr_to_buf(unsigned char address, unsigned char *buffer, unsigned char length);
extern unsigned char nRF24L01_write_to_addr_from_buf(unsigned char address, unsigned char *buffer, unsigned char length);
/**
 * @return
 *  0x10  达到最大重发次数，发送失败
 *  0x20  成功发送完成
 *  0xff  发送失败
 */
extern unsigned char nRF24L01_transmit_packet(unsigned char *buffer);
/**
 * @return
 *  0   成功收到数据
 *  1   没有收到数据
 */
extern unsigned char nRF24L01_receive_packet(unsigned char *buffer);
/**
 * @return
 *  0: 存在
 *  1: 不存在
 */
extern unsigned char nRF24L01_check();
extern void nRF24L01_configuration();
extern void nRF24L01_send_buffer(unsigned char *buffer);

#endif  // __NRF24L01_H__
