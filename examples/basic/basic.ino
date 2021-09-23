#include <espBoilerplate.h>
#define YOURSSID "test"
#define YOURPSK "testpresharedkey"
void setup() {
  // put your setup code here, to run once:
  espBoilerplate.begin(YOURSSID,YOURPSK);
}

void loop() {
  // put your main code here, to run repeatedly:

}
