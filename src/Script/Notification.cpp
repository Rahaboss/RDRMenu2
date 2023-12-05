#include "pch.h"
#include "Notification.h"
#include "Rage/natives.h"
#include "Rage/Guid.h"
#include "Thread/Thread.h"

static constexpr uint32_t MessageDuration = 10000; // Milliseconds

// Wrapper for MISC::VAR_STRING
static const char* CreateVarString(const char* String)
{
	return MISC::VAR_STRING(10, "LITERAL_STRING", String);
}

void Script::NotifyHelp(const char* Message)
{
	TRY
	{
		Guid<4> struct1;
		struct1[0] = MessageDuration;

		Guid<4> struct2;
		struct2.At<const char*>(1) = CreateVarString(Message);

		UIFEED::_UI_FEED_POST_HELP_TEXT(struct1.get(), struct2.get(), true);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::NotifyHelpFormat(_Printf_format_string_ const char* const Text, ...)
{
	TRY
	{
		va_list Args;
		va_start(Args, Text);

		const int Format = _vscprintf(Text, Args);
		assert(Format >= 0);
		size_t Length = static_cast<size_t>(Format) + 1;
		char* Buffer = new char[Length * sizeof(char)];
		assert(Buffer);
		int Format2 = vsprintf_s(Buffer, Length, Text, Args);
		assert(Format2 >= 0);

		NotifyHelp(Buffer);

		delete[] Buffer;
		va_end(Args);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::NotifyFeed(const char* Message)
{
	TRY
	{
		Guid<4> struct1;
		struct1[0] = MessageDuration; // Duration is always 3 seconds

		Guid<4> struct2;
		struct2.At<const char*>(1) = CreateVarString(Message);

		UIFEED::_UI_FEED_POST_FEED_TICKER(struct1.get(), struct2.get(), true);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::NotifyObjective(const char* Message)
{
	TRY
	{
		Guid<4> struct1;
		struct1[0] = MessageDuration;

		Guid<4> struct2;
		struct2.At<const char*>(1) = CreateVarString(Message);

		UIFEED::_UI_FEED_POST_OBJECTIVE(struct1.get(), struct2.get(), true);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::NotifyLocation(const char* Message)
{
	TRY
	{
		const char* Location = "LANDMARK_CINCO_TORRES";
		
		Guid<4> struct1;
		struct1[0] = MessageDuration;

		Guid<4> struct2;
		struct2.At<const char*>(1) = CreateVarString(Location);
		struct2.At<const char*>(2) = CreateVarString(Message);

		UIFEED::_UI_FEED_POST_LOCATION_SHARD(struct1.get(), struct2.get(), true, true);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::NotifyToast(const char* Message, const char* Description)
{
	TRY
	{
		constexpr Hash IconDict = RAGE_JOAAT("HUD_TOASTS");
		constexpr Hash Icon = RAGE_JOAAT("TOAST_PLAYER_DEADEYE");

		Guid<4> struct1;
		struct1[0] = MessageDuration;

		Guid<6> struct2;
		struct2.At<const char*>(1) = CreateVarString(Message);
		struct2.At<const char*>(2) = CreateVarString(Description);
		struct2[4] = IconDict;
		struct2[5] = Icon;

		UIFEED::_UI_FEED_POST_SAMPLE_TOAST(struct1.get(), struct2.get(), true, true);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::NotifyError(const char* Message, const char* Description)
{
	TRY
	{
		Guid<4> struct1;
	
		Guid<8> struct2;
		struct2.At<const char*>(1) = CreateVarString(Message);
		struct2.At<const char*>(2) = CreateVarString(Description);

		const int msgId = UISTICKYFEED::_UI_STICKY_FEED_CREATE_ERROR_MESSAGE(struct1.get(), struct2.get(), true);

		Thread::YieldThread(MessageDuration);

		UISTICKYFEED::_UI_STICKY_FEED_CLEAR_MESSAGE(msgId);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::NotifyDeath(const char* Message)
{
	TRY
	{
		Guid<4> struct1;
		struct1.At<const char*>(0) = CreateVarString("HUD_PENALTY_SOUNDSET");
		struct1.At<const char*>(1) = CreateVarString("HUD_FAIL");
		struct1[2] = 4;

		Guid<8> struct2;
		struct2.At<const char*>(1) = CreateVarString(Message);

		const int msgId = UISTICKYFEED::_UI_STICKY_FEED_CREATE_DEATH_FAIL_MESSAGE(struct1.get(), struct2.get(), true);

		Thread::YieldThread(MessageDuration);

		UISTICKYFEED::_UI_STICKY_FEED_CLEAR_MESSAGE(msgId);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Script::NotifyWarning(const char* Message, const char* Description)
{
	TRY
	{
		Guid<4> struct1;
		struct1.At<const char*>(0) = CreateVarString("HUD_PENALTY_SOUNDSET");
		struct1.At<const char*>(1) = CreateVarString("HUD_FAIL");
		struct1[2] = 4;

		Guid<8> struct2;
		struct2.At<const char*>(2) = CreateVarString(Message);
		struct2.At<const char*>(3) = CreateVarString(Description);

		const int msgId = UISTICKYFEED::_UI_STICKY_FEED_CREATE_WARNING_MESSAGE(struct1.get(), struct2.get(), true);

		Thread::YieldThread(MessageDuration);

		UISTICKYFEED::_UI_STICKY_FEED_CLEAR_MESSAGE(msgId);
	}
	EXCEPT{ LOG_EXCEPTION(); }
}
