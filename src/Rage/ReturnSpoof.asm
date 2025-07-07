
.DATA
	; These are global variables and cause problems in multi-threaded situations!
	ReturnAddress	dq	0
	RealRBX			dq	0

.CODE
	CallASM PROC				; void CallASM(rage::scrNativeCallContext* Context, rage::scrNativeHandler Handler, void* Return)
		
		;sub rsp, 32

		mov RealRBX, rbx		; Back up rbx
		mov r9, [rsp]			; Move real return address (NativeContext::EndCall) to r9
		mov ReturnAddress, r9	; Backup real return address (NativeContext::EndCall)
		lea rbx, RetASM			; Copy RetASM function address to rbx
		mov [rsp], r8			; Overwrite real return address with "jmp rbx" address
		jmp rdx					; Jump to native handler (return to "jmp rbx" to RetASM)

	RetASM:
		mov rbx, RealRBX		; Restore rbx
		mov rcx, ReturnAddress	; Move NativeContext::EndCall return address to rcx
		
		;add rsp, 32

		jmp rcx					; Jump back to NativeContext::EndCall
	CallASM ENDP

END
