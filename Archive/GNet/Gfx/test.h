#pragma once
#ifdef GFX_API_EXPORT
#define GFX_API __declspec(dllexport)
#else
#define GFX_API __declspec(dllimport)
#endif


namespace Gfx
{
	class test
	{
	public:
		test();
		~test();

		int A = 4;

		GFX_API int getA();

	};

}