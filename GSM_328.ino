//HOLA
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include <TimerOne.h>
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
#define CONFIG_GSM_RXPIN 3
#define CONFIG_GSM_TXPIN 2
#define CONFIG_GSM_BAUDRATE 9600
#define LED_AUX 9
#define IN1 A2
#define IN2 A3
#define RELE_1 4
#define RELE_2 5
#define RELE_3 6
#define RELE_4 7
#define ON    1
#define OFF   0

#define EE_DESTINATARIO_1  0
#define EE_DESTINATARIO_2  14
#define EE_DESTINATARIO_3  28
#define EE_DESTINATARIO_4  42
#define EE_DESTINATARIO_5  56
#define EE_DESTINATARIO_6  70
#define EE_DESTINATARIO_7  84
#define EE_DESTINATARIO_8  98
#define EE_DESTINATARIO_9  112
#define EE_DESTINATARIO_10 126
#define EE_CANTIDAD_DESTINOS 141
#define EE_TIEMPO_OUT1_1 142
#define EE_TIEMPO_OUT1_2 143
#define EE_TIEMPO_OUT2_1 144
#define EE_TIEMPO_OUT2_2 145
#define EE_TIEMPO_OUT3_1 146
#define EE_TIEMPO_OUT3_2 147
#define EE_TIEMPO_OUT4_1 148
#define EE_TIEMPO_OUT4_2 149
#define EE_OUT1_TEMPORIZADA 150
#define EE_OUT2_TEMPORIZADA 151
#define EE_OUT3_TEMPORIZADA 152
#define EE_OUT4_TEMPORIZADA 153

unsigned int Estado_Equipo;
unsigned int TEMPORIZADOR_OUT1;
unsigned int TEMPORIZADOR_OUT2;
unsigned int TEMPORIZADOR_OUT3;
unsigned int TEMPORIZADOR_OUT4;
unsigned int TIEMPO_OUT1;
unsigned int TIEMPO_OUT2;
unsigned int TIEMPO_OUT3;
unsigned int TIEMPO_OUT4;
unsigned char i;
unsigned int CANTIDAD_DESTINOS; 
unsigned char VALOR;  
char DESTINATARIO_1[15];  
unsigned char DESTINATARIO_2[15];  
unsigned char DESTINATARIO_3[15];  
unsigned char DESTINATARIO_4[15];  
unsigned char DESTINATARIO_5[15];  
unsigned char DESTINATARIO_6[15];  
unsigned char DESTINATARIO_7[15];  
unsigned char DESTINATARIO_8[15];  
unsigned char DESTINATARIO_9[15];  
unsigned char DESTINATARIO_10[15];  
char DESTINOS[10][15]; // 10 destinatarios con un maximo de 14 caracteres
char replybuffer[50];                    //este es un gran búfer para las respuestas
char fonaNotificationBuffer[30];          // 64 para notificaciones del FONA
char smsBuffer[20];
bool ESTADO_LED = 1;
bool ESTADO_ANTERIOR_IN1;
bool ESTADO_ANTERIOR_IN2;
bool PROGRAMACION=0;
bool bG_PrimeraEntrada;  
bool OUT1_TEMPORIZADA;
bool OUT2_TEMPORIZADA;
bool OUT3_TEMPORIZADA;
bool OUT4_TEMPORIZADA;
static char callerIDbuffer[20];     
String Recibir;
SoftwareSerial swseri = SoftwareSerial(CONFIG_GSM_RXPIN, CONFIG_GSM_TXPIN);
Adafruit_FONA fona = Adafruit_FONA(10);// OBJETO ADAFRUIT_FONA USADO PARA COMUNICARSE CON EL SIM800L
static char texto= "pruebaaa";
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);


void setup() 
{
  // PREPARAR PINES IO
  Serial.println(F("ARDUINO SE REINICIO"));
  pinMode(RELE_1, OUTPUT);  
  pinMode(RELE_2, OUTPUT);  
  pinMode(RELE_3, OUTPUT); 
  pinMode(RELE_4, OUTPUT);
  pinMode(IN1, INPUT_PULLUP);
  pinMode(IN2, INPUT_PULLUP);
  pinMode(LED_AUX, OUTPUT);
  
  swseri.begin(9600);
  Serial.begin(9600);
  
  Timer1.initialize(1000000);                   // Dispara cada 100 ms
  Timer1.attachInterrupt(Timer100ms);           // Activa la interrupcion y la asocia a Timer100ms 
  ESTADO_ANTERIOR_IN1 = HIGH;
  ESTADO_ANTERIOR_IN2 = HIGH;

  // INICIAR LA COMUNICACION CON EL MODULO GSM
  // PASAMOS REFERENCIA AL PUERTO SERIE USADO PARA LA COMUNICACION CON EL MODEM
  if (fona.begin(swseri))
    Serial.println(F("MODULO GSM OK"));
    else
    {
      Serial.println(F("NO SE ENCUENTRA MODULO GSM"));
      //while (1);
    }

//  if (!fona.sendSMS(DESTINATARIO_1, "SISTEMA ENCENDIDO")) // AQUI SE REALIZA EL ENVIO DEL MENSAJE SMS
//  {
//    Serial.println(F("ENVIADO"));
//  } else {
//    Serial.println(F("ERROR"));
//  }
int j;
for (j=0;j<21;j++)
{
fona.deleteSMS(j);
}
  swseri.print("AT+CNMI=2,1\r\n");  //configurar el FONA para enviar una notificación + CMTI cuando se reciba un SMS
  Serial.println("Equipo Listo ");
  bG_PrimeraEntrada=true;

}

void loop() 

{

 if(PROGRAMACION == false) // SI MODO PROGRAMACION ESTA DESACTIVADO ENTRA
  {
   if(bG_PrimeraEntrada == true)  // PRIMERA ENTRADA PARA INICIALIZAR VARIABLES 
    { 
     //wdt_disable();//DESACTIVO PERRO GUARDION
     InicializarVariables();
     bG_PrimeraEntrada = false;
    }

 
       if (Estado_Equipo >= 1) // ESTADO DEL EQUIPO CADA 1 SEGUNDOS CAMBIA ESTADO DE LED
         {
          Estado_Equipo=0;
          ESTADO_LED = !ESTADO_LED;
          digitalWrite( LED_AUX,ESTADO_LED);
         }


 if (digitalRead(IN1)==LOW && (ESTADO_ANTERIOR_IN1)== HIGH)
    {
        delay(10);
        
       if (digitalRead(IN1)==LOW && (ESTADO_ANTERIOR_IN1)== HIGH)
        {   

            for (i=0;i<CANTIDAD_DESTINOS;i++)
              {
              if (!fona.sendSMS(DESTINOS[i], "ENTRADA 1 ACTIVA")) 
              {
              Serial.println(F("ENVIADO"));
              } else {
               Serial.println(F("ERROR"));
              }
              } 
             ESTADO_ANTERIOR_IN1=LOW;
             
          }   
        
    } 

     if (digitalRead(IN1)==HIGH && (ESTADO_ANTERIOR_IN1)==LOW) 
        {    
          delay(10);
        if (digitalRead(IN1)==HIGH && (ESTADO_ANTERIOR_IN1)==LOW) 
           {     
          if (!fona.sendSMS(DESTINOS[i], "ENTRADA 1 DESACTIVA")) 
            {
            Serial.println(F("ENVIADO"));
            } else {
             Serial.println(F("ERROR"));
            }
            
             ESTADO_ANTERIOR_IN1=HIGH;
           }
         }

   if (digitalRead(IN2)==LOW && (ESTADO_ANTERIOR_IN2)== HIGH)
         {
           delay(10);
         if (digitalRead(IN2)==LOW && (ESTADO_ANTERIOR_IN2)== HIGH)
               { 
               for (i=0;i<CANTIDAD_DESTINOS;i++)
              {
                   
                  if (!fona.sendSMS(DESTINOS[i], "ENTRADA 2 ACTIVA")) 
                  {
                  Serial.println(F("ENVIADO"));
                  } else {
                   Serial.println(F("ERROR"));
                  }
                  delay(4000);
              }
             ESTADO_ANTERIOR_IN2=LOW;
            } 
          } 
     if (digitalRead(IN2)==HIGH && (ESTADO_ANTERIOR_IN2)==LOW)    
        {    
         delay(10);
        if (digitalRead(IN2)==HIGH && (ESTADO_ANTERIOR_IN2)==LOW) 
           {  
              for (i=0;i<CANTIDAD_DESTINOS;i++)
              {  
                if (!fona.sendSMS(DESTINOS[i], "ENTRADA 2 DESACTIVA")) 
                {
                Serial.println(F("ENVIADO"));
                } else {
                 Serial.println(F("ERROR"));
                }
                 delay(4000);
            }
              ESTADO_ANTERIOR_IN2=HIGH;
           }
        }

 
  if (Recibir == "OUT1A/VALOR")
  {
      char callerIDbuffer[20];
      int valor=analogRead(A1);
      char cstr[16];
      itoa(valor, cstr, 10);
        if (!fona.sendSMS(callerIDbuffer,cstr)) 
       {
        Serial.println(F("Confirmacion Enviada"));
        } else {
        Serial.println(F("No se envio confirmacion"));
        }  
    Recibir ="SALIR";    
  }

//    if (Recibir == "OUT2A/VALOR")
//  {
//      char callerIDbuffer[20];
//      int valor=analogRead(A0);
//      char cstr[16];
//      itoa(valor, cstr, 10);
//        if (!fona.sendSMS(callerIDbuffer,cstr)) 
//       {
//        Serial.println(F("Confirmacion Enviada"));
//        } else {
//        Serial.println(F("No se envio confirmacion"));
//        }  
//    Recibir ="SALIR";    
//  }
//  if (Recibir == "OUT3A/VALOR")
//  {
//      char callerIDbuffer[20];
//      int valor=analogRead(A7);
//      char cstr[16];
//      itoa(valor, cstr, 10);
//        if (!fona.sendSMS(callerIDbuffer,cstr)) 
//       {
//        Serial.println(F("Confirmacion Enviada"));
//        } else {
//        Serial.println(F("No se envio confirmacion"));
//        }  
//    Recibir ="SALIR";    
//  }
//
//    if (Recibir == "OUT4A/VALOR")
//  {
//      char callerIDbuffer[20];
//      int valor=analogRead(A6);
//      char cstr[16];
//      itoa(valor, cstr, 10);
//        if (!fona.sendSMS(callerIDbuffer,cstr)) 
//       {
//        Serial.println(F("Confirmacion Enviada"));
//        } else {
//        Serial.println(F("No se envio confirmacion"));
//        }  
//    Recibir ="SALIR";    
//  }
  else if (Recibir == "CONECTAR")
  {
   PROGRAMACION=true;
   //Serial.println(Recibir);  
   char callerIDbuffer[20];   
   if (!fona.sendSMS(callerIDbuffer, "CONECTADO OK")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }  
  // Enviar_Respuesta ("CONEXION OK");
  }
  else if (Recibir == "OUT1/ON" )
   {
   digitalWrite(RELE_1, HIGH);
   Serial.println(Recibir);
   TEMPORIZADOR_OUT1=0;
   char callerIDbuffer[20];   
   if (!fona.sendSMS(callerIDbuffer, "SALIDA1 ON")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }
   
   Recibir ="SALIR"; 
  }

   else if (Recibir == "OUT1/OFF" )
  {
   digitalWrite(RELE_1, LOW);
   //Serial.println(Recibir);
      char callerIDbuffer[20];   
   if (!fona.sendSMS(callerIDbuffer, "SALIDA1 OFF")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }
   Recibir ="SALIR"; 
  }
   else if (Recibir == "OUT2/ON" )
  {
   digitalWrite(RELE_2, HIGH);
   Serial.println(Recibir);
   char callerIDbuffer[20];
    if (!fona.sendSMS(callerIDbuffer, "SALIDA2 ON")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }
   TEMPORIZADOR_OUT2=0;
   Recibir ="SALIR"; 
  }

    else if (Recibir == "OUT2/OFF" )
  {
   digitalWrite(RELE_2, LOW);
   Serial.println(Recibir);
   char callerIDbuffer[20];
     if (!fona.sendSMS(callerIDbuffer, "SALIDA2 OFF")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }
   Recibir ="SALIR"; 
  }

   else if (Recibir == "OUT3/ON" )
  {
   digitalWrite(RELE_3, HIGH);
   Serial.println(Recibir);
   TEMPORIZADOR_OUT3=0;
    char callerIDbuffer[20];
     if (!fona.sendSMS(callerIDbuffer, "SALIDA3 ON")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }
   Recibir ="SALIR"; 
  }

   else  if (Recibir == "OUT3/OFF" )
  {
   digitalWrite(RELE_3, LOW);
   Serial.println(Recibir);
     char callerIDbuffer[20];
     if (!fona.sendSMS(callerIDbuffer, "SALIDA3 OFF")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }
   Recibir ="SALIR"; 
  }

   else  if (Recibir == "OUT4/ON" )
  {
   digitalWrite(RELE_4, HIGH);
   Serial.println(Recibir);
   TEMPORIZADOR_OUT4=0;
     char callerIDbuffer[20];
     if (!fona.sendSMS(callerIDbuffer, "SALIDA4 ON")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }
   Recibir ="SALIR"; 
  }
   else  if (Recibir == "OUT4/OFF" )
  {
   digitalWrite(RELE_4, LOW);
   Serial.println(Recibir);
     char callerIDbuffer[20];
     if (!fona.sendSMS(callerIDbuffer, "SALIDA4 OFF")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }
   Recibir ="SALIR"; 
  }
    else if (Recibir == "OUTS/ON" )
  {
   digitalWrite(RELE_1, HIGH);
   digitalWrite(RELE_2, HIGH);
   digitalWrite(RELE_3, HIGH);
   digitalWrite(RELE_4, HIGH);
   TEMPORIZADOR_OUT1=0;
   TEMPORIZADOR_OUT2=0;
   TEMPORIZADOR_OUT3=0;
   TEMPORIZADOR_OUT4=0;
    char callerIDbuffer[20];
     if (!fona.sendSMS(callerIDbuffer, "SALIDAS ON")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }
   Serial.println(Recibir);
   Recibir ="SALIR"; 
  }
   else  if (Recibir == "OUTS/OFF" )
  {
   digitalWrite(RELE_1, LOW);
   digitalWrite(RELE_2, LOW);
   digitalWrite(RELE_3, LOW);
   digitalWrite(RELE_4, LOW);
      char callerIDbuffer[20];
     if (!fona.sendSMS(callerIDbuffer, "SALIDAS OFF")) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }
   Serial.println(Recibir);
   Recibir ="SALIR"; 
  }

    else if (OUT1_TEMPORIZADA  == true && TEMPORIZADOR_OUT1 > TIEMPO_OUT1)
   {
   digitalWrite(RELE_1, LOW);
   TEMPORIZADOR_OUT1=0;
   Recibir ="SALIR"; 
   }

   else if (OUT2_TEMPORIZADA  == true && TEMPORIZADOR_OUT2 > TIEMPO_OUT2)
   {
   digitalWrite(RELE_2, LOW);
   TEMPORIZADOR_OUT2=0;
   Recibir ="SALIR"; 
   }

   else if (OUT3_TEMPORIZADA  == true && TEMPORIZADOR_OUT3 > TIEMPO_OUT3 )
   {
   digitalWrite(RELE_3, LOW);
   TEMPORIZADOR_OUT3=0;
   Recibir ="SALIR"; 
   }
  else  if (OUT4_TEMPORIZADA  == true && TEMPORIZADOR_OUT4 > TIEMPO_OUT4 )
   {
   digitalWrite(RELE_4, LOW);
   TEMPORIZADOR_OUT4=0;
   Recibir ="SALIR"; 
   }



RX();




}

 else
  {
    Serial.println("Modo Programacion");   
    Programacion(); //llama a la funcion programacion y se queda ahi esta que cambia PROGRAMACION = 0;
    bG_PrimeraEntrada = true; // primera entrada true para que vuelva a configurar las variables leidas de eeprom
    //wdt_enable(WDTO_15MS);
    delay(100);//activar DTW
   
  } 
 }



    void Timer100ms(void)
  {

     Estado_Equipo++;
     TEMPORIZADOR_OUT1++;
     TEMPORIZADOR_OUT2++;
     TEMPORIZADOR_OUT3++;
     TEMPORIZADOR_OUT4++;
  }



  void InicializarVariables(void)
{ 

r_eeprom(DESTINATARIO_1, EE_DESTINATARIO_1, 14);
r_eeprom(DESTINATARIO_2, EE_DESTINATARIO_2, 14);
r_eeprom(DESTINATARIO_3, EE_DESTINATARIO_3, 14);
r_eeprom(DESTINATARIO_4, EE_DESTINATARIO_4, 14);
r_eeprom(DESTINATARIO_5, EE_DESTINATARIO_5, 14);
r_eeprom(DESTINATARIO_6, EE_DESTINATARIO_6, 14);
r_eeprom(DESTINATARIO_7, EE_DESTINATARIO_7, 14);
r_eeprom(DESTINATARIO_8, EE_DESTINATARIO_8, 14);
r_eeprom(DESTINATARIO_9, EE_DESTINATARIO_9, 14);
r_eeprom(DESTINATARIO_10, EE_DESTINATARIO_10,14);
OUT1_TEMPORIZADA = EEPROM.read(EE_OUT1_TEMPORIZADA);
OUT2_TEMPORIZADA = EEPROM.read(EE_OUT2_TEMPORIZADA);
OUT3_TEMPORIZADA = EEPROM.read(EE_OUT3_TEMPORIZADA);
OUT4_TEMPORIZADA = EEPROM.read(EE_OUT4_TEMPORIZADA);

byte OUT1_TIEMPO_1 = EEPROM.read(EE_TIEMPO_OUT1_1);
byte OUT1_TIEMPO_2 = EEPROM.read(EE_TIEMPO_OUT1_2);
byte SUMA_OUT1_TIEMPO [2] = {OUT1_TIEMPO_2, OUT1_TIEMPO_1};
Serial.println(OUT1_TIEMPO_1,DEC);
Serial.println(OUT1_TIEMPO_2,DEC);
TIEMPO_OUT1 = ObtenerValor(SUMA_OUT1_TIEMPO, 0);

byte OUT2_TIEMPO_1 = EEPROM.read(EE_TIEMPO_OUT2_1);
byte OUT2_TIEMPO_2 = EEPROM.read(EE_TIEMPO_OUT2_2);
byte SUMA_OUT2_TIEMPO [2] = {OUT2_TIEMPO_2, OUT2_TIEMPO_1};
TIEMPO_OUT2 = ObtenerValor(SUMA_OUT2_TIEMPO, 0);

byte OUT3_TIEMPO_1 = EEPROM.read(EE_TIEMPO_OUT3_1);
byte OUT3_TIEMPO_2 = EEPROM.read(EE_TIEMPO_OUT3_2);
byte SUMA_OUT3_TIEMPO [2] = {OUT3_TIEMPO_2, OUT3_TIEMPO_1};
TIEMPO_OUT3 = ObtenerValor(SUMA_OUT3_TIEMPO, 0);

byte OUT4_TIEMPO_1 = EEPROM.read(EE_TIEMPO_OUT4_1);
byte OUT4_TIEMPO_2 = EEPROM.read(EE_TIEMPO_OUT4_2);
byte SUMA_OUT4_TIEMPO [2] = {OUT4_TIEMPO_2, OUT4_TIEMPO_1};
TIEMPO_OUT4 = ObtenerValor(SUMA_OUT4_TIEMPO, 0);

//Serial.println(TIEMPO_OUT1,DEC);
//Serial.println(OUT1_TEMPORIZADA,DEC);
//Serial.println(OUT2_TEMPORIZADA,DEC);
//Serial.println(OUT3_TEMPORIZADA,DEC);
//Serial.println(OUT4_TEMPORIZADA,DEC);
//
unsigned a,b,c,d,e,f,g,h,i;
//
//Serial.print(F("Destinatario 1:"));
//for (a=0;a<15;a++)
//{
// Serial.print(char(DESTINATARIO_1[a]));
//}
//Serial.println();
//Serial.print(F("Destinatario 2:"));
//for (b=0;b<15;b++)
//{
// Serial.print(char(DESTINATARIO_2[b]));
//}
//Serial.println();
//Serial.print(F("Destinatario 3:"));
//for (c=0;c<15;c++)
//{
// Serial.print(char(DESTINATARIO_3[c]));
//}
VALOR=EEPROM.read(EE_CANTIDAD_DESTINOS);
Serial.println();
Serial.println(VALOR,DEC);

CANTIDAD_DESTINOS = VALOR-48;

//Serial.println();
//
    for (d=0;d<14;d++)
    {
     DESTINOS[0][d]=DESTINATARIO_1[d];
     DESTINOS[1][d]=DESTINATARIO_2[d];
     DESTINOS[2][d]=DESTINATARIO_3[d];
     DESTINOS[3][d]=DESTINATARIO_4[d];
     DESTINOS[4][d]=DESTINATARIO_5[d];
     DESTINOS[5][d]=DESTINATARIO_6[d];
     DESTINOS[6][d]=DESTINATARIO_7[d];
     DESTINOS[7][d]=DESTINATARIO_8[d];
     DESTINOS[8][d]=DESTINATARIO_9[d];
     DESTINOS[9][d]=DESTINATARIO_10[d];
    }

 return;
  
}


void Programacion(void)
{
  while(PROGRAMACION == true)
  {

char* bufPtr = fonaNotificationBuffer;    //handy buffer pointer
  
  if (fona.available())      //¿Algún dato disponible del FONA?
  {
    byte slot = 0;            //este será el número de ranura del SMS
    int charCount = 0;
   // Leer la notificación en fonaInBuffer
    do  {
      *bufPtr = fona.read();
      Serial.write(*bufPtr);
      delay(1);
    } while ((*bufPtr++ != '\n') && (fona.available()) && (++charCount < (sizeof(fonaNotificationBuffer)-1)));
    
    // Agrega un terminal NULL a la cadena de notificación
    *bufPtr = 0;

    // Escanea la cadena de notificación en busca de una notificación recibida por SMS.
    // Si es un mensaje SMS, obtendremos el número de ranura en 'ranura'
    
    if (1 == sscanf(fonaNotificationBuffer, "+CMTI: " FONA_PREF_SMS_STORAGE ",%d", &slot)) 
    {
      Serial.print("slot: "); Serial.println(slot);
      
      //char callerIDbuffer[20];  // almacenaremos el número del remitente del SMS aquí
 
      if (! fona.getSMSSender(slot, callerIDbuffer, 20)) // Recuperar la dirección / número de teléfono del remitente de SMS.
      {
        Serial.println(F("Didn't find SMS message in slot!"));
      }
         Serial.print(F("FROM: ")); Serial.println(callerIDbuffer); // F Siginfica guardar cadena en memoria flash
     
          uint16_t smslen; // Recuperar valor de SMS.
         if (fona.readSMS(slot, smsBuffer, 20, &smslen)) // ¡pasa el búfer y el máximo len!
         { 
          Serial.println(smsBuffer);
          Recibir = smsBuffer;              
         }
       
        if (fona.deleteSMS(slot)) //eliminar el mensaje original después de que se procese
      {
        delay(100);
        Serial.println(F("MENSAJE ELIMINADO"));
      } else {
        int i;
        for (i =0; i < 2; i++) fona.deleteSMS(slot);
        Serial.print(F("NO SE PUDO ELIMINAR EL MENSAJE")); Serial.println(slot);
        fona.print(F("AT+CMGD=?\r\n"));
      }
     

    }
  }
  

          if (Recibir.startsWith("U", 0)) // registrar cantidad de destinatarios
            {
              Recibir.remove(0, 1);              
              EEPROM.write(141,Recibir[0]);                          
              //Serial.println("Cantidad de Destinos");
              //Serial.println(Recibir);
              Recibir= "SALIR";
              delay(1000);
            }
          
         else if (Recibir.startsWith("1", 0)) // registrar destino 1
          {
              Recibir.remove(0, 1); 
              // w_eeprom(EE_DESTINATARIO_1 ,Recibir, 14);
              for(i=0;i<14;i++)
              {             
                EEPROM.write(i,Recibir[i]);            
              }
              // Serial.println("Destino 1 Registrado OK");
              //Serial.println(Recibir);
              delay(1000);
              }

         else if (Recibir.startsWith("2", 0)) // registrar destino 2
          {
              Recibir.remove(0, 1); 
                for(i=14;i<28;i++)
              {             
                EEPROM.write(i,Recibir[i-14]);            
              }
              //Serial.println("Destino 2 Registrado OK");
              //Serial.println(Recibir);
              delay(1000);
              }
         else  if (Recibir.startsWith("3", 0)) // registrar destino 3
          {
                Recibir.remove(0, 1); 
                for(i=28;i<42;i++)
              {             
                EEPROM.write(i,Recibir[i-28]);            
              }
              //Serial.println("Destino 3 Registrado OK");
              //Serial.println(Recibir);
              delay(1000);
              }

          else if (Recibir.startsWith("4", 0)) // registrar destino 4
          {
                Recibir.remove(0, 1); 
                for(i=42;i<56;i++)
              {             
                EEPROM.write(i,Recibir[i-42]);            
              }
              //Serial.println("Destino 4 Registrado OK");
              //Serial.println(Recibir);
              delay(1000);
              }
         else if (Recibir.startsWith("5", 0)) // registrar destino 5
          {
                Recibir.remove(0, 1); 
                for(i=56;i<70;i++)
              {             
                EEPROM.write(i,Recibir[i-56]);            
              }
              //Serial.println("Destino 5 Registrado OK");
              //Serial.println(Recibir);
              delay(1000);
              }

          else if (Recibir.startsWith("6", 0)) // registrar destino 6
              {
                Recibir.remove(0, 1); 
                for(i=70;i<84;i++)
              {             
                EEPROM.write(i,Recibir[i-70]);         
              }
              //Serial.println("Destino 6 Registrado OK");
              //Serial.println(Recibir);
              delay(1000);
              }
           else if (Recibir.startsWith("7", 0)) // registrar destino 7
             {
                Recibir.remove(0, 1); 
                for(i=84;i<98;i++)
              {             
                EEPROM.write(i,Recibir[i-84]);            
              }
                //Serial.println("Destino 7 Registrado OK");
                //Serial.println(Recibir);
                delay(1000);
                }

          else if (Recibir.startsWith("8", 0)) // registrar destino 8
              {
                Recibir.remove(0, 1); 
                for(i=98;i<112;i++)
              {             
                EEPROM.write(i,Recibir[i-98]);            
              }
              //Serial.println("Destino 8 Registrado OK");
              //Serial.println(Recibir);
              delay(1000);
              }
          else  if (Recibir.startsWith("9", 0)) // registrar destino 9
             {
                Recibir.remove(0, 1); 
                for(i=112;i<126;i++)
              {             
                EEPROM.write(i,Recibir[i-112]);            
              }
              //Serial.println("Destino 9 Registrado OK");
              //Serial.println(Recibir);
              delay(1000);
              }

          else if (Recibir.startsWith("0", 0)) // registrar destino 10
              {
                Recibir.remove(0, 1); 
                for(i=126;i<140;i++)
              {             
                EEPROM.write(i,Recibir[i-126]);            
              }
              //Serial.println("Destino 10 Registrado OK");
              //Serial.println(Recibir);
              delay(1000);
              }
            else if (Recibir == "OUT1/TEMP/ON")
              {
              EEPROM.write(EE_OUT1_TEMPORIZADA, 1); 
              if (!fona.sendSMS(callerIDbuffer, "OUT1 TEMPORIZADA")) // Enviar una respuesta automática
                 {
                  Serial.println(F("Confirmacion Enviada"));
                  } else {
                  Serial.println(F("No se envio confirmacion"));
                  } 
              Recibir = "SALIR";               
              }
             else if (Recibir == "OUT1/TEMP/OFF")
              {
              EEPROM.write(EE_OUT1_TEMPORIZADA, 0);
              Recibir = "SALIR"; 
              }
            else if (Recibir == "OUT2/TEMP/ON")
              {
              EEPROM.write(EE_OUT2_TEMPORIZADA, 1);
              if (!fona.sendSMS(callerIDbuffer, "OUT2 TEMPORIZADA")) // Enviar una respuesta automática
                 {
                  Serial.println(F("Confirmacion Enviada"));
                  } else {
                  Serial.println(F("No se envio confirmacion"));
                  } 
              Recibir = "SALIR"; 
              }
             else if (Recibir == "OUT2/TEMP/OFF")
              {
              EEPROM.write(EE_OUT2_TEMPORIZADA, 0);
              Recibir = "SALIR"; 
              }
              if (Recibir == "OUT3/TEMP/ON")
              {
              EEPROM.write(EE_OUT3_TEMPORIZADA, 1);
              if (!fona.sendSMS(callerIDbuffer, "OUT3 TEMPORIZADA")) // Enviar una respuesta automática
                 {
                  Serial.println(F("Confirmacion Enviada"));
                  } else {
                  Serial.println(F("No se envio confirmacion"));
                  } 
              Recibir = "SALIR"; 
              }
           else  if (Recibir == "OUT3/TEMP/OFF")
              {
              EEPROM.write(EE_OUT3_TEMPORIZADA, 0);
              Recibir ="SALIR"; 
              }
           else if (Recibir == "OUT4/TEMP/ON")
              {
              EEPROM.write(EE_OUT4_TEMPORIZADA, 1);
              if (!fona.sendSMS(callerIDbuffer, "OUT4 TEMPORIZADA")) // Enviar una respuesta automática
                 {
                  Serial.println(F("Confirmacion Enviada"));
                  } else {
                  Serial.println(F("No se envio confirmacion"));
                  } 
              Recibir = "SALIR"; 
              }
           else if (Recibir == "OUT4/TEMP/OFF")
              {
              EEPROM.write(EE_OUT4_TEMPORIZADA, 0);
              Recibir = "SALIR"; 
              }

          else if (Recibir.startsWith("T1/", 0)) // Tiempo de temporizado de la salida 1
            {
              Recibir.remove(0, 3);
              unsigned int OUT1_TIEMPO;
              OUT1_TIEMPO = Recibir.toInt();            
              Serial.println(OUT1_TIEMPO);
              byte OUT1_1;
              byte OUT1_2;
              OUT1_1 = OUT1_TIEMPO & 0XFF;
              OUT1_2 = OUT1_TIEMPO >>8&0XFF;
              EEPROM.write(EE_TIEMPO_OUT1_1, OUT1_1);
              EEPROM.write(EE_TIEMPO_OUT1_2, OUT1_2);
              Recibir= "SALIR";
              delay(1000);
            }
            
          else  if (Recibir.startsWith("T2/", 0)) // Tiempo de temporizado de la salida 2
            {
              Recibir.remove(0, 3);
              unsigned int OUT2_TIEMPO;
              OUT2_TIEMPO = Recibir.toInt();            
              Serial.println(OUT2_TIEMPO);
              byte OUT2_1;
              byte OUT2_2;
              OUT2_1 = OUT2_TIEMPO & 0XFF;
              OUT2_2 = OUT2_TIEMPO >>8&0XFF;
              EEPROM.write(EE_TIEMPO_OUT2_1, OUT2_1);
              EEPROM.write(EE_TIEMPO_OUT2_2, OUT2_2);
              Recibir= "SALIR";
              delay(1000);
            }

           else  if (Recibir.startsWith("T3/", 0)) // Tiempo de temporizado de la salida 3
            {
              Recibir.remove(0, 3);
              unsigned int OUT3_TIEMPO;
              OUT3_TIEMPO = Recibir.toInt();            
              Serial.println(OUT3_TIEMPO);
              byte OUT3_1;
              byte OUT3_2;
              OUT3_1 = OUT3_TIEMPO & 0XFF;
              OUT3_2 = OUT3_TIEMPO >>8&0XFF;
              EEPROM.write(EE_TIEMPO_OUT3_1, OUT3_1);
              EEPROM.write(EE_TIEMPO_OUT3_2, OUT3_2);
              Recibir= "SALIR";
              delay(1000);
            }

             else if (Recibir.startsWith("T4/", 0)) // Tiempo de temporizado de la salida 4
            {
              Recibir.remove(0, 3);
              unsigned int OUT4_TIEMPO;
              OUT4_TIEMPO = Recibir.toInt();            
              Serial.println(OUT4_TIEMPO);
              byte OUT4_1;
              byte OUT4_2;
              OUT4_1 = OUT4_TIEMPO & 0XFF;
              OUT4_2 = OUT4_TIEMPO >>8&0XFF;
              EEPROM.write(EE_TIEMPO_OUT4_1, OUT4_1);
              EEPROM.write(EE_TIEMPO_OUT4_2, OUT4_2);
              Recibir= "SALIR";
              delay(1000);
            }

            if (Recibir == "DESCONECTAR")
              {
                 
                 Serial.println(Recibir);  
                 if (!fona.sendSMS(callerIDbuffer, "DESCONECTADO OK"))
                 {
                  Serial.println(F("Confirmacion Enviada"));
                  } else {
                  Serial.println(F("No se envio confirmacion"));
                  }  
                  PROGRAMACION=false;
                  Recibir = "SALIR"; 
              }
      


  }
}

void w_eeprom(unsigned char DirEE, unsigned char *pDato, unsigned char cL_CantidadDeBytes)
{
  static unsigned char i;
  for(i=0; i<cL_CantidadDeBytes;i++)
  {
    EEPROM.update(DirEE+i, *(pDato+i));
  }
  return;
}
void r_eeprom(unsigned char *pDato, unsigned char DirEE, unsigned char cL_CantidadDeBytes)
{
  static unsigned char i;
  for(i=0; i<cL_CantidadDeBytes;i++)
  {
    *(pDato+i)=EEPROM.read(DirEE+i);
  }
  return;
}



unsigned int ObtenerValor(byte paquete [], byte i)
{

  unsigned int valor;
  valor = (valor * 256)+ paquete [i];
  valor = (valor * 256)+ paquete [i+1];
  return valor;
}

void RX (void)
{

 char* bufPtr = fonaNotificationBuffer;    //handy buffer pointer
  
  if (fona.available())      //¿Algún dato disponible del FONA?
  {
    byte slot = 0;            //este será el número de ranura del SMS
    int charCount = 0;
   // Leer la notificación en fonaInBuffer
    do  {
      *bufPtr = fona.read();
      Serial.write(*bufPtr);
      delay(1);
    } while ((*bufPtr++ != '\n') && (fona.available()) && (++charCount < (sizeof(fonaNotificationBuffer)-1)));
    
    // Agrega un terminal NULL a la cadena de notificación
    *bufPtr = 0;

    // Escanea la cadena de notificación en busca de una notificación recibida por SMS.
    // Si es un mensaje SMS, obtendremos el número de ranura en 'ranura'
    
    if (1 == sscanf(fonaNotificationBuffer, "+CMTI: " FONA_PREF_SMS_STORAGE ",%d", &slot)) 
    {
      Serial.print("slot: "); Serial.println(slot);
      
      char callerIDbuffer[15];  // almacenaremos el número del remitente del SMS aquí


 
      if (! fona.getSMSSender(slot, callerIDbuffer, 15)) // Recuperar la dirección / número de teléfono del remitente de SMS.
      {
        Serial.println(F("Didn't find SMS message in slot!"));
      }
         Serial.print(F("FROM: ")); Serial.println(callerIDbuffer); // F Siginfica guardar cadena en memoria flash


     
          uint16_t smslen; // Recuperar valor de SMS.
         if (fona.readSMS(slot, smsBuffer, 20, &smslen)) // ¡pasa el búfer y el máximo len!
         { 
          Serial.println(smsBuffer);
          Recibir = smsBuffer;              
         }
       
        if (fona.deleteSMS(slot)) //eliminar el mensaje original después de que se procese
      {
        delay(100);
        Serial.println(F("MENSAJE ELIMINADO"));
      } else {
        int i;
        for (i =0; i < 2; i++) fona.deleteSMS(slot);
        Serial.print(F("NO SE PUDO ELIMINAR EL MENSAJE")); Serial.println(slot);
        fona.print(F("AT+CMGD=?\r\n"));
      }
     

    }
  }
  

}
