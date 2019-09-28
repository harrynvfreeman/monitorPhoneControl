
#define BAUD 9600

const uint8_t IPUSART3 = 24;

const unsigned long leadTime = 9000;
const unsigned long spaceTime = 4500;
const unsigned long constTime = 560;
const unsigned long zeroTime = 560;
const unsigned long oneTime = 1690;
const unsigned long icrTime = 0x00D2;
const unsigned long ocTime = icrTime >> 1;

const int addressLow = 0x02;
const int addressHigh = 0x7D;

const uint8_t powerRec = 0x70;
const uint8_t enterRec = 0x0A;
const uint8_t zeroRec = 0x30;
const uint8_t oneRec = 0x31;
const uint8_t twoRec = 0x32;
const uint8_t threeRec = 0x33;
const uint8_t fourRec = 0x34;
const uint8_t fiveRec = 0x35;
const uint8_t sixRec = 0x36;
const uint8_t sevenRec = 0x37;
const uint8_t eightRec = 0x38;
const uint8_t nineRec = 0x39;

const uint8_t powerTrans = 0x46;
const uint8_t enterTrans = 0x4A;
const uint8_t zeroTrans = 0x00;
const uint8_t oneTrans = 0x01;
const uint8_t twoTrans = 0x02;
const uint8_t threeTrans = 0x03;
const uint8_t fourTrans = 0x04;
const uint8_t fiveTrans = 0x05;
const uint8_t sixTrans = 0x06;
const uint8_t sevenTrans = 0x07;
const uint8_t eightTrans = 0x08;
const uint8_t nineTrans = 0x09;

int main() {
  
  //pwm
  init_pwm();

  //serial
  init_usart();

  _delay_ms(30);
  
  while(true) {
    SMCR &= 245;
  }
}

void init_pwm() {
  ICR4 = icrTime;
  TCCR4B = 0x00;
  TCCR4B = 0x11;
  TCCR4A = 0x0A; 
  DDRH |= 32;
}

void init_usart() {
  uint8_t store = SREG;
  SREG = 0;
  
  //baud rate
  UBRR3H = ((F_CPU / 16) / BAUD - 1) >> 8;
  UBRR3L = ((F_CPU / 16) / BAUD - 1);
  
  //set up receiver
  UCSR3B = (1<<RXEN3)|(1<<RXCIE3);

  //Enable global interrupt flag
  SREG = store | 128;
} 

//006C
ISR(USART3_RX_vect) {
  uint8_t receiveVal = UDR3;
  switch (receiveVal) {
    case powerRec:
      sendMessage(powerTrans);
      break;
    case enterRec:
      sendMessage(enterTrans);
      break;
    case zeroRec:
      sendMessage(zeroTrans);
      break;
    case oneRec:
      sendMessage(oneTrans);
      break;
    case twoRec:
      sendMessage(twoTrans);
      break;
    case threeRec:
      sendMessage(threeTrans);
      break;
    case fourRec:
      sendMessage(fourTrans);
      break;
    case fiveRec:
      sendMessage(fiveTrans);
      break;
    case sixRec:
      sendMessage(sixTrans);
      break;
    case sevenRec:
      sendMessage(sevenTrans);
      break;
    case eightRec:
      sendMessage(eightTrans);
      break;
    case nineRec:
      sendMessage(nineTrans);
      break;
  }
}

void sendMessage(uint8_t message) {
  sendLead();
  sendSpace();
  sendAddress();
  
  for (int i = 0; i < 8; i++) {
    if ((message >> i) & 0x01) {
      sendOne();
    } else {
      sendZero();
    } 
  }

  for (int i = 0; i < 8; i++) {
    if ((message >> i) & 0x01) {
      sendZero();
    } else {
      sendOne();
    } 
  }

  sendZero();
}

void sendAddress() {
  for (int i = 0; i < 8; i++) {
    if ((addressLow >> i) & 0x01) {
      sendOne();
    } else {
      sendZero();
    } 
  }

  for (int i = 0; i < 8; i++) {
    if ((addressHigh) >> i & 0x01) {
      sendOne();
    } else {
      sendZero();
    } 
  }
}

void sendOne() {
  pwmOn();
  _delay_us(constTime - 30);
  pwmOff();
  _delay_us(oneTime - 30);
}

void sendZero() {
  pwmOn();
  _delay_us(constTime - 30);
  pwmOff();
  _delay_us(zeroTime - 30);
}

void sendLead() {
  pwmOn();
  _delay_us(leadTime - 30);
}

void sendSpace() {
  pwmOff();
  _delay_us(spaceTime - 20);
}

void pwmOn() {
  OCR4C = ocTime;
}

void pwmOff() {
  OCR4C = 0;
}
