#include <Arduino.h>
#include <EEPROM.h>

//offset of byte in EEPROM
#define I2C_OFFSET 0
//restricted I2C address
#define I2C_MASTER 0

//counter to show that Arduino is alive
long cuntr = 0;

//calculate the crc
//source: https://docs.arduino.cc/learn/programming/eeprom-guide
unsigned long eeprom_crc(void) {
  const unsigned long crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };

  unsigned long crc = ~0L;

  for (int index = 0 ; index < EEPROM.length()  ; ++index) {
    crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }

  return crc;
}

//erase all data
void clear_eeprom(void) {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }

  Serial.println("EEPROM Cleared!");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  randomSeed(analogRead(0));
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  Serial.print("EEPROM CRC: ");
  Serial.println(eeprom_crc(), HEX);

  Serial.print("My I2C address is: ");
  Serial.println(EEPROM[I2C_OFFSET]);

  if (EEPROM[I2C_OFFSET] == I2C_MASTER) {
    Serial.println("Oh no, I shouldn't be a master!");
    const int newAddr = random(1, 127);
    EEPROM.write(I2C_OFFSET, newAddr);
    Serial.print("New randomized address is: ");
    Serial.println(newAddr);
  }
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.print("Hip ");
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  Serial.print("Hop (");
  Serial.print(cuntr++);
  Serial.println(")");
  delay(1000);                       // wait for a second

  if (Serial.available() > 0) {
    // read two bytes, so single character in Arduino IDE's serial client
    uint8_t buf[2];
    Serial.readBytes(buf, 2);

    clear_eeprom();
  }
}
