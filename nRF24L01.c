#include "nRF24L01.h"

unsigned char nRF24L01_read_while_write(unsigned char data)
{
  unsigned char i;
  for(i=0; i<8; i++)
  {
    MOSI = data & 0x80;
    data <<= 1;
    SCK = 1;
    data |= MISO;
    SCK = 0;
  }
}

unsigned char nRF24L01_write_to_addr(unsigned char address, unsigned char data)
{
  unsigned char status;
  CSN = 0;
  status = nRF24L01_read_while_write(address);
  nRF24L01_read_while_write(data);
  CSN = 1;
  return status;
}

unsigned char nRF24L01_read_from_addr(unsigned char address)
{
  unsigned char data;
  CSN = 0;
  nRF24L01_read_while_write(address);
  data = nRF24L01_read_while_write(NOP);
  CSN = 1;
  return data;
}

unsigned char nRF24L01_read_from_addr_to_buf(unsigned char address, unsigned char *buffer, unsigned char length)
{
  unsigned char status, i;
  CSN = 0;
  status = nRF24L01_read_while_write(address);
  for(i=0; i<length; i++)
  {
    buffer[i] = nRF24L01_read_while_write(NOP);
  }
  CSN = 1;
  return status;
}

unsigned char nRF24L01_write_to_addr_from_buf(unsigned char address, unsigned char *buffer, unsigned char length)
{
  unsigned char status, i;
  CSN = 0;
  status = nRF24L01_read_while_write(address);
  for(i=0; i<length; i++)
  {
    nRF24L01_read_while_write(*buffer++);
  }
  CSN = 1;
  return status;
}

/**
 * @return
 *  0x10  达到最大重发次数，发送失败
 *  0x20  成功发送完成
 *  0xff  发送失败
 */
unsigned char nRF24L01_transmit_packet(unsigned char *buffer)
{
  unsigned char state;
  CE = 0;
  nRF24L01_write_to_address_from_buffer(WR_TX_PLOAD, buffer, TX_PLOAD_WIDTH);
  CE = 1;
  // 等待发送完成
  while(NRF_IRQ==1);
  state = nRF24L01_read_from_address(STATUS);
  nRF24L01_write_to_address(WRITE_REG + STATUS, state);
  // 达到最大重发次数
  if(state & MAX_TX)
  {
    // 清除TX FIFO寄存器
    nRF24L01_write_to_address(FLUSH_TX, 0xFF);
    return MAX_TX;
  }
  if(state & TX_OK)
  {
    return TX_OK;
  }
  return 0xFF;
}

/**
 * @return
 *  0   成功收到数据
 *  1   没有收到数据
 */
unsigned char nRF24L01_receive_packet(unsigned char *buffer)
{
  unsigned char state;
  state = nRF24L01_read_from_address(STATUS);
  // 清除TX_DS或MAX_RT中断标志
  nRF24L01_write_to_address(WRITE_REG + STATUS, state);
  if(state & RX_OK)
  {
    CE = 0;
    nRF24L01_read_from_address_to_buffer(RD_RX_PLOAD, buffer, RX_PLOAD_WIDTH);
    nRF24L01_write_to_address(FLUSH_RX, 0xFF);
    CE = 1;
    // delay 150 us
    return 0;
  }
  return 1;
}

/**
 * @return
 *  0: 存在
 *  1: 不存在
 */
unsigned char nRF24L01_check()
{
  unsigned char i, length = 5;
  unsigned char in_buffer[length] = { 0x11, 0x22, 0x33, 0x44, 0x55 };
  unsigned char out_buffer[length] = { 0x00 };
  SCK=0;
  CSN=1;
  CE=0;
  nRF24L01_write_to_address_from_buffer(WRITE_REG + TX_ADDR, in_buffer, length);
  nRF24L01_read_from_address_to_buffer(READ_REG + TX_ADDR, out_buffer, length);
  for(i=0; i<length; i++)
    if(out_buffer[i] != in_buffer[i])
      return 1;
  return 0;
}

void nRF24L01_configuration()
{
  CE=0;
  // 选择通道0的有效数据宽度
  nRF24L01_write_to_address(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
  // 清除RX FIFO寄存器
  nRF24L01_write_to_address(FLUSH_RX, 0xff);
  // 写TX节点地址
  nRF24L01_write_to_address_from_buffer(WRITE_REG + TX_ADDR, (uchar*)TX_ADDRESS, TX_ADR_WIDTH);
  // 设置TX节点地址,主要为了使能ACK
  nRF24L01_write_to_address_from_buffer(WRITE_REG + RX_ADDR_P0, (uchar*)RX_ADDRESS, RX_ADR_WIDTH);
  // 使能通道0的自动应答
  nRF24L01_write_to_address(WRITE_REG + EN_AA, 0x01);
  // 使能通道0的接收地址
  nRF24L01_write_to_address(WRITE_REG + EN_RXADDR,  0x01);
  // 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  nRF24L01_write_to_address(WRITE_REG + SETUP_RETR, 0x1a);
  // 设置RF通道为2.400GHz  频率=2.4+0GHz
  nRF24L01_write_to_address(WRITE_REG + RF_CH, 0);
  // 设置TX发射参数,0db增益,2Mbps,低噪声增益开启
  nRF24L01_write_to_address(WRITE_REG + RF_SETUP, 0x0F);
  // 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
  nRF24L01_write_to_address(WRITE_REG + CONFIG, 0x0f);
  // CE置高，使能发送
  CE=1;
}

void nRF24L01_send_buffer(unsigned char *buffer)
{
  CE = 0;
  nRF24L01_write_to_address(WRITE_REG + CONFIG, 0x0E);
  CE = 1;
  // delay 15 us
  nRF24L01_transmit_packet(buffer);
  CE = 0;
  nRF24L01_write_to_address(WRITE_REG + CONFIG, 0x0F);
  CE = 1;
}
