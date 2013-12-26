/** @file TVP5150cmn.h
 *
 *  @brief This file provides the function calls for TVP5150 decoder
 *
 *  @author CTWu
 *  @version 0.1.0
 *  @date 2012/03/08
 *
 */

#ifndef _TVP5150_H_
#define _TVP5150_H_

/*
 * @This function for read TVP5150 i2c register 
 *
 * @int TVP5150_data_read(u8 i2c_addr, u8 reg, int num, unsigned char *buf)
 * @param i2c_addr i2c address
 * @param reg i2c register offset 
 * @param num register read amount
 * @param buf register read out data
 * @return 0 on success, <0 on error
 */ 
extern int tvp5150_data_read(u8 i2c_addr, u8 reg, int num, unsigned char *buf);

/*
 * @This function for write TVP5150 i2c register 
 *
 * @int TVP5150_data_write(u8 i2c_addr, u8 reg, int num, unsigned char *data)
 * @param i2c_addr i2c address
 * @param reg i2c register offset 
 * @param num register read amount
 * @param data register write data
 * @return 0 on success, <0 on error
 */ 
extern int tvp5150_data_write(u8 i2c_addr, u8 reg, int num, unsigned char *data);
 
/*
 * @This function for get TVP5150 input clock frequency
 *
 * @unsigned int TVP5150_get_input_clk(void)
 * @return clock frequency
 */ 
extern unsigned int tvp5150_get_input_clk(void);

/*
 * @This function for get TVP5150 i2c adapter
 *
 * @void *TVP5150_get_i2c_adapter(void)
 * @return pointer of i2c adapter
 */ 
extern void *tvp5150_get_i2c_adapter(void);
#endif /*_TVP5150_H_*/
