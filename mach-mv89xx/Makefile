#
# Makefile for the linux kernel.
#

# Common support
obj-$(CONFIG_ARCH_MV89XX) += \
			io.o \
			power.o \
			pwm.o \
			sys.o \

CFLAGS_time.o += -DDEBUG

# Memmory pool manager
obj-$(CONFIG_ION) += ion.o

# Specific board support
obj-$(CONFIG_VICTORIA_FPGA_BOARD) += board-victoria-fpga.o
