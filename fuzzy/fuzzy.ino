
float esuhu = 1;
float dsuhu = 1;
float EN,N,EP,KB,KK,Z,PK,PB;
float rule[15]={};
float z_pemanas[15]={};
float z_kipas[15]={};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    Serial.println();
      Serial.println();
        Serial.println();
          Serial.println();
            Serial.println();
              Serial.println();
                 Serial.println();
      Serial.println();
        Serial.println();
          Serial.println();
            Serial.println();
              Serial.println();
                 Serial.println();
      Serial.println();
        Serial.println();
          Serial.println();
            Serial.println();
              Serial.println();
                
Serial.println();
Serial.println();
Serial.println();
  Serial.println("Tes Fuzzy");

  fuzzyfikasi();
  fuzzy_inferensi();

  

}

void loop() {
  // put your main code here, to run repeatedly:

}

//---------------------------------------------FUZZYFIKASI--------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------Fuzzy Input Error_Suhu---------------------------------------------------------------------------------------------------------------------
//linier turun
float fuzzyfikasi_EN(float x){
    float val;
    if(x <= -3){
      val = 1;
      return val;
      } 
    else if(x <= 0 && x >= -3 ) {
      val = (0-(x))/(3);
      return val;
      }
    else if(x >= 0){
      val = 0;
      return val;
      }
}

//segitiga
float fuzzyfikasi_N(float x){
  float val;
  if(x <= -3 || x >= 3) {
    val = 0;
    return val;
    }
  else if(x >= -3 && x <= 0){
    val = (x+3)/3;
    return val;
    }
  else if(x >= 0 && x <= 3){
    val = (3-x)/3;
    return val;
    }   
}  

//linier naik

float fuzzyfikasi_EP(float x){
  float val;
  if(x <= 0){
    val = 0;
    return val;
    }
  else if(x >= 0 && x <= 3){
    val = x/3;
    return val;
    }
  else if(x >= 3){
    val = 1;
    return val;
    } 
}

//-----------------------------------------------------------------END fuzzy Input Error_Suhu---------------------------------------------------------------------------------------------


//-----------------------------------------------------------------Fuzzy Input Delta_error_Suhu-------------------------------------------------------------------------------------------

//linier turun
float fuzzyfikasi_KB(float x){
    float val;
    if(x <= -1){
      val = 1.0;
      return val;
      } 
    else if(x <= -0.5 && x >= -1 ) {
      val = (-0.5-(x))/(0.5);
      return val;
      }
    else if(x >= -0.5){
      val = 0.0;
      return val;
      }
}

// Segitiga
float fuzzyfikasi_KK(float x){
 float val;
  if(x <= -1 || x >= 0) {
    val = 0;
    return val;
    }
  else if(x >= -1 && x <= -0.5){
    val = (x+1)/0.5;
    return val;
    }
  else if(x >= -0.5 && x <= 0){
    val = (0-x)/0.5;
    return val;
    }     
}

// Segitiga
float fuzzyfikasi_Z(float x){
 float val;
  if(x <= -0.5 || x >= 0.5 ) {
    val = 0;
    return val;
    }
  else if(x >= -0.5 && x <= 0){
    val = (x+0.5)/0.5;
    return val;
    }
  else if(x >= 0 && x <= 0.5){
    val = (0.5-x)/0.5;
    return val;
    }     
}

// Segitiga
float fuzzyfikasi_PK(float x){
 float val;
  if(x <= 0 || x >= 1 ) {
    val = 0;
    return val;
    }
  else if(x >= 0 && x <= 0.5){
    val = x/0.5;
    return val;
    }
  else if(x >= 0.5 && x <= 1){
    val = (1-x)/0.5;
    return val;
    }     
}

//linier naik
float fuzzyfikasi_PB(float x){
  float val;
  if(x <= 0.5){
    val = 0;
    return val;
    }
  else if(x >= 0.5 && x <= 1){
    val = (x-0.5)/0.5;
    return val;
    }
  else if(x >= 1){
    val = 1;
    return val;
    } 
}

void fuzzyfikasi(){
  EN = fuzzyfikasi_EN(esuhu);
  N  = fuzzyfikasi_N(esuhu);
  EP  = fuzzyfikasi_EP(esuhu);
  KB  = fuzzyfikasi_KB(dsuhu);
  KK  = fuzzyfikasi_KK(dsuhu);
  Z  = fuzzyfikasi_Z(dsuhu);
  PK  = fuzzyfikasi_PK(dsuhu);
  PB  = fuzzyfikasi_PB(dsuhu);
  }
//-----------------------------------------------------------------END Fuzzy Input Delta_error_Suhu---------------------------------------------------------------------------------------------------------


//EN, N, EP, KB , KK, Z, PK,PB
void fuzzy_inferensi (){

  
//  Serial.print("Error Suhu EN : ");
//  Serial.println(EN);
//  Serial.print("Error Suhu N : ");
//  Serial.println(N);
//  Serial.print("Error Suhu EP : ");
//  Serial.println(EP);
//  Serial.print("D_Error Suhu KB : ");
//  Serial.println(KB);
//  Serial.print("D_Error Suhu KK : ");
//  Serial.println(KK);
//  Serial.print("D_Error Suhu Z : ");
//  Serial.println(Z);
//  Serial.print("D_Error Suhu PK : ");
//  Serial.println(PK);
//  Serial.print("D_Error Suhu PB : ");
//  Serial.println(PB);

// rule 1
  rule[0] = min(EN,KB);
  z_pemanas[0] = 0;
  z_kipas[0] = 255;
// rule 2
  rule[1] = min(EN,Z);
  z_pemanas[1] = 0;
  z_kipas[1] = 255;
// rule 3
  rule[2] = min(EN,KK);
  z_pemanas[2] = 0;
  z_kipas[2] = 255;
// rule 4
  rule[3] = min(EN,PB);
  z_pemanas[3] = 0;
  z_kipas[3] = 255;
// rule 5
  rule[4] = min(EN,PK);
  z_pemanas[4] = 0;
  z_kipas[4] = 255;
// rule 6
  rule[5] = min(N,KB);
  z_pemanas[5] = 0;
  z_kipas[5] = 150;
// rule 7
  rule[6] = min(N,KK);
  z_pemanas[6] = 0;
  z_kipas[6] = 150;
// rule 8
  rule[7] = min(N,Z);
  z_pemanas[7] = 0;
  z_kipas[7] = 150;
// rule 9
  rule[8] = min(N,PK);
  z_pemanas[8] = 150;
  z_kipas[8] = 150;
// rule 10
  rule[9] = min(N,PB);
  z_pemanas[9] = 150;
  z_kipas[9] = 150;
// rule 11
  rule[10] = min(EP,KB);
  z_pemanas[10] = 150;
  z_kipas[10] = 200;
// rule 12
  rule[11] = min(EP,KK);
  z_pemanas[11] = 150;
  z_kipas[11] = 200;
// rule 13
  rule[12] = min(EP,Z);
  z_pemanas[12] = 200;
  z_kipas[12] = 200;
// rule 14
  rule[13] = min(EP,PK);
  z_pemanas[13] = 200;
  z_kipas[13] = 200;
// rule 15
  rule[14] = min(EP,PB);
  z_pemanas[14] = 255;
  z_kipas[14] = 255;

  float z_total_pemanas = ((rule[0]*z_pemanas[0])+(rule[1]*z_pemanas[1])+(rule[2]*z_pemanas[2])+(rule[3]*z_pemanas[3])+(rule[4]*z_pemanas[4])+(rule[5]*z_pemanas[5])+(rule[6]*z_pemanas[6])+(rule[7]*z_pemanas[7])+(rule[8]*z_pemanas[8])+(rule[9]*z_pemanas[9])+(rule[10]*z_pemanas[10])+(rule[11]*z_pemanas[11])+(rule[12]*z_pemanas[12])+(rule[13]*z_pemanas[13])+(rule[14]*z_pemanas[14]))/(rule[0]+rule[1]+rule[2]+rule[3]+rule[4]+rule[5]+rule[6]+rule[7]+rule[8]+rule[9]+rule[10]+rule[11]+rule[12]+rule[13]+rule[13]+rule[14]);
  float z_total_kipas = ((rule[0]*z_kipas[0])+(rule[1]*z_kipas[1])+(rule[2]*z_kipas[2])+(rule[3]*z_kipas[3])+(rule[4]*z_kipas[4])+(rule[5]*z_kipas[5])+(rule[6]*z_kipas[6])+(rule[7]*z_kipas[7])+(rule[8]*z_kipas[8])+(rule[9]*z_kipas[9])+(rule[10]*z_kipas[10])+(rule[11]*z_kipas[11])+(rule[12]*z_kipas[12])+(rule[13]*z_kipas[13])+(rule[14]*z_kipas[14]))/(rule[0]+rule[1]+rule[2]+rule[3]+rule[4]+rule[5]+rule[6]+rule[7]+rule[8]+rule[9]+rule[10]+rule[11]+rule[12]+rule[13]+rule[13]+rule[14]);
  Serial.print("nilai e_suhu : ");
  Serial.println(esuhu);
  Serial.print("nilai d_suhu : ");
  Serial.println(dsuhu);
  Serial.print("nilai Output Kipas : ");
  Serial.println(z_total_kipas);
  Serial.print("nilai Output Pemanas : ");
  Serial.println(z_total_pemanas);
     Serial.println();
      Serial.println();
        Serial.println();
          Serial.println();
            Serial.println();
              Serial.println();
                 Serial.println();
      Serial.println();
        Serial.println();
          Serial.println();
            Serial.println();
              Serial.println();
  
  
 }
