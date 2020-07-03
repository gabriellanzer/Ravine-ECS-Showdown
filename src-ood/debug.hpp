#pragma once

//Std Include
#include <Windows.h>
#include <stdio.h>
#include <strstream>
#include <typeinfo>
#include <map>

//Defines
#define rvDebug rv::Debug::getDebug()
#define rvDebugLog(X) rv::Debug::getDebug().Log(X)
#define rvDebugLog(Y, X) rv::Debug::getDebug().Log<Y>(X)
#define rvDebugLogError(X) rv::Debug::getDebug().Log<rv::RV_ERROR_MSG>(X)
#define rvDebugLogWarning(X) rv::Debug::getDebug().Log<rv::RV_WARNING_MSG>(X)
#define rvDebugLogVar(X) rv::Debug::getDebug().Log<rv::RV_VARIABLE_MSG>(X)

namespace rv
{
	struct RV_ERROR_MSG {};
	struct RV_WARNING_MSG {};
	struct RV_VARIABLE_MSG {};

	typedef const char* cstr;

	class Debug
	{
	private:
		//Private Constructors for singleton
		inline Debug();
		inline ~Debug();

		//Console Color Codes
		enum ColorCodes
		{
			Black,
			Blue,
			Green,
			Aqua,
			Red,
			Purple,
			Yellow,
			White,
			Gray,
			Light_Blue,
			Light_Green,
			Light_Aqua,
			Light_Red,
			Light_Purple,
			Light_Yellow,
			Bright_White
		};

		//Set console color according to given foreground and background
		inline void SetConsoleColor(ColorCodes foreground = White, ColorCodes background = Black);

		//IO Handlers and Indexes
		HANDLE hstdin;
		HANDLE hstdout;
		WORD index;
		CONSOLE_SCREEN_BUFFER_INFO csbi;

	public:
		//Delete Override operations
		inline Debug(Debug const&) = delete;
		inline void operator=(Debug const&) = delete;

		//Static get for instancing
		inline static Debug& getDebug();

		//Throw message to console of given message type
		template<typename MSG_TYPE>
		inline void Log(const std::string& message);

		//Throw message to console with default type
		inline void Log(const std::string& message);
	};

	inline Debug::Debug()
	{
		//Get STD Handlers and ScreenBuffer Information
		hstdin = GetStdHandle(STD_INPUT_HANDLE);
		hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
		index = 0;
		GetConsoleScreenBufferInfo(hstdout, &csbi);

		SetConsoleColor(Black, Light_Yellow);
		fprintf(stdout, "\n DEBUG INITIALIZED \n\n");
		SetConsoleColor();
	}

	inline Debug::~Debug()
	{

	}

	inline void Debug::SetConsoleColor(ColorCodes foreground, ColorCodes background)
	{
		SetConsoleTextAttribute(hstdout, static_cast<WORD>(((background & 0xff) << 4) + (foreground & 0xff)));
	}

	inline Debug& Debug::getDebug()
	{
		static Debug deb;
		return deb;
	}

	inline void Debug::Log(const std::string& message)
	{
		SetConsoleColor(Bright_White);
		fprintf(stdout, "Log:");
		SetConsoleColor();
		fprintf(stdout, " ");

		//Output message
		fprintf(stdout, " %s\n", message.data());

		//Reset Console Color
		SetConsoleColor();
	}

	template<>
	inline void Debug::Log<RV_VARIABLE_MSG>(const std::string& message)
	{
		SetConsoleColor(Bright_White, Light_Blue);
		fprintf(stdout, "Var:");
		SetConsoleColor();
		fprintf(stdout, " ");

		//Output message
		fprintf(stdout, " %s\n", message.data());

		//Reset Console Color
		SetConsoleColor();
	}

	template<>
	inline void Debug::Log<RV_WARNING_MSG>(const std::string& message)
	{
		SetConsoleColor(Light_Yellow, Yellow);
		fprintf(stdout, "Warning:");
		SetConsoleColor(Black, Yellow);
		fprintf(stdout, " ");

		//Output message
		fprintf(stdout, " %s\n", message.data());

		//Reset Console Color
		SetConsoleColor();
	}

	template<>
	inline void Debug::Log<RV_ERROR_MSG>(const std::string& message)
	{
		SetConsoleColor(Light_Yellow, Red);
		fprintf(stdout, "Error:");
		SetConsoleColor(Bright_White, Red);
		fprintf(stdout, " ");

		//Output message
		fprintf(stdout, " %s\n", message.data());

		//Reset Console Color
		SetConsoleColor();
	}

}