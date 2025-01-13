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
	{"keyword", 0},
	{"comment", 1},
	{"identifier", 2},
	{"number", 3},
	{"operator", 4},
	{"separator", 5}
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

bool match_keyword() {
	int state = 21;
	char ch;
	while((ch = src_file.peek()) != EOF) {
		switch(state) {
		case 1:
			switch(ch) {
			case 'I':
				state = 13;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'i':
				state = 13;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 2:
			switch(ch) {
			case 'A':
				state = 16;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'P':
				state = 19;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'a':
				state = 16;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'p':
				state = 19;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 3:
			switch(ch) {
			case 'A':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'a':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 4:
			switch(ch) {
			case 'T':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 't':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 5:
			switch(ch) {
			case 'T':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			case 't':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 6:
			switch(ch) {
			case 'T':
				state = 17;
				buffer += ch;
				src_file.get(ch);
				break;
			case 't':
				state = 17;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 7:
			switch(ch) {
			case 'F':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'f':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 8:
			switch(ch) {
			case 'I':
				state = 6;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'i':
				state = 6;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 9:
			switch(ch) {
			case 'H':
				state = 20;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'h':
				state = 20;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 10:
			switch(ch) {
			case 'R':
				state = 8;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'r':
				state = 8;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 11:
			switch(ch) {
			case 'N':
				state = 4;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'n':
				state = 4;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 12:
			switch(ch) {
			case 'N':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'n':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 13:
			switch(ch) {
			case 'L':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'l':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 14:
			switch(ch) {
			case 'L':
				state = 15;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'N':
				state = 16;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'l':
				state = 15;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'n':
				state = 16;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 15:
			switch(ch) {
			case 'S':
				state = 17;
				buffer += ch;
				src_file.get(ch);
				break;
			case 's':
				state = 17;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 16:
			switch(ch) {
			case 'D':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'd':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 17:
			switch(ch) {
			case 'E':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'e':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 18:
			switch(ch) {
			case 'E':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'e':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 19:
			switch(ch) {
			case 'E':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'e':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 20:
			switch(ch) {
			case 'E':
				state = 12;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'e':
				state = 12;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 21:
			switch(ch) {
			case 'E':
				state = 14;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'I':
				state = 7;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'R':
				state = 18;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'T':
				state = 9;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'U':
				state = 11;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'W':
				state = 10;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'e':
				state = 14;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'i':
				state = 7;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'r':
				state = 18;
				buffer += ch;
				src_file.get(ch);
				break;
			case 't':
				state = 9;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'u':
				state = 11;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'w':
				state = 10;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 22:
			switch(ch) {
			default:
				token = "keyword";
				return true;
			}
			break;
		}
	}
	if(state == 22 ) {
		token = "keyword";
		return true;
	};
	return false;
}

bool match_comment() {
	int state = 1;
	char ch;
	while((ch = src_file.peek()) != EOF) {
		switch(state) {
		case 1:
			switch(ch) {
			case '{':
				state = 2;
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
			case '%':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '(':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case ')':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
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
			case '-':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '/':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case ':':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case ';':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '<':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '=':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '>':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '^':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '}':
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
				token = "comment";
				return true;
			}
			break;
		}
	}
	if(state == 3 ) {
		token = "comment";
		return true;
	};
	return false;
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
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			}
			switch(ch) {
			case '0':
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
				token = "number";
				return true;
			}
			break;
		case 3:
			if(ch >= '0' && ch <= '9') {
				state = 3;
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
		}
	}
	if(state == 2 ) {
		token = "number";
		return true;
	};
	return false;
}

bool match_operator() {
	int state = 2;
	char ch;
	while((ch = src_file.peek()) != EOF) {
		switch(state) {
		case 1:
			switch(ch) {
			case '=':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 2:
			switch(ch) {
			case '%':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case '*':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case '+':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case '-':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case '/':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case ':':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '<':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case '=':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case '>':
				state = 4;
				buffer += ch;
				src_file.get(ch);
				break;
			case '^':
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
				token = "operator";
				return true;
			}
			break;
		case 4:
			switch(ch) {
			case '=':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				token = "operator";
				return true;
			}
			break;
		case 5:
			switch(ch) {
			case '=':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case '>':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				token = "operator";
				return true;
			}
			break;
		}
	}
	if(state == 5 ) {
		token = "operator";
		return true;
	};
	return false;
}

bool match_separator() {
	int state = 1;
	char ch;
	while((ch = src_file.peek()) != EOF) {
		switch(state) {
		case 1:
			switch(ch) {
			case '(':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case ')':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case ';':
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
				token = "separator";
				return true;
			}
			break;
		}
	}
	if(state == 2 ) {
		token = "separator";
		return true;
	};
	return false;
}

int main(void) {
	res_file << "0" << " " << "keyword"<< " " << "1" << " " << "comment"<< " " << "2" << " " << "identifier"<< " " << "3" << " " << "number"<< " " << "4" << " " << "operator"<< " " << "5" << " " << "separator"<< endl;
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
		if(!match_comment()) buf_err = buffer;
		else if(buffer.size() > buf_suc.size()) {
			buf_suc = buffer;
			tok_suc = token;
		}
		buffer.clear();
		src_file.seekg(read_pos, ios::beg);
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
		if(!match_separator()) buf_err = buffer;
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