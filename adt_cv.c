/***********************************************************************
* Project		:	Joker Game
* File Name		:	adt_cv.c
* Version		:	final
* Milestone		:	Complete adt_cv.c
* Author		:	George Karagiannis
* Start Date	:	12 Jan. 2016
* End Date		:	28 Feb. 2016
***********************************************************************/

#include "adt_cv.h"

typedef int bool;
#define true 1;
#define false 0;

//------------------------------------------------------------------------------------------------------------------//
//												User-defined Data Types												//
//------------------------------------------------------------------------------------------------------------------//

struct draw{
	int id;
	char date[DTSL];
	int nums[WNAL];
	int joker;
	int winners;
	draw_ptr next;
	draw_ptr previous;
};


struct dates{
	int ub_date_req;			//->	upper bound requsted inverted integer
	int lb_date_req;			//->	lower bound requsted inverted integer
	char ub_date_req_str[DTSL];	//->	upper bound requsted date string
	char lb_date_req_str[DTSL];	//->	lower bound requsted date string
};


struct coupon{
	int id;
	int nums[WNAL];
	int joker;
	cpn_ptr next;
};

