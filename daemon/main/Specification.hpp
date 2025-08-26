/*
 *  This file is part of nzbget. See <https://nzbget.com>.
 *
 *  Copyright (C) 2025 Denis <denis@nzbget.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SPECIFICATION_H
#define SPECIFICATION_H

#include <memory>
#include <type_traits>

namespace Specification
{
	/**
	 * @brief A generic specification interface.
	 * @tparam T The type of the object to be checked.
	 */
	template <typename T, typename = void>
	class ISpecification
	{
	public:
		virtual ~ISpecification() = default;
		virtual bool IsSatisfiedBy(const T& candidate) const = 0;
	};

	template <typename T>
	class ISpecification<T, std::enable_if_t<std::is_trivially_copyable_v<T>>>
	{
	public:
		virtual ~ISpecification() = default;
		virtual bool IsSatisfiedBy(T candidate) const = 0;
	};

	template <typename T>
	using SpecificationPtr = std::shared_ptr<ISpecification<T>>;

	template <typename T>
	class AndSpecification : public ISpecification<T>
	{
	public:
		AndSpecification(SpecificationPtr<T> left, SpecificationPtr<T> right)
			: m_left(left), m_right(right)
		{
		}

		bool IsSatisfiedBy(const T& candidate) const override
		{
			return m_left->IsSatisfiedBy(candidate) && m_right->IsSatisfiedBy(candidate);
		}
	private:
		SpecificationPtr<T> m_left;
		SpecificationPtr<T> m_right;
	};

	/**
	 * @brief A combinator that performs a logical OR on two specifications.
	 */
	template <typename T>
	class OrSpecification : public ISpecification<T> {
	public:
		OrSpecification(SpecificationPtr<T> left, SpecificationPtr<T> right)
			: m_left(left), m_right(right)
		{
		}

		bool IsSatisfiedBy(const T& candidate) const override
		{
			return m_left->IsSatisfiedBy(candidate) || m_right->IsSatisfiedBy(candidate);
		}
	private:
		SpecificationPtr<T> m_left;
		SpecificationPtr<T> m_right;
	};

	/**
	 * @brief A combinator that performs a logical NOT on a specification.
	 */
	template <typename T>
	class NotSpecification : public ISpecification<T> {
	public:
		explicit NotSpecification(SpecificationPtr<T> operand)
			: m_operand(operand)
		{
		}

		bool IsSatisfiedBy(const T& candidate) const override
		{
			return !m_operand->IsSatisfiedBy(candidate);
		}
	private:
		SpecificationPtr<T> m_operand;
	};
}

#endif
