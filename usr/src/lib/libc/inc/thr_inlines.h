/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */
/*
 * Copyright 2017 Hayashi Naoyuki
 */

/*
 * Copyright 2019 Joyent, Inc.
 */

#ifndef _THR_INLINES_H
#define	_THR_INLINES_H

#include <sys/ccompile.h>

#if !defined(__lint) && defined(__GNUC__)
#if defined(__alpha)
#include <sys/pal.h>
#endif
#if defined(__aarch64)
#include <sys/controlregs.h>
#endif
/* inlines for gcc */

/*
 * ON-usable GCC 4.x emits register pseudo-ops declaring %g7 as ignored, rather
 * than scratch, GCC 3 does the reverse.  All uses, both ones it generated
 * (_curthread) and ones it didn't (__curthread) must agree.
 */
#if __GNUC__ > 3
#define	SPARC_REG_SPEC	"#ignore"
#else
#define	SPARC_REG_SPEC	"#scratch"
#endif

static __inline__ ulwp_t *
_curthread(void)
{
#if defined(__amd64)
	ulwp_t *__value;
	__asm__ __volatile__("movq %%fs:0, %0" : "=r" (__value));
#elif defined(__i386)
	ulwp_t *__value;
	__asm__ __volatile__("movl %%gs:0, %0" : "=r" (__value));
#elif defined(__sparc)
	register ulwp_t *__value __asm__("g7");
#elif defined(__alpha)
	ulwp_t *__value;
	asm volatile(
	    "call_pal %1; mov $0, %0"
		: "=r" (__value)
		: "i" (PAL_rdunique)
		: "$0");
#elif defined(__aarch64)
	ulwp_t *__value;
	asm volatile("mrs %0, tpidr_el0" : "=r"(__value) :: "memory");
#elif defined(__riscv)
	ulwp_t *__value;
	asm volatile("mv %0, tp" :"=r"(__value) :: "memory");
#else
#error	"port me"
#endif
	return (__value);
}

static __inline__ ulwp_t *
__curthread(void)
{
	ulwp_t *__value;
#if defined(__alpha)
	asm volatile(
	    "call_pal %1; mov $0, %0\n"
	    "beq %0, 1f\n"
	    "ldq %0, %2(%0)\n"
	    "1:\n"
		: "=r" (__value)
		: "i" (PAL_rdunique), "i"(__builtin_offsetof(ulwp_t, ul_self))
		: "$0");
#elif defined(__aarch64)
	asm volatile(
	    "mrs %0, tpidr_el0\n"
	    "cbz %0, 1f\n"
	    "ldr %0, [%0, %1]\n"
	    "1:\n"
	    : "=r"(__value) : "i"(__builtin_offsetof(ulwp_t, ul_self)));
#elif defined(__riscv)
	asm volatile(
	    "mv %0, tp\n"
	    "beqz %0, 1f\n"
	    "ld %0, %1(%0)\n"
	    "1:\n"
	    : "=r"(__value) : "i"(__builtin_offsetof(ulwp_t, ul_self)));
#else
	__asm__ __volatile__(
#if defined(__amd64)
	    "movq %%fs:0, %0\n\t"
#elif defined(__i386)
	    "movl %%gs:0, %0\n\t"
#elif defined(__sparcv9)
	    ".register %%g7, " SPARC_REG_SPEC "\n\t"
	    "ldx [%%g7 + 80], %0\n\t"
#elif defined(__sparc)
	    ".register %%g7, " SPARC_REG_SPEC "\n\t"
	    "ld [%%g7 + 80], %0\n\t"
#else
#error	"port me"
#endif
	    : "=r" (__value));
#endif

	return (__value);
}

static __inline__ greg_t
stkptr(void)
{
#if defined(__amd64)
	register greg_t __value __asm__("rsp");
#elif defined(__i386)
	register greg_t __value __asm__("esp");
#elif defined(__sparc)
	register greg_t __value __asm__("sp");
#elif defined(__alpha)
	register greg_t __value __asm__("$30");
#elif defined(__aarch64)
	register greg_t __value __asm__("sp");
#elif defined(__riscv)
	register greg_t __value __asm__("sp");
#else
#error	"port me"
#endif
	return (__value);
}

#if !defined(__alpha) && !defined(__aarch64) && !defined(__riscv)
static __inline__ hrtime_t
gethrtime(void)		/* note: caller-saved registers are trashed */
{
#if defined(__amd64)
	hrtime_t __value;
	__asm__ __volatile__(
	    "movl $3, %%eax\n\t"
	    "int $0xd2"
	    : "=a" (__value)
	    : : "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10", "r11", "cc");
#elif defined(__i386)
	hrtime_t __value;
	__asm__ __volatile__(
	    "movl $3, %%eax\n\t"
	    "int $0xd2"
	    : "=A" (__value)
	    : : "ecx", "cc");
#elif defined(__sparcv9)
	register hrtime_t __value __asm__("o0");
	__asm__ __volatile__(
	    "ta 0x24\n\t"
	    "sllx %%o0, 32, %0\n\t"
	    "or %%o1, %0, %0"
	    : "=r" (__value)
	    : : "o1", "o2", "o3", "o4", "o5", "cc");
#elif defined(__sparc)
	register hrtime_t __value __asm__("o0");
	__asm__ __volatile__(
	    "ta 0x24"
	    : "=r" (__value)
	    : : "o2", "o3", "o4", "o5", "cc");
#else
#error	"port me"
#endif
	return (__value);
}
#endif

static __inline__ int
set_lock_byte(volatile uint8_t *__lockp)
{
	int __value = 0;
#if defined(__x86)
	__asm__ __volatile__(
	    "movl $1, %0\n\t"
	    "xchgb %%dl, %1"
	    : "+d" (__value), "+m" (*__lockp));
#elif defined(__sparc)
	__asm__ __volatile__(
	    "ldstub %1, %0\n\t"
	    "membar #LoadLoad"
	    : "=r" (__value), "+m" (*__lockp));
#elif defined(__alpha) || defined(__aarch64) || defined(__riscv)
	__value = (uint8_t)__sync_lock_test_and_set(__lockp, 1);
#else
#error	"port me"
#endif
	return (__value);
}

#if !defined(__alpha) && !defined(__aarch64) && !defined(__riscv)
static __inline__ uint32_t
atomic_swap_32(volatile uint32_t *__memory, uint32_t __value)
{
#if defined(__x86)
	__asm__ __volatile__(
	    "xchgl %0, %1"
	    : "+q" (__value), "+m" (*__memory));
	return (__value);
#elif defined(__sparc)
	uint32_t __tmp1, __tmp2;
	__asm__ __volatile__(
	    "ld [%3], %0\n\t"
	    "1:\n\t"
	    "mov %4, %1\n\t"
	    "cas [%3], %0, %1\n\t"
	    "cmp %0, %1\n\t"
	    "bne,a,pn %%icc, 1b\n\t"
	    "  mov %1, %0"
	    : "=&r" (__tmp1), "=&r" (__tmp2), "=m" (*__memory)
	    : "r" (__memory), "r" (__value)
	    : "cc");
	return (__tmp2);
#else
#error	"port me"
#endif
}

static __inline__ uint32_t
atomic_cas_32(volatile uint32_t *__memory, uint32_t __cmp, uint32_t __newvalue)
{
	uint32_t __oldvalue;
#if defined(__x86)
	__asm__ __volatile__(
	    "lock; cmpxchgl %3, %0"
	    : "=m" (*__memory), "=a" (__oldvalue)
	    : "a" (__cmp), "r" (__newvalue));
#elif defined(__sparc)
	__asm__ __volatile__(
	    "cas [%2], %3, %1"
	    : "=m" (*__memory), "=&r" (__oldvalue)
	    : "r" (__memory), "r" (__cmp), "1" (__newvalue));
#else
#error	"port me"
#endif
	return (__oldvalue);
}

static __inline__ void
atomic_inc_32(volatile uint32_t *__memory)
{
#if defined(__x86)
	__asm__ __volatile__(
	    "lock; incl %0"
	    : "+m" (*__memory));
#elif defined(__sparc)
	uint32_t __tmp1, __tmp2;
	__asm__ __volatile__(
	    "ld [%3], %0\n\t"
	    "1:\n\t"
	    "add %0, 1, %1\n\t"
	    "cas [%3], %0, %1\n\t"
	    "cmp %0, %1\n\t"
	    "bne,a,pn %%icc, 1b\n\t"
	    "  mov %1, %0"
	    : "=&r" (__tmp1), "=&r" (__tmp2), "=m" (*__memory)
	    : "r" (__memory)
	    : "cc");
#else
#error	"port me"
#endif
}

static __inline__ void
atomic_dec_32(volatile uint32_t *__memory)
{
#if defined(__x86)
	__asm__ __volatile__(
	    "lock; decl %0"
	    : "+m" (*__memory));
#elif defined(__sparc)
	uint32_t __tmp1, __tmp2;
	__asm__ __volatile__(
	    "ld [%3], %0\n\t"
	    "1:\n\t"
	    "sub %0, 1, %1\n\t"
	    "cas [%3], %0, %1\n\t"
	    "cmp %0, %1\n\t"
	    "bne,a,pn %%icc, 1b\n\t"
	    "  mov %1, %0"
	    : "=&r" (__tmp1), "=&r" (__tmp2), "=m" (*__memory)
	    : "r" (__memory)
	    : "cc");
#else
#error	"port me"
#endif
}

static __inline__ void
atomic_and_32(volatile uint32_t *__memory, uint32_t __bits)
{
#if defined(__x86)
	__asm__ __volatile__(
	    "lock; andl %1, %0"
	    : "+m" (*__memory)
	    : "r" (__bits));
#elif defined(__sparc)
	uint32_t __tmp1, __tmp2;
	__asm__ __volatile__(
	    "ld [%3], %0\n\t"
	    "1:\n\t"
	    "and %0, %4, %1\n\t"
	    "cas [%3], %0, %1\n\t"
	    "cmp %0, %1\n\t"
	    "bne,a,pn %%icc, 1b\n\t"
	    "  mov %1, %0"
	    : "=&r" (__tmp1), "=&r" (__tmp2), "=m" (*__memory)
	    : "r" (__memory), "r" (__bits)
	    : "cc");
#else
#error	"port me"
#endif
}

static __inline__ void
atomic_or_32(volatile uint32_t *__memory, uint32_t __bits)
{
#if defined(__x86)
	__asm__ __volatile__(
	    "lock; orl %1, %0"
	    : "+m" (*__memory)
	    : "r" (__bits));
#elif defined(__sparc)
	uint32_t __tmp1, __tmp2;
	__asm__ __volatile__(
	    "ld [%3], %0\n\t"
	    "1:\n\t"
	    "or %0, %4, %1\n\t"
	    "cas [%3], %0, %1\n\t"
	    "cmp %0, %1\n\t"
	    "bne,a,pn %%icc, 1b\n\t"
	    "  mov %1, %0"
	    : "=&r" (__tmp1), "=&r" (__tmp2), "=m" (*__memory)
	    : "r" (__memory), "r" (__bits)
	    : "cc");
#else
#error	"port me"
#endif
}
#endif

#if defined(__sparc)	/* only needed on sparc */

static __inline__ ulong_t
caller(void)
{
	register ulong_t __value __asm__("i7");
	return (__value);
}

static __inline__ ulong_t
getfp(void)
{
	register ulong_t __value __asm__("fp");
	return (__value);
}

#endif	/* __sparc */

#if defined(__x86)	/* only needed on x86 */

static __inline__ void
ht_pause(void)
{
	__asm__ __volatile__("rep; nop");
}

#endif	/* __x86 */

#endif	/* !__lint && __GNUC__ */

#endif	/* _THR_INLINES_H */
