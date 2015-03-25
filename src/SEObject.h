#pragma once

#include "SEAPIIncludes.h"

namespace ServerEye
{
	class SEObject
	{
	public:
		virtual ~SEObject() {}

		virtual wstring getAsJSON() const = 0;
	};
}
