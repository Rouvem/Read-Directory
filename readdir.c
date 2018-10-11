#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_FILE_NAME 2048

static unsigned int total_files_and_folders = 0;
static unsigned int total_files = 0;
static unsigned int total_folders = 0;
static FILE *fp;
static char cwd[MAX_FILE_NAME];
static char **globalArgv;
static int printIndexArray[100];
static int printCounter = 0;
static int counted = 0;

void getDirName(char *str, const unsigned int size) {
	char temp[MAX_FILE_NAME];
	strcpy(str, "/cygdrive/c");
	getcwd(temp, MAX_FILE_NAME);
	strcat(str, temp);
	//printf("got cwd: %s\n");
	//printf("temp: %s\n");
}
void clearName(char *str, const unsigned int size) {
	int i;
	for( i = 0; i < size; i++)
	{
		str[i] = '\0';
	}
}

int num_files(DIR *dr, const char *cwd) {
	//getcwd(cwd, MAX_FILE_NAME);
	//printf("cwd: %s\n", cwd);
	//clearName(cwd, MAX_FILE_NAME);
	if(dr == NULL) {
		#ifdef DEBUG
		printf("Empty NULL directory\n");
		#endif //DEBUG
		return 0;
	}
	else {
		struct dirent *de;
		while(((de = readdir(dr)) != NULL)) {
			if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
				total_files_and_folders++;
				//printf("%s\n", de->d_name);
				if(de->d_type == DT_DIR) {
					char tempCwd[MAX_FILE_NAME];
					strcpy(tempCwd, cwd);
					strcat(tempCwd, "/");
					strcat(tempCwd, de->d_name);
					//printf("Folder name: %s\n", tempCwd);
					//chdir(cwd);
					DIR *drTemp = opendir(tempCwd);
					if(drTemp == NULL) {
						printf("Couldn't open: %s\n", tempCwd);
					}
					else {
						total_folders++;
					}
					num_files(drTemp, tempCwd);
					closedir(drTemp);
				} else {
					total_files++;
				}
			}
		}
	}
}

int print_files(DIR *dr, const char *cwd) {

	if(dr == NULL) {
		#ifdef DEBUG
		//printf("Empty NULL directory\n");
		#endif //DEBUG
		return 0;
	}
	else {
		struct dirent *de;
		while(((de = readdir(dr)) != NULL)) {
			if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
				printCounter++;
				int x;
				if(counted >= 0 && counted < 100) {
					if(printCounter == printIndexArray[counted]) {
						fprintf(stderr, "#");
						counted++;
					}
				}
				char tempCwd[MAX_FILE_NAME];
				strcpy(tempCwd, cwd);
				strcat(tempCwd, "/");
				strcat(tempCwd, de->d_name);
				
				char temp_file_name[MAX_FILE_NAME];
				strcpy(temp_file_name, tempCwd);
				strcat(temp_file_name, "\n");
				fputs(temp_file_name, fp);
				/*
				char temp_file_name[MAX_FILE_NAME];
				strcpy(temp_file_name, de->d_name);
				strcat(temp_file_name, "\n");
				fputs(temp_file_name, fp);*/
				if(de->d_type == DT_DIR) {
					
					//chdir(cwd);
					DIR *drTemp = opendir(tempCwd);
					if(drTemp == NULL) {
						//printf("Couldn't open: %s\n", tempCwd);
					}
					else {
						//total_folders++;
					}
					print_files(drTemp, tempCwd);
					closedir(drTemp);
				} else {
					//total_files++;
				}
			}
		}
	}
}

int main(int argc, char ** argv) {
	globalArgv = argv;
	struct dirent *de;
	//remove("Report.dat");
	fp = fopen("Report2.dat", "w+");
	if(fp == NULL) {
		printf("Error creating and opening file Report.dat\n");
	}
	DIR *dr = opendir(argv[1]);
 	char temp[MAX_FILE_NAME];
	strcpy(temp, argv[1]);
	if(dr == NULL){
		printf("Could not open directory: %s\n", argv[1]);
		return 0;
	} else {
		printf("Opened directory %s\n", argv[1]);
	}
	//total_files_and_folders++;

	num_files(dr, temp);
	printf("Total Files: %d\n", total_files);
	printf("Total Folders: %d\n", total_folders);
	printf("Total Files and Folders: %d\n", total_files_and_folders);
	closedir(dr);
	
	dr = opendir(argv[1]);
	
	
	int Calculate = total_files_and_folders / 100;
	int i;
	int tempCalc = Calculate;
	for(i = 0; i < 100; i++) {
		printIndexArray[i] = tempCalc;
		tempCalc += Calculate;
		//printf("Index, Value: %d %d\n", i, printIndexArray[i]);
	}
	
	printf("Writing data to files... ...\n");
	fprintf(stderr, "|____________________________________________________________________________________________________|");
	fprintf(stderr, "\r|");
	print_files(dr, temp);
	/*while((de = readdir(dr)) != NULL) {
		printf("%s\n", de->d_name);
	}*/
	closedir(dr);
	fclose(fp);
	return 0;
}
