#include "tel.h"

int main(){
	
	char x[20] = {'a'};
	uint8_t s;
	printf("Please enter a size then the value: \n");
	scanf("%d %s", &s, &x);
	

	uint16_t y = str2int(x, s);

	struct data d = receiveData(y);

	printf("%d\n%d\n%d\n%d\n", d.ID, d.value, d.check, d.time);

	return 0;
}

char* reportString(struct data d){
	
}

struct data receiveData(uint16_t r){
	uint8_t a[SIZE];
	uint16_t mask = 61440;

	for(uint8_t i = 0; i < SIZE; i++){
        a[i] = (r & mask) >> ((SIZE - 1 - i) * SIZE);
        mask = mask >> SIZE;
    }

    return constructor(a);
}

struct data constructor(uint8_t a[]){

	struct data d;

	d.ID = a[0];
	d.value = (a[1] << SIZE) | a[2];
	d.check = a[3];

	d.time = getTime();

	return d;

}

uint32_t getTime(){

    time_t current_time;
 
    current_time = time(NULL);

    uint32_t i_time = (uint32_t)current_time;
    
    return i_time;
}

uint16_t str2int(char a[], uint8_t s){

	uint16_t result = 0;

	for(uint8_t i = 1; i <= s; i++){
		result += ((a[i - 1] - '0') * pow(10, (s - i)));
	}

	return result;
}