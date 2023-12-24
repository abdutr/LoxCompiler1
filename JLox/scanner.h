#pragma once
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>

#include "token.h"
#include "tokenType.h"
#include "error.h"

static inline std::unordered_map<std::string, TokenType> keywords{
	{"and",TokenType::AND},
	{"class",TokenType::CLASS},
	{"else",TokenType::ELSE},
	{"false",TokenType::FALSE},
	{"for", TokenType::FOR},
	{"fun", TokenType::FUN},
	{"if", TokenType::IF},
	{"nil", TokenType::NIL},
	{"or", TokenType::OR},
	{"print", TokenType::PRINT},
	{"return", TokenType::RETURN},
	{"super", TokenType::SUPER},
	{"this", TokenType::THIS},
	{"true", TokenType::TRUE},
	{"var", TokenType::VAR},
	{"while", TokenType::WHILE}
};

class Scanner {
private:
	std::string source;
	std::vector<Token> tokens;

	int start{ 0 };
	int current{ 0 };
	int line{ 1 };
public:
	Scanner(std::string source) : source{ source } {}

	std::vector<Token> scanTokens() {
		while (!isAtEnd()) {
			start = current;
			scanToken();
		}

		tokens.emplace_back(TokenType::EOF_, "", std::monostate{}, line);
		return tokens;
	}
private:
	bool isAtEnd() {
		return current >= source.size();
	}

	void scanToken() {
		char c = advance();

		switch (c) {
		case '(': addToken(TokenType::LEFT_PAREN); break;
		case ')': addToken(TokenType::RIGHT_PAREN); break;
		case '{': addToken(TokenType::LEFT_BRACE); break;
		case '}': addToken(TokenType::RIGHT_BRACE); break;
		case ',': addToken(TokenType::COMMA); break;
		case '.': addToken(TokenType::DOT); break;
		case '-': addToken(TokenType::MINUS); break;
		case '+': addToken(TokenType::PLUS); break;
		case ';': addToken(TokenType::SEMICOLON); break;
		case '*': addToken(TokenType::STAR); break;
		case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
		case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
		case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
		case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
		case '/':
			if (match('/')) {
				while (peek() != '\n' && !isAtEnd()) advance();
			}
			else {
				addToken(TokenType::SLASH);
			}
			break;
		case ' ':
		case '\r':
		case '\t':
			// Ignore whitespace.
			break;
		case '\n':
			line++;
			break;
		case '"': string(); break;
		default:
			if (isDigit(c)) {
				number();
			}
			else if (std::isalpha(c)) {
				identifier();
			}
			else {
				Error::error(line, "Unexpected character.");
				break;
			}
		}
	}

	char advance() {
		current++;
		return source[current - 1];
	}

	void addToken(TokenType type) {
		addToken(type, std::monostate{});
	}

	void addToken(TokenType type, literalType literal) {
		std::string text{ source.substr(start,current - start) };
		tokens.emplace_back(type, text, literal, line);
	}

	bool match(char expected) {
		if (isAtEnd()) return false;
		if (source[current] != expected) return false;

		++current;
		return true;
	}

	char peek() {
		if (isAtEnd()) return '\n';
		return source[current];
	}

	char peekNext() {
		if (current + 1>= source.size()) return '\n';
		return source[current+1];
	}

	void string() {
		while (peek() != '"' && !isAtEnd()) {
			if (peek() == '\n') ++line;
			advance();
		}
		if (isAtEnd()) {
			Error::error(line, "Unterminated string");
			return;
		}

		advance();															//012345
		std::string value = source.substr(start + 1, current - start - 2);  //"abc"
		addToken(TokenType::STRING, value);
	}

	void identifier() {
		while (isAlphaNumeric(peek())) advance();

		std::string text = source.substr(start, current - start);
		TokenType type = [&]() {
			if (keywords.count(text) > 0) return keywords[text];
			return TokenType::IDENTIFIER;
		}();
		addToken(type);
	}

	bool isDigit(char c) {
		return c >= '0' && c <= '9';
	}

	bool isAlpha(char c) {
		return (c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			c == '_';
	}

	bool isAlphaNumeric(char c) {
		return isAlpha(c) || isDigit(c);
	}

	void number() {
		while (isDigit(peek())) advance();

		if (peek() == '.' && isDigit(peekNext())) {
			advance(); // Consume "."

			while (isDigit(peek())) advance();
		}

		addToken(TokenType::NUMBER,std::stod(source.substr(start,current-start)));
	}
};