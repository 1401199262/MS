//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft shared
// source or premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license agreement,
// you are not authorized to use this source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the SOURCE.RTF on your install media or the root of your tools installation.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/***
*gmtime64.c - breaks down a time value into GMT date/time info
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _gmtime64() - breaks the clock value down into GMT time/date
*       information; returns pointer to structure with the data.
*
*Revision History:
*       05-13-98  GJF   Created. Adapted from Win64 version of _gmtime64.c.
*       06-12-98  GJF   Fixed elapsed years calculation.
*       10-19-01  BWT   Return NULL on malloc failure in MT case instead of 
*                       using static buffer.
*       12-11-01  BWT   return null if getptd fails instead of exiting program
*       03-06-03  SSM   Assertions, validations and secure version
*       09-01-06  JKS   Modify Assertions to allow GMT and UTC adjustment
*
*******************************************************************************/

#include <cruntime.h>
#include <time.h>
#include <ctime.h>
#include <stddef.h>
#include <memory.h>
#include <internal.h>
#include <mtdll.h>
#include <malloc.h>
#include <stddef.h>
#include <dbgint.h>

/***
*errno_t _gmtime64_s(ptm, timp) - convert *timp to a structure (UTC)
*
*Purpose:
*       Converts the calendar time value, in 64 bit internal format, to
*       broken-down time (tm structure) with the corresponding UTC time.
*
*Entry:
*       const time_t *timp - pointer to time_t value to convert
*
*Exit:
*       errno_t = 0 success
*                 tm members filled-in
*       errno_t = non zero
*                 tm members initialized to -1 if ptm != NULL
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _gmtime64_s (
        struct tm *ptm,        
        const __time64_t *timp
        )
{
        __time64_t caltim;/* = *timp; *//* calendar time to convert */
        int islpyr = 0;                 /* is-current-year-a-leap-year flag */
        int tmptim;
        int *mdays;                     /* pointer to days or lpdays */
        struct tm *ptb = ptm;

        _VALIDATE_RETURN_ERRCODE( ( ptm != NULL ), EINVAL )

        memset( ptm, 0xff, sizeof( struct tm ) );

        _VALIDATE_RETURN_ERRCODE( ( timp != NULL ), EINVAL )
        caltim = *timp;

        _VALIDATE_RETURN_ERRCODE_NOEXC( ( caltim >= _MIN_LOCAL_TIME ), EINVAL )
        _VALIDATE_RETURN_ERRCODE_NOEXC( ( caltim <= _MAX__TIME64_T + _MAX_LOCAL_TIME ), EINVAL)

        /*
         * Determine the years since 1900. Start by ignoring leap years.
         */
        tmptim = (int)(caltim / _YEAR_SEC) + 70;
        caltim -= ((__time64_t)(tmptim - 70) * _YEAR_SEC);

        /*
         * Correct for elapsed leap years
         */
        caltim -= ((__time64_t)_ELAPSED_LEAP_YEARS(tmptim) * _DAY_SEC);

        /*
         * If we have underflowed the __time64_t range (i.e., if caltim < 0), 
         * back up one year, adjusting the correction if necessary.
         */
        if ( caltim < 0 ) {
            caltim += (__time64_t)_YEAR_SEC;
            tmptim--;
            if ( _IS_LEAP_YEAR(tmptim) ) {
                caltim += _DAY_SEC;
                islpyr++;
            }
        }
        else
            if ( _IS_LEAP_YEAR(tmptim) )
                islpyr++;

        /*
         * tmptim now holds the value for tm_year. caltim now holds the
         * number of elapsed seconds since the beginning of that year.
         */
        ptb->tm_year = tmptim;

        /*
         * Determine days since January 1 (0 - 365). This is the tm_yday value.
         * Leave caltim with number of elapsed seconds in that day.
         */
        ptb->tm_yday = (int)(caltim / _DAY_SEC);
        caltim -= (__time64_t)(ptb->tm_yday) * _DAY_SEC;

        /*
         * Determine months since January (0 - 11) and day of month (1 - 31)
         */
        if ( islpyr )
            mdays = _lpdays;
        else
            mdays = _days;


        for ( tmptim = 1 ; mdays[tmptim] < ptb->tm_yday ; tmptim++ ) ;

        ptb->tm_mon = --tmptim;

        ptb->tm_mday = ptb->tm_yday - mdays[tmptim];

        /*
         * Determine days since Sunday (0 - 6)
         */
        ptb->tm_wday = ((int)(*timp / _DAY_SEC) + _BASE_DOW) % 7;

        /*
         *  Determine hours since midnight (0 - 23), minutes after the hour
         *  (0 - 59), and seconds after the minute (0 - 59).
         */
        ptb->tm_hour = (int)(caltim / 3600);
        caltim -= (__time64_t)ptb->tm_hour * 3600L;

        ptb->tm_min = (int)(caltim / 60);
        ptb->tm_sec = (int)(caltim - (ptb->tm_min) * 60);

        ptb->tm_isdst = 0;
        return 0;

}


/***
*struct tm *_gmtime64(timp) - convert *timp to a structure (UTC)
*
*Purpose:
*       Converts the calendar time value, in internal 64-bit format to
*       broken-down time (tm structure) with the corresponding UTC time.
*
*Entry:
*       const __time64_t *timp - pointer to time_t value to convert
*
*Exit:
*       returns pointer to filled-in tm structure.
*       returns NULL if *timp < 0
*
*Exceptions:
*
*******************************************************************************/

struct tm * __cdecl _gmtime64 (
        const __time64_t *timp
        )
{
        errno_t e;
        struct tm *ptm = __getgmtimebuf();                 /* will point to gmtime buffer */
        if ( ptm == NULL )
        {
            return NULL;
        }

        e = _gmtime64_s( ptm, timp );
        if ( e != 0 )
        {
            return NULL;
        }
        return ptm;
}
