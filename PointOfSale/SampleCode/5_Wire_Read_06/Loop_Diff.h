/*
 * Program:
 *  Loop_Diff
 * Created by:
 *  Jonathon Weeks
 * On date:
 *  2018-10-25
 * 
 * Description:
 *  Given a start value, end value, and variable type, this
 *  program will calculate the difference between those two
 *  numbers, with the assumption that the variable has the
 *  potential to loop such that the end time may be below the
 *  start time.
 *  This is intended for use when calculating elapsed time.
 */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~
//~~~~~~~~

#ifndef LOOP_DIFF_H
#define LOOP_DIFF_H

#define U_LONG_MAX 4294967295
#define U_INT_MAX 4294967295

unsigned long loop_diff(unsigned long s,unsigned long e,int t);

#endif
