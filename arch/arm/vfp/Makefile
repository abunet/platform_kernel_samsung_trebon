#
# linux/arch/arm/vfp/Makefile
#
# Copyright (C) 2001 ARM Limited
#

# ccflags-y := -DDEBUG
# asflags-y := -DDEBUG

ifdef CONFIG_LINARO
KBUILD_AFLAGS   :=$(KBUILD_AFLAGS:-msoft-float=-Wa,-mfpu=neon-vfpv4 -funsafe-math-optimizations -funsafe-loop-optimizations)
else
KBUILD_AFLAGS	:=$(KBUILD_AFLAGS:-msoft-float=-Wa,-mfpu=softvfp+vfp -mfloat-abi=soft)
endif
LDFLAGS		+=--no-warn-mismatch

obj-y			+= vfp.o

vfp-$(CONFIG_VFP)	+= vfpmodule.o entry.o vfphw.o vfpsingle.o vfpdouble.o
