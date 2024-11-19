#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

ifstream src_file("sample.txt", ios::in);
ofstream res_file("sample.lex", ios::out | ios::trunc);
int read_pos;
string token;
string buffer;
string buf_suc;
string tok_suc;
string buf_err;

void ignoreSpace() {
	char ch;
	while(src_file.get(ch)) {
		read_pos++;
		if(ch == '\n') read_pos++;
		if(!isspace(ch)) {
			read_pos--;
			src_file.unget();
			break;
		}
	}
}

int main(void) {
	char ch;
	ignoreSpace();
	while((ch = src_file.peek()) != EOF) {
		tok_suc.clear();
		buf_suc.clear();
		if(buf_suc.size() == 0){ 
			res_file << "UNKNOWN: " << buf_err << endl;
			exit(1);
		}
		res_file << tok_suc << " " << buf_suc << endl;
		read_pos += buf_suc.size();
		src_file.seekg(read_pos, ios::beg);
		ignoreSpace();
	}
	return 0;
}