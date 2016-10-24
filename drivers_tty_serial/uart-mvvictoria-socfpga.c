/*
 *  drivers/serial/mvvictoria_fpga-uart.c
 *
 *  Copyright (C) 2010 MtekVision Ltd.
 *  	Tein Oh <ohti@mtekvision.com>
 *	Jinyoung Park <parkjy@mtekvision.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#if defined(CONFIG_SERIAL_MVVICTORIA_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/serial_reg.h>
#include <linux/circ_buf.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>

#include <linux/of.h>
#include <linux/mod_devicetable.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>

#include <linux/io.h>
#include <asm/irq.h>

#include <mach/map.h>
#include <mach/hardware.h>
#include <mach/sys.h>
#include <mach/regs-uart.h>
#include <mach/uart.h>

#define DRV_NAME		"mvvictoria-uart"
#define MVFORMULA_MAX_UART	4
#define MV_DEVICENAME		"ttyS"

#ifdef CONFIG_OF
#if defined (CONFIG_VICTORIA_DEBUG_UART0)
static unsigned int uart_id = 0;
#elif defined(CONFIG_VICTORIA_DEBUG_UART1)
static unsigned int uart_id = 1;
#elif defined(CONFIG_VICTORIA_DEBUG_UART2)
static unsigned int uart_id = 2;
#elif defined(CONFIG_VICTORIA_DEBUG_UART3)
static unsigned int uart_id = 3;
#endif
#endif

#ifndef CONFIG_OF
extern struct platform_device *mv_default_console_device;
#endif

struct mv_uart_port {
	struct uart_port port;
	char *name;
	u32 int_mask;
	u32 cts_status;
	int debug;

	struct irq_domain *irq_dm;
};

static void mv_uart_stop_tx(struct uart_port *port);

static void mv_uart_hw_init(struct mv_uart_port *up)
{
	if (up->port.mapbase == UART0_BASE)
		mv_sys_set_dev_clk_power(SYS_CLK_UART0, 1);
	else if (up->port.mapbase == UART1_BASE)
		mv_sys_set_dev_clk_power(SYS_CLK_UART1, 1);
	else if (up->port.mapbase == UART2_BASE)
		mv_sys_set_dev_clk_power(SYS_CLK_UART2, 1);
	else
		mv_sys_set_dev_clk_power(SYS_CLK_UART3, 1);
}

static void mv_uart_hw_uninit(struct mv_uart_port *up)
{
	if (up->debug)
		return;

	if (up->port.mapbase == UART0_BASE)
		mv_sys_set_dev_clk_power(SYS_CLK_UART0, 0);
	else if (up->port.mapbase == UART1_BASE)
		mv_sys_set_dev_clk_power(SYS_CLK_UART1, 0);
	else if (up->port.mapbase == UART2_BASE)
		mv_sys_set_dev_clk_power(SYS_CLK_UART2, 0);
	else
		mv_sys_set_dev_clk_power(SYS_CLK_UART3, 0);
}

static inline void mv_uart_flush_fifo(struct mv_uart_port *up, u8 tx, u8 rx)
{
	u32 val;

	val = readl(up->port.membase + UART_FIFO_STATUS);
	if (tx)
		val |= UART_FIFO_STATUS_TX_FLUSH_MASK;
	if (rx)
		val |= UART_FIFO_STATUS_RX_FLUSH_MASK;
	writel(val, up->port.membase + UART_FIFO_STATUS);
}

static inline int mv_uart_get_fifo_cnt(struct mv_uart_port *up, u8 type)
{
	u32 val;

	val = readl(up->port.membase + UART_FIFO_STATUS);
	if (type) /* rx */
		val = (val & UART_FIFO_STATUS_RX_CNT_MASK) >>
				UART_FIFO_STATUS_RX_CNT_SHIFT;
	else /* tx */
		val = (val & UART_FIFO_STATUS_TX_CNT_MASK) >>
				UART_FIFO_STATUS_TX_CNT_SHIFT;

	return (int)val;
}

static inline unsigned int mv_uart_get_cts(struct mv_uart_port *up)
{
	u32 val;

	val = (readl(up->port.membase + UART_INT_STATUS)
		& UART_INT_CTS_RAW_MASK) >> UART_INT_CTS_RAW_SHIFT;

	return val;
}

static void mv_uart_set_baudrate(struct mv_uart_port *up, u32 baudrate)
{
	u32 val;
	u32 osc_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
	u32 baud_div;

	for (baud_div = 16; baud_div < UART_BAUDRATE_TX_DIV_WIDTH; baud_div++) {
		if ((osc_clk_hz / baud_div) <= baudrate)
			break;
	}

	baud_div *= 16;
	baud_div /= 5;
	val = (baud_div & UART_BAUDRATE_RX_DIV_WIDTH)
			<< UART_BAUDRATE_RX_DIV_SHIFT;
	val |= (baud_div & UART_BAUDRATE_TX_DIV_WIDTH)
			<< UART_BAUDRATE_TX_DIV_SHIFT;
	writel(val, up->port.membase + UART_BAUDRATE);
}

static inline void mv_uart_tx_chars(struct mv_uart_port *up)
{
	// struct circ_buf *xmit = &up->port.info->xmit;
	struct circ_buf *xmit = &up->port.state->xmit;

	if (up->port.x_char) {
		writeb(up->port.x_char, up->port.membase + UART_TX_FIFO);
		up->port.icount.tx++;
		up->port.x_char = 0;
		return;
	}

	if (uart_tx_stopped(&up->port) || uart_circ_empty(xmit)) {
		mv_uart_stop_tx(&up->port);
		return;
	}

	while (!uart_circ_empty(xmit) && (mv_uart_get_fifo_cnt(up, 0) > 0)) {
		writeb(xmit->buf[xmit->tail], up->port.membase + UART_TX_FIFO);
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		up->port.icount.tx++;
	}

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(&up->port);

	if (uart_circ_empty(xmit))
		mv_uart_stop_tx(&up->port);
}

static inline void mv_uart_rx_chars(struct mv_uart_port *up)
{
	// struct tty_struct *tty = up->port.info->port.tty;
	// struct tty_struct *tty = up->port.state->port.tty;
	unsigned int c;
	char flag;
	u32 int_status;
	int max_count = up->port.fifosize - 16;

	while (max_count-- > 0) {
		if (mv_uart_get_fifo_cnt(up, 1) == 0)
			break;

		c = readb(up->port.membase + UART_RX_FIFO) & 0xFF;
		flag = TTY_NORMAL;
		up->port.icount.rx++;

		int_status = readl(up->port.membase + UART_INT_STATUS);
		if (int_status & UART_INT_PARITY_ERR_MASK)
			up->port.icount.parity++;
		else if (int_status & UART_INT_FRAME_ERR_MASK)
			up->port.icount.frame++;

		writel(UART_INT_FRAME_ERR_MASK | UART_INT_PARITY_ERR_MASK,
			up->port.membase + UART_INT_CLR);

		if (int_status & UART_INT_RX_PUSH_ERR_MASK) {
			up->port.icount.overrun++;
			writel(UART_INT_RX_PUSH_ERR_MASK,
				up->port.membase + UART_INT_CLR);
		}

		int_status &= up->port.read_status_mask;
		if (int_status & UART_INT_PARITY_ERR_MASK)
			flag = TTY_PARITY;
		else if (int_status & UART_INT_FRAME_ERR_MASK)
			flag = TTY_FRAME;

		if (uart_handle_sysrq_char(&up->port, c))
			continue;

		uart_insert_char(&up->port, int_status,
				UART_INT_RX_PUSH_ERR_MASK, c, flag);
	}

	tty_flip_buffer_push(&up->port.state->port);
}

static inline void mv_uart_check_modem_status(struct mv_uart_port *up)
{
	u32 delta_status;
	u32 status;

	status = mv_uart_get_cts(up);
	delta_status = status ^ up->cts_status;
	if (!delta_status)
		return;

	uart_handle_cts_change(&up->port, status);
	up->cts_status = status;
	// wake_up_interruptible(&up->port.info->delta_msr_wait);
	wake_up_interruptible(&up->port.state->port.delta_msr_wait);
}

static irqreturn_t mv_uart_irq(int irq, void *dev_id)
{
	struct mv_uart_port *up = (struct mv_uart_port *)dev_id;
	u32 int_rx_masks = (UART_INT_RX_FF_MASK | UART_INT_RX_HF_MASK
			| UART_INT_RX_FE_MASK);
	u32 int_tx_masks = (UART_INT_TX_FF_MASK | UART_INT_TX_FE_MASK);
	u32 int_status;

	writel(0xFFFFFFFF, up->port.membase + UART_INT_MASK);
	int_status = readl(up->port.membase + UART_INT_STATUS);
	if (int_status & int_rx_masks)
		mv_uart_rx_chars(up);

	mv_uart_check_modem_status(up);

	if (int_status & int_tx_masks)
		mv_uart_tx_chars(up);

	writel(0xFFFFFFFF, up->port.membase + UART_INT_CLR);
	writel(up->int_mask, up->port.membase + UART_INT_MASK);

	return IRQ_HANDLED;
}

static const char *mv_uart_type(struct uart_port *port)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;

	return up->name;
}

static unsigned int mv_uart_tx_empty(struct uart_port *port)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;
	unsigned long flags;
	unsigned int ret = 0;

	spin_lock_irqsave(&up->port.lock, flags);
	if (mv_uart_get_fifo_cnt(up, 0) == up->port.fifosize)
		ret = TIOCSER_TEMT;
	spin_unlock_irqrestore(&up->port.lock, flags);

	return ret;
}

static unsigned int mv_uart_get_mctrl(struct uart_port *port)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;
	u32 val;
	unsigned int ret = 0;

	val = readl(up->port.membase + UART_INT_STATUS);
	if (val & UART_INT_CTS_RAW_MASK)
		ret |= TIOCM_CTS;

	return ret;
}

static void mv_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;
	u32 val;

	val = readl(up->port.membase + UART_CFG);

	if (mctrl & TIOCM_RTS)
		val |= UART_CFG_RTS_ENA_MASK;
	else
		val &= ~UART_CFG_RTS_ENA_MASK;

	writel(val, up->port.membase + UART_CFG);
}

static void mv_uart_stop_tx(struct uart_port *port)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;

	up->int_mask |= (UART_INT_TX_FE_MASK | UART_INT_TX_FF_MASK);
	writel(up->int_mask, up->port.membase + UART_INT_MASK);
}

static void mv_uart_start_tx(struct uart_port *port)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;

	up->int_mask &= ~(UART_INT_TX_FE_MASK | UART_INT_TX_FF_MASK);
	writel(up->int_mask, up->port.membase + UART_INT_MASK);

	mv_uart_tx_chars(up);
}

static void mv_uart_stop_rx(struct uart_port *port)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;

	up->int_mask |= (UART_INT_RX_FF_MASK | UART_INT_RX_HF_MASK \
			| UART_INT_RX_FE_MASK);
	writel(up->int_mask, up->port.membase + UART_INT_MASK);
}

static void mv_uart_enable_ms(struct uart_port *port)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;
#if 0
	/* CTS interrupt triggering is falling edge,
	 * but Formula's interrupt triggering is level.
	 * So CTS interrupt is disabled. */
	up->int_mask &= ~UART_INT_CTS_RAW_MASK;
#endif
	writel(up->int_mask, up->port.membase + UART_INT_MASK);
}

static void mv_uart_break_ctl(struct uart_port *port, int break_state)
{
}

static int mv_uart_startup(struct uart_port *port)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;
	u32 val;
	int ret = 0;

	mv_uart_hw_init(up);

	writel(0xFFFFFFFF, up->port.membase + UART_INT_MASK);
	writel(0xFFFFFFFF, up->port.membase + UART_INT_CLR);

	ret = request_irq(up->port.irq, mv_uart_irq, 0, up->name, up);
	if (ret) {
		pr_err(DRV_NAME ": %s: Can't get irq %d ret %d\n", up->name,
			up->port.irq, ret);
		goto out;
	}

	mv_uart_flush_fifo(up, 1, 1);

	val = 1 << UART_FIFO_CFG_RX_HE_SHIFT;
	val |= (up->port.fifosize / 2) << UART_FIFO_CFG_TX_HF_SHIFT;
	writel(val, up->port.membase + UART_FIFO_CFG);

	val = readl(up->port.membase + UART_CFG);
	val &= ~UART_CFG_RX_FIFO_SIZE_MASK;
	val |= (UART_CFG_FIFO_SIZE_8BITS << UART_CFG_RX_FIFO_SIZE_SHIFT);
	val &= ~UART_CFG_TX_FIFO_SIZE_MASK;
	val |= (UART_CFG_FIFO_SIZE_8BITS << UART_CFG_TX_FIFO_SIZE_SHIFT);
	val |= UART_CFG_EXT_PAD_EN_MASK;
	val &= ~UART_CFG_RX_SAMPLING_POS_MASK;
	val |= (2 << UART_CFG_RX_SAMPLING_POS_SHIFT);
	val &= ~UART_CFG_RTS_FIFO_LEVEL_MASK;
	val |= (16 << UART_CFG_RTS_FIFO_LEVEL_SHIFT);
	writel(val, up->port.membase + UART_CFG);

	up->cts_status = mv_uart_get_cts(up);

	up->int_mask &= ~(UART_INT_RX_PUSH_ERR_MASK | UART_INT_PARITY_ERR_MASK
			| UART_INT_FRAME_ERR_MASK | UART_INT_RX_HF_MASK);
	writel(up->int_mask, up->port.membase + UART_INT_MASK);
	writel(0xFFFFFFFF, up->port.membase + UART_INT_CLR);

out:
	return ret;
}

static void mv_uart_shutdown(struct uart_port *port)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;
	u32 val;

	up->int_mask = 0xFFFFFFFF;
	writel(up->int_mask, up->port.membase + UART_INT_MASK);
	writel(0xFFFFFFFF, up->port.membase + UART_INT_CLR);

	free_irq(up->port.irq, up);

	mv_uart_flush_fifo(up, 1, 1);

	val = readl(up->port.membase + UART_CFG);
	val &= ~UART_CFG_EXT_PAD_EN_MASK;
	writel(val, up->port.membase + UART_CFG);

	mv_uart_hw_uninit(up);
}

static void mv_uart_set_termios(struct uart_port *port, struct ktermios *new,
			struct ktermios *old)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;
	u32 cfg_val, mode_val;
	u32 baud;
	u32 osc_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
	unsigned long flags;

	cfg_val = readl(up->port.membase + UART_CFG);
	mode_val = readl(up->port.membase + UART_MODE);

	/* set high speed mode */
	mode_val |= UART_MODE_HS_MASK;

	/* set bits per char */
	mode_val &= ~(UART_MODE_RX_WORD_LEN_MASK | UART_MODE_TX_WORD_LEN_MASK);
	switch (new->c_cflag & CSIZE) {
	case CS8:
		mode_val |= (UART_MODE_WORD_LEN_8BITS
				<< UART_MODE_RX_WORD_LEN_SHIFT);
		mode_val |= (UART_MODE_WORD_LEN_8BITS
				<< UART_MODE_TX_WORD_LEN_SHIFT);
		break;
	case CS7:
		mode_val |= (UART_MODE_WORD_LEN_7BITS
				<< UART_MODE_RX_WORD_LEN_SHIFT);
		mode_val |= (UART_MODE_WORD_LEN_7BITS
				<< UART_MODE_TX_WORD_LEN_SHIFT);
		break;
	case CS6:
		mode_val |= (UART_MODE_WORD_LEN_6BITS
				<< UART_MODE_RX_WORD_LEN_SHIFT);
		mode_val |= (UART_MODE_WORD_LEN_6BITS
				<< UART_MODE_TX_WORD_LEN_SHIFT);
		break;
	case CS5:
		mode_val |= (UART_MODE_WORD_LEN_5BITS
				<< UART_MODE_RX_WORD_LEN_SHIFT);
		mode_val |= (UART_MODE_WORD_LEN_5BITS
				<< UART_MODE_TX_WORD_LEN_SHIFT);
		break;
	default:
		pr_err(DRV_NAME ": %s: Invalied CSIZE: 0x%x\n",
			up->name, new->c_cflag & CSIZE);
		BUG();
	}

	/* set stop bit */
	cfg_val &= ~UART_CFG_2STOP_BITS_MASK;
	if (new->c_cflag & CSTOPB)
		cfg_val |= UART_CFG_2STOP_BITS_MASK;

	/* set parity */
	cfg_val &= ~UART_CFG_PARITY_MASK;
	if (new->c_cflag & PARENB) {
		if (new->c_cflag & PARODD)
			cfg_val |= (UART_CFG_ODD_PARITY
				<< UART_CFG_PARITY_SHIFT);
		else
			cfg_val |= (UART_CFG_EVEN_PARITY
				<< UART_CFG_PARITY_SHIFT);
	} else
		cfg_val |= (UART_CFG_NO_PARITY << UART_CFG_PARITY_SHIFT);

	/* set baudrate */
	baud = uart_get_baud_rate(port, new, old,
				osc_clk_hz / UART_BAUDRATE_RX_DIV_WIDTH,
				osc_clk_hz / 16);

	/* set CTS and RTS */
	cfg_val &= ~(UART_CFG_CTS_ENA_MASK | UART_CFG_RTS_ENA_MASK);
	if (new->c_cflag & CRTSCTS)
		cfg_val |= (UART_CFG_CTS_ENA_MASK | UART_CFG_RTS_ENA_MASK);

	/* CTS flow control flag and modem status interrupts */
	up->int_mask |= UART_INT_CTS_RAW_MASK;
#if 0
	if (UART_ENABLE_MS(&up->port, new->c_cflag))
		up->int_mask &= ~UART_INT_CTS_RAW_MASK;
#endif

	spin_lock_irqsave(&up->port.lock, flags);

	/* update the per-port timout */
	uart_update_timeout(port, new->c_cflag, baud);

	/* configure read status bits */
	up->port.read_status_mask = (UART_INT_RX_PUSH_ERR_MASK
				| UART_INT_RX_HF_MASK);
	if (new->c_iflag & INPCK)
		up->port.read_status_mask |= (UART_INT_PARITY_ERR_MASK
					| UART_INT_FRAME_ERR_MASK);

	/* configure ignore status bits */
	up->port.ignore_status_mask = 0;
	if (new->c_iflag & IGNPAR)
		up->port.ignore_status_mask |= (UART_INT_PARITY_ERR_MASK
					| UART_INT_FRAME_ERR_MASK);
	/* ignore all characters */
	if ((new->c_cflag & CREAD) == 0)
		up->port.ignore_status_mask |= UART_INT_RX_HF_MASK;

	mv_uart_set_baudrate(up, baud);
	writel(cfg_val, up->port.membase + UART_CFG);
	writel(mode_val, up->port.membase + UART_MODE);
	writel(up->int_mask, up->port.membase + UART_INT_MASK);
	mv_uart_set_mctrl(&up->port, up->port.mctrl);
	spin_unlock_irqrestore(&up->port.lock, flags);

	dev_dbg(up->port.dev, "CSIZE=%s, CSTOPB=%d, PARENB=%s, CRTSCTS=%d, "
		"INPCK=%d, IGNPAR=%d, CREAD=%d, CLOCAL=%d, CBAUD=%u\n",
		new->c_cflag & CS8 ? "8" : (new->c_cflag & CS7 ? "7" :
			(new->c_cflag & CS6 ? "6" : (new->c_cflag & CS5 ? "5" :
			"UNKNOWN"))),
		new->c_cflag & CSTOPB ? 1 : 0,
		new->c_cflag & PARENB ?
			(new->c_cflag & PARODD ? "ODD" : "EVEN") : "NONE",
		new->c_cflag & CRTSCTS ? 1 : 0,
		new->c_iflag & INPCK ? 1 : 0,
		new->c_iflag & IGNPAR ? 1 : 0,
		new->c_cflag & CREAD ? 1 : 0,
		new->c_cflag & CLOCAL ? 1 : 0,
		baud);
}

static void mv_uart_pm(struct uart_port *port, unsigned int state,
			unsigned int oldstate)
{
}

static void mv_uart_release_port(struct uart_port *port)
{
}

static int mv_uart_request_port(struct uart_port *port)
{
	return 0;
}

static void mv_uart_config_port(struct uart_port *port, int flags)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;

	up->port.type = PORT_MVVICTORIA;
}

static int mv_uart_verify_port(struct uart_port *port,
				struct serial_struct *ser)
{
	int ret = 0;

	if ((ser->type != PORT_UNKNOWN) && (ser->type != PORT_MVVICTORIA)) {
		ret = -EINVAL;
		goto out;
	}

	if (port->irq != ser->irq) {
		ret = -EINVAL;
		goto out;
	}

out:
	return ret;
}

struct uart_ops mv_uart_ops = {
	.tx_empty	= mv_uart_tx_empty,
	.set_mctrl	= mv_uart_set_mctrl,
	.get_mctrl	= mv_uart_get_mctrl,
	.stop_tx	= mv_uart_stop_tx,
	.start_tx	= mv_uart_start_tx,
	.stop_rx	= mv_uart_stop_rx,
	.enable_ms	= mv_uart_enable_ms,
	.break_ctl	= mv_uart_break_ctl,
	.startup	= mv_uart_startup,
	.shutdown	= mv_uart_shutdown,
	.set_termios	= mv_uart_set_termios,
	.pm		= mv_uart_pm, .type		= mv_uart_type,
	.release_port	= mv_uart_release_port,
	.request_port	= mv_uart_request_port,
	.config_port	= mv_uart_config_port,
	.verify_port	= mv_uart_verify_port,
};

// static int __devinit mv_uart_init_port(struct platform_device *pdev,
//							 			  struct mv_uart_port *up)
#ifdef CONFIG_OF
static int mv_uart_init_port(struct platform_device *pdev,
							  struct mv_uart_port *up)
{
	struct resource resource;
	struct device_node *np = pdev->dev.of_node;
	unsigned char __iomem *mem_base = NULL;
	unsigned int irq;
	u32 prop;
	int ret = 0;

	memset(up, 0, sizeof(struct uart_port));

	ret = of_address_to_resource(np, 0, &resource);
	if (ret)
		return ret;

	mem_base = of_iomap(np, 0);
	if (!mem_base)
		return -EINVAL;

	mv_uart_hw_init(up);
	
	up->port.type     = PORT_MVVICTORIA;
	up->port.iotype   = UPIO_MEM;
	up->port.mapbase  = resource.start;
	up->port.membase  = mem_base;
	up->port.irq      = irq_of_parse_and_map(np, 0);
	up->port.uartclk  = 115200;
	up->port.fifosize = 128;
	up->port.ops      = &mv_uart_ops;
	up->port.line     = pdev->id;
	up->port.dev      = &pdev->dev;
	up->debug         = 1;

	up->int_mask = 0xFFFFFFFF;
	writel(up->int_mask, up->port.membase + UART_INT_MASK);
	writel(0xFFFFFFFF, up->port.membase + UART_INT_CLR);

out:
	return ret;
}
#else
static int mv_uart_init_port(struct platform_device *pdev,
							  struct mv_uart_port *up)
{
	struct mv_serial_platform_data *board_data;
	int ret = 0;

	board_data = pdev->dev.platform_data;
	if (!board_data) {
		ret = -ENXIO;
		goto out;
	}

	if (board_data->mapbase == UART0_BASE)
		up->name = "mvvictoria_fpga-uart0";
	else if (board_data->mapbase == UART1_BASE)
		up->name = "mvvictoria_fpga-uart1";
	else if (board_data->mapbase == UART2_BASE)
		up->name = "mvvictoria_fpga-uart2";
	else if (board_data->mapbase == UART3_BASE)
		up->name = "mvvictoria_fpga-uart3";
	else {
		ret = -EINVAL;
		goto out;
	}

	mv_uart_hw_init(up);

	up->port.type = PORT_MVVICTORIA;
	up->port.iotype = UPIO_MEM;
	up->port.mapbase = board_data->mapbase;
	up->port.membase = (unsigned char __iomem *)board_data->membase;
	up->port.irq = board_data->irq;
	up->port.uartclk = board_data->uartclk;
	up->port.fifosize = 128;
	up->port.ops = &mv_uart_ops;
	up->port.line = pdev->id;
	up->port.dev = &pdev->dev;
	up->debug = board_data->debug;

	up->int_mask = 0xFFFFFFFF;
	writel(up->int_mask, up->port.membase + UART_INT_MASK);
	writel(0xFFFFFFFF, up->port.membase + UART_INT_CLR);

out:
	return ret;
}
#endif

static struct mv_uart_port mv_uart_ports[MVFORMULA_MAX_UART];
static struct uart_driver mv_uart_driver;

#ifdef CONFIG_SERIAL_MVVICTORIA_CONSOLE
static void mv_uart_console_putchar(struct uart_port *port, int c)
{
	struct mv_uart_port *up = (struct mv_uart_port *)port;

	while (mv_uart_get_fifo_cnt(up, 0) < 1)
		continue;

	writeb(c, up->port.membase + UART_TX_FIFO);
}

/*
 * Print a string to the serial port trying not to disturb
 * any possible real use of the port...
 *
 *	The console_lock must be held when we get here.
 */
static void mv_uart_console_write(struct console *co, const char *s,
				unsigned int count)
{
	struct mv_uart_port *up = &mv_uart_ports[co->index];
	u32 int_mask;

	int_mask = readl(up->port.membase + UART_INT_MASK);
	writel(0xFFFFFFFF, up->port.membase + UART_INT_MASK);

	uart_console_write(&up->port, s, count, mv_uart_console_putchar);

	while (mv_uart_get_fifo_cnt(up, 0) < up->port.fifosize)
		continue;

	writel(int_mask, up->port.membase + UART_INT_MASK);
}

static int __init mv_uart_console_setup(struct console *co, char *options)
{
	struct mv_uart_port *up;
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	if (co->index == -1 || co->index >= mv_uart_driver.nr)
		co->index = 0;

	up = &mv_uart_ports[co->index];

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);

	return uart_set_options(&up->port, co, baud, parity, bits, flow);
}

static struct console mv_uart_console = {
	.name	= MV_DEVICENAME,
	.write	= mv_uart_console_write,
	.device	= uart_console_device,
	.setup	= mv_uart_console_setup,
	.flags	= CON_PRINTBUFFER,
	.index	= -1,
	.data	= &mv_uart_driver,
};

#ifdef CONFIG_OF
static int of_platform_mv_serial_setup(struct platform_device *ofdev,
									   int type, struct uart_port *port);
#endif
static int __init mv_uart_console_init(void)
{
#ifdef CONFIG_OF
	struct platform_device pdev;
	const char *path = NULL;
	unsigned int id = uart_id;
	int err;

	memset(&pdev, 0, sizeof(pdev));

	err = of_property_read_string(of_aliases, "mv,uart0", &path);
	if (WARN(err, "Falied to get serial path!"))
		return;

	pdev.dev.of_node = of_find_node_by_path(path);
	if (WARN(!pdev.dev.of_node, "Failed to find uart node"))
		return;

	pdev.id = id;

	add_preferred_console(MV_DEVICENAME, id, "115200n8");
	mv_uart_init_port(&pdev, &mv_uart_ports[id]);
	register_console(&mv_uart_console);
#else
	if (mv_default_console_device) {
		add_preferred_console(MV_DEVICENAME,
				mv_default_console_device->id, "115200n8");
		mv_uart_init_port(mv_default_console_device,
				&mv_uart_ports[mv_default_console_device->id]);
		register_console(&mv_uart_console);
	}
#endif
	return 0;
}

console_initcall(mv_uart_console_init);

#define MVFORMULA_CONSOLE	(&mv_uart_console)
#else
#define MVFORMULA_CONSOLE	NULL
#endif /* CONFIG_SERIAL_MVFORMULA_CONSOLE */

static struct uart_driver mv_uart_driver = {
	.owner		 = THIS_MODULE,
	.driver_name = DRV_NAME,
	.dev_name	 = MV_DEVICENAME,
	.major		 = TTY_MAJOR,
	.minor		 = 64,
	.nr			 = MVFORMULA_MAX_UART,
	.cons		 = MVFORMULA_CONSOLE,
};

#ifdef CONFIG_OF
static int of_platform_mv_serial_setup(struct platform_device *ofdev,
									   int type, struct uart_port *port)
{
	struct resource resource;
	struct device_node *np = ofdev->dev.of_node;
	unsigned char __iomem *mem_base = NULL;
	unsigned int irq;
	u32 prop;
	int ret = 0;

	memset(port, 0, sizeof(struct uart_port));

	ret = of_address_to_resource(np, 0, &resource);
	if (ret)
		return ret;

	mem_base = of_iomap(np, 0);
	if (!mem_base)
		return -EINVAL;

	port->type     = type;
	port->iotype   = UPIO_MEM;
	port->mapbase  = resource.start;
	port->membase  = mem_base;
	port->irq      = irq_of_parse_and_map(np, 0);
	port->uartclk  = 115200;
	port->fifosize = 128;
	port->ops      = &mv_uart_ops;
	port->line     = ofdev->id;
	port->dev      = &ofdev->dev;

out:
	return ret;
}
#endif

#ifdef CONFIG_OF
static struct of_device_id of_platform_mv_serial_table[] = {
	{ .compatible = "serial-uart0", .data = (void *)PORT_MVVICTORIA, },
	{ .compatible = "serial-uart1", .data = (void *)PORT_MVVICTORIA, },
	{ .compatible = "serial-uart2", .data = (void *)PORT_MVVICTORIA, },
	{ .compatible = "serial-uart3", .data = (void *)PORT_MVVICTORIA, },
	{ .type = "serial", .data = (void *)PORT_UNKNOWN, },
	{},
};
#endif
static int mv_uart_probe(struct platform_device *pdev)
{
#ifdef CONFIG_OF
	struct device_node *np_uart = NULL;
	struct device_node *np_intc = NULL;
	const struct of_device_id *match = NULL;
	const char *path = NULL;
	char idx;
	// unsigned int virq;
	// unsigned int hwirq;
	int err;
#endif
	struct uart_port port;
	int port_type;
	int id;
	int ret;

#ifdef CONFIG_OF
	id = uart_id;
	pdev->id = id;

	err = of_property_read_string(of_aliases, "mv,uart0", &path);
	if (WARN(err, "Falied to get serial path!"))
		return;

	np_uart = of_find_node_by_path(path);
	if (WARN(!np_uart, "Failed to find uart node"))
		return;

	err = of_property_read_string(of_aliases, "mv,interrupt-controller", &path);
	if (WARN(err, "Failed to read intc path!"))
		return;
	
	np_intc = of_find_node_by_path(path);
	if (WARN(!np_intc, "Failed to find intc node!"))
		return;

	mv_uart_ports[id].irq_dm = irq_find_host(np_intc);
	if (WARN(!mv_uart_ports[id].irq_dm, "Failed to get irq domain!"))
		return;

	// virq = irq_of_parse_and_map(np_uart, IRQ_UART0);
	// virq = irq_find_mapping(irq_dm, IRQ_UART0);
	// if (virq == 0) {
	//	pr_warning("Try to map uart hwirq\n");
	//	err = irq_create_identity_mapping(irq_dm, IRQ_UART0);
	//	err = irq_create__mapping(irq_dm, IRQ_UART0);
	//	if (WARN(err, "Failed to map uart hwirq"))
	//		return;
	// }

	ret = uart_register_driver(&mv_uart_driver);
	if (ret != 0)
		goto out;

	match = of_match_device(&of_platform_mv_serial_table[id], &pdev->dev);
	if (!match)
		return -EINVAL;

	port_type = (unsigned long)match->data;
	mv_uart_ports[id].name = "mvvictoria_fpga-uart";
	sprintf(&idx, "%d", id);
	strcat(mv_uart_ports[id].name, &idx);
	ret = of_platform_mv_serial_setup(pdev, port_type, &mv_uart_ports[id].port);
	if (ret)
		goto out;

	switch (port_type) {
	case PORT_MVVICTORIA:
	{
		dev_info(&pdev->dev, "Port type: PORT_MVVICTORIA\n");

		mv_uart_hw_init(&mv_uart_ports[id]);
	
		writel(0xFFFFFFFF, mv_uart_ports[id].port.membase + UART_INT_MASK);
		writel(0xFFFFFFFF, mv_uart_ports[id].port.membase + UART_INT_CLR);

		ret = uart_add_one_port(&mv_uart_driver, &mv_uart_ports[id].port);
		if (ret)
			goto out;

		break;
	}
	default:
	case PORT_UNKNOWN:
		dev_err(&pdev->dev, "Unknown serial port found. ignored\n");
		ret = -ENODEV;
		break;
	}

	platform_set_drvdata(pdev, &mv_uart_ports[id]);
#else
	ret = mv_uart_init_port(pdev, &mv_uart_ports[pdev->id]);
	if (ret)
		goto out;

	ret = uart_add_one_port(&mv_uart_driver, &mv_uart_ports[pdev->id].port);
	if (ret)
		goto out;
	
	platform_set_drvdata(pdev, &mv_uart_ports[pdev->id]);
#endif

out:
#ifdef CONFIG_OF
	irq_dispose_mapping(port.irq);
#endif
	return ret;
}

static int mv_uart_remove(struct platform_device *dev)
{
	struct mv_uart_port *sport = platform_get_drvdata(dev);

	platform_set_drvdata(dev, NULL);

	mv_uart_hw_uninit(sport);

	if (sport)
		uart_remove_one_port(&mv_uart_driver, &sport->port);

	return 0;
}

static int mv_uart_suspend(struct platform_device *dev, pm_message_t state)
{
	struct mv_uart_port *sport = platform_get_drvdata(dev);

	if (sport)
		uart_suspend_port(&mv_uart_driver, &sport->port);

	return 0;
}

static int mv_uart_resume(struct platform_device *dev)
{
	struct mv_uart_port *sport = platform_get_drvdata(dev);

	if (sport)
		uart_resume_port(&mv_uart_driver, &sport->port);

	return 0;
}

static struct platform_driver mv_uart_platform_driver = {
	.probe  = mv_uart_probe,
	.remove = mv_uart_remove,
	.suspend = mv_uart_suspend,
	.resume	 = mv_uart_resume,
	.driver = {
		.name  = DRV_NAME,
		.owner = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = of_platform_mv_serial_table,
#endif
	},
};

int __init mv_uart_init(void)
{
	int ret;

	ret = uart_register_driver(&mv_uart_driver);
	if (ret != 0)
		goto out;

	ret = platform_driver_register(&mv_uart_platform_driver);
	if (ret != 0)
		goto out_uart_unregister;

	goto out;

out_uart_unregister:
	uart_unregister_driver(&mv_uart_driver);
out:
	return ret;
}

void __exit mv_uart_exit(void)
{
	platform_driver_unregister(&mv_uart_platform_driver);
	uart_unregister_driver(&mv_uart_driver);
}

#ifdef CONFIG_OF
module_platform_driver(mv_uart_platform_driver);
#else
module_init(mv_uart_init);
module_exit(mv_uart_exit);
#endif

MODULE_LICENSE("GPL");
