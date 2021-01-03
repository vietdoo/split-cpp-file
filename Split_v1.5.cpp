#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <Windows.h>
#include <filesystem>

using namespace std;

string a[] = {"string", "int", "double", "float", "bool", "char", "void"}, cf;

vector <string> header, func, maincpp, includecpp, v, f_name, fname, content[100];


void SetWindowSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    SMALL_RECT WindowSize;
    WindowSize.Top = 0;
    WindowSize.Left = 0;
    WindowSize.Right = width;
    WindowSize.Bottom = height;
    SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}


void clear_data() {
	header.clear();
	func.clear();
	maincpp.clear();
	includecpp.clear();
	v.clear();
}

string current_filename () {
	TCHAR filename[MAX_PATH]; 
	GetModuleFileName(NULL, filename, MAX_PATH);
	std::filesystem::path path_ = std::filesystem::path( filename ).filename();
	string path_string{path_.u8string()};
	return path_string;
}

void get_file_name () {
	f_name.push_back("");
	fname.push_back("");
	DIR *dr;
	struct dirent *en;
	dr = opendir("."); 
	string name =  current_filename();
	cf = name;
	name = name.substr(0,name.size()-4) + ".cpp";
	if (dr) {
	while ((en = readdir(dr)) != NULL) {
		string k = en->d_name;

		if (k.find(".cpp") < 10000 && k != name) {
			f_name.push_back(k);
			fname.push_back(k.substr(0,k.size()-4));
		}
	}
	closedir(dr); 
	}
}

string removeChar(string s, char ch)  {
	for (int i = 0; i <= s.size() - 1 ; i++) {
		if (s[i] == ch) {
			s.erase(i, 1);
			return s;
		}
	} 
	return s;
}


void make_header (int number) {

	string name = fname[number] + "_H";
	string file_name = fname[number] + ".h";
	freopen(file_name.c_str(), "w", stdout);
	cout << "#ifndef " + name << '\n';
	cout << "#define " + name << '\n' << '\n';
	
	for (auto i : includecpp) {
		cout << i << '\n';
	}
	cout << "\nusing namespace std;\n\n";
	
	for (auto i : v) {
		cout << i << '\n';
	}
	
	cout << '\n';
	for (auto i : header) {
		cout << i << ';' << '\n';
	}
	cout << "\n#endif";
	fclose (stdout);
}

void make_maincpp (int number) {
	
	string file_name = fname[number] + "_main.cpp";
	ofstream outf;
	outf.open (file_name);
	outf << "#include " << "\"" << fname[number] << ".h\"\n\n";
	for (auto i : maincpp) {
		outf << i << '\n';
	}
	outf.close();
}

void make_functioncpp (int number) {
	string file_name = fname[number] + "_function.cpp";
	freopen(file_name.c_str(), "w", stdout);
	cout << "#include " << "\"" << fname[number] << ".h\"\n\n";
	for (auto i : func) 
		cout << i << '\n';
	fclose (stdout);
}

void split (int k) {	
	int i = -1, m = content[k].size() - 1;
	while (i <= m){
		i++;
		string n = content[k][i];
		if (n.find("main") < 10000) {
			while (i <= m) {
				n = content[k][i++];
				maincpp.push_back(n);
			}
		}
		else if (n.find("#include") < 10000) {
			includecpp.push_back(n);
	
		}
		else {
			int x = n.find(";"), y = -1;
			for (auto j : a) 
				y = max(y, int(n.find(j)));
			if (x < 0 && y > -1) {
				int d = 0;
				if (n.find('{') < 10000) d++;
				header.push_back(removeChar(n, '{'));
				func.push_back(n);
				while (true) {
					n = content[k][++i];
					func.push_back(n);
					if (n.find('{') < 10000) {
						d++;
					}
					if (n.find('}') < 10000) {
						d--;
						if (d == 0)	
							break;
					}
				}
				func.push_back("");
			}
			if (x > -1 && y > -1)
				v.push_back(n);
		}			
	}
}


void create_content() {
	int k = 0;
	for (auto i : f_name) {
		string s;
		ifstream file(i);
		if (file.is_open()) {
			k ++;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << '\n' << i << '\n';
			while (getline(file, s)) {
				content[k].push_back(s);
				for (auto x : s) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rand() % 10 + 1 );
					cout << x;
				}
				cout << '\n';
			}
			file.close();
		}
	}
	if (f_name.size() > 1) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		cout << '\n';
		for (int i = 1 ; i <= 20 ; i++) {
			cout << "*/";
			usleep(800);
		}
		cout << "\n Load 100%\n";
		for (int i = 1 ; i <= 20 ; i++) {
			cout << "*/";
			usleep(800);
		}
	}
}

void run_program() {
	cout << "\n\n**********";
	cout << "\nPhan mem Tach File C++ AUTO.\nMade by Viet Quoc Do 20KDL.\nVersion 1.5 - Date Release 11/12/2020";
	cout << "\n**********\n";
	cout << "\nHuong dan su dung : ";
	cout << "\nDat chung file "+cf+" vao FOLDER chua cac file .CPP can tach va chay file "+cf+".\nChuong trinh se thuc hien tu dong\n\n\n\n";
	
	for (int i = 1; i <= f_name.size() - 1 ; i++) {
		clear_data();
		split(i);
		if (func.size() > 0) {
			remove(f_name[i].c_str());
			make_maincpp(i);
			make_functioncpp(i);
			make_header(i);
		}
	}
	string nofi = "THANH CONG - Xu ly xong " + to_string(f_name.size() - 1) + " file :"; 
	for (auto  i : f_name) {
		nofi += i + "\n";
	}
	if (f_name.size() == 1) {
		MessageBox(NULL, "THAT BAI - Vui long doc HDSD", "Viet Quoc Do 20KDL CopyRight", MB_OK);
	} else MessageBox(NULL, nofi.c_str(), "Viet Quoc Do 20KDL CopyRight", MB_OK);
}

int main () {
//	SetWindowSize(200,100);
	get_file_name();
	create_content();
	run_program();
	
	return 0;
}
