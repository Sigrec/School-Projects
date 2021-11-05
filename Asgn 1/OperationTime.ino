float startTime;
float endTime;
float resultTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  IntegerTimeOperations(1, 1);
  FloatTimeOperations(1, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void IntegerTimeOperations(int num1, int num2){
  startTime = micros();
  num1 = num1 + num2;
  endTime = micros();
  Serial.print("Integer '+' Time: ");
  Serial.println(endTime - startTime);

  delay(10);
  
  startTime = micros();
  num1 = num1 - num2;
  endTime = micros();
  Serial.print("Integer '-' Time: ");
  Serial.println(endTime - startTime);

  delay(10);
  
  startTime = micros();
  num1 = num1 * num2;
  endTime = micros();
  Serial.print("Integer '*' Time: ");
  Serial.println(endTime - startTime);

  delay(10);
  
  startTime = micros();
  num1 = num1 / num2;
  endTime = micros();
  Serial.print("Integer '/' Time: ");
  Serial.println(endTime - startTime);

  delay(10);
  
  startTime = micros();
  num1 = num1 % num2;
  endTime = micros();
  Serial.print("Integer '%' Time: ");
  Serial.println(endTime - startTime);

  delay(10);
}

void FloatTimeOperations(float num1, float num2){
  startTime = micros();
  num1 = num1 + num2;
  endTime = micros();
  Serial.print("Float '+' Time: "); 
  Serial.println(endTime - startTime);

  delay(10);
  
  startTime = micros();
  num1 = num1 - num2;
  endTime = micros();
  Serial.print("Float '-' Time: "); 
  Serial.println(endTime - startTime);

  delay(10);
  
  startTime = micros();
  num1 = num1 * num2;
  endTime = micros();
  Serial.print("Float '*' Time: "); 
  Serial.println(endTime - startTime);

  delay(10);
  
  startTime = micros();
  num1 = num1 / num2;
  endTime = micros();
  Serial.print("Float '/' Time: "); 
  Serial.println(endTime - startTime);
}
