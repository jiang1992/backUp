#include<stdio.h>
#include"backup.h"
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<utime.h>
#include<time.h>
#include<unistd.h>
#define ERROR_STRING "something wrong"
#define ERROR_STRING2 "something wrong 2"
#define ARGC_SIZE 50
#define ARGC_SIZE_BIGGER 200
#define DIR_SIZE 500
#define FILE_SIZE 200000
#define _REG "DT_REG"
#define _DIR "DT_DIR"
void recurDeleteDir( struct dirent *ent, char *directory, int tab);
void dirString( char *dirName, char *directory);
int getNumberOfBackup( char *destinationDir);
//void copyDir( char *BACK_UP_DIR, char *DEST);
void recurCreateDir( struct dirent *ent, FILE *fp, char *directory, int tab, char *dupFilePath);
void recurDeleteDir( struct dirent *ent, char *directory, int tab);

int main(){


	//copyDir(DEFAULT_SOURCE, DEFAULT_DEST_DIR);
//	getNumberOfBackup("homes/jiang46/cs240/project3/project3_src/2");
//	int i = remove("/homes/jiang46/cs240/log.new");
//	removeOldestBackup(DEFAULT_DEST_DIR);
	

	removeOldestBackup("/homes/jiang46/cs240/project3/project3_src/new");
	//printf("%d",getNumberOfBackup("/homes/jiang46/cs240/project3/project3_src/new"));
//getNumberOfBackup
//	printf("%d",i);
}

int getNumberOfBackup( char *destinationDir){

	int listLen = 0;
	struct dirent **dirList;
	listLen = scandir(destinationDir, &dirList, NULL, alphasort);
	int i = 0;
	int numberOfFile = 2; //this is NOT magic number. This is the "." and ".." files
	for( i = 0; i < listLen; i++){
		
		if( strcmp(dirList[i]->d_name, "log.new") == 0){
			
			numberOfFile = numberOfFile + 1;
		
		}
		else if(strcmp(dirList[i]->d_name, "log.last") == 0){
			
			numberOfFile = numberOfFile + 1;
		
		}

	}

	return listLen - numberOfFile;
}
int getNumberOfFile( char *destinationDir){

	int listLen = 0;
	struct dirent **dirList;
	listLen = scandir(destinationDir, &dirList, NULL, alphasort);
	int i = 0;
	int numberOfFile = 2; //this is NOT magic number. This is the "." and ".." files

	return listLen - numberOfFile;
}



int removeOldestBackup( char *destinationDir){
	
//	FILE *fp = NULL;
	int tab = 0;
	char *fileName = malloc(sizeof(char) * DIR_SIZE);
	char *tempName = malloc(sizeof(char) * DIR_SIZE);
	printf("remove\n");
	struct dirent **dirList;
	int listLen = scandir(destinationDir, &dirList, NULL, alphasort);
	int i =0;// listLen;
//	listLen = 0;
//	printf("%d",listLen);
	//while( i++  && i < listLen){
	while(1){
	for(i = 0; i < listLen; i++){
	//	printf("xx/n");
		if(strcmp(dirList[i]->d_name, ".") ==0|| strcmp(dirList[i]->d_name, "..") == 0){}
		else{
			
		//	printf("%s\n", dirList[i]->d_name);
			strcpy(tempName, dirList[i] -> d_name);
			dirString(tempName, destinationDir);
		//	printf("%s\n", tempName);
			recurDeleteDir(dirList[i], tempName, tab);
			break;
		}
	}
		int p = remove(tempName);
		if(p != -1){break;}
	}
	return 1;

}

void recurDeleteDir( struct dirent *ent, char *directory, int tab){

  	if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name,"..") == 0){return;}
	struct dirent **dirList;
	int listLen = scandir(directory, &dirList, NULL, alphasort);
	DIR *dir;
	int i = 0;
	char *tempName = malloc(sizeof(char)*DIR_SIZE);
//	printf("directory: %s\n", directory);

//	strcpy(fileName, directory);
	for( i = 0; i<listLen; i++){
		if(strcmp(dirList[i] -> d_name, ".") == 0 || strcmp(dirList[i]->d_name, "..")==0){}
		else{
			strcpy(tempName, dirList[i]->d_name);
			dirString(tempName, directory);
			dir = opendir(tempName);
			if( dir == NULL){
			//	printf("ABOUT TO REMOVE A FILE%s\n", tempName);
				remove(tempName);
			}
			else{
				
				if(getNumberOfFile(tempName) == 0){ /*printf("Empty File: %s\n", dirList[i]->d_name); */remove(tempName);}
				else{
					
				//	printf("File: %s  has %d\n", tempName getNumberOfFile(tempName));
					recurDeleteDir(dirList[i], tempName, 0);			
				}	
			}
		//	printf("%s\n", tempName);
		}
	}
	free(tempName);

}










































































int copyDir( char *BACK_UP_DIR, char *DEST){

//	time_t time;

	FILE *fp = NULL;
	char *fileName = malloc(sizeof(char) * DIR_SIZE);
//	char *fileName2 = malloc(sizeof(char) * DIR_SIZE);
	char *fileName3 = malloc(sizeof(char) * DIR_SIZE);
	getTimeString(fileName);
	dirString(fileName, DEST);
	mkdir(fileName,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	char *directory = malloc(DIR_SIZE * sizeof(char));          //// Here's the string of the directory	
	int listLen = 0;
	struct dirent **dirList;

	listLen = scandir(BACK_UP_DIR, &dirList, NULL, alphasort);
	int tab = -1;
	while(listLen-- && listLen>=0){
		if( strcmp(dirList[listLen]->d_name, ".")==0||strcmp(dirList[listLen]->d_name, "..")==0){}
	   	else{
	     		//printf("%s\n", dirList[listLen]->d_name);
//	      		 strcpy(fileName2, fileName);
	       		strcpy(fileName3, dirList[listLen]->d_name);
	       		dirString(fileName3, fileName);
	      		 printf("%s\n",fileName3);
	      		 strcpy(directory, BACK_UP_DIR);
	     		 // filter(dirList[listLen], directory);
	      		 recurCreateDir( dirList[listLen] , fp, directory, tab, fileName3);	
	   	}
	 }
}


void recurCreateDir( struct dirent *ent, FILE *fp, char *directory, int tab, char *dupFilePath){

  	if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name,"..") == 0){return;}

	DIR *dir;
	struct dirent *temp;
	char tempName[ARGC_SIZE_BIGGER];                       //ADDING STRING TO THE END OF THE DIR STRING!!!
	strcpy(tempName, ent->d_name);
	dirString(tempName, directory);                        
	dir = opendir(tempName);
	int listLen = 0;
	struct dirent **dirList;
	int size = 4096;

	char *fileName = malloc(DIR_SIZE*sizeof(char));
	char *fileName3 = malloc(DIR_SIZE* sizeof(char));
	strcpy(fileName, dupFilePath);
	if( dir == NULL){
		tab++; 
		copyFile(tempName, dupFilePath);     //THINGS I JUST ADDED
	//	size = getFileSize(tempName);
	//	writeToLog(ent, fp, 1, tab, size,tempName);
	}
	else{	
		tab++;
	//	writeToLog(ent, fp, 0, tab, size,tempName);
		
		listLen = scandir(tempName, &dirList, NULL, alphasort);
		mkdir(fileName,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
		while( listLen-- && listLen>=0){
			strcpy(fileName3, dirList[listLen]->d_name);
	       		dirString(fileName3, fileName);
	      		printf("%s\n",fileName3);
			recurCreateDir( dirList[listLen], fp, tempName, tab,fileName3);
			
		}
	}		
}
void getTimeString( char *timeString){
	
	time_t timeNow;
	time(&timeNow);
	struct tm *myTime = localtime(&timeNow);
	int year = myTime->tm_year + 1990;
	int month = myTime->tm_mon + 1;
	int day = myTime->tm_mday;
	int hour = myTime->tm_hour;
	int min = myTime->tm_min;
	int sec  = myTime->tm_sec;
	strcpy(timeString, asctime(myTime));
	printf ( "%s", asctime (myTime) );
	//printf("%i", myTime->tm_wday);
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

