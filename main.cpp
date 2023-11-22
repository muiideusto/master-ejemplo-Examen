/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "Grove_LCD_RGB_Backlight.h"

#define WAIT_TIME_MS 500 
#define R0 100000.0
#define R 100000.0
#define Beta 4250.0
#define T0 298.3
#define Vcc 5.0
#define Vref 3.3

AnalogIn ntc(A0); //Entrada analogica para leer la tensióo de la NTC
Grove_LCD_RGB_Backlight rgbLCD(PB_9,PB_8); //El switch de la placa de grove hay que seleccionarlo en 5V para que se ilumine bien



float temp; // variable para guardar el valor de la temperatura
char cadena[16]; //Cadena para generar el texto a ver en el LCD
 
float calculaTemp(float dato){ //Función que calcula la temperatura
    float vNTC; //Tensión del divisor de tensión entre la NTC y la resistencia Fija
    float rNTC; //Resistencia de la NTC a la temperatura ambiente
    vNTC=dato*Vref; //La tensión de referencia de la tarjeta es de 3.3, el read devuelve un dato entre 0 y 1.
    rNTC=((Vcc*R)/vNTC)-R; //Calcula la resistencia de la NTC, La tensión de VCC es 5V ya que es el voltaje que pasa el cable al sensor. Para que el LCD funcione.
    return (Beta/(log(rNTC/R0)+(Beta/T0)))-273.3;
}

int main()
{
    rgbLCD.setRGB(0xff, 0xff, 0xff);                

    while (true)
    {
        rgbLCD.locate(0,0); //Lo siguiente que se mande al LCD en primera fila, primera columna
        rgbLCD.print("La temp es:"); //Escribe un texto fijo
        temp=calculaTemp(ntc.read());   //Convierte el valor del adc en temperatura
        sprintf(cadena,"%.2f C",temp); //Calcula la cadena para visualizar la temperatura en el LCD
        rgbLCD.locate(0,1); //Lo siguiente lo escribe en la segunda fila, primera columna
        rgbLCD.print(cadena); //Manda el texto formateado al LCD
        thread_sleep_for(WAIT_TIME_MS); //Espera medio segundo.
    }
}
