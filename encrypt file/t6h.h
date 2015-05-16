



long getLength(char* doc);
long random(long max);
void randomString(char* stuff, long length);

void scramble1(char* code, long length, char* doc,long pwSize);
void dscramble1(char* code, long length, char* doc,long pwSize);

void scramble2(char* code,long length,  char* doc,  long pwSize);

void dscramble2(char* code,long length,  char* doc,  long pwSize);

void stringToChar(char* theString, char* theCharArray,long arrayLength);

void pwm1(char* password, long pwsize);
void dpwm1(char* password, long pwsize);

void DScramble(char* passwordR, long &pwSize, char* doc, long length, int executefR[], int executeNum);
void Scramble(char* passwordR, long &pwSize, char* doc, long length, int executefR[], int executeNum);
