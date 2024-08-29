#pragma once
#include <string>
#include <MinHook.h>

//while it's generally considered bad practice to be using namespace directives in header files, an exception is made as this file is relatively small

class detour
{
public:
	template <class T>
	detour(const std::string_view name, T address, void* detour)
		:
		m_name(name),
		m_address(reinterpret_cast<void**>(&address)),
		m_detour(detour),
		m_original(reinterpret_cast<void*>(*m_address))
	{
		const auto result = MH_CreateHook(*m_address, m_detour, &m_original);
		if (result != MH_OK) {
			printf("hook failed: %p \n", *m_address);
			MessageBoxA(0, "hook failed", "hook error", MB_OK);
		}
	}

	~detour() {
		MH_RemoveHook(this->m_address);
	}

	void enable_hook() {
		MH_EnableHook(this->m_address);
	}

	void disable_hook() {
		MH_DisableHook(this->m_address);
	}

public:
	std::string_view m_name;
	void** m_address;
	void* m_detour;
	void* m_original;
};
