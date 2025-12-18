#include <iostream> // std::cout
#include <fstream> // std:ofstream
#include <chrono> // std::chrono::milliseconds
#include <filesystem> // std::filesystem

#include "dirmonitor.hpp"
#include "logger.hpp"

/*
 g++ -std=c++17 -pthread -Iinclude src/dirmonitor.cpp src/dllloader.cpp src/logger.cpp
  src/handleton.cpp test/dirmonitor_test.cpp -ldl
*/
#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RED "\033[1;31m"
#define BOLD_BLUE "\033[1;34m"


using namespace ilrd;

template <typename T>
void PrintTest(const std::string& test_name, T result, T expected, int line);
static size_t g_test_num = 0;

class FileCreatedCallback: public BaseCallback<const std::string&>
{
	public:
	explicit FileCreatedCallback(std::string msg): m_is_file_created(false), m_msg(std::move(msg)) 
    {
        LOG_DEBUG("FileCreatedCallback ctor");
    }

	void Notify(const std::string& created_file)
	{
		if (m_is_file_created)
		{
			return; // ignore repeated inotify events triggered by our own write
		}
		LOG_DEBUG("FileCreatedCallback Notify " + m_msg);
		m_is_file_created = 1;
		std::ofstream file;
		file.open(created_file, std::ios::app);
		file << m_msg << std::endl;
		file.close();
	}

	bool GetFileCreationStatus() const {return m_is_file_created;}

	private:
	bool m_is_file_created;
	std::string m_msg;
}; // class FileCreatedCallback

int main()
{
	std::filesystem::create_directories("./plugin");
	std::filesystem::remove("./plugin/test1.txt");

	DirMonitor dir_monitor("./plugin");

	FileCreatedCallback hello("hello");
	FileCreatedCallback world("world");

	dir_monitor.Register(&hello);
	dir_monitor.Register(&world);

	dir_monitor.Run();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	PrintTest("Before file creation #1", hello.GetFileCreationStatus(), false, __LINE__);
	PrintTest("Before file creation #2", world.GetFileCreationStatus(), false, __LINE__);

	std::ofstream file("./plugin/test1.txt");
	

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	PrintTest("After file creation #1", hello.GetFileCreationStatus(), true, __LINE__);
	PrintTest("After file creation #2", world.GetFileCreationStatus(), true, __LINE__);

	//std::remove("./plugin/test1.txt");

	return 0;

}

template <typename T>
void PrintTest(const std::string& test_name, T result, T expected, int line)
{
	std::cout << "Test No" << ++g_test_num << ": " << BOLD_BLUE << test_name << COLOR_RESET << ": ";

	if (result == expected)
	{
		std::cout << COLOR_GREEN "PASSED!" COLOR_RESET << std::endl;
	}
	else
	{
		std::cout << COLOR_RED "FAILED!" COLOR_RESET << " at line " << line << std::endl;
		std::cout << "Expected: " << expected << ", Got: " << result << std::endl;
	}
}
