// Crashes after 235
int count = 0;
int espMem = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  espMemory(0);
}

void loop() {
  
}

int espMemory(int espMem) {
  int crash[espMem];
  Serial.println(count++);
  espMem *= 2;
  espMemory(espMem);
}
