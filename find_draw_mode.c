/***********************************************************************
* Project		:	Joker Game
* File Name		:	find_draws_mode.c
* Version		:	final
* Milestone		:	Complete find_draws_mode.c
* Author		:	George Karagiannis
* Start Date	:	12 Jan. 2016
* End Date		:	28 Feb. 2016
***********************************************************************/

#define NDEBUG

//------------------------------------------------------------------------------------------------------------------//
//													Header Files													//
//------------------------------------------------------------------------------------------------------------------//
#include <assert.h>
#include <stdio.h>

#include "adt_cv.h"
#include "globals.h"
#include "find_draw_mode.h"

#define NDEBUG

//------------------------------------------------------------------------------------------------------------------//
//												  Global Variables  												//
//------------------------------------------------------------------------------------------------------------------//
extern FILE *rf_ptr;


//------------------------------------------------------------------------------------------------------------------//
//												Functions Definitions												//
//------------------------------------------------------------------------------------------------------------------//

/***********************************************************************
* function	:	find_draw()
* purpose	:	implements menu of "Find Draw" mode
* argument	:
*   — void
* return	:	0 if successful
* notes		:
*   —
***********************************************************************/
int find_draw(){

	int exe = 0;
	char input = '0';

	while ((input != 'r') && (input != 'R')){
		printf("\n\n Your options are :\n\tSearch by draw's (I)D\n\tSearch by draw's (D)ate\n\t(R)eturn");
		printf("\n Give new choice :\t");
		fflush(stdin);
		scanf("%c", &input);
		getchar();

		if ((input != 'r') && (input != 'R')){
			switch (input){
			case 'I':
			case 'i':
				exe = id_search();
				assert(exe == 0);
				break;
			case 'D':
			case 'd':
				exe = date_search();
				assert(exe == 0);
				break;
			default:
				printf("\n\a Invalid input.");
			}
		}
	}

	return 0;
}


/***********************************************************************
* function	:	id_search()
* purpose	:	search for a specific draw by ID number
* argument	:
*	— void
* return	:	0 if successful
* notes		:
*   —
***********************************************************************/
int id_search(){

	int id_req, *win_nums, arr_pos;
	char id_req_str[IDSL], id_str[IDSL], date_str[DTSL], win_nums_str[WNSL], joker_str[JSL];
	bool exit_loop = false;

	rewind(rf_ptr);

	/* ask for desirable id */
	while (!exit_loop){
		printf("\n Please give draw's ID in xxxx format\t:\t");
		fflush(stdin);
		scanf("%s", &id_req_str);
		getchar();
		exit_loop = is_num(id_req_str);
		if (exit_loop)
			id_req = atoi(id_req_str);
		else
			printf("\a Invalid input");
	}

	/*
	*	Scan reading file line by line. Every line contains four fields, id,
	*	date, winning numbers and joker of a draw, respectively. For every
	*	reading line, check if id field equals to requested. Search continuous
	*	until find either EOF, or requested id.
	*/
	exit_loop = false;
	while (!exit_loop){
		fscanf(rf_ptr, "%s%s%s%s", id_str, date_str, win_nums_str, joker_str);
		if (atoi(id_str) == id_req){
			printf("\n\t---------------------------------------------");
			printf("\n\tWinninng Numbers\t\t\tJoker\t\n");
			printf("\t---------------------------------------------\n");

			win_nums = winnums_to_int(win_nums_str);
			if (win_nums == NULL)
				return -1;
			for (arr_pos = 0; arr_pos < WNAL; arr_pos++){
				printf("\t%d", win_nums[arr_pos]);
			}
			free(win_nums);

			printf("\t%s", joker_str);

			printf("\n\t---------------------------------------------\n");
			exit_loop = true;
		}
		else if (feof(rf_ptr)){
			printf("\n Sorry! There is no draw with ID %d", id_req);
			exit_loop = true;
		}
	}

	return 0;
}


/***********************************************************************
* function	:	date_search()
* purpose	:	search for a specific draw by DATE occurred
* argument	:
*   — void
* return	:	0 if successful
* notes		:
*   —
***********************************************************************/
int date_search(){

	int arr_pos;
	draw_ptr draw_list = NULL;
	dates_ptr dt_bnd = NULL;
	bool exit_loop = false;

	/* ask for desirable date*/
	dt_bnd = ask_dates(false);
	if (dt_bnd == NULL)
		return -1;

	/* search requested draw, if exist */
	draw_list = search_draws(dt_bnd);
	if (draw_list == NULL)
		return -2;

	free(dt_bnd);

	printf("\n\t---------------------------------------------");
	printf("\n\tWinninng Numbers\t\t\tJoker\t\n");
	printf("\t---------------------------------------------\n");
	for (arr_pos = 0; arr_pos < WNAL; arr_pos++){
		printf("\t%d", draw_list->nums[arr_pos]);
	}
	printf("\t%d", draw_list->joker);
	printf("\n\t---------------------------------------------\n");

	free(draw_list);

	return 0;
}

