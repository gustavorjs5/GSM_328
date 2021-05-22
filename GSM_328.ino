
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include <TimerOne.h>
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
#include <avr/pgmspace.h> 
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
#define EE_CLAVE                154
#define EE_TEXTO_IN1_ON         158
#define EE_TEXTO_IN1_OFF        198
#define EE_TEXTO_IN2_ON         238
#define EE_TEXTO_IN2_OFF        278
#define EE_LONGITUD_IN1_ON        318
#define EE_LONGITUD_IN1_OFF       319
#define EE_LONGITUD_IN2_ON        320
#define EE_LONGITUD_IN2_OFF       321

static unsigned char LONG_TXT_IN1_ON;
static unsigned char LONG_TXT_IN1_OFF;
static unsigned char LONG_TXT_IN2_ON;
static unsigned char LONG_TXT_IN2_OFF;
unsigned char Estado_Equipo;
unsigned int TEMPORIZADOR_OUT1;
unsigned int TEMPORIZADOR_OUT2;
unsigned int TEMPORIZADOR_OUT3;
unsigned int TEMPORIZADOR_OUT4;
unsigned int TIEMPO_OUT1;
unsigned int TIEMPO_OUT2;
unsigned int TIEMPO_OUT3;
unsigned int TIEMPO_OUT4;
unsigned char i;
unsigned char CANTIDAD_DESTINOS; 
unsigned char VALOR;  
unsigned char ESTADO_SALIDAS;
unsigned char ESTADO_ENTRADAS;
unsigned char DESTINATARIO_1[15];  
unsigned char DESTINATARIO_2[15];  
unsigned char DESTINATARIO_3[15];  
unsigned char DESTINATARIO_4[15];  
unsigned char DESTINATARIO_5[15];  
unsigned char DESTINATARIO_6[15];  
unsigned char DESTINATARIO_7[15];  
unsigned char DESTINATARIO_8[15];  
unsigned char DESTINATARIO_9[15];  
unsigned char DESTINATARIO_10[15];
unsigned char CLAVE[5];
unsigned char IN1_TXT_ON [40];
unsigned char IN1_TXT_OFF[40];
unsigned char IN2_TXT_ON [40];
unsigned char IN2_TXT_OFF[40];
String CLAVE_STRING;
String IN1_TXT_ON_STRING;
String IN1_TXT_OFF_STRING;
String IN2_TXT_ON_STRING;
String IN2_TXT_OFF_STRING;
char DESTINOS[10][15]; // 10 destinatarios con un maximo de 14 caracteres
char replybuffer[20];                    //este es un gran búfer para las respuestas
char fonaNotificationBuffer[30];          // 64 para notificaciones del FONA
char smsBuffer[60];
bool ESTADO_LED = 1;
bool ESTADO_ANTERIOR_IN1;
bool ESTADO_ANTERIOR_IN2;
bool PROGRAMACION=0;
bool bG_PrimeraEntrada;  
bool OUT1_TEMPORIZADA;
bool OUT2_TEMPORIZADA;
bool OUT3_TEMPORIZADA;
bool OUT4_TEMPORIZADA;
bool Respuesta_CMD = false;
static char callerIDbuffer[15];     
String static Recibir;
const  char OUT1ON                     [] PROGMEM = "OUT1/ON/";
const  char OUT1OFF                    [] PROGMEM = "OUT1/OFF/";
const  char OUT2ON                     [] PROGMEM = "OUT2/ON/";
const  char OUT2OFF                    [] PROGMEM = "OUT2/OFF/";
const  char OUT3ON                     [] PROGMEM = "OUT3/ON/";
const  char OUT3OFF                    [] PROGMEM = "OUT3/OFF/";
const unsigned char RESP_CONEXION      [] PROGMEM = "CNTOK";
const unsigned char RESP_DESCONEXION   [] PROGMEM = "DCNTOK";
const unsigned char CONECTAR           [] PROGMEM = "CONECTAR";
const unsigned char DESCONECTAR        [] PROGMEM = "DESCONECTAR";
const unsigned char CONSULTA_INA1      [] PROGMEM = "INA1/VALOR";
const unsigned char CONSULTA_INA2      [] PROGMEM = "INA2/VALOR";
const unsigned char OUT1_TEMP_ON       [] PROGMEM = "OUT1/TEMP/ON";
const unsigned char OUT2_TEMP_ON       [] PROGMEM = "OUT2/TEMP/ON";
const unsigned char OUT3_TEMP_ON       [] PROGMEM = "OUT3/TEMP/ON";
const unsigned char OUT1_TEMP_OFF      [] PROGMEM = "OUT1/TEMP/OFF";
const unsigned char OUT2_TEMP_OFF      [] PROGMEM = "OUT2/TEMP/OFF";
const unsigned char OUT3_TEMP_OFF      [] PROGMEM = "OUT3/TEMP/OFF";
const unsigned char RESP_OUT1_TEMP_ON  [] PROGMEM = "S1TONOK";
const unsigned char RESP_OUT2_TEMP_ON  [] PROGMEM = "S2TONOK";
const unsigned char RESP_OUT3_TEMP_ON  [] PROGMEM = "S3TONOK";
const unsigned char RESP_OUT1_TEMP_OFF [] PROGMEM = "S1TOFFOK";
const unsigned char RESP_OUT2_TEMP_OFF [] PROGMEM = "S2TOFFOK";
const unsigned char RESP_OUT3_TEMP_OFF [] PROGMEM = "S3TOFFOK";
const unsigned char RESP_OUT1_ON       [] PROGMEM = "S1ONOK";
const unsigned char RESP_OUT2_ON       [] PROGMEM = "S2ONOK";
const unsigned char RESP_OUT3_ON       [] PROGMEM = "S3ONOK";
const unsigned char RESP_OUT1_OFF      [] PROGMEM = "S1OFFOK";
const unsigned char RESP_OUT2_OFF      [] PROGMEM = "S2OFOK";
const unsigned char RESP_OUT3_OFF      [] PROGMEM = "S3OFFOK";
const unsigned char RESP_OUTS_ON       [] PROGMEM = "SONOK";
const unsigned char RESP_OUTS_OFF      [] PROGMEM = "SOFFOK";
const unsigned char OUTS_ON            [] PROGMEM = "OUTS/ON/";
const unsigned char OUTS_OFF           [] PROGMEM = "OUTS/OFF/";
const unsigned char OUT1_TIEMPO        [] PROGMEM = "S1TOK";
const unsigned char OUT2_TIEMPO        [] PROGMEM = "S2TOK";
const unsigned char OUT3_TIEMPO        [] PROGMEM = "S3TOK";
const unsigned char CLAVE_CONFIG       [] PROGMEM = "CLAVEOK";
const unsigned char CANTIDAD_USUARIOS  [] PROGMEM = "DESTINOSOK";

SoftwareSerial swseri = SoftwareSerial(CONFIG_GSM_RXPIN, CONFIG_GSM_TXPIN);
Adafruit_FONA fona = Adafruit_FONA(10);// OBJETO ADAFRUIT_FONA USADO PARA COMUNICARSE CON EL SIM800L
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

const char *const string_table[] PROGMEM = {OUT1ON, OUT1OFF, OUT2ON, OUT2OFF, OUT3ON, OUT3OFF, RESP_CONEXION, RESP_DESCONEXION, CONECTAR,DESCONECTAR, CONSULTA_INA1, 
CONSULTA_INA2, OUT1_TEMP_ON, OUT2_TEMP_ON, OUT3_TEMP_ON, OUT1_TEMP_OFF, OUT2_TEMP_OFF, OUT3_TEMP_OFF, RESP_OUT1_TEMP_ON, RESP_OUT2_TEMP_ON, RESP_OUT3_TEMP_ON, 
RESP_OUT1_TEMP_OFF, RESP_OUT2_TEMP_OFF, RESP_OUT3_TEMP_OFF, RESP_OUT1_ON, RESP_OUT2_ON, RESP_OUT3_ON, RESP_OUT1_OFF, RESP_OUT2_OFF, RESP_OUT3_OFF, RESP_OUTS_ON, 
RESP_OUTS_OFF, OUTS_ON, OUTS_OFF, OUT1_TIEMPO, OUT2_TIEMPO, OUT3_TIEMPO, CLAVE_CONFIG, CANTIDAD_USUARIOS};


char buffer[25];  // make sure this is large enough for the largest string it must hold

void setup() 
{

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
  if (fona.begin(swseri))
    Serial.println(F("MODULO GSM OK"));
    else
    {
      Serial.println(F("NO SE ENCUENTRA MODULO GSM"));
      //while (1);
    }

    unsigned char b;
    for (b=0;b<21;b++)
    {
    fona.deleteSMS(b);
    }
      swseri.print("AT+CNMI=2,1\r\n");  //configurar el FONA para enviar una notificación + CMTI cuando se reciba un SMS
      bG_PrimeraEntrada=true;

}

void loop() 

{

 if(PROGRAMACION == false) // SI MODO PROGRAMACION ESTA DESACTIVADO ENTRA
  {
   if(bG_PrimeraEntrada == true)  // PRIMERA ENTRADA PARA INICIALIZAR VARIABLES 
    { 
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
           bitWrite(ESTADO_ENTRADAS, 0, 1);

            for (i=0;i<CANTIDAD_DESTINOS;i++)
              {
              if (!fona.sendSMS(DESTINOS[i], IN1_TXT_ON)) 
              {
              Serial.println(F("ENVIADO"));
              } else {
               Serial.println(F("ERROR"));
              }
              delay(5000);
              } 
               ESTADO_ANTERIOR_IN1=LOW;
             
          }   
        
    } 

     if (digitalRead(IN1)==HIGH && (ESTADO_ANTERIOR_IN1)==LOW) 
        {    
          delay(10);
        if (digitalRead(IN1)==HIGH && (ESTADO_ANTERIOR_IN1)==LOW) 
           {   
             bitWrite(ESTADO_ENTRADAS, 0, 1);

               for (i=0;i<CANTIDAD_DESTINOS;i++)
              {
                if (!fona.sendSMS(DESTINOS[i], IN1_TXT_OFF)) 
                  {
                  Serial.println(F("ENVIADO"));
                  } else {
                   Serial.println(F("ERROR"));
                  }
                  delay(5000);
             }
             ESTADO_ANTERIOR_IN1=HIGH;
           }
         }

   if (digitalRead(IN2)==LOW && (ESTADO_ANTERIOR_IN2)== HIGH)
         {
           delay(10);
         if (digitalRead(IN2)==LOW && (ESTADO_ANTERIOR_IN2)== HIGH)
               { 
                 bitWrite(ESTADO_ENTRADAS, 1, 1);
               for (i=0;i<CANTIDAD_DESTINOS;i++)
              {
                   
                  if (!fona.sendSMS(DESTINOS[i], IN2_TXT_ON)) 
                  {
                  Serial.println(F("ENVIADO"));
                  } else {
                   Serial.println(F("ERROR"));
                  }
                  delay(5000);
              }
             ESTADO_ANTERIOR_IN2=LOW;
            } 
          } 
     if (digitalRead(IN2)==HIGH && (ESTADO_ANTERIOR_IN2)==LOW)    
        {    
         delay(10);
        if (digitalRead(IN2)==HIGH && (ESTADO_ANTERIOR_IN2)==LOW) 
           {  
               bitWrite(ESTADO_ENTRADAS, 1, 0);
              for (i=0;i<CANTIDAD_DESTINOS;i++)
              {  
                if (!fona.sendSMS(DESTINOS[i], IN2_TXT_OFF)) 
                {
                Serial.println(F("ENVIADO"));
                } else {
                 Serial.println(F("ERROR"));
                }
                 delay(5000);
            }
              ESTADO_ANTERIOR_IN2=HIGH;
           }
        }

 
  if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[10]))))
  {
        char Concatenacion_1 [15];
        char Texto_1[]="INA1";
        int  Valor_1=analogRead(A1);
        sprintf(Concatenacion_1,"%s = %i",Texto_1,Valor_1);
        fona.sendSMS(callerIDbuffer,Concatenacion_1);
        Recibir ="SALIR";    
  }

    if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[11]))))
    {
        char Concatenacion_2 [15];
        char Texto_2[]="INA2";
        int  Valor_2=analogRead(A0);
        sprintf(Concatenacion_2,"%s = %i",Texto_2,Valor_2);
        fona.sendSMS(callerIDbuffer,Concatenacion_2); 
        Recibir ="SALIR";    
  }

   if (Recibir == "S/EST")
    {
        char Concatenacion_3 [15];
        char Texto_3[]="SA";
        sprintf(Concatenacion_3,"%s=%i",Texto_3,ESTADO_SALIDAS);
        fona.sendSMS(callerIDbuffer,Concatenacion_3); 
        Recibir ="SALIR";    
  }

     if (Recibir == "E/EST")
    {
        char Concatenacion_4 [15];
        char Texto_4[]="EN";
        sprintf(Concatenacion_4,"%s=%i",Texto_4,ESTADO_ENTRADAS);
        fona.sendSMS(callerIDbuffer,Concatenacion_4);   
        Recibir ="SALIR";    
  }

  else if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[8]))))
  {
   PROGRAMACION=true;   
   if (!fona.sendSMS(callerIDbuffer, strcpy_P(buffer, (char *)pgm_read_word(&(string_table[6]))))) // Enviar una respuesta automática
   {
    Serial.println(F("Confirmacion Enviada"));
    } else {
    Serial.println(F("No se envio confirmacion"));
    }  
    Recibir ="SALIR"; 
  }
  else if (Recibir ==  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[0])))+CLAVE_STRING)
   {
     digitalWrite(RELE_1, HIGH);
      bitWrite(ESTADO_SALIDAS, 0, 1);
     TEMPORIZADOR_OUT1=0;
     fona.sendSMS(callerIDbuffer, strcpy_P(buffer, (char *)pgm_read_word(&(string_table[24]))));
   
   Recibir ="SALIR"; 
  }

   else if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[1])))+CLAVE_STRING)
  {
   digitalWrite(RELE_1, LOW);
   bitWrite(ESTADO_SALIDAS, 0, 0);
   fona.sendSMS(callerIDbuffer, strcpy_P(buffer, (char *)pgm_read_word(&(string_table[27]))));

   Recibir ="SALIR"; 
  }
   else if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[2])))+CLAVE_STRING)
  {
   digitalWrite(RELE_2, HIGH);
   bitWrite(ESTADO_SALIDAS, 1, 1);
   Serial.println(Recibir);
   fona.sendSMS(callerIDbuffer, strcpy_P(buffer, (char *)pgm_read_word(&(string_table[25]))));

   TEMPORIZADOR_OUT2=0;
   Recibir ="SALIR"; 
  }

    else if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[3])))+CLAVE_STRING)
  {
   digitalWrite(RELE_2, LOW);
   bitWrite(ESTADO_SALIDAS, 1, 0);
   Serial.println(Recibir);
   fona.sendSMS(callerIDbuffer, strcpy_P(buffer, (char *)pgm_read_word(&(string_table[28]))));

   Recibir ="SALIR"; 
  }

   else if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[4])))+CLAVE_STRING)
  {
   digitalWrite(RELE_3, HIGH);
   bitWrite(ESTADO_SALIDAS, 2, 1);
   Serial.println(Recibir);
   TEMPORIZADOR_OUT3=0;
   fona.sendSMS(callerIDbuffer, strcpy_P(buffer, (char *)pgm_read_word(&(string_table[26]))));
 
   Recibir ="SALIR"; 
  }

   else  if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[5])))+ CLAVE_STRING)
  {
   digitalWrite(RELE_3, LOW);
   bitWrite(ESTADO_SALIDAS, 2, 0);
   Serial.println(Recibir);
   fona.sendSMS(callerIDbuffer, strcpy_P(buffer, (char *)pgm_read_word(&(string_table[29])))); 
   Recibir ="SALIR"; 
  }

    else if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[32])))+CLAVE_STRING)
  {
   digitalWrite(RELE_1, HIGH);
   digitalWrite(RELE_2, HIGH);
   digitalWrite(RELE_3, HIGH);
   digitalWrite(RELE_4, HIGH);
   ESTADO_SALIDAS = 7;
   TEMPORIZADOR_OUT1=0;
   TEMPORIZADOR_OUT2=0;
   TEMPORIZADOR_OUT3=0;
   TEMPORIZADOR_OUT4=0;
   fona.sendSMS(callerIDbuffer, strcpy_P(buffer, (char *)pgm_read_word(&(string_table[30]))));
   Recibir ="SALIR"; 
  }
   else  if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[33])))+CLAVE_STRING)
  {
       digitalWrite(RELE_1, LOW);
       digitalWrite(RELE_2, LOW);
       digitalWrite(RELE_3, LOW);
       digitalWrite(RELE_4, LOW);
       ESTADO_SALIDAS = 0;
       fona.sendSMS(callerIDbuffer, strcpy_P(buffer, (char *)pgm_read_word(&(string_table[31]))));
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
        unsigned char i;
        for (i =0; i < 2; i++) fona.deleteSMS(slot);
        Serial.print(F("NO SE PUDO ELIMINAR EL MENSAJE")); Serial.println(slot);
        fona.print(F("AT+CMGD=?\r\n"));
      }
     
    }
  }

}

   else
    {
      Serial.println("Modo Programacion");   
      Programacion(); //llama a la funcion programacion y se queda ahi esta que cambia PROGRAMACION = 0;
      bG_PrimeraEntrada = true; // primera entrada true para que vuelva a configurar las variables leidas de eeprom 
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
    r_eeprom(DESTINATARIO_10,EE_DESTINATARIO_10,14);

     unsigned char d;
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

    r_eeprom(CLAVE, EE_CLAVE,4);
    CLAVE[4]=0;//le agrego un NULL para convertirlo en string
    CLAVE_STRING = CLAVE;
    
    unsigned char Longitud_IN1_ON;
    Longitud_IN1_ON = EEPROM.read(EE_LONGITUD_IN1_ON );
    r_eeprom(IN1_TXT_ON, EE_TEXTO_IN1_ON, Longitud_IN1_ON );
    IN1_TXT_ON [Longitud_IN1_ON + 1] = 0 ; //NULL
    IN1_TXT_ON_STRING = IN1_TXT_ON;
    
    unsigned char Longitud_IN1_OFF;
    Longitud_IN1_OFF = EEPROM.read(EE_LONGITUD_IN1_OFF );
    r_eeprom(IN1_TXT_OFF,EE_TEXTO_IN1_OFF,Longitud_IN1_OFF);
    IN1_TXT_OFF [Longitud_IN1_OFF + 1]=0; //NULL
    IN1_TXT_OFF_STRING = IN1_TXT_OFF;
    
    unsigned char Longitud_IN2_ON;
    Longitud_IN2_ON = EEPROM.read(EE_LONGITUD_IN2_ON );
    r_eeprom(IN2_TXT_ON,EE_TEXTO_IN2_ON,Longitud_IN2_ON);
    IN2_TXT_ON [Longitud_IN2_ON + 1]=0; //NULL
    IN2_TXT_ON_STRING = IN2_TXT_ON;
    
    unsigned char Longitud_IN2_OFF;
    Longitud_IN2_OFF = EEPROM.read(EE_LONGITUD_IN2_OFF );
    r_eeprom(IN2_TXT_OFF,EE_TEXTO_IN2_OFF,19);
    IN2_TXT_OFF [Longitud_IN2_OFF + 1]=0; //NULL
    IN2_TXT_OFF_STRING = IN2_TXT_OFF;
    
    OUT1_TEMPORIZADA = EEPROM.read(EE_OUT1_TEMPORIZADA);
    OUT2_TEMPORIZADA = EEPROM.read(EE_OUT2_TEMPORIZADA);
    OUT3_TEMPORIZADA = EEPROM.read(EE_OUT3_TEMPORIZADA);
    
    byte OUT1_TIEMPO_1 = EEPROM.read(EE_TIEMPO_OUT1_1);
    byte OUT1_TIEMPO_2 = EEPROM.read(EE_TIEMPO_OUT1_2);
    byte SUMA_OUT1_TIEMPO [2] = {OUT1_TIEMPO_2, OUT1_TIEMPO_1};
    
    TIEMPO_OUT1 = ObtenerValor(SUMA_OUT1_TIEMPO, 0);
    
    byte OUT2_TIEMPO_1 = EEPROM.read(EE_TIEMPO_OUT2_1);
    byte OUT2_TIEMPO_2 = EEPROM.read(EE_TIEMPO_OUT2_2);
    byte SUMA_OUT2_TIEMPO [2] = {OUT2_TIEMPO_2, OUT2_TIEMPO_1};
    
    TIEMPO_OUT2 = ObtenerValor(SUMA_OUT2_TIEMPO, 0);
    
    byte OUT3_TIEMPO_1 = EEPROM.read(EE_TIEMPO_OUT3_1);
    byte OUT3_TIEMPO_2 = EEPROM.read(EE_TIEMPO_OUT3_2);
    byte SUMA_OUT3_TIEMPO [2] = {OUT3_TIEMPO_2, OUT3_TIEMPO_1};
    
    TIEMPO_OUT3 = ObtenerValor(SUMA_OUT3_TIEMPO, 0);
    VALOR = EEPROM.read(EE_CANTIDAD_DESTINOS);
    CANTIDAD_DESTINOS = VALOR-48;  

//     
// Serial.print(F("========== CONFIGURACIONES DE EQUIPO GSM I/O ================="));
// Serial.println();
// Serial.println();
// Serial.print(F("CLAVE CONTROL SALIDAS =    "));Serial.println(CLAVE_STRING);
// Serial.print(F("CANTIDAD DE USUARIOS =     "));
// Serial.print(CANTIDAD_DESTINOS);
// Serial.println();
// Serial.print(F("DESTINATARIO 1 =           "));Leer_Destinos(DESTINATARIO_1);
// Serial.print(F("DESTINATARIO 2 =           "));Leer_Destinos(DESTINATARIO_2);
// Serial.print(F("DESTINATARIO 3 =           "));Leer_Destinos(DESTINATARIO_3);
// Serial.print(F("DESTINATARIO 4 =           "));Leer_Destinos(DESTINATARIO_4);
// Serial.print(F("DESTINATARIO 5 =           "));Leer_Destinos(DESTINATARIO_5);
// Serial.print(F("DESTINATARIO 6 =           "));Leer_Destinos(DESTINATARIO_6);
// Serial.print(F("DESTINATARIO 7 =           "));Leer_Destinos(DESTINATARIO_7);
// Serial.print(F("DESTINATARIO 8 =           "));Leer_Destinos(DESTINATARIO_8);
// Serial.print(F("DESTINATARIO 9 =           "));Leer_Destinos(DESTINATARIO_9);
// Serial.print(F("DESTINATARIO 10 =          "));Leer_Destinos(DESTINATARIO_10);
// Serial.print(F("TEXTO ENT. 1 ON =          " ));Serial.println(IN1_TXT_ON_STRING);
// Serial.print(F("TEXTO ENT. 1 OFF =         " ));Serial.println(IN1_TXT_OFF_STRING);
// Serial.print(F("TEXTO ENT. 2 ON =          " ));Serial.println(IN2_TXT_ON_STRING);
// Serial.print(F("TEXTO ENT. 2 OFF =         " ));Serial.println(IN2_TXT_OFF_STRING);
// Serial.print(F("SALIDA 1 TEMPORIZADA =     " ));Serial.println(OUT1_TEMPORIZADA);
// Serial.print(F("SALIDA 2 TEMPORIZADA =     " ));Serial.println(OUT2_TEMPORIZADA);
// Serial.print(F("SALIDA 3 TEMPORIZADA =     " ));Serial.println(OUT3_TEMPORIZADA);
// Serial.print(F("SALIDA 1 TIEMPO =          " ));Serial.print(TIEMPO_OUT1);Serial.print(F(" Segundos" ));
// Serial.println();
// Serial.print(F("SALIDA 2 TIEMPO =          " ));Serial.print(TIEMPO_OUT2);Serial.print(F(" Segundos" ));
// Serial.println();
// Serial.print(F("SALIDA 3 TIEMPO =          " ));Serial.print(TIEMPO_OUT3);Serial.print(F(" Segundos" )); 
   // Imprimir_Configuracion();  
    return;
  
}


void Programacion(void)
{
  while(PROGRAMACION == true)
  {

            if (Respuesta_CMD == true)  
            {          
              fona.sendSMS(callerIDbuffer, "CMDOK");      
              Respuesta_CMD = false;                
            }

         if (Recibir.startsWith("U", 0)) // registrar cantidad de destinatarios
            {
              Recibir.remove(0, 1);              
              EEPROM.write(141,Recibir[0]);         
              Respuesta_CMD = true; 
              Recibir= "SALIR";    
            }
          
          if (Recibir.startsWith("1", 0)) // registrar destino 1
          {
                Recibir.remove(0, 1); 
                unsigned char t=0;
                for(t=0;t<14;t++)
                {                             
                  EEPROM.write(t,Recibir[t]);                                
                }
                 Respuesta_CMD = true;   
            }

         if (Recibir.startsWith("2", 0)) // registrar destino 2
          {
              Recibir.remove(0, 1); 
                  for(i=14;i<28;i++)
                {             
                  EEPROM.write(i,Recibir[i-14]); 
                  Respuesta_CMD = true;              
                }
           }
         if(Recibir.startsWith("3", 0)) // registrar destino 3
          {
                Recibir.remove(0, 1); 
                for(i=28;i<42;i++)
                {             
                  EEPROM.write(i,Recibir[i-28]); 
                  Respuesta_CMD = true;                 
                }

           }

          if (Recibir.startsWith("4", 0)) // registrar destino 4
            {
                Recibir.remove(0, 1); 
                  for(i=42;i<56;i++)
                  {             
                  EEPROM.write(i,Recibir[i-42]);   
                  Respuesta_CMD = true;               
                  }

            }
          if (Recibir.startsWith("5", 0)) // registrar destino 5
          {
                Recibir.remove(0, 1); 
                  for(i=56;i<70;i++)
                  {             
                    EEPROM.write(i,Recibir[i-56]); 
                    Respuesta_CMD = true;             
                  }

           }

          if (Recibir.startsWith("6", 0)) // registrar destino 6
              {
                Recibir.remove(0, 1); 
                  for(i=70;i<84;i++)
                  {             
                  EEPROM.write(i,Recibir[i-70]);  
                  Respuesta_CMD = true;             
                  }

              }
           if (Recibir.startsWith("7", 0)) // registrar destino 7
             {
                Recibir.remove(0, 1); 
                    for(i=84;i<98;i++)
                    {             
                    EEPROM.write(i,Recibir[i-84]); 
                    Respuesta_CMD = true;               
                    }

              }

          if (Recibir.startsWith("8", 0)) // registrar destino 8
              {
                Recibir.remove(0, 1); 
                  for(i=98;i<112;i++)
                  {             
                  EEPROM.write(i,Recibir[i-98]);  ; 
                  Respuesta_CMD = true;                
                  }

              }
          if (Recibir.startsWith("9", 0)) // registrar destino 9
             {
                Recibir.remove(0, 1); 
                  for(i=112;i<126;i++)
                  {             
                  EEPROM.write(i,Recibir[i-112]);  
                  Respuesta_CMD = true;                 
                  }
             }

          if (Recibir.startsWith("0", 0)) // registrar destino 10
              {
                Recibir.remove(0, 1); 
                  for(i=126;i<140;i++)
                  {             
                    EEPROM.write(i,Recibir[i-126]);   
                    Respuesta_CMD = true;   
                  }

              }

         if (Recibir.startsWith("TX1ON", 0)) // TEXTO ENTRADA 1 ACTIVA
             {
                  Recibir.remove(0, 5); 
                  LONG_TXT_IN1_ON = Recibir.length();
                  EEPROM.write(EE_LONGITUD_IN1_ON ,LONG_TXT_IN1_ON); 
                  for(i=158;i< (158+LONG_TXT_IN1_ON);i++)
                  {             
                  EEPROM.write(i,Recibir[i-158]);            
                  }
                  Respuesta_CMD = true;
                  Recibir = "SALIR";
             }
          if (Recibir.startsWith("TX1OFF", 0)) // TEXTO ENTRADA 1 ACTIVA
             {
                  Recibir.remove(0, 6); 
                  LONG_TXT_IN1_OFF = Recibir.length(); 
                  EEPROM.write(EE_LONGITUD_IN1_OFF ,LONG_TXT_IN1_OFF); 
                  for(i=198;i< (198+LONG_TXT_IN1_OFF);i++)
                  {             
                  EEPROM.write(i,Recibir[i-198]);            
                  }
                  Respuesta_CMD = true;
                  Recibir = "SALIR";
             }

         if (Recibir.startsWith("TX2ON", 0)) // TEXTO ENTRADA 1 ACTIVA
             {
                  Recibir.remove(0, 5); 
                  LONG_TXT_IN2_ON = Recibir.length(); 
                  EEPROM.write(EE_LONGITUD_IN2_ON ,LONG_TXT_IN2_ON); 
                  for(i=238;i< (238+LONG_TXT_IN2_ON );i++)
                  {             
                  EEPROM.write(i,Recibir[i-238]);            
                  }
                  Respuesta_CMD = true;
                  Recibir = "SALIR";
             }

          if (Recibir.startsWith("TX2OFF", 0)) // TEXTO ENTRADA 1 ACTIVA
             {
                  Recibir.remove(0, 6); 
                  LONG_TXT_IN2_OFF  = Recibir.length(); 
                  EEPROM.write(EE_LONGITUD_IN2_OFF ,LONG_TXT_IN2_OFF); 
                  unsigned int j=0;
                  for(j=278;j <(278+LONG_TXT_IN2_OFF);j++)
                  {         
                  EEPROM.write(j,Recibir[j-278]);            
                  }
                  Respuesta_CMD = true;
                  Recibir = "SALIR";
             }

           if (Recibir.startsWith("C", 0)) // registra clave
              {
                  Recibir.remove(0, 1); 
                  for(i=154;i<158;i++)
                  {             
                  EEPROM.write(i,Recibir[i-154]);          
                  }
                  Respuesta_CMD = true;
                  Recibir = "SALIR";
              }
              
            if (Recibir ==  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[12]))))
              {
                EEPROM.write(EE_OUT1_TEMPORIZADA, 1); 
                Respuesta_CMD = true;
                Recibir = "SALIR";               
              }
              
             if (Recibir ==  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[15]))))
              {
                EEPROM.write(EE_OUT1_TEMPORIZADA, 0);
                Respuesta_CMD = true;
                Recibir = "SALIR"; 
              }
            if (Recibir ==  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[13]))))
              {
                EEPROM.write(EE_OUT2_TEMPORIZADA, 1);
                Respuesta_CMD = true; 
                Recibir = "SALIR"; 
              }
             if (Recibir ==  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[16]))))
              {
                EEPROM.write(EE_OUT2_TEMPORIZADA, 0);
                Respuesta_CMD = true;
                Recibir = "SALIR"; 
              }
              if (Recibir ==  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[14]))))
              {
                EEPROM.write(EE_OUT3_TEMPORIZADA, 1);
                Respuesta_CMD = true;
                Recibir = "SALIR"; 
              }
           if (Recibir ==  strcpy_P(buffer, (char *)pgm_read_word(&(string_table[17]))))
              {
                EEPROM.write(EE_OUT3_TEMPORIZADA, 0);
                Respuesta_CMD = true;
                Recibir ="SALIR"; 
              }

          if (Recibir.startsWith("T1/", 0)) // Tiempo de temporizado de la salida 1
            {
              Recibir.remove(0, 3);
              unsigned int OUT1_TIEMPO;
              OUT1_TIEMPO = Recibir.toInt();            
              byte OUT1_1;
              byte OUT1_2;
              OUT1_1 = OUT1_TIEMPO & 0XFF;
              OUT1_2 = OUT1_TIEMPO >>8&0XFF;
              EEPROM.write(EE_TIEMPO_OUT1_1, OUT1_1);
              EEPROM.write(EE_TIEMPO_OUT1_2, OUT1_2);
              Respuesta_CMD = true;
              Recibir= "SALIR";
            }
            
          if (Recibir.startsWith("T2/", 0)) // Tiempo de temporizado de la salida 2
            {
              Recibir.remove(0, 3);
              unsigned int OUT2_TIEMPO;
              OUT2_TIEMPO = Recibir.toInt();            
              byte OUT2_1;
              byte OUT2_2;
              OUT2_1 = OUT2_TIEMPO & 0XFF;
              OUT2_2 = OUT2_TIEMPO >>8&0XFF;
              EEPROM.write(EE_TIEMPO_OUT2_1, OUT2_1);
              EEPROM.write(EE_TIEMPO_OUT2_2, OUT2_2);
              Respuesta_CMD = true;
              Recibir= "SALIR";
            }

           if (Recibir.startsWith("T3/", 0)) // Tiempo de temporizado de la salida 3
            {
              Recibir.remove(0, 3);
              unsigned int OUT3_TIEMPO;
              OUT3_TIEMPO = Recibir.toInt();            
              byte OUT3_1;
              byte OUT3_2;
              OUT3_1 = OUT3_TIEMPO & 0XFF;
              OUT3_2 = OUT3_TIEMPO >>8&0XFF;
              EEPROM.write(EE_TIEMPO_OUT3_1, OUT3_1);
              EEPROM.write(EE_TIEMPO_OUT3_2, OUT3_2);
              Respuesta_CMD = true;
              Recibir= "SALIR";
            }

            if (Recibir == strcpy_P(buffer, (char *)pgm_read_word(&(string_table[9]))))
              {

                 if (!fona.sendSMS(callerIDbuffer, strcpy_P(buffer, (char *)pgm_read_word(&(string_table[7])))))
                 {
                  Serial.println(F("Confirmacion Enviada"));
                  } else {
                  Serial.println(F("No se envio confirmacion"));
                  }  
                  PROGRAMACION=false;
                  Recibir = "SALIR"; 
              }


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
         if (fona.readSMS(slot, smsBuffer, 100, &smslen)) // ¡pasa el búfer y el máximo len!
         { 
          Serial.println(smsBuffer);
          Recibir = smsBuffer;              
         }
       
        if (fona.deleteSMS(slot)) //eliminar el mensaje original después de que se procese
      {
        Serial.println(F("MENSAJE ELIMINADO"));
      } else {
        unsigned char i;
        for (i =0; i < 2; i++) fona.deleteSMS(slot);
        Serial.print(F("NO SE PUDO ELIMINAR EL MENSAJE")); Serial.println(slot);
        fona.print(F("AT+CMGD=?\r\n"));
      }

    }
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
void r_eeprom(unsigned char *pDato, unsigned int DirEE, unsigned char cL_CantidadDeBytes)
{
  static unsigned char i;
  for(i=0; i<cL_CantidadDeBytes;i++)
  {
    *(pDato+i)=EEPROM.read(DirEE+i);
  }
  return;
}

void Leer_Destinos(unsigned char *pDato)
{
  static unsigned char i;
  for(i=0; i<14;i++)
  {
    Serial.print(char(pDato[i]));
  }
  Serial.println();
  return;
}



unsigned int ObtenerValor(byte paquete [], byte i)
{

  unsigned int valor;
  valor = (valor * 256)+ paquete [i];
  valor = (valor * 256)+ paquete [i+1];
  return valor;
}

//void Imprimir_Configuracion(void)
//{
//
// Serial.print(F("========== CONFIGURACIONES DE EQUIPO GSM I/O ================="));
// Serial.println();
// Serial.println();
// Serial.print(F("CLAVE CONTROL SALIDAS =    "));Serial.println(CLAVE_STRING);
// Serial.print(F("CANTIDAD DE USUARIOS =     "));
// Serial.print(CANTIDAD_DESTINOS);
// Serial.println();
// Serial.print(F("DESTINATARIO 1 =           "));Leer_Destinos(DESTINATARIO_1);
// Serial.print(F("DESTINATARIO 2 =           "));Leer_Destinos(DESTINATARIO_2);
// Serial.print(F("DESTINATARIO 3 =           "));Leer_Destinos(DESTINATARIO_3);
// Serial.print(F("DESTINATARIO 4 =           "));Leer_Destinos(DESTINATARIO_4);
// Serial.print(F("DESTINATARIO 5 =           "));Leer_Destinos(DESTINATARIO_5);
// Serial.print(F("DESTINATARIO 6 =           "));Leer_Destinos(DESTINATARIO_6);
// Serial.print(F("DESTINATARIO 7 =           "));Leer_Destinos(DESTINATARIO_7);
// Serial.print(F("DESTINATARIO 8 =           "));Leer_Destinos(DESTINATARIO_8);
// Serial.print(F("DESTINATARIO 9 =           "));Leer_Destinos(DESTINATARIO_9);
// Serial.print(F("DESTINATARIO 10 =          "));Leer_Destinos(DESTINATARIO_10);
// Serial.print(F("TEXTO ENT. 1 ON =          " ));Serial.println(IN1_TXT_ON_STRING);
// Serial.print(F("TEXTO ENT. 1 OFF =         " ));Serial.println(IN1_TXT_OFF_STRING);
// Serial.print(F("TEXTO ENT. 2 ON =          " ));Serial.println(IN2_TXT_ON_STRING);
// Serial.print(F("TEXTO ENT. 2 OFF =         " ));Serial.println(IN2_TXT_OFF_STRING);
// Serial.print(F("SALIDA 1 TEMPORIZADA =     " ));Serial.println(OUT1_TEMPORIZADA);
// Serial.print(F("SALIDA 2 TEMPORIZADA =     " ));Serial.println(OUT2_TEMPORIZADA);
// Serial.print(F("SALIDA 3 TEMPORIZADA =     " ));Serial.println(OUT3_TEMPORIZADA);
// Serial.print(F("SALIDA 1 TIEMPO =          " ));Serial.print(TIEMPO_OUT1);Serial.print(F(" Segundos" ));
// Serial.println();
// Serial.print(F("SALIDA 2 TIEMPO =          " ));Serial.print(TIEMPO_OUT2);Serial.print(F(" Segundos" ));
// Serial.println();
// Serial.print(F("SALIDA 3 TIEMPO =          " ));Serial.print(TIEMPO_OUT3);Serial.print(F(" Segundos" ));  
// return;
//}
