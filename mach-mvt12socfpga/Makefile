#
# Makefile for the linux kernel.
#

# Common support
obj-$(CONFIG_ARCH_MVT12SOCFPGA) += \
			io.o \
			power.o \
			pwm.o \
			sys.o \

CFLAGS_time.o += -DDEBUG

# Memmory pool manager
obj-$(CONFIG_ION) += ion.o

# Specific board support
obj-$(CONFIG_MFP_7V2000T_Q_FPGA_BOARD) += board-mfp7v2000tq-socfpga.o
