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

bool match_keyword() {
	int state = 1;
	char ch;
	while((ch = src_file.peek()) != EOF) {
		switch(state) {
		case 1:
			switch(ch) {
			case 'i':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 2:
			switch(ch) {
			case 'f':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 3:
			switch(ch) {
			default:
				token = "keyword";
				return true;
			}
			break;
		}
	}
	if(state == 3 ) {
		token = "keyword";
		return true;
	};
	return false;
}

bool match_test() {
	int state = 1;
	char ch;
	while((ch = src_file.peek()) != EOF) {
		switch(state) {
		case 1:
			switch(ch) {
			case 'b':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'a':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				token = "test";
				return true;
			}
			break;
		}
	}
	if(state == 1 ) {
		token = "test";
		return true;
	};
	return false;
}

int main(void) {
	char ch;
	ignoreSpace();
	while((ch = src_file.peek()) != EOF) {
		tok_suc.clear();
		buf_suc.clear();
		if(!match_keyword()) buf_err = buffer;
		else if(buffer.size() > buf_suc.size()) {
			buf_suc = buffer;
			tok_suc = token;
		}
		buffer.clear();
		src_file.seekg(read_pos, ios::beg);
		if(!match_test()) buf_err = buffer;
		else if(buffer.size() > buf_suc.size()) {
			buf_suc = buffer;
			tok_suc = token;
		}
		buffer.clear();
		src_file.seekg(read_pos, ios::beg);
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