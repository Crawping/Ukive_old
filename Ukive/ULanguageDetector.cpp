#include "UCommon.h"
#include "ULanguageDetector.h"


ULanguageDetector::ULanguageDetector()
{
	mService = nullptr;
}

ULanguageDetector::~ULanguageDetector()
{
}


bool ULanguageDetector::init()
{
	HRESULT result;
	DWORD servicesCount = 0;
	MAPPING_ENUM_OPTIONS enumOptions;

	::ZeroMemory(&enumOptions, sizeof(MAPPING_ENUM_OPTIONS));
	enumOptions.Size = sizeof(MAPPING_ENUM_OPTIONS);
	enumOptions.pGuid = (GUID *)&ELS_GUID_LANGUAGE_DETECTION;

	result = ::MappingGetServices(&enumOptions, &mService, &servicesCount);
	if (SUCCEEDED(result))
	{
		return true;
	}

	return false;
}

void ULanguageDetector::close()
{
	if (mService != nullptr)
	{
		::MappingFreeServices(mService);
		mService = nullptr;
	}
}

bool ULanguageDetector::detect(std::wstring text)
{
	return detect(text, 0);
}

bool ULanguageDetector::detect(std::wstring text, std::uint32_t start)
{
	HRESULT result;
	MAPPING_PROPERTY_BAG bag;

	if (mService == nullptr)
		return false;
	
	::ZeroMemory(&bag, sizeof(MAPPING_PROPERTY_BAG));
	bag.Size = sizeof(MAPPING_PROPERTY_BAG);

	result = ::MappingRecognizeText(
		mService, text.c_str(), text.length(), start, nullptr, &bag);
	if (SUCCEEDED(result))
	{
		for (DWORD i = 0; i < bag.dwRangesCount; ++i)
		{
			MAPPING_DATA_RANGE range = bag.prgResultRanges[i];
			std::wstring language((wchar_t*)range.pData, range.dwDataSize / 2);
			range.dwStartIndex;
		}

		result = ::MappingFreePropertyBag(&bag);
		return true;
	}

	return false;
}