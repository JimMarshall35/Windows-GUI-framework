#include "Command.h"
namespace CWPF
{
	Command::Command(std::function<void(void*)> fn)
		:m_Function(fn)
	{
	}
	void Command::Execute(void* e)
	{
		m_Function(e);
	}
	bool Command::CanExecute()
	{
		return true;
	}
}