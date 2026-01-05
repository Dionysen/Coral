#include "Coral/Attribute.hpp"
#include "Coral/Type.hpp"
#include "Coral/Assembly.hpp"
#include "Coral/String.hpp"

#include "CoralManagedFunctions.hpp"

namespace Coral {

	Type& Attribute::GetType()
	{
		if (!m_Type)
		{
			TypeId typeId;
			s_ManagedFunctions.GetAttributeTypeFptr(m_Handle, &typeId);
			if (m_Assembly)
			{
				m_Type = &m_Assembly->GetLocalType(typeId);
			}
		}

		return *m_Type;
	}

	template<>
	std::string Attribute::GetFieldValue(std::string_view InFieldName)
	{
		String result;
		GetFieldValueInternal(InFieldName, &result);
		return std::string(result);
	}

	template<>
	bool Attribute::GetFieldValue(std::string_view InFieldName)
	{
		Bool32 result;
		GetFieldValueInternal(InFieldName, &result);
		return result;
	}

	void Attribute::GetFieldValueInternal(std::string_view InFieldName, void* OutValue) const
	{
		auto fieldName = String::New(InFieldName);
		s_ManagedFunctions.GetAttributeFieldValueFptr(m_Handle, fieldName, OutValue);
		String::Free(fieldName);
	}

}
