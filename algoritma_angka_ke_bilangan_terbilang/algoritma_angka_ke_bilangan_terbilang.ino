void setup() {
  Serial.begin(9600);
  Serial.println("Program konversi angka. Duwiarsana.com");
}

void loop() {
  if(Serial.available()){
    delay(100);
    int f = Serial.parseInt();
    
    if(f>0)
    {
      baca(f);
      Serial.println();
    }
  }
}



void baca(int angka){
  if(angka>99)ratusan(angka);
  else if(angka>9)puluhan(angka);
  else if(angka>0)satuan(angka);
  
}

void puluhan(int angka){
  
  int tmp1,tmp2;
  tmp1=angka/10;
  tmp2=angka%10;
  
  switch(tmp1){
   case 1:{
    if(tmp2==0)        Serial.print("Sepuluh ");
    else if(tmp2==1)   Serial.print("Sebelas ");
    else if(tmp2==2)   Serial.print("Dua Belas ");
    else if(tmp2==3)   Serial.print("Tiga Belas ");
    else if(tmp2==4)   Serial.print("Empat Belas ");
    else if(tmp2==5)   Serial.print("Lima Belas ");
    else if(tmp2==6)   Serial.print("Enam belas ");
    else if(tmp2==7)   Serial.print("Tujuh Belas ");
    else if(tmp2==8)   Serial.print("Delapan Belas ");
    else if(tmp2==9)   Serial.print("Sembilan Belas ");
    break;
   } 
    case 2:Serial.print("Dua Puluh ");break;
    case 3:Serial.print("Tiga Puluh ");break;
    case 4:Serial.print("Empat Puluh ");break;
    case 5:Serial.print("Lima Puluh ");break;
    case 6:Serial.print("Enam Puluh ");break;
    case 7:Serial.print("Tujuh Puluh ");break;
    case 8:Serial.print("Delapan Puluh ");break;
    case 9:Serial.print("Sembilan Puluh ");break;
 
  }
  delay(1300);
  if(tmp2>0&&angka>19)satuan(tmp2);
}

void satuan(int angka){
  
 switch(angka){
  case 1:              Serial.print("Satu ");break;
  case 2:              Serial.print("Dua ");break;
  case 3:              Serial.print("Tiga ");break;
  case 4:              Serial.print("Empat ");break;
  case 5:              Serial.print("Lima ");break;
  case 6:              Serial.print("Enam ");break;
  case 7:              Serial.print("Tujuh ");break;
  case 8:              Serial.print("Delapan ");break;
  case 9:              Serial.print("Sembilan ");break;
 } 
 delay(700);
}


void ratusan(int angka){
  int tmp1,tmp2;
  tmp1=angka/100; tmp2=angka%100;
  //Serial.println();
  switch(tmp1){
case 1: Serial.print("Seratus ");break;
case 2: Serial.print("Dua Ratus ");break;
case 3: Serial.print("Tiga Ratus ");break;
case 4: Serial.print("Empat Ratus ");break;
case 5: Serial.print("Lima Ratus ");break;
case 6: Serial.print("Enam Ratus ");break;
case 7: Serial.print("Tujuh Ratus ");break;
case 8: Serial.print("Delapan Ratus ");break;
case 9: Serial.print("Sembilan Ratus ");break;
  }
  //Serial.println(tmp1);
  delay(1000);
  if(tmp2>9)puluhan(tmp2);
  else if(tmp2>0)satuan(tmp2);
}
