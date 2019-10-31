int irDet = 2;
int key = 0;
int data[12];
int state;

void setup() {
 Serial.begin(9600);                         //For debugging
 pinMode(irDet, INPUT);                      //IR detector connected to digital pin 2
}

void loop() {                                //Main loop
 if (pulseIn(irDet, LOW) > 2200) {          //Check for start pulse
   getIRkey();
   Serial.print("Key press is = ");
   Serial.println(key);
 }

}

int getIRkey() {                             //Read pulses
 data[0] = pulseIn(irDet, LOW);
 data[1] = pulseIn(irDet, LOW);
 data[2] = pulseIn(irDet, LOW);
 data[3] = pulseIn(irDet, LOW);
 data[4] = pulseIn(irDet, LOW);
 data[5] = pulseIn(irDet, LOW);
 data[6] = pulseIn(irDet, LOW);
 data[7] = pulseIn(irDet, LOW);
 data[8] = pulseIn(irDet, LOW);
 data[9] = pulseIn(irDet, LOW);
 data[10] = pulseIn(irDet, LOW);
 data[11] = pulseIn(irDet, LOW);

 for(int x = 0; x <= 11; x++) {              //Decide wether pulses are 1's or 0's
   if(data[x] > 1000) {
     data[x] = 1;
   }
   else {
     data[x] = 0;
   }
 }

 int result = 0;                            //Convert array into interger
 int seed = 1;
 for(int i=0;i<11;i++) {
   if(data[i] == 1) {
     result += seed;
   }
   seed = seed * 2;
 }
 key = result;
 return result;
}
