
/* : : generated by proto : : */
/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*          Copyright (c) 1996-2010 AT&T Intellectual Property          *
*                      and is licensed under the                       *
*                  Common Public License, Version 1.0                  *
*                    by AT&T Intellectual Property                     *
*                                                                      *
*                A copy of the License is available at                 *
*            http://www.opensource.org/licenses/cpl1.0.txt             *
*         (with md5 checksum 059e8cd6165cb4c31e351f2b69388fd9)         *
*                                                                      *
*              Information and Software Systems Research               *
*                            AT&T Research                             *
*                           Florham Park NJ                            *
*                                                                      *
*                 Glenn Fowler <gsf@research.att.com>                  *
*                                                                      *
***********************************************************************/
                  

/*
 * Glenn Fowler
 * AT&T Research
 *
 * checksum library interface
 */

#ifndef _SUM_H
#if !defined(__PROTO__)
#include <prototyped.h>
#endif
#if !defined(__LINKAGE__)
#define __LINKAGE__		/* 2004-08-11 transition */
#endif

#define _SUM_H

#include <ast.h>

#define SUM_SIZE	(1<<0)		/* print size too		*/
#define SUM_SCALE	(1<<1)		/* traditional size scale	*/
#define SUM_TOTAL	(1<<2)		/* print totals since sumopen	*/
#define SUM_LEGACY	(1<<3)		/* legacy field widths		*/

#define _SUM_PUBLIC_	const char*	name;

typedef struct Sumdata_s
{
	uint32_t	size;
	uint32_t	num;
	__V_*		buf;
} Sumdata_t;

typedef struct Sum_s
{
	_SUM_PUBLIC_
#ifdef	_SUM_PRIVATE_
	_SUM_PRIVATE_
#endif
} Sum_t;

extern __MANGLE__ Sum_t*	sumopen __PROTO__((const char*));
extern __MANGLE__ int	suminit __PROTO__((Sum_t*));
extern __MANGLE__ int	sumblock __PROTO__((Sum_t*, const __V_*, size_t));
extern __MANGLE__ int	sumdone __PROTO__((Sum_t*));
extern __MANGLE__ int	sumdata __PROTO__((Sum_t*, Sumdata_t*));
extern __MANGLE__ int	sumprint __PROTO__((Sum_t*, Sfio_t*, int, size_t));
extern __MANGLE__ int	sumusage __PROTO__((Sfio_t*));
extern __MANGLE__ int	sumclose __PROTO__((Sum_t*));

#endif
