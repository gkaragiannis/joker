/***********************************************************************
* Project		:	Joker Game
* File Name		:	bizarre_draws_mode.h
* Version		:	final
* Milestone		:	Complete bizarre_draws_mode.h
* Author		:	George Karagiannis
* Start Date	:	12 Jan. 2016
* End Date		:	28 Feb. 2016
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "adt_cv.h"
#include "globals.h"

extern FILE *rf_ptr;

//------------------------------------------------------------------------------------------------------------------//
//												Functions Definitions												//
//------------------------------------------------------------------------------------------------------------------//

/***********************************************************************
* function	:	bizarre_draws()
* purpose	:	implements menu of "Find Bizarre Draws" mode
* argument	:
*   — void
* return	:	0 if successful
* note		:
*   —
***********************************************************************/
int bizarre_draws(){

	int exe;
	char input = '0';

	while ((input != 'r') && (input != 'R')){
		printf("\n\n Your options are :\n\tThree (C)onsecutive Numbers\n\tFour Numbers in the Same (T)en\n\tThree Numbers on the Same (W)aning\n\t(R)eturn");
		printf("\n Give new choice :\t");
		fflush(stdin);
		scanf("%c", &input);
		getchar();

		if ((input != 'r') && (input != 'R')){
			switch (input){
			case 'C':
			case 'c':
			case 'T':
			case 't':
			case 'W':
			case 'w':
				exe = find_bizzare_draws(input);
				/*if (exe != 0){
				printf("\n concecutive_nums() function exited with code %d", exe);
				return exe;
				}*/
				break;
			default:
				printf("\n\a Invalid input.");
			}
		}
	}

	return 0;
}


/***********************************************************************
* function	:	find_bizzare_draws()
* purpose	:	search for bizarre draws
* argument	:
*   — char criterion :	search criterion
* return	:	0 if successful
* notes		:
*   —
***********************************************************************/
int find_bizzare_draws(char criterion){

	char id_str[IDSL], date_str[DTSL], win_nums_str[WNSL], joker_str[JSL];
	bool found = false, satisfy = false;
	draw_ptr new_draw;
	int *win_nums, arr_pos;

	rewind(rf_ptr);
	fscanf(rf_ptr, "%s%s%s%s", id_str, date_str, win_nums_str, joker_str);

	printf("\n ID\tDate\t\tWining Numbers\t\tJoker");
	printf("\n-----------------------------------------------------");

	while (!feof(rf_ptr)){

		fscanf(rf_ptr, "%s%s%s%s", id_str, date_str, win_nums_str, joker_str);
		win_nums = winnums_to_int(win_nums_str);
		if (win_nums == NULL)
			return -1;
		new_draw = (draw_ptr)malloc(sizeof(struct draw));
		new_draw->id = atoi(id_str);
		strcpy(new_draw->date, date_str);
		for (arr_pos = 0; arr_pos < WNAL; arr_pos++){
			new_draw->nums[arr_pos] = win_nums[arr_pos];
		}
		free(win_nums);
		new_draw->joker = atoi(joker_str);

		if ((criterion == 'c') || (criterion == 'C'))
			satisfy = crit_cons(new_draw);
		else if ((criterion == 't') || (criterion == 'T'))
			satisfy = crit_ten(new_draw);
		else if ((criterion == 'w') || (criterion == 'W'))
			satisfy = crit_wang(new_draw);
		else{
			//it should never come here
			return -1;
		}

		free(new_draw);

		found = found || satisfy;
	}

	if (found){
		printf("\n-----------------------------------------------------");
		printf("\n *Note: Numbers in parenthesis satisfy the requested criterion");
	}
	else{
		printf("\n There is no draw satisfies the requested criterion ");
		printf("\n-----------------------------------------------------");
	}

	return 0;

}


/***********************************************************************
* function	:	crit_cons()
* purpose	:	checks if criterion of consecutive numbers is met
* argument	:
*   — draw_ptr adraw :	pointer to struct of type draw
* return	:	true if criterion is met, else false
* notes		:
*   —
***********************************************************************/
bool crit_cons(draw_ptr adraw){

	int arr_pos, criterion, crt_pos;
	bool satisfy = false;

	if ((adraw->nums[0] + 1 == adraw->nums[1]) && (adraw->nums[1] + 1 == adraw->nums[2])){
		satisfy = true;
		criterion = adraw->nums[0];
		crt_pos = 0;
	}
	else if ((adraw->nums[1] + 1 == adraw->nums[2]) && (adraw->nums[2] + 1 == adraw->nums[3])){
		satisfy = true;
		criterion = adraw->nums[1];
		crt_pos = 1;
	}
	else if ((adraw->nums[2] + 1 == adraw->nums[3]) && (adraw->nums[3] + 1 == adraw->nums[4])){
		satisfy = true;
		criterion = adraw->nums[2];
		crt_pos = 2;
	}

	//print draw's data if criterio is satisfied
	if (satisfy){
		printf("\n %d\t%s\t", adraw->id, adraw->date);
		for (arr_pos = 0; arr_pos < WNAL; arr_pos++){
			if (adraw->nums[arr_pos] == criterion + arr_pos - crt_pos)
				printf("(%d) ", adraw->nums[arr_pos]);
			else
				printf("%d ", adraw->nums[arr_pos]);
		}
		printf("\t%d", adraw->joker);
	}

	return satisfy;
}


/***********************************************************************
* function	:	crit_ten()
* purpose	:	checks if criterion of numbers in same ten is met
* argument	:
*   — draw_ptr adraw :	pointer to struct of type draw
* return	:	true if criterion is met, else false
* notes		:
*   —
***********************************************************************/
bool crit_ten(draw_ptr adraw){

	int arr_pos, freq[WNAL], criterion;
	bool satisfy = false;

	//reset frequency array and found flag
	for (arr_pos = 0; arr_pos < sizeof(freq) / sizeof(int); arr_pos++){
		freq[arr_pos] = 0;
	}

	//compute new frequencies
	for (arr_pos = 0; arr_pos < WNAL; arr_pos++){
		if (adraw->nums[arr_pos] % 10 == 0)
			freq[(adraw->nums[arr_pos] / 10) - 1]++;
		else
			freq[(adraw->nums[arr_pos] / 10)]++;
	}

	//check if criterio is satisfied
	for (criterion = 0; criterion < sizeof(freq) / sizeof(int); criterion++){
		if (freq[criterion] >= 4){
			satisfy = true;
			break;
		}
	}

	//print draw's data if criterio is satisfied
	if (satisfy){
		printf("\n %d\t%s\t", adraw->id, adraw->date);
		for (arr_pos = 0; arr_pos < WNAL; arr_pos++){
			if ((adraw->nums[arr_pos] % 10 == 0) && ((adraw->nums[arr_pos] / 10) - 1 == criterion))
				printf("(%d) ", adraw->nums[arr_pos]);
			else if ((adraw->nums[arr_pos] % 10 != 0) && (adraw->nums[arr_pos] / 10 == criterion))
				printf("(%d) ", adraw->nums[arr_pos]);
			else
				printf("%d ", adraw->nums[arr_pos]);
		}
		printf("\t%d", adraw->joker);
	}

	return satisfy;
}


/***********************************************************************
* function	:	crit_wang()
* purpose	:	checks if criterion of numbers in same waning is met
* argument	:
*   — draw_ptr adraw :	pointer to struct of type draw
* return	:	true if criterion is met, else false
* notes		:
*   —
***********************************************************************/
bool crit_wang(draw_ptr adraw){

	int arr_pos, freq[2 * WNAL], criterion;
	bool satisfy = false;

	//reset frequency array and found flag
	for (arr_pos = 0; arr_pos < sizeof(freq) / sizeof(int); arr_pos++){
		freq[arr_pos] = 0;
	}

	//compute new frequencies
	for (arr_pos = 0; arr_pos < WNAL; arr_pos++){
		freq[adraw->nums[arr_pos] % 10]++;
	}

	//check if criterio is satisfied
	for (criterion = 0; criterion < sizeof(freq) / sizeof(int); criterion++){
		if (freq[criterion] >= 3){
			satisfy = true;
			break;
		}
	}

	//print draw's data if criterio is satisfied
	if (satisfy){
		printf("\n %d\t%s\t", adraw->id, adraw->date);
		for (arr_pos = 0; arr_pos < WNAL; arr_pos++){
			if (adraw->nums[arr_pos] % 10 == criterion)
				printf("(%d) ", adraw->nums[arr_pos]);
			else
				printf("%d ", adraw->nums[arr_pos]);
		}
		printf("\t%d", adraw->joker);
	}

	return satisfy;
}

