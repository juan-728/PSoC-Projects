#include <project.h>
#include <stdio.h> 

#define ADDR_DS3231 0x68
#define ADDR_DS1307 0x68
#define SlaveAddress 0x68
#define DS3231_REG_SECONDS  0x00 
#define DS3231_REG_CONTROL  0x0E
#define DS3231_REG_STATUS   0x0F
#define _I2C_WRITE   0
#define _I2C_READ    1

uint8 buf[7];
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6
#define SUNDAY 7

void REFRESH(){
    
    uint8 result,i;
        do{
           result=I2C_MasterSendStart(ADDR_DS3231, I2C_WRITE_XFER_MODE);
        }
        while(result!=I2C_MSTR_NO_ERROR);
        
        result=I2C_MasterWriteByte(0x00);
        result=I2C_MasterSendRestart(ADDR_DS3231, I2C_READ_XFER_MODE);
        for(i=0;i<6;i++){
            buf[i]=I2C_MasterReadByte(I2C_ACK_DATA);
        }
        buf[6]=I2C_MasterReadByte(I2C_NAK_DATA);
        I2C_MasterSendStop();
}

void WEEKDAY(){
    switch(buf[3]){
        case MONDAY:
            LCD_PrintString("MON");
            break;
        case TUESDAY:
            LCD_PrintString("TUE");
            break;
        case WEDNESDAY:
            LCD_PrintString("WED");
            break;
        case THURSDAY:
            LCD_PrintString("THU");
            break;
        case FRIDAY:
            LCD_PrintString("FRI");
            break;
        case SATURDAY:
            LCD_PrintString("SAT");
            break;
        case SUNDAY:
            LCD_PrintString("SUN");
            break;
        default: 
            LCD_PrintString("MON");
            break;    }

}

void DISPLAY(){
    LCD_ClearDisplay();
    LCD_Position(0,0);

    LCD_PrintNumber((buf[2]>>4)&0x03); //hour
    LCD_PrintNumber(buf[2]&0X0F);
    LCD_PutChar(':');
    LCD_PrintNumber(buf[1]>>4); //minute
    LCD_PrintNumber(buf[1]&0x0F);
    LCD_PutChar(':');
    LCD_PrintNumber(buf[0]>>4&0x07); //second
    LCD_PrintNumber(buf[0]&0x0F);

    LCD_Position(1,0);
    LCD_PrintString("20");
    LCD_PrintNumber(buf[6]>>4);
    LCD_PrintNumber(buf[6]&0x0F);//year
    LCD_PutChar('/');
    LCD_PrintNumber(buf[5]>>4);
    LCD_PrintNumber(buf[5]&0x0F);//month
    LCD_PutChar('/');
    LCD_PrintNumber(buf[4]>>4);
    LCD_PrintNumber(buf[4]&0x0F);//day
    LCD_PutChar(' ');
    WEEKDAY();      
}

void INIT_TIME_CONFIG(){
    uint8 result,i;

        buf[0] = 0x30; //30 sec
        buf[1] = 0x29; //29 min
        buf[2] = 0x60 | 0x02; //2pm: 12-hour format (bit6=1), PM(bit5=1)
        buf[3] = THURSDAY;
        buf[4] = 0x19; //date:19
        buf[5] = 0x09; //month: 9 sept
        buf[6] = 0x19; //2019
        
      do{
           result=I2C_MasterSendStart(ADDR_DS3231, I2C_WRITE_XFER_MODE);
      }
      while(result!=I2C_MSTR_NO_ERROR);
        
      result=I2C_MasterWriteByte(0x0);
     for(i=0;i<7;i++){
            result=I2C_MasterWriteByte(buf[i]);
      
    }
      I2C_MasterSendStop();
}

int main(){   
        
    int delay;
        
    CyGlobalIntEnable;
    
    LCD_Start();
    I2C_Start();
    I2C_Init();
    I2C_Enable();
 
    LCD_PrintString("Update Date?");
    
    delay = 20;
    while(delay--){
        if (!nSW_Read()){
            INIT_TIME_CONFIG();
            LCD_ClearDisplay();
            LCD_PrintString("Date Updated!");
            CyDelay(1000);
            break;
        }
        CyDelay(100);
    }
        
    for(;;){ 
       REFRESH();
       DISPLAY();
    
       CyDelay(200);
       if(buf[1]==0x14){
           LCD_ClearDisplay();
           LCD_Position(0,0);
           CyDelay(10000);
       }
    }
}

/*
uint8 Tabla[7];
#define LUNES 1
#define MARTES 2
#define MIERCOLES 3
#define JUEVES 4
#define VIERNES 5
#define SABADO 6
#define DOMINGO 7


////RTC   ///////
void RENOVARDATOS(){
    uint8 result,i;
        do{
           result=I2C_MasterSendStart(SlaveAddress, I2C_WRITE_XFER_MODE);
        }
        while(result!=I2C_MSTR_NO_ERROR);
        
        result=I2C_MasterWriteByte(0x00);
        result=I2C_MasterSendRestart(SlaveAddress, I2C_READ_XFER_MODE);
        for(i=0;i<6;i++){
            Tabla[i]=I2C_MasterReadByte(I2C_ACK_DATA);
        }
        Tabla[6]=I2C_MasterReadByte(I2C_NAK_DATA);
        I2C_MasterSendStop();
        
        
        
}
void DIADELASEMANA(){
    switch(Tabla[3]){
        case LUNES:
            LCD_PrintString("LUN");
            break;
        case MARTES:
            LCD_PrintString("MAR");
            break;
        case MIERCOLES:
            LCD_PrintString("MIE");
            break;
        case JUEVES:
            LCD_PrintString("JUE");
            break;
        case VIERNES:
            LCD_PrintString("VIE");
            break;
        case SABADO:
            LCD_PrintString("SAB");
            break;
        case DOMINGO:
            LCD_PrintString("DOM");
            break;
        default: 
            LCD_PrintString("LUN");
            break;    }

}
void MOSTRARDATOS(){
    LCD_ClearDisplay();
    LCD_Position(0,0);
   

    LCD_PrintNumber((Tabla[2]>>4)&0x03);// Horas 
    LCD_PrintNumber(Tabla[2]&0X0F);
    LCD_PutChar(':');
    LCD_PrintNumber(Tabla[1]>>4);// minutos
    LCD_PrintNumber(Tabla[1]&0x0F);
    LCD_PutChar(':');
    LCD_PrintNumber(Tabla[0]>>4&0x07);// Segundos
    LCD_PrintNumber(Tabla[0]&0x0F);


    LCD_Position(1,0);
    LCD_PrintString("20");
    LCD_PrintNumber(Tabla[6]>>4);
    LCD_PrintNumber(Tabla[6]&0x0F);//Año
    LCD_PutChar('/');
    LCD_PrintNumber(Tabla[5]>>4);
    LCD_PrintNumber(Tabla[5]&0x0F);//Mes
    LCD_PutChar('/');
    LCD_PrintNumber(Tabla[4]>>4);
    LCD_PrintNumber(Tabla[4]&0x0F);//dia
    LCD_PutChar(' ');
    DIADELASEMANA();
    
        
   
}
void CONFIGURARHORAINICIAL(){
    uint8 result,i;

    
    Tabla[0]=0x30;//segundos
    Tabla[1]=0x10;//minutos
    Tabla[2]=0x20;//horas
    Tabla[3]=0x03;//Dia LUN MAR...
    Tabla[4]=0x04;//Dia del mes
    Tabla[5]=0x10;//mes
    Tabla[6]=0x17;//year
        
      do{
           result=I2C_MasterSendStart(SlaveAddress, I2C_WRITE_XFER_MODE);
      }
      while(result!=I2C_MSTR_NO_ERROR);
        
      result=I2C_MasterWriteByte(0x0);
     for(i=0;i<7;i++){
            result=I2C_MasterWriteByte(Tabla[i]);
      
    }
      I2C_MasterSendStop();
    }

//////MAIN //////
int main()
    {   
    //CyGlobalIntEnable;
    
    LCD_Start();
    I2C_Start();

    //CONFIGURARHORAINICIAL();
 
    for(;;) 
        { 
       RENOVARDATOS();
       MOSTRARDATOS();
       CyDelay(200);
       if(Tabla[1]==0x14){
         LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("SENSOR ACTIVADO");
        CyDelay(10000);
    }

      
        }
    }

*/
/* [] END OF FILE */
