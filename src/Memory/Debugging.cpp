#include "pch.h"
#include "Debugging.h"
#include "Pointers.h"
#include "Signature.h"

static uint8_t s_TerminateProcessBytes[6]{};
static constexpr size_t s_TerminateProcessSize = sizeof(s_TerminateProcessBytes);

static uint8_t s_CheckForDebuggerBytesOriginal[2]{};
static uint8_t s_CheckForDebuggerBytesPatched[2]{ 0x38, 0xC0 }; // cmp al, al
static constexpr size_t s_CheckForDebuggerSize = sizeof(s_CheckForDebuggerBytesOriginal);

void Debugging::Create()
{
#if ENABLE_DEBUGGING && !defined(_DIST)
	LOG_TEXT("Creating debugging support.");

	Pointers::CheckForDebugger = Signature("E9 ? ? ? ? 48 8D 15 ? ? ? ? E9 ? ? ? ? 48 89 45 38").Add(1).Rip().Get<decltype(Pointers::CheckForDebugger)>();
	Pointers::TerminateProcess = Signature("48 83 C9 FF FF 15").Add(4).Get<decltype(Pointers::TerminateProcess)>();

	// Patch call to TerminateProcess
	memcpy_s(&s_TerminateProcessBytes, s_TerminateProcessSize, Pointers::TerminateProcess, s_TerminateProcessSize);
	memset(Pointers::TerminateProcess, 0x90, s_TerminateProcessSize);

	// Patch check for debugger
	memcpy_s(&s_CheckForDebuggerBytesOriginal, s_CheckForDebuggerSize, Pointers::CheckForDebugger, s_CheckForDebuggerSize);
	memcpy_s(Pointers::CheckForDebugger, s_CheckForDebuggerSize, &s_CheckForDebuggerBytesPatched, s_CheckForDebuggerSize);
#endif // ENABLE_DEBUGGING && !defined(_DIST)
}

void Debugging::Destroy()
{
#if ENABLE_DEBUGGING && !defined(_DIST)
	LOG_TEXT("Destroying debugging support.");

	// Restore call to TerminateProcess
	memcpy_s(Pointers::TerminateProcess, s_TerminateProcessSize, &s_TerminateProcessBytes, s_TerminateProcessSize);

	// Restore check for debugger
	memcpy_s(Pointers::CheckForDebugger, s_CheckForDebuggerSize, &s_CheckForDebuggerBytesOriginal, s_CheckForDebuggerSize);
#endif // ENABLE_DEBUGGING && !defined(_DIST)
}
