#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2017 Hayashi Naoyuki
#

include ../../../Makefile.master

HDRS=  \
	archsystm.h \
	asm_linkage.h \
	bootconf.h \
	bootinfo.h \
	bootsvcs.h \
	bootvfs.h \
	controlregs.h \
	cpu.h \
	ddi_isa.h \
	fasttrap_isa.h \
	fp.h \
	frame.h \
	gic.h \
	inline.h \
	kdi_machimpl.h \
	kdi_regs.h \
	machlock.h \
	machsig.h \
	machtypes.h \
	mutex_impl.h \
	obpdefs.h \
	old_procfs.h \
	pcb.h \
	platform_module.h \
	polled_io.h \
	privregs.h \
	procfs_isa.h \
	prom_plat.h \
	promif.h \
	promimpl.h \
	psw.h \
	pte.h \
	reg.h \
	regset.h \
	spl.h \
	stack.h \
	stat_impl.h \
	synch32.h \
	sysconfig_impl.h \
	trap.h \
	ucontext.h \
	utrap.h \
	vmparam.h \
	x_call.h

ROOTDIR=	$(ROOT)/usr/include/sys
ROOTHDRS=	$(HDRS:%=$(ROOTDIR)/%)
ROOTDIRS=	$(ROOTDIR)

install_h: $(ROOTHDRS)

$(ROOTDIR)/%: % $(ROOTDIR)
	$(INS.file)

$(ROOTDIRS):
	$(INS.dir)

