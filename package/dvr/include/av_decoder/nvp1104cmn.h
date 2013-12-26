/** @file nvp1104cmn.h
 *
 *  @brief This file provides the function calls for NVP1104 decoder
 *
 *  @author Jerson Lin
 *  @version 0.1.0
 *  @date 2010/04/12
 *
 */

#ifndef _NVP1104_H_
#define _NVP1104_H_

/*
 * @This function for read nvp1104 i2c register 
 *
 * @int nvp1104_data_read(u8 i2c_addr, u8 reg, int num, unsigned char *buf)
 * @param i2c_addr i2c address
 * @param reg i2c register offset 
 * @param num register read amount
 * @param buf register read out data
 * @return 0 on success, <0 on error
 */
extern int nvp1104_data_read(u8 i2c_addr, u8 reg, int num, unsigned char *buf);

/*
 * @This function for write nvp1104 i2c register 
 *
 * @int nvp1104_data_write(u8 i2c_addr, u8 reg, int num, unsigned char *data)
 * @param i2c_addr i2c address
 * @param reg i2c register offset 
 * @param num register read amount
 * @param data register write data
 * @return 0 on success, <0 on error
 */
extern int nvp1104_data_write(u8 i2c_addr, u8 reg, int num, unsigned char *data);

/*
 * @This function for get nvp1104 input clock frequency
 *
 * @unsigned int nvp1104_get_input_clk(void)
 * @return clock frequency
 */
extern unsigned int nvp1104_get_input_clk(void);

/*
 * @This function for get nvp1104 i2c adapter
 *
 * @void *nvp1104_get_i2c_adapter(void)
 * @return pointer of i2c adapter
 */
extern void *nvp1104_get_i2c_adapter(void);
#endif /*_NVP1104_H_*/


