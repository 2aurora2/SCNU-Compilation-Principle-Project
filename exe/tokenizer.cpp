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
			case 'R':
				state = 14;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'e':
				state = 8;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'T':
				state = 7;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'E':
				state = 8;
				buffer += ch;
				src_file.get(ch);
				break;
			case 't':
				state = 7;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'i':
				state = 19;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'r':
				state = 14;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'U':
				state = 10;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'W':
				state = 20;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'w':
				state = 20;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'u':
				state = 10;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'I':
				state = 19;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 2:
			switch(ch) {
			case 'T':
				state = 21;
				buffer += ch;
				src_file.get(ch);
				break;
			case 't':
				state = 21;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 3:
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
		case 4:
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
		case 5:
			switch(ch) {
			case 'p':
				state = 16;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'A':
				state = 13;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'a':
				state = 13;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'P':
				state = 16;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 6:
			switch(ch) {
			case 'A':
				state = 4;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'a':
				state = 4;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 7:
			switch(ch) {
			case 'H':
				state = 15;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'h':
				state = 15;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 8:
			switch(ch) {
			case 'N':
				state = 13;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'L':
				state = 18;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'n':
				state = 13;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'l':
				state = 18;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 9:
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
		case 10:
			switch(ch) {
			case 'N':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'n':
				state = 2;
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
		case 12:
			switch(ch) {
			case 'i':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'I':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 13:
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
		case 14:
			switch(ch) {
			case 'e':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'E':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 15:
			switch(ch) {
			case 'e':
				state = 11;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'E':
				state = 11;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 16:
			switch(ch) {
			case 'e':
				state = 6;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'E':
				state = 6;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 17:
			switch(ch) {
			case 'e':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'E':
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
		case 19:
			switch(ch) {
			case 'f':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'F':
				state = 22;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 20:
			switch(ch) {
			case 'R':
				state = 12;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'r':
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
			case 'i':
				state = 9;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'I':
				state = 9;
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
	int state = 2;
	char ch;
	while((ch = src_file.peek()) != EOF) {
		switch(state) {
		case 1:
			switch(ch) {
			case '/':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'h':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'W':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'L':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'd':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case ':':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'X':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'o':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '1':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 's':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'l':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'K':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'R':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'F':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'C':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '}':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'j':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '%':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'E':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'c':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '9':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'g':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '5':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'I':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'S':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '>':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'B':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'Y':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '7':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'a':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'y':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '+':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'T':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'f':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'G':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'Q':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '8':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'x':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'V':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'A':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '4':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'q':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '-':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '6':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'w':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'u':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'N':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case ')':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'P':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '=':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '*':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'b':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'r':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'p':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'z':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'Z':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '0':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'n':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'U':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'O':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '3':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'J':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '^':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 't':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'm':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'i':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'k':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'e':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'v':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '<':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'M':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case ';':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'D':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '2':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'H':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			case '(':
				state = 1;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 2:
			switch(ch) {
			case '{':
				state = 1;
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
			switch(ch) {
			case 'n':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'u':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'h':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 's':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'p':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'D':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'B':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'f':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'A':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'l':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'Y':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'w':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'K':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'c':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 't':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'J':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'T':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'a':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'G':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'F':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'N':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'e':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'I':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'd':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'm':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'Z':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'R':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'E':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'W':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'X':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'i':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'v':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'o':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'k':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'U':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'j':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'x':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'C':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'H':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'S':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'P':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'r':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'V':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'z':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'q':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'O':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'y':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'Q':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'g':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'b':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'L':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'M':
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
			case 'n':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'u':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'h':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 's':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'p':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'D':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'B':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'f':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'A':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '1':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'l':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '0':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '3':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'Y':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'w':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '2':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'K':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'c':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '9':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 't':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'J':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '5':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'T':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'a':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'G':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'F':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'N':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'e':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '7':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'I':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'd':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'm':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'Z':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'R':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'E':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'W':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'X':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'i':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'v':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'o':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'k':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'U':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'j':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'x':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'C':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'H':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'S':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '6':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'P':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'r':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '4':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'V':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'z':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'q':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'O':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'y':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'Q':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '8':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'g':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'b':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'L':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case 'M':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
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
			switch(ch) {
			case '9':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '2':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '4':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '8':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '5':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '0':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case '3':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '1':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '6':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '7':
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
			case '9':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '2':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '4':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '8':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '5':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '0':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '3':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '1':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '6':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case '7':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
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
			case '<':
				state = 3;
				buffer += ch;
				src_file.get(ch);
				break;
			case '=':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case '-':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case '%':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case '+':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case '*':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case '>':
				state = 4;
				buffer += ch;
				src_file.get(ch);
				break;
			case '/':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case '^':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case ':':
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
			case '=':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				return false;
			}
			break;
		case 3:
			switch(ch) {
			case '=':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			case '>':
				state = 5;
				buffer += ch;
				src_file.get(ch);
				break;
			default:
				token = "operator";
				return true;
			}
			break;
		case 4:
			switch(ch) {
			case '=':
				state = 5;
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
			case ';':
				state = 2;
				buffer += ch;
				src_file.get(ch);
				break;
			case ')':
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
		res_file << tok_suc << " " << buf_suc << endl;
		read_pos += buf_suc.size();
		src_file.seekg(read_pos, ios::beg);
		ignoreSpace();
	}
	return 0;
}