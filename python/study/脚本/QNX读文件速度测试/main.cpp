#include <iostream>  
#include <fstream>   
#include<time.h>
#include <string>

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

using namespace std;

int read_file(string file_path);
string& trim(string& s);
bool is_blank(char c);
void print_now_date(char* out_str);

void print_now_date(char* str_buf)
{
	time_t nowtime;
	 struct tm *timeinfo;
	 time( &nowtime );
	 timeinfo = localtime( &nowtime );
	 int year, month, day, h, m, s;
	 year = timeinfo->tm_year + 1900;
	 month = timeinfo->tm_mon + 1;
	 day = timeinfo->tm_mday;
	 h = timeinfo->tm_hour;
	 m = timeinfo->tm_min;
	 s = timeinfo->tm_sec;
	 sprintf(str_buf,"\n%d-%d-%d %d:%d:%d\n", year, month, day, h, m, s);
	 printf("\n%d-%d-%d %d:%d:%d\n", year, month, day, h, m, s);
}

int main (int argc, char *argv[])
{
	char start_date_str[256] = {0};
	char end_date_str[256] = {0};
	double totaltime;
	clock_t start,finish;
	cout << argv[1] << endl;
	ifstream in(argv[1]);
	string s;
	cout << "\nStart Read All Files." << endl;
	print_now_date(start_date_str);
	start=clock();
	int count = 0;
	while(getline(in,s))
	{
		s = trim(s);
		int read_ret = read_file(s);
		if(1==read_ret)
		{
			cout << "\nFile [" << s << "]" << "Read Faile" << endl;
		}
		if(0==read_ret)
		{
			cout << "\nFile [" << s << "]" << "Read Finish" << endl;
			count++;
		}

	}
	finish=clock();
   totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
   cout<<"\nTotal run time:"<<totaltime<<"sec£¡"<<endl;
	print_now_date(end_date_str);

	cout << "\nAll Files Read Finished" << endl;
	cout << "\nRead [" << count << "] Files" << endl;
	cout << "\nStart Time:" <<  start_date_str << endl;
	cout << "\nEnd Time:" <<  end_date_str << endl;
	return 0;
}

bool is_blank(char c)
{
	if (c==' ' || c=='\t' || c=='\n' || c=='\r')
	{
		return true;
	}else{
		return false;
	}
}

string& trim(string& s) {
	if (s.empty()) {
		return s;
	}

	string::iterator c;
	for (c = s.begin(); c != s.end() && is_blank(*c++);)
		s.erase(s.begin(), --c);

	for (c = s.end(); c != s.begin() && is_blank(*--c););
	s.erase(++c, s.end());

	return s;
}

int read_file(string file_path)
{
	char buffer[32768];
	int fd,size;
	fd=open(file_path.c_str(),O_RDONLY);
	if(fd<0)
	{
		cout << "Error opening file["  << file_path <<  "]" << endl;
		return 1;
	}
	size=read(fd,buffer,sizeof(buffer));
	while(size!=0&&size!=-1)
	{
		size=read(fd,buffer,sizeof(buffer));
	}
	close(fd);
	/*
	ifstream in(file_path.c_str(),std::ios::binary);
	if (! in.is_open())  
	{ 
		cout << "Error opening file"; 
		exit (1); 
		return 1;
	}

	while (!in.eof() )  
   {  
	   in.read(buffer,sizeof(char)*256);
   }  
   in.close();
   */
   return 0;
}

