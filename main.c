/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define PN532_I2C_ADDRESS       (0x48 >> 1) // this is from PN532 DATASHEET
#define _I2C_WRITE  0
#define _I2C_READ   1

bool Read_I2C(uint8_t addr, uint8_t reg, uint8_t length, uint8_t *buf);
bool Write_I2C(uint8_t addr, uint8_t reg, uint8_t length, uint8_t *buf);

#define BCD2DEC(bcd)        ((bcd>>4)*10 + (bcd&0x0F))

   uint8_t  buf[8];
   uint8_t  err;
   int      i;


void REFRESH(){
    uint8 result,i;
        do{
           result=I2C_MasterSendStart(PN532_I2C_ADDRESS, I2C_WRITE_XFER_MODE);
        }
        while(result!=I2C_MSTR_NO_ERROR);
        
        result=I2C_MasterWriteByte(0x00);
        result=I2C_MasterSendRestart(PN532_I2C_ADDRESS, I2C_READ_XFER_MODE);
        for(i=0;i<6;i++){
            buf[i]=I2C_MasterReadByte(I2C_ACK_DATA);
        }
        buf[6]=I2C_MasterReadByte(I2C_NAK_DATA);
        I2C_MasterSendStop();
}


int main(void)
{

   
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Start();
    I2C_Init();
    I2C_Enable();
    
    for(;;)
    {
    }
}
bool Read_I2C(uint8_t addr, uint8_t reg, uint8_t length, uint8_t *buf){
    uint8_t err;
    int     i;
    
    
    I2C_MasterSendStart(addr, _I2C_WRITE);
    if (err != I2C_MSTR_NO_ERROR){
        I2C_MasterSendStop();
        return false; 
    }
    err = I2C_MasterWriteByte(reg);
     if (err != I2C_MSTR_NO_ERROR){
        I2C_MasterSendStop();
        return false; 
    }
    err = I2C_MasterSendRestart(addr, _I2C_READ);
     if (err != I2C_MSTR_NO_ERROR){
        I2C_MasterSendStop();
        return false; 
    }
    for (i = 0; i< length; i++){
       if (i == (length-1))  
       buf[i] = I2C_MasterReadByte(I2C_NAK_DATA); 
       else
       buf[i] = I2C_MasterReadByte(I2C_ACK_DATA);
    }
     I2C_MasterSendStop();
     return true; 
   }
//////////////////////////////////////////////////////////////////////////
bool Write_I2C(uint8_t addr, uint8_t reg, uint8_t length, uint8_t *buf){

    uint8_t err;
    int     i;
    
    I2C_MasterSendStart(addr, _I2C_WRITE);
    if (err != I2C_MSTR_NO_ERROR){
        I2C_MasterSendStop();
        return false; 
    }
    err = I2C_MasterWriteByte(reg);
     if (err != I2C_MSTR_NO_ERROR){
        I2C_MasterSendStop();
        return false; 
    }
   
    for (i = 0; i< length; i++){
      I2C_MasterWriteByte(buf[i]);
    }
     I2C_MasterSendStop();
     return true; 

}
/* [] END OF FILE */
