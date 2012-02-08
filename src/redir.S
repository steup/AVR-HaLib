; This file is part of avr-halib. See COPYING for copyright details.

#include <avr/io.h>

; global redirection routine
; saves registers as needed and redirect to the
; address specified in redirection memory variable
.global redir_func
.func redir_func
redir_func:
	push r0				; save temporary register
					; long names (__zero_reg__) 
					; first avaiable in macros.inc
	in r0, _SFR_IO_ADDR(SREG)	; load status. SREG is a SFR macro

	sbiw ZL, 0			; test for zero
	breq redir_func_restore		; skip, if zero				

					; save used registers. Saves all 
					; call-used registers. Called routines
					; SHOULD do this by default. But 
					; ignored for the sake of clarity and 
					; in case of naked routines 
	push r0				; save loaded status. Assumes irqs are 
					; disabled
	push r1				; save zero register
	eor r1, r1			; clear zero register, as assumed to
					; always be zero
	push r18			; save other call-used registers
	push r19			;
	push r20			;
	push r21			;
	push r22			;
	push r23			;
	push r24			;
	push r25			;
	push XL				;
	push XH				;
				
	
	icall				; jump to address pointed by Z pointer

					; back from routine.
					; restore used registers
	pop XH				; restore call-used registers
	pop XL				;
	pop r25				;
	pop r24				;
	pop r23				;
	pop r22				;
	pop r21				;
	pop r20				;
	pop r19				;
	pop r18				;
	pop r1				; restore zero register	 
	pop r0				; restore status register into temporary
redir_func_restore:
	out _SFR_IO_ADDR(SREG), r0	; load status register
	pop r0				; restore temporary register	 
	pop ZL				; restore lower Z pointer	 
	pop ZH				; restore higher Z pointer	 
	reti				; return from redirection and so return
					; from interrupt
.endfunc				; end of redirection routine	

.end		
