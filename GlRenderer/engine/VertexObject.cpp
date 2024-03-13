#include "VertexObject.h"

VertexObject::VertexObject(WithResultOf&& res) noexcept
{
	VertexObject other = res.func();

	m = std::move(other.m);
}
