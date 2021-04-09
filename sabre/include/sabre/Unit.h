#pragma once

#include "sabre/Exports.h"
#include "sabre/Tkn.h"
#include "sabre/Err.h"

#include <mn/Str.h>
#include <mn/Buf.h>
#include <mn/Str_Intern.h>

namespace sabre
{
	struct Unit
	{
		// path of the given unit
		mn::Str filepath;
		mn::Str content;
		// line ranges
		mn::Buf<Rng> lines;
		mn::Str_Intern str_interner;
		mn::Buf<Err> errs;
	};

	SABRE_EXPORT Unit*
	unit_from_file(const mn::Str& filepath);

	inline static Unit*
	unit_from_file(const char* filepath)
	{
		return unit_from_file(mn::str_lit(filepath));
	}

	SABRE_EXPORT void
	unit_free(Unit* self);

	inline static void
	destruct(Unit* self)
	{
		unit_free(self);
	}

	inline static const char*
	unit_intern(Unit* self, const char* begin_it, const char* end_it)
	{
		return mn::str_intern(self->str_interner, begin_it, end_it);
	}

	inline static const char*
	unit_intern(Unit* self, const char* str)
	{
		return mn::str_intern(self->str_interner, str);
	}

	inline static void
	unit_err(Unit* self, Err err)
	{
		mn::buf_push(self->errs, err);
	}
}