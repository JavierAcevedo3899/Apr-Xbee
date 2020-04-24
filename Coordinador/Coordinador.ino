//Valores a configurar
//char DireccionCoordinador = 'A';

#include <SoftwareSerial.h>
SoftwareSerial XBee(10, 11);//Rx = 0; Tx = 1; //Rx = 10; Tx = 11; //Rx = 50; Tx = 51;
/*
  Software serial multple serial test

  Receives from the hardware serial, sends to software serial.
  Receives from software serial, sends to hardware serial.

  The circuit:
   RX is digital pin 10 (connect to TX of other device)
   TX is digital pin 11 (connect to RX of other device)

  Note:
  Not all pins on the Mega and Mega 2560 support change interrupts,
  so only the following can be used for RX:
  10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

  Not all pins on the Leonardo and Micro support change interrupts,
  so only the following can be used for RX:
  8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

  created back in the mists of time
  modified 25 May 2012
  by Tom Igoe
  based on Mikal Hart's example

  This example code is in the public domain.

*/

int sensorPin = A0;
  const int led = 9;
void setup() {
Serial.begin(9600);
XBee.begin(9600);

}

void loop() {
//long chexsum = 0x10 + 0x01
    if (XBee.available() > 21) {
        if(XBee.read() == 0x7E){
            for(int i = 0; i <18; i++){
              byte descartar = XBee.read();
            }
            int analogMSB = XBee.read();// Toma el valor de los 2 byte de  lectura del canal analogo
            int analogLSB = XBee.read();
            int analogReading = 0;
            /*int analogReading = analogLSB + (analogMSB + 256); // Une los dos bytes
            temp = analogReading * (1.0/1024); // Convierte a Voltaje
            tempC = (temp - 0.5) * 100; // Se lleva a temperatura*/
            Serial.print("analogMSB: ");
            Serial.println(analogMSB);
            Serial.print("analogLSB: ");
            Serial.println(analogLSB);
            /*Serial.print("analogReading: ");
            Serial.println(analogReading);
            Serial.print("Nivel de Agua: ");
            Serial.println(temp);
            Serial.println("---------------");*/
                    
                   switch (analogMSB) {
                      case 0:
                      Serial.println("Estoy en el 0");
                        analogReading = analogLSB;
                        break;
                      case 1:
                      Serial.println("Estoy en el 1");
                        analogReading = analogLSB + 256;
                        break;
                      case 2:
                      Serial.println("Estoy en el 2");
                        analogReading = analogLSB + 512;
                        break;
                      case 3:
                      Serial.println("Estoy en el 3");
                        analogReading = analogLSB + 768;
                        break;
                      default:
                        Serial.println("No reconoce los valores");
                        break;
                    }
            Serial.print("analogReading: ");
            Serial.println(analogReading); // 0 - 1023
            analogReading = analogReading / 4; //0 - 255
            analogWrite(led, analogReading);
            /*float voltaje = analogReading * 0.00322;  // 3.3 v / 1024 = 0.00322
            Serial.print("Voltaje: ");
            Serial.println(voltaje);*/
            Serial.println("-----------------------------");
        }        
    }
}
/*
--------------------------------------------------------------------
    incoming_char = XBee.read();    ///// 1 Informacion del Texto Ingresado Caracter por Caracter
    Serial.print(incoming_char);
    DATOSxbee1 = incoming_char;
    delay(10);
----------------------------------------------------
    delay(500); //Tiempo para que se envie datos por partes
    numeroCaracteres = 2; //Longitud del texto que se va a enviar, ejemplo: xbee1
    textoEnvioXbee1 = 'L'; //Linea
    textoEnvioXbee2 = 'A'; //Activar

    RespaldoEnvioXbee(); //Almacena los Textos a ReEnviar
    MeEscuchoConfirmacion = 1; //Si la central no responde en cada cierto tiempo vuelve a enviar la solicitud

    EnviarMensajeXbee();
----------------------------------
CodigoZona8[0]
//----------------------------------------------------------
///////////////////////////////////////////////////////////////////////////LLAMADO DE FUNCIONES////////////////////////////////////////
void EnviarMensajeXbee() {
  Serial.println("YO Hablo xbee");
  long longitud = 0x0E;
  long valor_0xFF = 0xFF;
  long valorMensaje = 0x00;
  for (int i = 1; i <= numeroCaracteres; i++) {

    longitud = longitud + 0x01;
  }

  XBee.write(0x7E); //byte de inicio
  XBee.write((byte)0x00); //Longitud (Parte alta)
  XBee.write(longitud); //longitud (Parte Baja) 16 bytes
  XBee.write(0x10); //Tipo de mensaje, Solicitud de comando AT // TOMAR EL VALOR DESDE AQUI PARA EL Checksum
  XBee.write((byte)0x01); //Secuencia del mensaje
  //Direccion de destino (0xFFFF) Broadcast
  //Coordinador = 0013A2004178934D
  XBee.write((byte)0x00); // 0x00
  XBee.write((byte)0x00); // 0x13
  XBee.write((byte)0x00); // 0xA2
  XBee.write((byte)0x00); // 0x00
  XBee.write((byte)0x00); // 0x41
  XBee.write((byte)0x00); // 0x78
  XBee.write((byte)0x00); // 0x93
  XBee.write((byte)0x00); // 0x4D
  // Direccion de la red de destino (0xFFFE) Broadcast
  XBee.write(0xFF);
  XBee.write(0xFE);
  //Comando Remoto
  XBee.write((byte)0x00);
  XBee.write((byte)0x00);

  // Nombre del comando AT
  for (int i = 1; i <= numeroCaracteres; i++) {
    if ( i == 1) {
      delay(10);
      XBee.write(textoEnvioXbee1);
      Serial.print(textoEnvioXbee1);
      valorMensaje = valorMensaje + textoEnvioXbee1;
    }

    if ( i == 2) {
      delay(10);
      XBee.write(textoEnvioXbee2);
      Serial.print(textoEnvioXbee2);
      valorMensaje = valorMensaje + textoEnvioXbee2;
    }

    if ( i == 3) {
      delay(10);
      XBee.write(textoEnvioXbee3);
      Serial.print(textoEnvioXbee3);
      valorMensaje = valorMensaje + textoEnvioXbee3;
    }

    if ( i == 4) {
      delay(10);
      XBee.write(textoEnvioXbee4);
      Serial.print(textoEnvioXbee4);
      valorMensaje = valorMensaje + textoEnvioXbee4;
    }

    if ( i == 5) {
      delay(10);
      XBee.write(textoEnvioXbee5);
      Serial.print(textoEnvioXbee5);
      valorMensaje = valorMensaje + textoEnvioXbee5;
    }

    if ( i == 6) {
      delay(10);
      XBee.write(textoEnvioXbee6);
      Serial.print(textoEnvioXbee6);
      valorMensaje = valorMensaje + textoEnvioXbee6;
    }

    if ( i == 7) {
      delay(10);
      XBee.write(textoEnvioXbee7);
      Serial.print(textoEnvioXbee7);
      valorMensaje = valorMensaje + textoEnvioXbee7;
    }

    if ( i == 8) {
      delay(10);
      XBee.write(textoEnvioXbee8);
      Serial.print(textoEnvioXbee8);
      valorMensaje = valorMensaje + textoEnvioXbee8;
    }

    if ( i == 9) {
      delay(10);
      XBee.write(textoEnvioXbee9);
      Serial.print(textoEnvioXbee9);
      valorMensaje = valorMensaje + textoEnvioXbee9;
    }

    if ( i == 10) {
      delay(10);
      XBee.write(textoEnvioXbee10);
      Serial.print(textoEnvioXbee10);
      valorMensaje = valorMensaje + textoEnvioXbee10;
    }
    if ( i == 11) {
      delay(10);
      XBee.write(textoEnvioXbee11);
      Serial.print(textoEnvioXbee11);
      valorMensaje = valorMensaje + textoEnvioXbee11;
    }
    if ( i == 12) {
      delay(10);
      XBee.write(textoEnvioXbee12);
      Serial.print(textoEnvioXbee12);
      valorMensaje = valorMensaje + textoEnvioXbee12;
    }
    if ( i == 13) {
      delay(10);
      XBee.write(textoEnvioXbee13);
      Serial.print(textoEnvioXbee13);
      valorMensaje = valorMensaje + textoEnvioXbee13;
    }
    if ( i == 14) {
      delay(10);
      XBee.write(textoEnvioXbee14);
      Serial.print(textoEnvioXbee14);
      valorMensaje = valorMensaje + textoEnvioXbee14;
    }
  }
  Serial.println(' ');
  //Checksum
  //Coordinador = 0013A2004178934D
  long chexsum = 0x10 + 0x01 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0xFF + 0xFE + 0x00 + 0x00 + valorMensaje;
  chexsum = chexsum & 0xFF;
  chexsum = valor_0xFF - chexsum;
  XBee.write(chexsum);// Checksum

  ///////////////// Tiempo EEPROM
  T1Wifi = 0;
  RecargarTiempoWifi = 0;
  t1Wifi = 0;
  RtaWifi = 0;
  //////////////////////////////////
  //Serial.println(borrar);
}*/
//--------------------------------------------------------
/*void EspacioVaciosMensajeAPP()
{

  if (Msg1 == '\0') { // Para Cubrir los espacios vacios .... \n == a enter
    Msg1  = (char)32; // Datos del texto 1
    Msg2  = (char)32;
    Msg3  = (char)32;
    Msg4  = (char)32;
    Msg5  = (char)32;
    Msg6  = (char)32;
    Msg7  = (char)32;
    Msg8  = (char)32;
    Msg9  = (char)32;
    Msg10 = (char)32;
  }

  if (Msg2 == '\0') { // Para Cubrir los espacios vacios .... \n == a enter
    Msg2  = (char)32;
    Msg3  = (char)32;
    Msg4  = (char)32;
    Msg5  = (char)32;
    Msg6  = (char)32;
    Msg7  = (char)32;
    Msg8  = (char)32;
    Msg9  = (char)32;
    Msg10 = (char)32;
  }

  if (Msg3 == '\0') { // Para Cubrir los espacios vacios .... \n == a enter
    Msg3  = (char)32;
    Msg4  = (char)32;
    Msg5  = (char)32;
    Msg6  = (char)32;
    Msg7  = (char)32;
    Msg8  = (char)32;
    Msg9  = (char)32;
    Msg10 = (char)32;
  }

  if (Msg4 == '\0') { // Para Cubrir los espacios vacios .... \n == a enter
    Msg4  = (char)32;
    Msg5  = (char)32;
    Msg6  = (char)32;
    Msg7  = (char)32;
    Msg8  = (char)32;
    Msg9  = (char)32;
    Msg10 = (char)32;
  }

  if (Msg5 == '\0') { // Para Cubrir los espacios vacios .... \n == a enter
    Msg5  = (char)32;
    Msg6  = (char)32;
    Msg7  = (char)32;
    Msg8  = (char)32;
    Msg9  = (char)32;
    Msg10 = (char)32;
  }

  if (Msg6 == '\0') { // Para Cubrir los espacios vacios .... \n == a enter
    Msg6  = (char)32;
    Msg7  = (char)32;
    Msg8  = (char)32;
    Msg9  = (char)32;
    Msg10 = (char)32;
  }

  if (Msg7 == '\0') { // Para Cubrir los espacios vacios .... \n == a enter
    Msg7  = (char)32;
    Msg8  = (char)32;
    Msg9  = (char)32;
    Msg10 = (char)32;
  }

  if (Msg8 == '\0') { // Para Cubrir los espacios vacios .... \n == a enter
    Msg8  = (char)32;
    Msg9  = (char)32;
    Msg10 = (char)32;
  }

  if (Msg9 == '\0') { // Para Cubrir los espacios vacios .... \n == a enter
    Msg9  = (char)32;
    Msg10 = (char)32;
  }

  if (Msg10 == '\0') { // Para Cubrir los espacios vacios .... \n == a enter
    Msg10 = (char)32;
  }
}*/
///////////////////////////////////////////////////////////////
//------------------------------------------------------------------
//char respaldoTextoXbee1  = (char)32;
/////////////////////////////////
