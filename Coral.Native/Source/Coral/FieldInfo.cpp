#include "Coral/FieldInfo.hpp"
#include "Coral/Type.hpp"
#include "Coral/Attribute.hpp"
#include "Coral/Assembly.hpp"

#include "CoralManagedFunctions.hpp"

namespace Coral {

	String FieldInfo::GetName() const
	{
		return s_ManagedFunctions.GetFieldInfoNameFptr(m_Handle);
	}

	Type& FieldInfo::GetType()
	{
		if (!m_Type)
		{
			TypeId fieldTypeId;
			s_ManagedFunctions.GetFieldInfoTypeFptr(m_Handle, &fieldTypeId);
			if (m_Assembly)
			{
				m_Type = &m_Assembly->GetLocalType(fieldTypeId);
			}
		}

		return *m_Type;
	}

	TypeAccessibility FieldInfo::GetAccessibility() const
	{
		return s_ManagedFunctions.GetFieldInfoAccessibilityFptr(m_Handle);
	}

	std::vector<Attribute> FieldInfo::GetAttributes() const
	{
		int32_t attributeCount;
		s_ManagedFunctions.GetFieldInfoAttributesFptr(m_Handle, nullptr, &attributeCount);
		std::vector<ManagedHandle> attributeHandles(static_cast<size_t>(attributeCount));
		s_ManagedFunctions.GetFieldInfoAttributesFptr(m_Handle, attributeHandles.data(), &attributeCount);

		std::vector<Attribute> result(attributeHandles.size());
		for (size_t i = 0; i < attributeHandles.size(); i++)
		{
			result[i].m_Handle = attributeHandles[i];
			result[i].m_Assembly = m_Assembly;
		}

		return result;
	}

}
