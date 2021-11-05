void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  GetEndianness();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void GetEndianness(){
  uint32_t x = 0x00000010;
  const uint8_t* p = reinterpret_cast<const uint8_t*>(&x);
  if (p[0] == 16){
    Serial.println("Little Endian");
  }
  else{
    Serial.println("Big Endian");
  }
}
