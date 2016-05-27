/***********************************************************************
* Project		:	Joker Game
* File Name		:	adt_cv.h
* Version		:	final
* Milestone		:	Complete adt_cv.h
* Author		:	George Karagiannis
* Start Date	:	12 Jan. 2016
* End Date		:	28 Feb. 2016
***********************************************************************/

#pragma once

//------------------------------------------------------------------------------------------------------------------//
//											User-defined Constant Values											//
//------------------------------------------------------------------------------------------------------------------//
#ifndef __CONSTANT_VALUES__
#define __CONSTANT_VALUES__

#define WNAL 5		// WinningNumbersArrayLong
#define IDSL 5		// IDStringLong
#define DTSL 11		// DaTeStringLong
#define WNSL 15		// WinningNumbersStringLong
#define JSL 6		// JokerStringLong
#define MINV 1		// MINimumValue
#define MAXV 45		// MAXimumValue
#define MAXJ 20		// MAXimumJoker

#endif /*__CONSTANT_VALUES__*/

//------------------------------------------------------------------------------------------------------------------//
//												User-defined Data Types												//
//------------------------------------------------------------------------------------------------------------------//
#ifndef __DATA_TYPES__
#define __DATA_TYPES__

typedef int bool;
#define true 1
#define false 0

typedef struct draw *draw_ptr;
struct draw{
	int id;
	char date[DTSL];
	int nums[WNAL];
	int joker;
	int winners;
	draw_ptr next;
	draw_ptr previous;
};

typedef struct dates *dates_ptr;
struct dates{
	int ub_date_req;			//->	upper bound requsted inverted integer
	int lb_date_req;			//->	lower bound requsted inverted integer
	char ub_date_req_str[DTSL];	//->	upper bound requsted date string
	char lb_date_req_str[DTSL];	//->	lower bound requsted date string
};

typedef struct coupon *cpn_ptr;
struct coupon{
	int id;
	int nums[WNAL];
	int joker;
	cpn_ptr next;
};

#endif /*__DATA_TYPES__*/
