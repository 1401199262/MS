/*******************************************************************************
*                        Voxware Proprietary Material                          *
*                        Copyright 1997, Voxware, Inc                          *
*                            All Rights Reserved                               *
*                                                                              *
*                       DISTRIBUTION PROHIBITED without                        *
*                      written authorization from Voxware                      *
*******************************************************************************/
/*******************************************************************************
* Filename:     CompPow.h
*                                                                              
* Purpose:      approximate pow(x, 1/8) using polynomial evaluation.
*                                                                              
* Functions:    powerpoly_125()
*                                                                              
* Author/Date:  Wei Wang, 07/08/97. (powerpoly_125 is modified from RZ's 
*               code in enhance.c of RT29).
********************************************************************************
* Modifications:
*                                                                              
* Comments:                                                                    
* 
* Concerns:
*
* $Header:   /export/phobos/pvcs/r_and_d/archives/scaleable/pea/CompPow.h_v   1.1   02 Apr 1998 14:46:54   weiwang  $
*******************************************************************************/

#ifndef _COMP_POW_H_
#define _COMP_POW_H_


/*******************************************************************************
* Function:        powerpoly_125()
*
* Action:         calculate fX^(1/8)
*
* Input:          fX -- floating number which should be between [0 1].
*
* Output:         none
*
* Globals:        none
*
* Return:         fX^(0.125)
********************************************************************************
* Implementation/Detailed Description:
*     - routine is for a 5th order approximation.
*     - polynomial should be :
*         z = y(0).x^5 + y(1).x^4 ... + y(4).x + y(5)
*
*     - x^y is broken into 4 parts accroding to x (x always > 1)
*           poly1 : 1         <= x < BOUNDARY1
*           poly2 : BOUNDARY1 <= x < BOUNDARY2
*           poly3 : BOUNDARY2 <= x < BOUNDARY3
*           poly4 : BOUNDARY3 <= x < BOUNDARYMAX
*
*     - y should be ordered poly1, poly2, poly3, poly4
*
* References:
*     - used Matlab :=> polyfit, polyval
* RZ - 01/17/97
********************************************************************************
* Modifications:  Wei Wang: x is broken into 6 parts. And regenerate the 
*                 coefficients for x.^(1/8).   07/08/97.
* Comments:
*
* Concerns/TBD:
*******************************************************************************/

float powerpoly_125(float fX);

#endif
