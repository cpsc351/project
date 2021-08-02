#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <fstream>
#include <iostream>

/* data shared by thread(s) */
/* email, date, total number of minutes. */
const std::string studentEmail;
int totalMinutes, date;

/* producer reads file through in this function */
void *readFile (const char *param)
{
  std::string word; // holds one word string read by ifstream
  std::vector<char*> fileData; // holds each char address read by ifstream
	std::ifstream readFile; // file reader
  unsigned last_pos = 0;

	/* reads text file into fileData */
	readFile.open("Zoomreport.txt");
	if (readFile.is_open())
	{
    std::string temp;
		while (readFile >> temp)
		{
      word.append(temp);
      // word.push_back(" ");
			for (unsigned i = last_pos; i < word.length(); i++)
			{
				fileData.push_back(&word[i]);
			}
      last_pos = word.length();
		}
		readFile.close();
	}
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  readFile("Zoomreport.txt");
  std::cout << "The student with " << studentEmail << " attended a total of "
            << totalMinutes << " of minutes during the entire semester, and the following dates: "
            << date << "\n";
}
