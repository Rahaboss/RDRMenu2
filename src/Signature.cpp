#include "pch.h"
#include "Signature.h"

Signature::Signature(std::string_view ida_pattern)
{
	// Helper functions
	auto to_upper = [](const char& c) -> char
	{
		return ((c >= 'a' && c <= 'z') ? c + ('A' - 'a') : c);
	};

	auto to_hex = [&](const char& c) -> uint8_t
	{
		switch (to_upper(c))
		{
		case '0': return static_cast<uint8_t>(0x0);
		case '1': return static_cast<uint8_t>(0x1);
		case '2': return static_cast<uint8_t>(0x2);
		case '3': return static_cast<uint8_t>(0x3);
		case '4': return static_cast<uint8_t>(0x4);
		case '5': return static_cast<uint8_t>(0x5);
		case '6': return static_cast<uint8_t>(0x6);
		case '7': return static_cast<uint8_t>(0x7);
		case '8': return static_cast<uint8_t>(0x8);
		case '9': return static_cast<uint8_t>(0x9);
		case 'A': return static_cast<uint8_t>(0xA);
		case 'B': return static_cast<uint8_t>(0xB);
		case 'C': return static_cast<uint8_t>(0xC);
		case 'D': return static_cast<uint8_t>(0xD);
		case 'E': return static_cast<uint8_t>(0xE);
		case 'F': return static_cast<uint8_t>(0xF);
		}
		return 0;
	};

	// Loop through pattern
	for (size_t i = 0; i < ida_pattern.size(); i++)
	{
		// Skip gaps
		if (ida_pattern[i] == ' ')
			continue;

		// Unknown byte
		if (ida_pattern[i] == '?')
		{
			// Check for double question mark
			if (ida_pattern[i + 1] == '?')
				i++;
			m_bytes.push_back(0);
			continue;
		}

		// Skip last byte if incomplete
		if (i == (ida_pattern.size() - 1))
			break;

		// Construct whole byte from two characters
		uint8_t c1 = to_hex(ida_pattern[i]);
		uint8_t c2 = to_hex(ida_pattern[i + 1]);
		m_bytes.push_back((c1 * 0x10) + c2);
		// Skip over the second part of the byte
		i++;
	}
	Scan();
}

Signature::Signature(void* code_pattern, std::string_view mask)
{
	// Loop through pattern
	for (size_t i = 0; i < mask.size(); i++)
	{
		// Check for unknown byte in mask
		if (mask[i] == '?')
		{
			m_bytes.push_back(0);
			continue;
		}

		m_bytes.push_back(static_cast<uint8_t*>(code_pattern)[i]);
	}
	Scan();
}

Signature::Signature(const uintptr_t address)
{
	m_result = address;
}

Signature& Signature::Scan()
{
	// Helper function to scan for pattern
	auto check_pattern = [this](const uintptr_t& location) -> bool
	{
		for (size_t i = 0; i < m_bytes.size(); i++)
		{
			if (m_bytes[i] && m_bytes[i] != *reinterpret_cast<uint8_t*>(location + i))
				return false;
		}
		return true;
	};

	// Check if already scanned
	if (m_result)
		return *this;

	// Get information about process
	MODULEINFO module_info;
	GetModuleInformation(GetCurrentProcess(), g_game_module, &module_info, sizeof(MODULEINFO));
	MEMORY_BASIC_INFORMATION mbi;

	const auto begin = g_base_address; // Begin location of scan (base address)
	const auto size = module_info.SizeOfImage; // Total size of process (area to be scanned)

	// Loop through memory regions
	for (uintptr_t curr = begin; curr < begin + size; curr += mbi.RegionSize)
	{
		// Check if current region is invalid
		if (!VirtualQuery(reinterpret_cast<LPCVOID>(curr), &mbi, sizeof(mbi))
			|| mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
			continue;

		// Loop through current region
		for (size_t i = 0; i < size; ++i)
		{
			// Check if pattern matches at current location
			if (check_pattern(curr + i))
			{
				m_result = curr + i;
				return *this;
			}
		}
	}

	// At this point the scan has failed so m_result is 0
	assert(m_result);
	return *this;
}

Signature& Signature::Add(const ptrdiff_t& n)
{
	m_result += n;
	return *this;
}

Signature& Signature::Sub(const ptrdiff_t& n)
{
	m_result -= n;
	return *this;
}

Signature& Signature::Rip()
{
	return Add(*reinterpret_cast<uint32_t*>(m_result)).Add(4);
}

uintptr_t Signature::GetRaw()
{
	return m_result;
}
