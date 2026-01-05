#include "Coral/MethodInfo.hpp"
#include "Coral/Type.hpp"
#include "Coral/Attribute.hpp"
#include "Coral/Assembly.hpp"

#include "CoralManagedFunctions.hpp"

namespace Coral {

	String MethodInfo::GetName() const
	{
		return s_ManagedFunctions.GetMethodInfoNameFptr(m_Handle);
	}

	Type& MethodInfo::GetReturnType()
	{
		if (!m_ReturnType)
		{
			TypeId returnTypeId;
			s_ManagedFunctions.GetMethodInfoReturnTypeFptr(m_Handle, &returnTypeId);
			if (m_Assembly)
			{
				m_ReturnType = &m_Assembly->GetLocalType(returnTypeId);
			}
		}

		return *m_ReturnType;
	}

	const std::vector<Type*>& MethodInfo::GetParameterTypes()
	{
		if (m_ParameterTypes.empty())
		{
			int32_t parameterCount;
			s_ManagedFunctions.GetMethodInfoParameterTypesFptr(m_Handle, nullptr, &parameterCount);

			std::vector<TypeId> parameterTypes(static_cast<size_t>(parameterCount));
			s_ManagedFunctions.GetMethodInfoParameterTypesFptr(m_Handle, parameterTypes.data(), &parameterCount);

			m_ParameterTypes.resize(parameterTypes.size());

			if (m_Assembly)
			{
				for (size_t i = 0; i < parameterTypes.size(); i++)
				{
					m_ParameterTypes[i] = &m_Assembly->GetLocalType(parameterTypes[i]);
				}
			}
		}

		return m_ParameterTypes;
	}

	TypeAccessibility MethodInfo::GetAccessibility() const
	{
		return s_ManagedFunctions.GetMethodInfoAccessibilityFptr(m_Handle);
	}

	std::vector<Attribute> MethodInfo::GetAttributes() const
	{
		int32_t attributeCount;
		s_ManagedFunctions.GetMethodInfoAttributesFptr(m_Handle, nullptr, &attributeCount);

		std::vector<ManagedHandle> attributeHandles(static_cast<size_t>(attributeCount));
		s_ManagedFunctions.GetMethodInfoAttributesFptr(m_Handle, attributeHandles.data(), &attributeCount);

		std::vector<Attribute> result(attributeHandles.size());
		for (size_t i = 0; i < attributeHandles.size(); i++)
		{
			result[i].m_Handle = attributeHandles[i];
			result[i].m_Assembly = m_Assembly;
		}

		return result;
	}

}
