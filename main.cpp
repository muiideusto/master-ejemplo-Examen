/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "Grove_LCD_RGB_Backlight.h"
#include <cstdio>

#define WAIT_TIME_MS 500 
DigitalOut LED(D3);
DigitalIn Pulsador(D2);
AnalogIn Detector(A0); 
PwmOut Cerradura(D5);
Grove_LCD_RGB_Backlight Pantalla(PB_9,PB_8);

int Activa = 0;
int Saltado = 0;
int Posicion;
Timer Time;
Timer Time2;

char cadena1[17];
char cadena2[17];
char cadena3[17];
char cadena4[17];

enum estados {Reposo, Pulsado, Espera, Alarma,Detectado,Pulsado2} estado;

void Pantalla_Control(int Activa, int Saltado);
void Reposando();
void Pulsando();
void Esperando();
void Alarmando();
void Detectando();
void Pulsando2();


int main()
{
    estado = Reposo;
    Posicion = 0;
    
    sprintf(cadena1,"Alarma Activa   ");
    sprintf(cadena2,"Alarma no Activa");
    sprintf(cadena3,"Sonando");
    sprintf(cadena4,"Sin Sonar");

    while (true)
    {        
        Pantalla_Control(Activa, Saltado);
        switch (estado) 
        {
        case Reposo:
        Reposando();
        break;
        case Pulsado:        
        Pulsando();
        break;
        case Espera:
        Esperando();
        break;
        case Alarma:
        Alarmando();
        break;
        case Detectado:
        Detectando();     
        break;
        case Pulsado2:
        
        Pulsando2();
        break;
        }
    }
}

void Pantalla_Control(int Activa, int Saltado)
{
    if(Activa == 1)
    {
        Pantalla.setRGB(0xff, 0x00, 0xff);  
        Pantalla.locate(0,0);        
        Pantalla.print(cadena1);
    }else
    {
        Pantalla.setRGB(0xff, 0x00, 0xff);  
        Pantalla.locate(0,0);        
        Pantalla.print(cadena2);    
    }
    if(Saltado == 1)
    {
        Pantalla.setRGB(0xff, 0x00, 0xff);  
        Pantalla.locate(0,1);        
        Pantalla.print(cadena3);
    }else
    {
        Pantalla.setRGB(0xff, 0x00, 0xff);  
        Pantalla.locate(0,1);        
        Pantalla.print(cadena4);    
    }
}

void Reposando()
{
        if(Posicion == 10)
        {
            Posicion = 0;
            Cerradura.pulsewidth_us(500);
        }
        LED = 0;
        Activa = 0;
        Saltado = 0;
        Time.reset();
        if(Pulsador == 1)
        {
            Pantalla.clear();
            estado = Pulsado;            
        }
}
void Pulsando()
{
        if(Pulsador == 0)
        {
            Pantalla.clear();
            estado = Espera;
            Time.start();
        }
}
void Esperando()
{
        if(Time.read() >= 10)
        {
            Pantalla.clear();
            Cerradura.pulsewidth_us(1000);
            Posicion = 10;
            estado = Alarma;
        }
}
void Alarmando()
{
        Activa = 1;
        Time.reset();
        if(Pulsador == 1)
        {
            Pantalla.clear();
            estado = Pulsado2;            
        }
        if(Detector.read() > 0.5f)
        {
            Pantalla.clear();
            estado = Detectado;
            Saltado = 1;            
        }
}
void Detectando()
{
        if(LED == 0)
        {
           if(Time2.read() == 0)
           {
               LED = 1;
               Time2.start();
           }else if (Time2.read() >= 1)
           {
               LED = 1;
               Time2.reset();
               Time2.start();
           }     
        }else
        {
           if (Time2.read() >= 1)
           {
               LED = 0;
               Time2.reset();
               Time2.start();
           }   
        }
        if(Pulsador == 1)
        {
            Pantalla.clear();
            estado = Pulsado2;            
        }   
}
void Pulsando2()
{
        if(Pulsador == 0)
        {
            Pantalla.clear();
            estado = Reposo;
            Time.reset();
            Time2.reset();
        }
}
