#pragma once

#define sLog Log::Instance()

namespace D3D11_Framework
{
	class Log
	{
	public:
		Log();
		~Log();

		static Log* Instance() { return _instance; }

		void Print(const char* message, ...);
		void Debug(const char* message, ...);
		void Err(const char* message, ...);
	
	private:
		static Log* _instance;
		void _Init();
		void _Close();
		void _Print(const char* levtext, const char* text);

		FILE* _file;
	};
}


