#pragma once
#include <string>
#include <format>
#include <variant>
#include <utility>

#include "tokentype.h"

using literalType = std::variant<std::monostate, double, std::string>;
class Token {
	TokenType type;
	std::string lexeme;
	literalType literal;
	int line;

public:
	template<typename T>
	Token(TokenType type, std::string lexeme, T literal, int line) {
		this->type = type;
		this->lexeme = std::move(lexeme);
		this->literal = literal;
		this->line = line;
	}

	std::string toString() const {
		auto visit_lambda = [&](auto&& args) {
			auto typeStr{ tokenTypetoString(type) };
			if constexpr (std::is_same_v<std::decay_t<decltype(args)>, std::monostate>) {
				return std::format("{} {} empty_literal", typeStr, lexeme);
			}
			else {
				return std::format("{} {} {}", typeStr, lexeme, args);
			}

		};
		return std::visit(visit_lambda,literal);
	}

};