#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>
using namespace std;

ifstream src_file("sample.txt", ios::in);
ofstream res_file("sample.lex", ios::out | ios::trunc);
int read_pos;
string token;
string buffer;
string buf_suc;
string tok_suc;
string buf_err;

std::unordered_map<std::string, int> types_map = {
	{"identifier", 0},
	{"number", 1},
	{"operator", 2}
};

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

bool match_identifier() {
	int state = 1;
	char ch;
	while((ch = src_file.peek()) != EOF) {
		switch(state) {
		case 1:
			if(ch >= 'A' && ch <= 'Z') {
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			}
			if(ch >= 'a' && ch <= 'z') {
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			}
			switch(ch) {
			default:
				return false;
			}
			break;
		case 2:
			if(ch >= '0' && ch <= '9') {
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			}
			if(ch >= 'A' && ch <= 'Z') {
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			}
			if(ch >= 'a' && ch <= 'z') {
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			}
			switch(ch) {
			default:
				token = "identifier";
				return true;
			}
			break;
		}
	}
	if(state == 2 ) {
		token = "identifier";
		return true;
	};
	return false;
}

bool match_number() {
	int state = 1;
	char ch;
	while((ch = src_file.peek()) != EOF) {
		switch(state) {
		case 1:
			if(ch >= '1' && ch <= '9') {
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			}
			switch(ch) {
			case '0':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 2:
			if(ch >= '0' && ch <= '9') {
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			}
			switch(ch) {
			default:
				token = "number";
				return true;
			}
			break;
		case 3:
			switch(ch) {
			default:
				token = "number";
				return true;
			}
			break;
		}
	}
	if(state == 3 ) {
		token = "number";
		return true;
	};
	return false;
}

bool match_operator() {
	int state = 1;
	char ch;
	while((ch = src_file.peek()) != EOF) {
		switch(state) {
		case 1:
			switch(ch) {
			case '*':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '+':
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
			default:
				token = "operator";
				return true;
			}
			break;
		}
	}
	if(state == 2 ) {
		token = "operator";
		return true;
	};
	return false;
}

int main(void) {
	res_file << "0" << " " << "identifier"<< " " << "1" << " " << "number"<< " " << "2" << " " << "operator"<< endl;
	char ch;
	ignoreSpace();
	while((ch = src_file.peek()) != EOF) {
		tok_suc.clear();
		buf_suc.clear();
		if(!match_identifier()) buf_err = buffer;
		else if(buffer.size() > buf_suc.size()) {
			buf_suc = buffer;
			tok_suc = token;
		}
		buffer.clear();
		src_file.seekg(read_pos, ios::beg);
		if(!match_number()) buf_err = buffer;
		else if(buffer.size() > buf_suc.size()) {
			buf_suc = buffer;
			tok_suc = token;
		}
		buffer.clear();
		src_file.seekg(read_pos, ios::beg);
		if(!match_operator()) buf_err = buffer;
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
		res_file << types_map[tok_suc] << " " << buf_suc << endl;
		read_pos += buf_suc.size();
		src_file.seekg(read_pos, ios::beg);
		ignoreSpace();
	}
	return 0;
}