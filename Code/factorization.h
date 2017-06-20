#pragma once
#include "polynomial.h"
#include "monomial.h"
namespace Htto
{
	namespace Count
	{
		class factorization
		{
		public:
			static std::vector<Polynomial> factoring(Polynomial poly);
			static Monomial get_public_factor(const Monomial & m1, const Monomial & m2);
		};
	}
}