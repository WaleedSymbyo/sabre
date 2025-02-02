#include "sabre/IR.h"

#include <mn/Assert.h>

namespace sabre::spirv
{
	inline static Value*
	_module_value_new(Module* self, Type* type)
	{
		auto value = mn::alloc_zerod_from<Value>(self->arena);
		value->id = ++self->id_generator;
		value->type = type;

		mn::map_insert(self->entities, value->id, entity_from_value(value));
		return value;
	}

	// API
	Value*
	basic_block_add(Basic_Block* self, Value* op1, Value* op2)
	{
		if (type_is_int(op1->type) && type_is_int(op2->type))
		{
			Instruction ins{};
			ins.kind = Instruction::Op_IAdd;
			ins.as_iadd.op1 = op1;
			ins.as_iadd.op2 = op2;
			ins.as_iadd.res = _module_value_new(self->func->module, op1->type);
			mn::buf_push(self->instructions, ins);

			return ins.as_iadd.res;
		}
		return nullptr;
	}

	Value*
	basic_block_ret(Basic_Block* self, Value* res)
	{
		Instruction ins{};
		ins.kind = Instruction::Op_ReturnValue;
		ins.as_return.value = res;
		mn::buf_push(self->instructions, ins);

		return ins.as_return.value;
	}

	Module*
	module_new()
	{
		auto self = mn::alloc_zerod<Module>();
		self->arena = mn::allocator_arena_new();
		return self;
	}

	void
	module_free(Module* self)
	{
		if (self)
		{
			mn::allocator_free(self->arena);
			mn::map_free(self->entities);
			mn::free(self);
		}
	}

	Type*
	module_type_void_new(Module* self)
	{
		auto type = mn::alloc_zerod_from<Type>(self->arena);
		type->kind = Type::KIND_VOID;
		type->id = ++self->id_generator;

		mn::map_insert(self->entities, type->id, entity_from_type(type));
		return type;
	}

	Type*
	module_type_int_new(Module* self, int bit_width, bool is_signed)
	{
		auto type = mn::alloc_zerod_from<Type>(self->arena);
		type->kind = Type::KIND_INT;
		type->id = ++self->id_generator;
		type->as_int.bit_width = bit_width;
		type->as_int.is_signed = is_signed ? 1 : 0;

		mn::map_insert(self->entities, type->id, entity_from_type(type));
		return type;
	}

	Type*
	module_type_func_new(Module* self, Type* return_type)
	{
		auto type = mn::alloc_zerod_from<Type>(self->arena);
		type->kind = Type::KIND_FUNC;
		type->id = ++self->id_generator;
		type->as_func.return_type = return_type;
		type->as_func.args = mn::buf_with_allocator<Type*>(self->arena);

		mn::map_insert(self->entities, type->id, entity_from_type(type));
		return type;
	}

	void
	module_type_func_arg(Type* func, Type* arg)
	{
		mn_assert(func->kind == Type::KIND_FUNC);

		mn::buf_push(func->as_func.args, arg);
	}

	Func*
	module_func_new(Module* self, Type* func_type)
	{
		mn_assert(func_type->kind == Type::KIND_FUNC);

		auto func = mn::alloc_zerod_from<Func>(self->arena);
		func->module = self;
		func->id = ++self->id_generator;
		func->type = func_type;
		func->args = mn::buf_with_allocator<Value*>(self->arena);
		func->blocks = mn::buf_with_allocator<Basic_Block*>(self->arena);

		mn::buf_reserve(func->args, func_type->as_func.args.count);
		for (auto arg_type: func_type->as_func.args)
			mn::buf_push(func->args, _module_value_new(self, arg_type));

		mn::map_insert(self->entities, func->id, entity_from_func(func));
		return func;
	}

	Basic_Block*
	func_basic_block_new(Func* self)
	{
		auto module = self->module;

		auto basic_block = mn::alloc_zerod_from<Basic_Block>(module->arena);
		basic_block->func = self;
		basic_block->id = ++module->id_generator;
		basic_block->instructions = mn::buf_with_allocator<Instruction>(self->module->arena);

		mn::map_insert(module->entities, basic_block->id, entity_from_basic_block(basic_block));
		mn::buf_push(self->blocks, basic_block);

		return basic_block;
	}

	Value*
	func_arg(Func* self, size_t i)
	{
		return self->args[i];
	}
}