/***********************************************************************
* Project		:	Joker Game
* File Name		:	globals.c
* Version		:	final
* Milestone		:	Complete globals.c
* Author		:	George Karagiannis
* Start Date	:	12 Jan. 2016
* End Date		:	28 Feb. 2016
***********************************************************************/

#define NDEBUG

//------------------------------------------------------------------------------------------------------------------//
//													Header Files													//
//------------------------------------------------------------------------------------------------------------------//
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "adt_cv.h"
#include "globals.h"


//------------------------------------------------------------------------------------------------------------------//
//												  Global Variables  												//
//------------------------------------------------------------------------------------------------------------------//
FILE *rf_ptr;

//------------------------------------------------------------------------------------------------------------------//
//												Functions Definitions												//
//------------------------------------------------------------------------------------------------------------------//

/***********************************************************************
* function	:	open_db()
* purpose	:	open "database" file
* argument	:
*	— void
* return	:	
* notes		:
*	—
***********************************************************************/
void open_db(){

	rf_ptr = fopen("arxeioKlhrwsewn.txt", "r");
	if (rf_ptr == NULL){
		fprintf(stderr, "\n Value of errno: %d\n", errno);
		fprintf(stderr, " Error opening file: %s\n", strerror(errno));
		exit(1);
	}
}


/***********************************************************************
* function	:	close_db()
* purpose	:	close "database" file
* argument	:
*	— void
* return	:
* notes		:
*	—
***********************************************************************/
void close_db(){
	errno = fclose(rf_ptr);
	if (errno != 0){
		fprintf(stderr, "Value of errno: %d\n", errno);
		fprintf(stderr, "Error closing file: %s\n", strerror(errno));
		exit(2);
	}
}


/***********************************************************************
* function	:	ask_dates()
* purpose	:	ask user for date(s) and store it/them in a struct
*				of type dates
* argument	:
*	— bool multiple :	true to ask for two dates
*						false to ask for one date
* return	:	 pointer to struct of type dates
* notes		:
*	—
***********************************************************************/
dates_ptr ask_dates(bool multiple){

	dates_ptr dt = NULL;
	bool exit_loop = false;
	int ub_date_req = 0, lb_date_req = 0, swap;
	char ub_date_req_str[DTSL], lb_date_req_str[DTSL], swap_str[DTSL];

	/***********************************************************************
	*@	bool type
	*	multiple				->	date range (true) or specific date (false)
	*@	int type
	*	ub_date_req				->	upper bound date integer
	*	lb_date_req				->	lower bound date integer
	*	swap					->	auxiliary integer for swap
	*@	char type
	*	ub_date_req_str[DTSL]	->	upper bound date string
	*	lb_date_req_str[DTSL]	->	lower bound date string
	*	swap_str[DTSL]			->	auxiliary string for swap
	*@	dates_ptr type
	*	request					->	pointer to struct containing dates
	***********************************************************************/
	while (!exit_loop){
		if (multiple)
			printf("\n                                                                          /  /\r");
		else
			printf("\n                                                          /  /\r");

		printf(" Please give draw's ");
		if ((multiple) && (ub_date_req <= 0))
			printf("UPPER bound ");
		else if ((multiple) && (lb_date_req <= 0))
			printf("LOWER bound ");
		printf("DATE in dd/mm/yyyy format\t:\t");
		fflush(stdin);

		if (ub_date_req <= 0){
			scanf("%s", &ub_date_req_str);
			ub_date_req = date_to_int(ub_date_req_str);
			if (!multiple){
				strcpy(lb_date_req_str, ub_date_req_str);
				lb_date_req = ub_date_req;
			}
		}
		else{
			scanf("%s", &lb_date_req_str);
			lb_date_req = date_to_int(lb_date_req_str);
		}
		getchar();

		exit_loop = (lb_date_req > 0) && (ub_date_req > 0);

		if ((ub_date_req <= 0) && (lb_date_req <= 0))
			printf("\n\a Invalid date\n");
	}

	/* swap if inputs not in correct order */
	if (lb_date_req > ub_date_req){
		swap = ub_date_req;
		strcpy(swap_str, ub_date_req_str);

		ub_date_req = lb_date_req;
		strcpy(ub_date_req_str, lb_date_req_str);

		lb_date_req = swap;
		strcpy(lb_date_req_str, swap_str);
	}

	dt = (dates_ptr)malloc(sizeof(struct dates));
	assert(dt);
	dt->lb_date_req = lb_date_req;
	dt->ub_date_req = ub_date_req;
	strcpy(dt->lb_date_req_str, lb_date_req_str);
	strcpy(dt->ub_date_req_str, ub_date_req_str);

	return dt;
}


/***********************************************************************
* function	:	winnums_to_int()
* purpose	:	converts a string of digit characters and four ","
*				punctuation characters, in an five integer array
* argument	:
*   — char *str :	the string to be converted
* return	:	pointer to an array of integers
* notes		:
*   —
***********************************************************************/
int *winnums_to_int(char *str){

	int arr_pos = 0, *res_ptr;
	char *token;
	/*
	str			->	string to convert into integers
	res_ptr		->	pointer to save integers
	arr_pos		->	position in returned integer array
	token		->	part of initial str when use strtok()
	*/

	res_ptr = (int*)malloc(WNAL*sizeof(int));
	assert(res_ptr);
	token = strtok(str, ",");
	while (token != NULL){
		res_ptr[arr_pos] = atoi(token);
		arr_pos++;
		token = strtok(NULL, ",");
	}

	return res_ptr;
}


/***********************************************************************
* function	:	valid_date()
* purpose	:	checks the validity of a date
* argument	:
*	—	int day	  : day part of a date
*	—	int month : month part of a date
*	—	int year  : year part of a date
* return	: a positive integer value if successful
* notes		:
*	— returned integer result from expression :
year * 10000 + month * 100 + day
***********************************************************************/
int valid_date(int day, int month, int year){

	struct tm *today;
	bool leap_year;
	time_t now;

	leap_year = (year % 4 == 0) && (year % 100 != 0) || (year % 4 == 0);

	if ((day > 31) || (day < 1)){
		printf("\a Invalid day value");
		return -1;
	}

	if ((month > 12) || (month < 1)){
		printf("\a Invalid month value");
		return -2;
	}

	if ((month == 2) && (leap_year) && (day > 29)){
		printf("\a %d was leap year. February has at most 29 days", year);
		return -3;
	}
	else if ((month == 2) && (!leap_year) && (day > 28)){
		printf("\a %d was NOT leap year. February has at most 28 days", year);
		return -4;
	}

	if (((month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day > 30)){
		if (month == 4)
			printf(" April");
		if (month == 6)
			printf(" June");
		if (month == 9)
			printf(" September");
		if (month == 11)
			printf(" November");
		printf("\a has at most 30 days");
		return -5;
	}

	if (year < 2014){
		printf(" This is too old for me");
		return -6;
	}

	time(&now);
	today = localtime(&now);
	if ((year > today->tm_year + 1900) || (((month > today->tm_mon + 1) || ((day > today->tm_mday) && (month == today->tm_mon + 1))) && (year == today->tm_year + 1900))){
		printf(" If I could predict the future, I would have won the 'Joker'!");
		return -7;
	}

	return year * 10000 + month * 100 + day;
}


/***********************************************************************
* function	:	date_to_int()
* purpose	:	converts a date string in dd/mm/yyyy format in integer
* argument	:
*	— char *str	: the string to be converted
* return	: a positive integer value if successful
* notes		:
*	— returns the return value of valid_date()
***********************************************************************/
int date_to_int(char *str){

	int index, vld_puncts = 0, day, month, year;
	char *token, tmp[BUFSIZ];
	/***********************************************************************
	*	index		->	index on str position
	*	date_ret	->	integer date returned in yyyy/mm/dd
	*	token		->	part of initial str when use strtok()
	***********************************************************************/

	/*
	*	Check if string is in valid format
	*/
	for (index = 0; index < strlen(str); index++){
		if (isalpha(str[index])){
			printf("\a Your input should NOT contain any letters");
			return 0;
		}
		if (ispunct(str[index]) && str[index] != '/'){
			printf("\a Your input is NOT in valid format. Seperate values with '/' symbols");
			return 0;
		}
		else if (ispunct(str[index]) && str[index] == '/')
			vld_puncts++;
	}
	if (vld_puncts != 2)
		return 0;

	strcpy(tmp, str);

	token = strtok(tmp, "/");
	day = atoi(token);
	token = strtok(NULL, "/");
	month = atoi(token);
	token = strtok(NULL, "/");
	year = atoi(token);

	return(valid_date(day, month, year));
}


/***********************************************************************
* function	:	search_draws()
* purpose	:	search for draws in date range specified by dt
* argument	:
*	— dates_ptr dt :	pointer to struct of type dates
* return	:	pointer to struct of type draw
* notes		:
*	— returned pointer points to list of structs of type draw
***********************************************************************/
draw_ptr search_draws(dates_ptr dt){

	int date_fnd, *win_nums, arr_pos = 0;
	char id_str[IDSL], date_str[DTSL], win_nums_str[WNSL], joker_str[JSL];
	draw_ptr new_draw, list_head = NULL;
	/***********************************************************************
	*@	int type
	*	date_fnd		->	checking draw's date inverted integer
	*	win_nums		->	pointer to checking draw's wining numbers integer array
	*	arr_pos			->	index on array position
	*@	char type
	*	id_str			->	checking draw's id string
	*	date_str		->	checking draw's date string
	*	win_nums_str	->	checking draw's wining numbers string
	*	joker_str		->	checking draw's joker string
	*@	dates_ptr
	*	dt				->	pointer to struct containing dates
	*@	draw_ptr type
	*	list_head		->	pointer to list of structs represents draws
	*	new_draw		->	pointer to new struct draw
	***********************************************************************/

	rewind(rf_ptr);
	fscanf(rf_ptr, "%s%s%s%s", id_str, date_str, win_nums_str, joker_str);

	/*
	*	Scan reading file line by line. Every line contains four fields, id,
	*	date, winning numbers and joker of a draw, respectively. For every 
	*	reading line, check if date is less than upper date bound and more 
	*	than lower date bound, simultaneously. If that indeed happend, save
	*	draws data in a struct of type draw and put it in head of a list.
	*	Repeat until find either EOF, or date of a draw is less than lower
	*	date bound.
	*/
	do{
		if (feof(rf_ptr))
			break;
		fscanf(rf_ptr, "%s%s%s%s", id_str, date_str, win_nums_str, joker_str);
		date_fnd = date_to_int(date_str);
		if ((dt->ub_date_req >= date_fnd) && (dt->lb_date_req <= date_fnd)){
			new_draw = (draw_ptr)malloc(sizeof(struct draw));
			assert(new_draw);
			new_draw->id = atoi(id_str);
			strcpy(new_draw->date, date_str);
			win_nums = winnums_to_int(win_nums_str);
			if (win_nums == NULL)
				return NULL;
			for (arr_pos = 0; arr_pos < WNAL; arr_pos++){
				new_draw->nums[arr_pos] = win_nums[arr_pos];
			}
			free(win_nums);
			new_draw->joker = atoi(joker_str);
			new_draw->winners = 0;
			new_draw->next = NULL;
			new_draw->previous = NULL;
			if (list_head == NULL)
				list_head = new_draw;
			else{
				list_head->previous = new_draw;
				new_draw->next = list_head;
				list_head = list_head->previous;
			}
		}
	} while (date_fnd >= dt->lb_date_req);

	/*	if not found any draw	*/
	if (list_head == NULL){		
		if (dt->lb_date_req != dt->ub_date_req)
			printf("\n Sorry! No draws were held between DATEs %s & %s", dt->lb_date_req_str, dt->ub_date_req_str);
		else
			printf("\n Sorry! No draw was held on DATE %s", dt->ub_date_req_str);
	}

	return list_head;
}


/***********************************************************************
* function	:	create_file()
* purpose	:	creates a .txt file
* argument	:
*   — char mode[1] : character used in file name
* return	: pointer to created file if successful, else NULL
* notes		:
*   —
***********************************************************************/
FILE *create_file(char mode[1]){

	char filename[FILENAME_MAX];
	struct tm *cur_dt_tm;
	time_t cur_time;
	FILE *f_ptr;

	/* set filename */
	time(&cur_time);
	cur_dt_tm = localtime(&cur_time);
	strftime(filename, FILENAME_MAX, "%d%b%Y_%H.%M.%S_", cur_dt_tm);
	strcat(filename, mode);
	strcat(filename, ".txt");
	/* create file */
	f_ptr = fopen(filename, "w");
	if (f_ptr == NULL){
		fprintf(stderr, "\n Value of errno: %d\n", errno);
		fprintf(stderr, " Error opening file: %s\n", strerror(errno));
		return NULL;
	}

	return f_ptr;
}


/***********************************************************************
* function	:	is_num()
* purpose	:	checks if given string is a number
* argument	:
*	— char *str	:	string to be checked
* return	: true if successful
* notes		:
*   —
***********************************************************************/
bool is_num(char *str){

	unsigned int index;
	bool valid = true;

	for (index = 0; index < strlen(str); index++){
		valid = isdigit(str[index]);
		if (!valid)
			break;
	}

	return (bool)valid;
}


/***********************************************************************
* function	:	fillin_cpns()
* purpose	:	
* argument	:
*   — int cpn_num	:	number of coupons
*	— bool manual	:	
* return	:	pointer to struct of type coupon
* notes		:
*   — returned pointer points to list of structs of type coupon
***********************************************************************/
cpn_ptr fillin_cpns(int cpn_num, bool manual){

	int pos, cpn_ctr;
	bool nums[MAXV];
	cpn_ptr new_cpn = NULL, aux, list_head = NULL;
	time_t cur_time;

	srand((unsigned)time(&cur_time));

	/*	Fill in all cpn_num coupons	*/
	for (cpn_ctr = 0; cpn_ctr < cpn_num; cpn_ctr++){

		/* initialize mun aray to all false */
		for (pos = 0; pos < MAXV; pos++){
			nums[pos] = false;
		}

		/* select fill in mode */
		if (manual){
			new_cpn = fill_manual(cpn_ctr, nums);
		}
		else{
			new_cpn = fill_auto(cpn_ctr, nums);
		}

		/* add new coupon on pn_list */
		if (list_head == NULL){
			list_head = new_cpn;
			aux = new_cpn;
		}
		else{
			aux->next = new_cpn;
			aux = aux->next;
		}
	}

	return list_head;

}


/***********************************************************************
* function	:	fill_manual()
* purpose	:	ask user to fill in a coupon
* argument	:
*   — int cpn_id	:	coupons id
*	— bool *nums	:	pointer to boolean array representing coupon
* return	:	pointer to struct of type coupon
* notes		:
*   — returnes pointer returned from cpn_gen()
***********************************************************************/
cpn_ptr fill_manual(int cpn_id, bool *nums){

	char tmp[BUFSIZ];
	int gss_ctr, guess;

	for (gss_ctr = 0; gss_ctr < 6;){

		printf("\n Please give your guess for ");
		/* make guess for joker */
		if (gss_ctr == 5){
			printf("joker\t:\t");
			fflush(stdin);
			scanf("%s", &tmp);
			getchar();
			if (is_num(tmp) && (atoi(tmp) < (MAXJ + 1)) && (atoi(tmp) > (MINV - 1))){
				guess = atoi(tmp);
				gss_ctr++;
			}
			else{
				printf("\a Invalid input\n Please try again within 1 and 20");
			}
		}
		/* make guesses for winning numbers */
		else{
			printf("number #%d\t:\t", gss_ctr + 1);
			fflush(stdin);
			scanf("%s", &tmp);
			getchar();
			if (is_num(tmp) && (atoi(tmp) <= MAXV) && (atoi(tmp) >= MINV)){
				guess = atoi(tmp);
			}
			else{
				printf("\a Invalid input\n Please try again within 1 and 45");
			}

			/* validate guess */
			if (nums[guess - 1] == false){
				nums[guess - 1] = true;
				gss_ctr++;
			}
		}
	}

	return(cpn_gen(cpn_id, nums, guess));
}


/***********************************************************************
* function	:	fill_auto()
* purpose	:	automatic fill in a coupon 
* argument	:
*   — int cpn_id	:	coupons id
*	— bool *nums	:	pointer to boolean array representing coupon
* return	:	pointer to struct of type coupon
* notes		:
*   — returnes pointer returned from cpn_gen()
***********************************************************************/
cpn_ptr fill_auto(int cpn_id, bool *nums){

	char tmp[BUFSIZ];
	int gss_ctr, guess;

	for (gss_ctr = 0; gss_ctr < 6;){
		/* pseudorandom guess for joker */
		if (gss_ctr == 5){
			guess = (rand() % (MAXJ - 1)) + 1;
			assert((guess >= MINV) && (guess <= MAXJ));
			gss_ctr++;
		}
		/* pseudorandom guess for winning numbers */
		else{
			guess = (rand() % (MAXV - 1)) + 1;
			assert((guess >= MINV) && (guess <= MAXV));

			/* validate guess */
			if (nums[guess - 1] == false){
				nums[guess - 1] = true;
				gss_ctr++;
			}
		}
	}

	return(cpn_gen(cpn_id, nums, guess));
}


/***********************************************************************
* function	:	cpn_gen()
* purpose	:	construct a struct of type coupon
* argument	:
*   — int cpn_id	:	coupons id
*	— bool *nums	:	pointer to boolean array representing coupon
*	— int joker		:	guess for joker
* return	:	pointer to struct of type coupon
* notes		:
*   — 
***********************************************************************/
cpn_ptr cpn_gen(int cpn_id, bool *nums, int joker){

	cpn_ptr cpn = NULL;
	int pos = 0, cpn_pos = 0;

	cpn = (cpn_ptr)malloc(sizeof(struct coupon));
	assert(cpn);
	cpn->id = cpn_id;

	/* scan nums array to find guesses */
	while (pos < MAXV && cpn_pos < WNAL){
		if (nums[pos] == true){
			cpn->nums[cpn_pos] = pos + 1;
			cpn_pos++;
		}
		pos++;
	}

	cpn->joker = joker;
	cpn->next = NULL;

	return (cpn);
}


/***********************************************************************
* function	:	print_cpns()
* purpose	:	construct a struct of type coupon
* argument	:
*   — FILE *f_ptr		:	pointer to file that results will be written
*	— cpn_ptr cpn_list	:	pointer to head of list of structs of type coupon
* return	:	0 if successful
* notes		:
*   —
***********************************************************************/
int print_cpns(FILE *f_ptr, cpn_ptr cpn_list){

	int arr_pos = 0;
	cpn_ptr cpn_aux = NULL;

	if (f_ptr == NULL)
		return -1;

	fprintf(f_ptr, "\n-----------------------------------------------------\n COUPON:\n ID\tWining Numbers\t\t\t\tJoker\n-----------------------------------------------------");
	printf("\n ----------------------------------------------------\n COUPON:\n ID\tWining Numbers\t\t\t\tJoker\n ----------------------------------------------------");
	cpn_aux = cpn_list;
	while (cpn_aux != NULL){
		fprintf(f_ptr, "\n %04d\t", cpn_aux->id);
		//printf("\n %04d\t", cpn_aux->id);

		for (arr_pos = 0; arr_pos < WNAL; arr_pos++){
			fprintf(f_ptr, "%d\t", cpn_aux->nums[arr_pos]);
			//printf("%d\t", cpn_aux->nums[arr_pos]);
		}

		fprintf(f_ptr, "%d", cpn_aux->joker);
		//printf("%d", cpn_aux->joker);

		cpn_aux = cpn_aux->next;
	}

	fprintf(f_ptr, "\n-----------------------------------------------------");
	printf("\n ----------------------------------------------------");

	return 0;

}
