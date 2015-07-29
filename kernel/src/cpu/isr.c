#include <kernel/isr.h>
#include <kernel/irq.h> // handler_t
#include <kernel/tty.h>
#include <stdio.h>
#include <stdlib.h>

static char* exception_msgs[] = {
	"Division By Zero",
	"Debugger",
	"Non-Maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bounds",
	"Invalid Opcode",
	"Coprocessor Not Available",
	"Double fault",
	"Coprocessor Segment Overrun",
	"Invalid Task State Segment",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Reserved",
	"Math Fault",
	"Alignement Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

static handler_t isr_handlers[32];

void isr_handler(registers_t* regs) {
	if (isr_handlers[regs->int_no]) {
		handler_t handler = isr_handlers[regs->int_no];
		handler(regs);
	}
	else {
		printf("Unhandled hardware exception %d: %s\n", regs->int_no,
			exception_msgs[regs->int_no]);
		abort();
	}
}

void isr_register_handler(uint32_t num, handler_t handler) {
	if (isr_handlers[num]) {
		printf("Exception handler %d (%s) already registered\n", num,
			exception_msgs[num]);
	}
	else {
		isr_handlers[num] = handler;
	}
}