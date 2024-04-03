/*
* FILE : m4.cpp
* Project : Major4
* Programmer : Minchul Hwang
* FirstVersion : 2022-11-28
* Description : This program starts by loading the contents of a file into a variable.
*				The first file to be loaded is a file called teams.txt, and the contents in it are the names of each team and .txt.
*				In addition, the team name file contains the team's game results, and this is also imported as a variable.
*				It outputs the contents called as variables to the output according to the form.
* The functions in this file are used to processGames() and parsrLine()
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#pragma warning(disable: 4996)
#define kteamString 40

int processGames(char fileName[kteamString]);
int parseLine(char resultOfGame[kteamString], char enemyName[kteamString], char* homeScore, char* enemyScore);

struct team {
	int structHomeScore;
	char structHomeTeam[kteamString];
	char structHomeTeamTxt[kteamString];
	int structEnemyScore;
	char structEnemyTeam[kteamString];
} info;


int main() {
	FILE* teams = NULL;
	char record[kteamString] = { };
	int processValue = 0;
	int i = 0, j = 0;
	int line = 0;

	teams = fopen("teams.txt", "rt");
	
	if (teams == NULL) {
		printf("There is no teams.txt file.\n");
		return 1;
	}
	for (i=0; i < kteamString; i++) {
		fgets(record, sizeof record, teams);
		char temp_homeName[kteamString] = { };  //Declare to save Home Team Name temporary
		char homeName[kteamString] = { };

		if (feof(teams) != 0) {		//Check the end of file.
			break;
		}
		if (ferror(teams) != 0) {		//Check the error of the file.
			printf("Error to read teams.txt\n");
			if ((fclose(teams) != 0)) {
				printf("Can't close close teams.txt file\n");
			}
		}

		for (j = 0; record[j] != '.'; j++) {
			homeName[j] = record[j];		//Save Home Team name before .txt\n
		}
		strncpy(info.structHomeTeam, homeName, j+1);

		strncpy(info.structHomeTeamTxt, info.structHomeTeam, j + 1);
		strcat(info.structHomeTeamTxt, ".txt");

		printf("Processing %s: \n", info.structHomeTeamTxt);	//Home Team Name.txt(letter itself)

		processValue = processGames(info.structHomeTeam);	//Home Team Name(filename txt extension)

		if (processValue == 1) {
			printf("This program does not work perfectly\n");
			printf("\n");
			
		}
	}

	if (fclose(teams) != 0) {
		printf("Can't close teams.txt file\n");
		return 1;
	}

	return 0;
}


/*
* Fucntion : processGames()
* Description : This function plays the role of loading the contents of each line of the teams file into a variable.
*				and calculate the imported variable to calculate what the team's result will be.
* Parameters : char fileName[teamString]
* Returns : 0 if this function work perfectly.
*			1 if this funcion has an error.
*/
int processGames(char fileName[kteamString]) {
	char record[kteamString] = { };
	char* enemyName[kteamString] = { };
	char* enemyScore[kteamString] = { };
	char* homeName[kteamString] = { };
	char* homeScore[kteamString] = { };
	int i = 0;
	double resultCalculation = 0, win = 0, tie = 0, loss = 0;
	
	FILE* fp = fopen(info.structHomeTeamTxt, "rt");
	
	if (fp == NULL) {
		printf("Can't open file\n");
		return 1;
	}

	for (i = 0; i < kteamString; i++) {
		int returnValue = 0;
		
		fgets(record, sizeof record, fp);
		
		if (feof(fp) != 0) {		//Check the end of file.
			
			break;
		}
		
		if (ferror(fp) != 0) {		//Check the error of the file.
			printf("Error reading from file\n");
			if ((fclose(fp) != 0)) {
				printf("Can't close file reading\n");
			}
			return 1;
		}

		returnValue = parseLine(record, *enemyName, *homeScore, *enemyScore);
		
		if (returnValue == 1) {
			printf("\t%s beat %s %d-%d\n", info.structHomeTeam, info.structEnemyTeam, info.structHomeScore, info.structEnemyScore);
			win++;
			win =+ win;
		}
		else if (returnValue == 2) {
			printf("\t%s and %s tied at %d\n", info.structHomeTeam, info.structEnemyTeam, info.structHomeScore);
			tie++;
			tie =+ tie;
		}
		else if (returnValue == 3) {
			printf("\t%s lost to %s %d-%d\n", info.structHomeTeam, info.structEnemyTeam, info.structEnemyScore, info.structHomeScore);
			loss++;
			loss =+ loss;
		}
		else {
			printf("There is an Error in file!\n");
			return 1;
		}

		resultCalculation = (2 * win + tie) / (2 * (win + loss + tie));
	}

	if (fclose(fp) != 0 ) {
		printf("Can't close file.\n");
		return 1;
	}

	printf("Season result for %s: %.3lf (%.lf-%.lf-%.lf)\n", info.structHomeTeam, resultCalculation, win, loss, tie);
	printf("\n");

	return 0;
}


/*
* Fucntion : parseLine()
* Description : This function checks the error of the parameter received from the processGame() function.
* Parameters : char resultOfGame[teamString], char enemyName[teamString], int* homeScore, int* enemyScore
* Returns : 0 if the parameters does not have strings.
*			1 if *homeScore is bigger than *enemyScore.
*			2 if *homeScore is the same as *enemyScore.
*			3 if *homeScore is less than *enemyScore.
*/
int parseLine(char resultOfGame[kteamString], char enemyName[kteamString], char* homeScore, char* enemyScore) {
	
	int number_of_enemyScore = 0, number_of_homeScore = 0;
	char temp_enemyName[kteamString] = { };
	char *temp_homeName[kteamString] = { };
	char *homeName[kteamString] = { };
	int j = 0;
	
	enemyScore = strrchr(resultOfGame, '-');			//Save String from last '-'
	number_of_enemyScore = atoi(enemyScore + 1);		//Change string into number.
	
	homeScore = strrchr(resultOfGame, ',');			//Save String from last ','
	number_of_homeScore = atoi(homeScore + 1);		//Change string into number.
	
	for (j = 0; resultOfGame[j] != ','; j++) {					//Save enemy name before ','
		temp_enemyName[j] = resultOfGame[j];
	}
	enemyName = temp_enemyName;
	
	info.structHomeScore = number_of_homeScore;
	info.structEnemyScore = number_of_enemyScore;
	strncpy(info.structEnemyTeam, enemyName, j+1);
	
	if (resultOfGame == NULL || enemyName == NULL || homeScore == NULL || enemyScore == NULL) { 
		return 0;
	}
	if (isalnum(*enemyName) == 0) {
		return 0;
	}
	if (isdigit(number_of_enemyScore) != 0 || isdigit(number_of_homeScore) != 0) {
		return 0;
	}

	if (number_of_homeScore > number_of_enemyScore) {
		return 1;
	}
	else if (number_of_homeScore == number_of_enemyScore) {
		return 2;
	}
	else if (number_of_enemyScore > number_of_homeScore) {
		return 3;
	}
	return 0;
}

