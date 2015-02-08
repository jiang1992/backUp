#include <stdio.h>
#include "backup.h"
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <utime.h>
#include <string.h>
#include <sys/stat.h>

#define DIR_SIZE 200
void getFileName(char *sourcePath, char *fileName);
void appendString(char *string, char c);
void dirString( char *dirName, char *directory);
int main(){
	

	

	struct stat c;
	time_t t;

	
	//copyFile("/homes/jiang46/cs240/project3/project3_src/userData/cs240", "/homes/jiang46/cs240/project3/project3_src/cs240");
	//copyDir("asd","s");
	readFile("/homes/jiang46/cs240/project3/project3_src/userData/cs240");
	
}



void readFile( char *DEST){

	FILE *logNew = fopen( LOG_NEW_FILENAME,"r");
	if(logNew == NULL){printf("readFile Error"); exit(1);};
	int c = 0;

	char *string = malloc(sizeof(char) *DIR_SIZE);
	char *filePath = malloc(sizeof(char)*DIR_SIZE);
	strcpy(filePath, DEST);
	string[0] = '\0';

	while(1){
		
		c = fgetc(logNew);
		if(c == EOF){break;}
		else if( c == '\t' && (strcmp(string, "DT_REG") == 0||strcmp(string, "DT_DIR") == 0)){
			printf("data type: %s\n",string);
			string[0] = '\0';
		
		}
		else if(c == '\n'){
			printf("fileName; %s\n", string);
		//	dirString(string,filePath);
		//	printf("filePath: %s\n", filePath);
			string[0] = '\0';
		}
		else if(c == ' '){
			string[0] = '\0';
		}
		else{
			appendString(string, c);
		
		
		}
	}
}
void dirString( char *dirName, char *directory){
	
	int len = strlen(dirName);
	int dirLen = strlen( directory );

	char temp[len];

	strcpy(temp, dirName);
	int i = 0;

	for( i = 0; i < dirLen; i++){
	
	    dirName[i] = directory[i];
	
	}
	dirName[i] = '/';
	int k = i;
	int j = 0;
	for( j = 0; i < k + len; i++ ){
	
	    dirName[i+1] = temp[j];
	    j++;
	}
	dirName[i+1] = '\0';
}













int copyDir(char *source, char *backupDir){

	time_t current_time;
	time(&current_time);
	struct tm *myTime = localtime(&current_time);
	printf("%i", myTime->tm_mon);
}


int copyFile( char *sourcePath, char *destinationPath){
	
	FILE *fSource = fopen(sourcePath, "r");
	FILE *fDest = fopen(destinationPath, "w");
	if(fSource == NULL || fDest == NULL){ printf("error");return 0;}
	fclose(fDest);
	fDest = fopen(destinationPath, "a");
	int *buf = malloc(sizeof(buf));
	while(!feof(fSource)){
		
		fread(buf,1,1,fSource);
		fwrite(buf,1,1,fDest);
	}
	fclose(fDest);
	fclose(fSource);
	return 1;

}



void getFileName(char *sourcePath, char *fileName){
	
	char *string = malloc(sizeof(char) * 50);
	int i = 0;
	int len = strlen(sourcePath);
	for( i=0; i<len; i++){
		
		if(sourcePath[i] == '/'){
			string[0] = '\0';
		}
		else if( sourcePath[i] != '/'){
			//string[i] = sourcePath[i];
			appendString(string, sourcePath[i]);
		}
	}
	printf("%s",string);
}
void appendString(char *string, char c){
	
	int len = strlen(string);
	string[len] = c;
	string[len+1] = '\0';

}

