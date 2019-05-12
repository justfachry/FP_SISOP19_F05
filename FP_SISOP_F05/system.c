#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <stdbool.h>
pthread_t tid[7];
char song[40];
char contain[50];
int n;
bool on = true;

void* playandcount(void *arg){
	unsigned long i=0;
	pthread_t id=pthread_self();
	chdir("/home/cheese/Music");

	if(pthread_equal(id,tid[1])){
		system("clear");
		printf("Pilihan\n");
		printf("1. Perintah\n"); 
		printf("2. Playlist\n");
		printf("3. Mainkan\n");
		printf("4. Pause\n");
		printf("5. Continue\n");
		printf("6. Stop\n");
		printf("7. Exit\n");
	}
	else if(pthread_equal(id,tid[2])){
		system("clear");
		system("ls *.mp3");
	} 
	else if(pthread_equal(id,tid[3])){
		system("pkill mpg123"); 
		system("clear");
		snprintf(contain,50,"mpg123 %s",song);
		system(contain);
	}
	else if(pthread_equal(id, tid[4])){
		system("clear"); 
		system("pkill -STOP mpg123");
	}
	else if(pthread_equal(id, tid[5])){
		system("clear");
		system("pkill -CONT mpg123");
	}
	else if(pthread_equal(id,tid[6])){
		system("clear");
		system("pkill mpg123");
	}
	else if(pthread_equal(id,tid[7])){
		on = false;
	}
	return NULL;
}

int main(void){
	system("clear");
	printf("Pilihan\n");
		printf("1. Perintah\n"); 
		printf("2. Playlist\n");
		printf("3. Mainkan\n");
		printf("4. Pause\n");
		printf("5. Continue\n");
		printf("6. Stop\n");
		printf("7. Exit\n");
	int choices; 
	int err=0;
	while(on){
		scanf("%d", &choices);
		if (choices == 3)
			scanf("%s",song); 
		if (err!=0)
			printf("\n error thread : [%s]", strerror(err));
		else{
			err=pthread_create(&(tid[choices]), NULL, &playandcount, NULL);
		}
	} 
	return 0;
}
