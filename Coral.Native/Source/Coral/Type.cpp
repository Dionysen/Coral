#include "Coral/Type.hpp"
#include "Coral/Assembly.hpp"
#include "Coral/Attribute.hpp"

#include "CoralManagedFunctions.hpp"

namespace Coral {

	String Type::GetFullName() const
	{
		return s_ManagedFunctions.GetFullTypeNameFptr(m_Id);
	}

	String Type::GetAssemblyQualifiedName() const
	{
		return s_ManagedFunctions.GetAssemblyQualifiedNameFptr(m_Id);
	}

	Type& Type::GetBaseType()
	{
		if (!m_BaseType)
		{
			TypeId baseTypeId;
			s_ManagedFunctions.GetBaseTypeFptr(m_Id, &baseTypeId);
			if (m_Assembly)
			{
				m_BaseType = &m_Assembly->GetLocalType(baseTypeId);
			}
		}

		return *m_BaseType;
	}

	std::vector<Type*>& Type::GetInterfaceTypes()
	{
		if (!m_InterfaceTypes)
		{
			int count;
			s_ManagedFunctions.GetInterfaceTypeCountFptr(m_Id, &count);

			std::vector<TypeId> typeIds;
			typeIds.resize(static_cast<size_t>(count));
			s_ManagedFunctions.GetInterfaceTypesFptr(m_Id, typeIds.data());

			m_InterfaceTypes = std::vector<Type*>();
			m_InterfaceTypes->reserve(static_cast<size_t>(count));

			if (m_Assembly)
			{
				for (auto id : typeIds)
				{
					m_InterfaceTypes->emplace_back(&m_Assembly->GetLocalType(id));
				}
			}
		}

		return *m_InterfaceTypes;
	}

	int32_t Type::GetSize() const
	{
		return s_ManagedFunctions.GetTypeSizeFptr(m_Id);
	}

	bool Type::IsSubclassOf(const Type& InOther) const
	{
		return s_ManagedFunctions.IsTypeSubclassOfFptr(m_Id, InOther.m_Id);
	}

	bool Type::IsAssignableTo(const Type& InOther) const
	{
		return s_ManagedFunctions.IsTypeAssignableToFptr(m_Id, InOther.m_Id);
	}

	bool Type::IsAssignableFrom(const Type& InOther) const
	{
		return s_ManagedFunctions.IsTypeAssignableFromFptr(m_Id, InOther.m_Id);
	}

	std::vector<MethodInfo> Type::GetMethods() const
	{
		int32_t methodCount = 0;
		s_ManagedFunctions.GetTypeMethodsFptr(m_Id, nullptr, &methodCount);
		std::vector<ManagedHandle> handles(static_cast<size_t>(methodCount));
		s_ManagedFunctions.GetTypeMethodsFptr(m_Id, handles.data(), &methodCount);

		std::vector<MethodInfo> methods(handles.size());
		for (size_t i = 0; i < handles.size(); i++)
		{
			methods[i].m_Handle = handles[i];
			methods[i].m_Assembly = m_Assembly;
		}

		return methods;
	}

	std::vector<FieldInfo> Type::GetFields() const
	{
		int32_t fieldCount = 0;
		s_ManagedFunctions.GetTypeFieldsFptr(m_Id, nullptr, &fieldCount);
		std::vector<ManagedHandle> handles(static_cast<size_t>(fieldCount));
		s_ManagedFunctions.GetTypeFieldsFptr(m_Id, handles.data(), &fieldCount);

		std::vector<FieldInfo> fields(handles.size());
		for (size_t i = 0; i < handles.size(); i++)
		{
			fields[i].m_Handle = handles[i];
			fields[i].m_Assembly = m_Assembly;
		}

		return fields;
	}

	std::vector<PropertyInfo> Type::GetProperties() const
	{
		int32_t propertyCount = 0;
		s_ManagedFunctions.GetTypePropertiesFptr(m_Id, nullptr, &propertyCount);
		std::vector<ManagedHandle> handles(static_cast<size_t>(propertyCount));
		s_ManagedFunctions.GetTypePropertiesFptr(m_Id, handles.data(), &propertyCount);

		std::vector<PropertyInfo> properties(handles.size());
		for (size_t i = 0; i < handles.size(); i++)
		{
			properties[i].m_Handle = handles[i];
			properties[i].m_Assembly = m_Assembly;
		}

		return properties;
	}

	bool Type::HasAttribute(const Type& InAttributeType) const
	{
		return s_ManagedFunctions.HasTypeAttributeFptr(m_Id, InAttributeType.m_Id);
	}

	std::vector<Attribute> Type::GetAttributes() const
	{
		int32_t attributeCount;
		s_ManagedFunctions.GetTypeAttributesFptr(m_Id, nullptr, &attributeCount);
		std::vector<ManagedHandle> attributeHandles(static_cast<size_t>(attributeCount));
		s_ManagedFunctions.GetTypeAttributesFptr(m_Id, attributeHandles.data(), &attributeCount);

		std::vector<Attribute> result(attributeHandles.size());
		for (size_t i = 0; i < attributeHandles.size(); i++)
		{
			result[i].m_Handle = attributeHandles[i];
			result[i].m_Assembly = m_Assembly;
		}

		return result;
	}

	ManagedType Type::GetManagedType() const
	{
		return s_ManagedFunctions.GetTypeManagedTypeFptr(m_Id);
	}

	bool Type::IsSZArray() const
	{
		return s_ManagedFunctions.IsTypeSZArrayFptr(m_Id);
	}

	Type& Type::GetElementType()
	{
		if (!m_ElementType)
		{
			TypeId elementTypeId;
			s_ManagedFunctions.GetElementTypeFptr(m_Id, &elementTypeId);
			if (m_Assembly)
			{
				m_ElementType = &m_Assembly->GetLocalType(elementTypeId);
			}
		}

		return *m_ElementType;
	}

	bool Type::operator==(const Type& InOther) const
	{
		return m_Id == InOther.m_Id;
	}

	ManagedObject Type::CreateInstanceInternal(const void** InParameters, const ManagedType* InParameterTypes, size_t InLength) const
	{
		ManagedObject result;
		result.m_Handle = s_ManagedFunctions.CreateObjectFptr(m_Id, false, InParameters, InParameterTypes, static_cast<int32_t>(InLength));
		result.m_Type = this;
		return result;
	}

	void Type::InvokeStaticMethodInternal(std::string_view InMethodName, const void** InParameters, const ManagedType* InParameterTypes, size_t InLength) const
	{
		auto methodName = String::New(InMethodName);
		s_ManagedFunctions.InvokeStaticMethodFptr(m_Id, methodName, InParameters, InParameterTypes, static_cast<int32_t>(InLength));
		String::Free(methodName);
	}

	void Type::InvokeStaticMethodRetInternal(std::string_view InMethodName, const void** InParameters, const ManagedType* InParameterTypes, size_t InLength, void* InResultStorage) const
	{
		auto methodName = String::New(InMethodName);
		s_ManagedFunctions.InvokeStaticMethodRetFptr(m_Id, methodName, InParameters, InParameterTypes, static_cast<int32_t>(InLength), InResultStorage);
		String::Free(methodName);
	}


	ReflectionType::operator Type&() const
	{
		static Type s_NullType;
		// Note: ReflectionType doesn't have an Assembly reference.
		// This operator is deprecated and should be avoided.
		// Use Assembly::GetLocalType(TypeId) instead.
		return s_NullType;
	}

}
