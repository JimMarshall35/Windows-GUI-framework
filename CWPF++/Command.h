#pragma once
#include <functional>

namespace CWPF
{
	class ICommand
	{
	public:
		virtual void Execute(void* e) = 0;
		virtual bool CanExecute() = 0;
	};

	class Command : public ICommand
	{
	public:
		Command(std::function<void(void*)> fn);
		virtual void Execute(void* e) override;
		virtual bool CanExecute() override;
	private:
		std::function<void(void*)> m_Function;
	};
}

