#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License, Version 1.0 only
# (the "License").  You may not use this file except in compliance
# with the License.
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

LIBRARY = libcertdb.a
VERS = .1
OBJECTS = \
	alg1485.o \
	certdb.o \
	certv3.o \
	certxutl.o \
	crl.o \
	genname.o \
	stanpcertdb.o \
	polcyxtn.o \
	secname.o \
	xauthkid.o \
	xbsconst.o \
	xconst.o
include ../../Makefile.nss

HDRDIR=		$(NSS_BASE)/lib/certdb
SRCDIR=		$(NSS_BASE)/lib/certdb

LIBS =		$(LIBRARY)

MAPFILE=
CPPFLAGS +=
CFLAGS +=
LDLIBS +=
ROOTLIBS=
ROOTLINKS=

all: $(LIBS)
install: all $(ROOTLIBS) $(ROOTLINKS)

include $(SRC)/lib/Makefile.targ
