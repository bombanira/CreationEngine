# define A 7
# define B 4
# define C 5
# define D 3
# define E 2
# define F A0
# define G 6
# define M1 8
# define M2 9
# define M3 10
# define M4 11
# define M5 12
# define M6 13
void digit_reset(void);
void seg_on(uint8_t signal_bit);
void seg_table(byte digit_num, char character);
byte data_counter = 0;
char string[6] = {0};
bool reset_flg = false;
void setup() {
  // put your setup code here, to run once:
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  pinMode(M5, OUTPUT);
  pinMode(M6, OUTPUT);
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  digitalWrite(M3, LOW);
  digitalWrite(M4, LOW);
  digitalWrite(M5, LOW);
  digitalWrite(M6, LOW);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0)
  {
     char a = Serial.read();
     if(data_counter == 6)
     {
        data_counter = 0;
     }
     if(a != '\n')
     {
        Serial.println(a);
        string[data_counter] = a;
        data_counter++;
        Serial.println(data_counter);
     }
  }
  seg_table(M1, string[0]);
  seg_table(M2, string[1]);
  seg_table(M3, string[2]);
  seg_table(M4, string[3]);
  seg_table(M5, string[4]);
  seg_table(M6, string[5]);
}

void seg_table(byte digit_num, char character)
{
  digit_reset();
  num_reset();
  digitalWrite(digit_num, HIGH);
  // digitalWrite(A, LOW);
  switch(character)
  {
    case '1':
      seg_on(0b00000110);
      break;
    case '2':
      seg_on(0b01011011);
      break;
    case '3':
      seg_on(0b01001111);
      break;
    case '4':
      seg_on(0b01100110);
      break;
    case '5':
      seg_on(0b01101101);
      break;
    case '6':
      seg_on(0b01111101);
      break;
    case '7':
      seg_on(0b00100111);
      break;
    case '8':
      seg_on(0b11111111);
      break;
    case '9':
      seg_on(0b01100111);
      break;
    case '0':
      seg_on(0b00111111);
      break;
    case 'A':
      seg_on(0b01110111);
      break;
    case 'B':
      seg_on(0b01111100);
      break;
    case 'C':
      seg_on(0b00111001);
      break;
    case 'D':
      seg_on(0b01011110);
      break;
    case 'E':
      seg_on(0b01111001);
      break;
    case 'F':
      seg_on(0b01110001);
      break;
    case 'O':
      seg_on(0b01011100);
      break;
    case 'R':
      seg_on(0b01010000);
      break;
    case 'S':
      seg_on(0b01101101);
      break;
    case 'N':
      seg_on(0b01010100);
      break;
    case 'T':
      seg_on(0b01111000);
      break;
    case 'L':
      seg_on(0b00111000);
      break;
  }
}

void seg_on(uint8_t signal_bit)
{
  digitalWrite(A, !((signal_bit & 0b00000001) > 0));
  digitalWrite(A, HIGH);
  digitalWrite(B, !((signal_bit & 0b00000010) > 0));
  digitalWrite(B, HIGH);
  digitalWrite(C, !((signal_bit & 0b00000100) > 0));
  digitalWrite(C, HIGH);
  digitalWrite(D, !((signal_bit & 0b00001000) > 0));
  digitalWrite(D, HIGH);
  digitalWrite(E, !((signal_bit & 0b00010000) > 0));
  digitalWrite(E, HIGH);
  digitalWrite(F, !((signal_bit & 0b00100000) > 0));
  digitalWrite(F, HIGH);
  digitalWrite(G, !((signal_bit & 0b01000000) > 0));
  digitalWrite(G, HIGH);
}

void digit_reset(void)
{
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  digitalWrite(M3, LOW);
  digitalWrite(M4, LOW);
  digitalWrite(M5, LOW);
  digitalWrite(M6, LOW);
}
void num_reset(void)
{
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}
