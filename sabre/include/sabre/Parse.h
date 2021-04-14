#pragma once

#include "sabre/Exports.h"
#include "sabre/Tkn.h"

#include <mn/Buf.h>

namespace sabre
{
	struct Unit;
	struct Expr;
	struct Stmt;

	// contains the parser state
	struct Parser
	{
		Unit* unit;
		mn::Buf<Tkn> tokens;
		size_t it;
		size_t prev_it;
	};

	// creates a new parser instance
	SABRE_EXPORT Parser
	parser_new(Unit* unit);

	// frees the given parser instance
	SABRE_EXPORT void
	parser_free(Parser& self);

	inline static void
	destruct(Parser& self)
	{
		parser_free(self);
	}

	// returns whether the parser reached the end of file or not
	inline static bool
	parser_eof(const Parser& self)
	{
		return self.it >= self.tokens.count;
	}

	// parses an expression
	SABRE_EXPORT Expr*
	parser_parse_expr(Parser& self);

	// parses a statment
	SABRE_EXPORT Stmt*
	parser_parse_stmt(Parser& self);
}