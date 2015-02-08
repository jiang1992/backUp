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
#define ERROR_STRING "something wrong"
#define ERROR_STRING2 "something wrong 2"
#define ARGC_SIZE 500
#define ARGC_SIZE_BIGGER 200
#define DIR_SIZE 500
#define _REG "DT_REG"
#define _DIR "DT_DIR"

void printAll( char *BACK_UP_DIR, char *DEST, int MAX);
void createLog( char *BACK_UP_DIR, char *DENT);
void recurOpenDir( struct dirent *ent, FILE *fp, char *directory, int tab);
void writeToLog( struct dirent *ent, FILE *fp, int dirOrReg, int tab,int size, char *fileName);
void dirString( char *dirName, char *directory);
int filter( char *dir);
void appendString(char *string, char c);
char *BACK_UP_DIR;
void noEnter(char *timeStamp);
void getTimeString( char *timeString);
void recurCreateDir( struct dirent *ent, FILE *fp, char *directory, int tab, char *dupFilePath);
void ifDot(char *s1);
void addSlash(char *s1);
int copyFilecopyFile(char *s1, char *s2);
void addDot(char *s1);
void recurDeleteDir( struct dirent *ent, char *directory, int tab);
void checkArgs(char *DEST, char *destTemp);

int main(int argv, char **argc){

    BACK_UP_DIR = malloc(ARGC_SIZE);
    char DEST[ARGC_SIZE];
    int  MAX = DEFAULT_MAX_BACKUPS;
    strcpy(DEST, DEFAULT_DEST_DIR);
//    strcpy(BACK_UP_DIR,DEFAULT_SOURCE);	
    
    int i = 1;
    int isValid = 0;
    char *inputTemp = malloc(sizeof(char)*DIR_SIZE);   if(inputTemp == NULL){ printf("mallocFail");exit(1);}
    char *destTemp = malloc(sizeof(char)*DIR_SIZE);   if(destTemp == NULL){ printf("mallocFail");exit(1);}


    for( i = 1; i < argv; i++){


	if(strcmp(argc[i], "-s") == 0){
			
	    if(i == argv){
		
		fprintf(stderr,"source wrong");
		exit(1);
		
	    }
	    else{
	    	i++;
	    	isValid = 1;
		strcpy(inputTemp, argc[i]);
		addDot(inputTemp);
		ifDot( inputTemp);
	    	strcpy(BACK_UP_DIR, inputTemp);
	    }	
			
	}
	else if( strcmp(argc[i], "-d") == 0){
	    
	    if( i == argv){
		
	        fprintf(stderr,"destination wrong %s", argc[i]);
		exit( 1);

	    }
	    else{
	    	i++;
		strcpy(destTemp, argc[i]);
	        strcpy(inputTemp, argc[i]);
		addDot(inputTemp);
		ifDot( inputTemp);
		strcpy(DEST, inputTemp);
	    }

	}
	else if( strcmp(argc[i], "-m") == 0){

	    if( i == argv|| isNumber(argc[++i]) == 0){
		
	        fprintf(stderr,ERROR_STRING2);
		exit(1);

	    }
	    else{
		MAX = atoi(argc[i]);
            }

	}
	else{
	    fprintf(stderr,"wrong arguments %d %s", i,argc[i]);
	    exit(1);
	}

    }

    if( isValid == 0){
		
	fprintf(stderr,"no -s");
	exit( 1);

    }

//	printf("%s",DEST);
    addSlash(DEST);
    addSlash(BACK_UP_DIR);
//    printf("%s\n", DEST);

    checkArgs(DEST, destTemp);
 //       printf("%s\n", DEST);

	FILE *newFile, *oldFile;
	createLog(BACK_UP_DIR,DEST);
   	if(MAX == 0){return 0;}	
	char *filePath = malloc( DIR_SIZE*sizeof(char));     if(filePath == NULL){ printf("mallocFail");exit(1);}

	char *filePath2 = malloc( DIR_SIZE*sizeof(char));   if(filePath2 == NULL){ printf("mallocFail");exit(1);}

	strcpy(filePath, LOG_LAST_FILENAME);
	dirString(filePath, DEST);
	newFile = fopen(filePath, "r"); 
   
	strcpy(filePath2, LOG_NEW_FILENAME);
	dirString(filePath2, DEST);
	oldFile = fopen(filePath2, "r");  
	
	if( newFile == NULL){
		i = 0;
		newFile = fopen(filePath, "w");
		copyFile(filePath2, filePath);
		//fclose(oldFile);
	}
	else{ 
		i = compareLog(newFile, oldFile);
	//	printf("%d",i);
		fclose(newFile);
		fclose(oldFile);

	}
	if( i == 0 || oldFile == NULL){
		copyDir(BACK_UP_DIR, DEST);
		copyFile(filePath2, filePath);




	}
	
	
	int backup = 0;
	backup = getNumberOfBackup(DEST);
	int num = backup - MAX;
	//printf("%d", num);
	for( i = 0; i < num; i++){

	//printf("%s", DEST);

		removeOldestBackup(DEST);
	//	printf("%d\n",i);	
	
	
	}

}

void checkArgs( char *s1, char *s2){

	int i = 0;
	int len = strlen(s1);

	for(i = 0; i<len;i++){
		if(s1[i]=='/' &&s1 [i+1]== '/'){
			strcpy(s1, s2);
		}
		else{
		
		}
	}
	
}

void addSlash( char *s1){   //modifiy input

	if(s1[0] != '/'){
		
		char *temp = malloc(sizeof(char) * DIR_SIZE);  if(temp == NULL){ printf("mallocFail");exit(1);}

		
		strcpy(temp, s1);

		int i = 0;
		int len = strlen(s1);
		s1[0] = '/';
		for(i = 0; i<len; i++){
			s1[i+1] = temp[i];
		}
		s1[i+1] = '\0';
	}
	

}

int firstCharCom(char *s1){    
	
	if(s1[0] == '/' || s1[0] == '.'){return 1;}
	else{
		
		addDot(s1);
		return 1;
	}

}

void addDot(char *s1){
	
	char *temp = malloc(sizeof(char) * DIR_SIZE);  if(temp == NULL){ printf("mallocFail");exit(1);}

	
	strcpy(temp, s1);

	int i = 0;
	int len = strlen(s1);
	
	s1[0] = '.';
	s1[1] = '/';
	for( i = 0; i < len; i ++){
		
		s1[i+2] = temp[i];
	
	}
	s1[i+2] = '\0';
	free(temp);
}
int isNumber(char *s1){
	
	int i = 0;
	int len = strlen(s1);
	for(i=0; i<len; i++){
		if( s1[i] >= '0' && s1[i] <= '9'){}
		else{return 0;}
	}	
	return 1;

}

void printAll( char *BACK_UP_DIR, char *DEST, int MAX){
	
	printf("\n-s: %s\n", BACK_UP_DIR);
	printf("-d: %s\n", DEST);
	printf("-m: %d\n", MAX);

}
void createLog( char *BACK_UP_DIR, char *DEST){
	
	FILE *fp;
	char *filePath = malloc(sizeof(char) * DIR_SIZE);   if(filePath == NULL){ printf("mallocFail");exit(1);}

	strcpy(filePath, LOG_NEW_FILENAME);
	dirString( filePath,DEST);
	fp = fopen( filePath, "w");
	char *directory = malloc(DIR_SIZE * sizeof(char));    if(directory == NULL){ printf("mallocFail");exit(1);}
       //// Here's the string of the directory	
	int listLen = 0;
	struct dirent **dirList;

	listLen = scandir(BACK_UP_DIR, &dirList, NULL, alphasort);
	int tab = -1;
	while(listLen-- && listLen>=0){
	  if( strcmp(dirList[listLen]->d_name, ".")==0||strcmp(dirList[listLen]->d_name, "..")==0){}
	  else{
	    
	      strcpy(directory, BACK_UP_DIR);
	      recurOpenDir( dirList[listLen] , fp, directory, tab);
	
	  }
	}
	fclose(fp);
	free(filePath);
	free(directory);
}

void recurOpenDir( struct dirent *ent, FILE *fp, char *directory, int tab){

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
	if( dir == NULL){
		tab++; 
		size = getFileSize(tempName);
		writeToLog(ent, fp, 1, tab, size,tempName);
	}
	else{	
		tab++;
		writeToLog(ent, fp, 0, tab, size,tempName);
		
		listLen = scandir(tempName, &dirList, NULL, alphasort);
		
		while( listLen-- && listLen>=0){			
			recurOpenDir( dirList[listLen], fp, tempName, tab);			
		}
	}
		
}

void writeToLog( struct dirent *ent, FILE *fp, int dirOrReg, int tab, int size, char *tempName){
	
	
	int i = 0;
	
	if(fp == NULL){ exit(1);}
	else{
		
		
		struct stat c;
		time_t tCreation;
		time_t tLast;
		stat(tempName, &c);
		tLast = c.st_mtime;
		tCreation = c.st_mtime;
		char *timeStampC = malloc(sizeof(char) * 50);   if(timeStampC == NULL){ printf("mallocFail");exit(1);}

		strcpy(timeStampC, ctime(&tCreation)); noEnter(timeStampC);
		char *timeStampM = malloc(sizeof(char) * 50); if(timeStampM == NULL){ printf("mallocFail");exit(1);}

		strcpy(timeStampM, ctime(&tLast));     noEnter(timeStampM);
		if(dirOrReg == 0){
			for(i=0;i<tab;i++){fprintf(fp,"\t");}
		//	printf("%s\t%d\t%s\t%s\t%s\n",_DIR,size, timeStampC, timeStampM, ent->d_name);
			fprintf(fp,"%s\t%d\t%s\t%s\t%s\n",_DIR,size, timeStampC, timeStampM, ent->d_name);
		}
		else if(dirOrReg == 1){
			for(i=0;i<tab;i++){fprintf(fp,"\t");}
			//printf("%s\t%d\t%s\n", _REG, size,ent->d_name); 
			//fprintf(fp,"%s\t%d\t%s\n", _REG, size,ent->d_name); 
	//		printf("%s\t%d\t%s\t%s\t%s\n",_REG,size, timeStampC, timeStampM, ent->d_name);
			fprintf(fp,"%s\t%d\t%s\t%s\t%s\n",_REG,size, timeStampC, timeStampM, ent->d_name);

		}
	}
}
void noEnter(char *timeStamp){
	
	int len = strlen(timeStamp);
	int i = 0;
	while(1){
		
		if(timeStamp[i] == '\n'){
			
			timeStamp[i] = '\0';
			break;
		}
		else{i++;}
	
	}
}
int getFileSize( char *fileName){
	
	FILE *file;
	file = fopen(fileName, "r");
	fseek(file, 0L, SEEK_END);
	int size = ftell(file);

	return size;
}
/*
void filterSlash( char *s1, char *s2){
	
	if(){

	}


}
*/

void dirString( char *dirName, char *directory){
	
	int len = strlen(dirName);
	int dirLen = strlen( directory );

	char temp[len];

	strcpy(temp, dirName);
	int i = 0;

	for( i = 0; i < dirLen; i++){
	
	    dirName[i] = directory[i];
	
	}
	if( dirName[i-1] != '/'){
		dirName[i] = '/';
		int k = i;
		int j = 0;
		for( j = 0; i < k + len; i++ ){
	
		    dirName[i+1] = temp[j];
		    j++;
		}
		if(dirName[i] == '/'){dirName[i] = '\0';}
		dirName[i+1] = '\0';
	}
	else{

		dirName[i] = '/';
		int k = i;
		int j = 0;
		for( j = 0; i < k + len; i++ ){
	
		    dirName[i] = temp[j];
		    j++;
		}
		if(dirName[i-1] == '/'){dirName[i-1] = '\0';}
		dirName[i] = '\0';
	


	}
	
}

int compareLog( FILE *oldLogFile, FILE *newLogFile){
	
	char c1 = 0;
	char c2 = 0;
	while(1){
		
		c1 = fgetc(oldLogFile);
		c2 = fgetc(newLogFile);
		
		if(c1 != c2){
		//	printf("c1: %c != c2: %c\n", c1, c2);
		//	printf(" c1:    %d       c2:      %d\n", atoi(c1), atoi(c2));
			break;
		}
		else if(c1 == c2 && c1 == EOF && c2 == EOF){
		//	printf("c1: %c || c2: %c\n", c1, c2);

			return 1;
		}

		else if( c1 == 	EOF){
		//	printf("c1: %c || c2: %c\n", c1, c2);
			printf("2");
			break;
		}
		else if(c2 == EOF){
		//	printf("c1: %c || c2: %c\n", c1, c2);
		
			break;
			printf("3");
		}
		else{
		//	printf("ELSE: c1: %c || c2: %c\n", c1, c2);

		}
	}
//	printf("yeweh\n");
	return 0;
}

int copyFile( char *sourcePath, char *destinationPath){
	
	FILE *fSource = fopen(sourcePath, "r");
	FILE *fDest = fopen(destinationPath, "w");
	if(fSource == NULL){ /*printf("error fSource");*/return 0;}
	else if(fDest == NULL){ /*printf("error fDest");*/ return 0;}
//	fclose(fDest);
//	fDest = fopen(destinationPath, "a");
	int c = 0;
	int *buf = malloc(sizeof(buf));  if(buf == NULL){ printf("mallocFail");exit(1);}

	while(1){
		
		c = fgetc(fSource);
		if(c == EOF){break;}

		fputc(c, fDest);
		/*
		fread(buf,1,1,fSource);
		
		fwrite(buf,1,1,fDest);*/
		//if(buf == EOF){break;}
	}		

	//*buf = EOF;
	//fwrite(buf,1,1,fDest);
//	free(buf);    /////11111111111111111111111111111111111111111111111111111111111111111111111111111111111111
	fclose(fDest);
	fclose(fSource);
	return 1;
}

/*
void getFileName(char *sourcePath, char *fileName){
	
	char *string = malloc(sizeof(char) * 50);
	int i = 0;
	int len = strlen(sourcePath);
	for( i=0; i<len; i++){
		
		if(sourcePath[i] == '/'){
			string[0] = '\0';
		}
		else if( sourcePath[i] != '/'){

			appendString(string, sourcePath[i]);
		}
	}
//	printf("%s",string);

}
*/
void appendString(char *string, char c){
	
	int len = strlen(string);
	string[len] = c;
	string[len+1] = '\0';

}
/*
void readFile(char *DEST){



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
		else if( c == '\t' && (strcpy(string, "DT_REG") == 0||strcpy(string, "DT_REG") == 0)){
		
			string[0] = '\0';
		
		}
		else if(c == '\n'){
		
			strcpy(string, filePath);

		}
	}
}
*/
int copyDir( char *BACK_UP_DIR, char *DEST){

	FILE *fp = NULL;
	char *fileName = malloc(sizeof(char) * DIR_SIZE);   if(fileName == NULL){ printf("mallocFail");exit(1);}

	char *fileName3 = malloc(sizeof(char) * DIR_SIZE); if(fileName3 == NULL){ printf("mallocFail");exit(1);}

	getTimeString(fileName);
	dirString(fileName, DEST);
	mkdir(fileName,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	char *directory = malloc(DIR_SIZE * sizeof(char));         if(directory == NULL){ printf("mallocFail");exit(1);}  //// Here's the string of the directory	
	int listLen = 0;
	struct dirent **dirList;

	listLen = scandir(BACK_UP_DIR, &dirList, NULL, alphasort);
	int tab = -1;
	while(listLen-- && listLen>=0){
		if( strcmp(dirList[listLen]->d_name, ".")==0||strcmp(dirList[listLen]->d_name, "..")==0){}
	   	else{

	       		 strcpy(fileName3, dirList[listLen]->d_name);
	       		 dirString(fileName3, fileName);
	      		// printf("%s\n",fileName3);
	      		 strcpy(directory, BACK_UP_DIR);
	      		 recurCreateDir( dirList[listLen] , fp, directory, tab, fileName3);	
	   	}
	 }
	free(fileName);free(fileName3);free(directory);	/////////////////////////////////////free
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

	char *fileName = malloc(DIR_SIZE*sizeof(char));   if(fileName == NULL){ printf("mallocFail");exit(1);}
	char *fileName3 = malloc(DIR_SIZE* sizeof(char)); if(fileName3 == NULL){ printf("mallocFail");exit(1);}

	strcpy(fileName, dupFilePath);
	if( dir == NULL){
		tab++; 
		copyFile(tempName, dupFilePath);     //THINGS I JUST ADDED

	}
	else{	
		tab++;	
		listLen = scandir(tempName, &dirList, NULL, alphasort);
		mkdir(fileName,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
		while( listLen-- && listLen>=0){
			strcpy(fileName3, dirList[listLen]->d_name);
	       		dirString(fileName3, fileName);
	      		//printf("%s\n",fileName3);
			recurCreateDir( dirList[listLen], fp, tempName, tab,fileName3);
			
		}
	}
	free(fileName);free(fileName3);		
}
void getTimeString( char *timeString){
	
	time_t timeNow;
	time(&timeNow);
	struct timeval tv;
	struct tm *tm;

	char buf[72];// = malloc(sizeof(char) * DIR_SIZE);
	char fmt[72];// = malloc(sizeof(char) * DIR_SIZE);
	gettimeofday(&tv, NULL);
	if((tm = localtime(&tv.tv_sec)) != NULL){
		
		strftime(fmt, sizeof fmt,"%Y-%m-%d-%H-%M-%S",tm);
		snprintf(buf, sizeof buf, fmt, tv.tv_sec);
		//printf("%s\n",buf);
		strcpy(timeString, buf);
	}
//	exit(1);
//	free(buf);
//	free(fmt);
//	struct tm *myTime = localtime(&timeNow);
//	strcpy(timeString, asctime(myTime));
//	printf ( "%s", asctime (myTime) );
//	printf("%i", myTime->tm_wday);
}
void ifDot( char *s1){
	
	if(s1[0] == '.'){
			
		char *temp = malloc(sizeof(char) * DIR_SIZE); if(temp == NULL){printf("malloc fail"); exit(1);}
		getcwd(temp, 100);
		//strcpy(temp, get_current_dir_name());
		//printf("%s",temp);
		//
		//
		int i = 0;
		int len = strlen(temp);
		char *temp2 = malloc(sizeof(char) * DIR_SIZE); if(temp2 == NULL){ printf("malloc Fail");exit(1);}

		for( i = 0; i < len; i++){
			
			temp2[i] = temp[i];
		
		}

		temp2[i] = '\0';
		len = strlen(s1);
		int j = 0;
		for( j = 0; j < len; j++){

			temp2[i+j] = s1[j+1];
		
		}
		temp2[i+j] = '\0';
		strcpy(s1, temp2);
	//	printf("%s",s1);

		free(temp);free(temp2);
	}
	else{}
	
	

}






///////////////////////


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
	char *fileName = malloc(sizeof(char) * DIR_SIZE); if(fileName == NULL){ printf("malloc fail"); exit(1);}
	char *tempName = malloc(sizeof(char) * DIR_SIZE); if(tempName == NULL){ printf("malloc fail"); exit(1);}
//	printf("remove\n");
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
	free(fileName);free(tempName);
	return 1;

}

void recurDeleteDir( struct dirent *ent, char *directory, int tab){

  	if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name,"..") == 0){return;}
	struct dirent **dirList;
	int listLen = scandir(directory, &dirList, NULL, alphasort);
	DIR *dir;
	int i = 0;
	char *tempName = malloc(sizeof(char)*DIR_SIZE); if(tempName == NULL){ printf("MALLOC FAIL"); exit(1);}
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



