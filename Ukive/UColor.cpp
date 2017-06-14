#include "UCommon.h"
#include "UNumber.h"
#include "UColor.h"


D2D1::ColorF UColor::parse(std::wstring color)
{
	if (color.empty() || color.at(0) != L'#')
		throw std::invalid_argument("unknown color");

	if (color.length() == 7)
	{
		int r = UNumber::parseInt(color.substr(1, 2), 16);
		int g = UNumber::parseInt(color.substr(3, 2), 16);
		int b = UNumber::parseInt(color.substr(5, 2), 16);

		return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f);
	}
	else if (color.length() == 9)
	{
		int a = UNumber::parseInt(color.substr(1, 2), 16);
		int r = UNumber::parseInt(color.substr(3, 2), 16);
		int g = UNumber::parseInt(color.substr(5, 2), 16);
		int b = UNumber::parseInt(color.substr(7, 2), 16);

		return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	}
	else
		throw std::invalid_argument("unknown color");
}

D2D1::ColorF UColor::rgb(int r, int g, int b)
{
	return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f);
}

D2D1::ColorF UColor::rgb(float r, float g, float b)
{
	return D2D1::ColorF(r, g, b);
}

D2D1::ColorF UColor::argb(int r, int g, int b, int a)
{
	return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

D2D1::ColorF UColor::argb(float r, float g, float b, float a)
{
	return D2D1::ColorF(r, g, b, a);
}


//Color White.
D2D1::ColorF UColor::White = UColor::parse(L"#FFFFFF");
//Color Black.
D2D1::ColorF UColor::Black = UColor::parse(L"#000000");

//Material Color Red.
D2D1::ColorF UColor::Red50 = UColor::parse(L"#FFEBEE"); 
D2D1::ColorF UColor::Red100 = UColor::parse(L"#FFCDD2");
D2D1::ColorF UColor::Red200 = UColor::parse(L"#EF9A9A"); 
D2D1::ColorF UColor::Red300 = UColor::parse(L"#E57373");
D2D1::ColorF UColor::Red400 = UColor::parse(L"#EF5350"); 
D2D1::ColorF UColor::Red500 = UColor::parse(L"#F44336");
D2D1::ColorF UColor::Red600 = UColor::parse(L"#E53935"); 
D2D1::ColorF UColor::Red700 = UColor::parse(L"#D32F2F");
D2D1::ColorF UColor::Red800 = UColor::parse(L"#C62828"); 
D2D1::ColorF UColor::Red900 = UColor::parse(L"#B71C1C");

//Material Color Yellow.
D2D1::ColorF UColor::Yellow50 = UColor::parse(L"#FFFDE7");
D2D1::ColorF UColor::Yellow100 = UColor::parse(L"#FFF9C4");
D2D1::ColorF UColor::Yellow200 = UColor::parse(L"#FFF59D");
D2D1::ColorF UColor::Yellow300 = UColor::parse(L"#FFF176");
D2D1::ColorF UColor::Yellow400 = UColor::parse(L"#FFEE58");
D2D1::ColorF UColor::Yellow500 = UColor::parse(L"#FFEB3B");
D2D1::ColorF UColor::Yellow600 = UColor::parse(L"#FDD835");
D2D1::ColorF UColor::Yellow700 = UColor::parse(L"#FBC02D");
D2D1::ColorF UColor::Yellow800 = UColor::parse(L"#F9A825");
D2D1::ColorF UColor::Yellow900 = UColor::parse(L"#F57F17");

//Material Color Pink.
D2D1::ColorF UColor::Pink50 = UColor::parse(L"#FCE4EC"); 
D2D1::ColorF UColor::Pink100 = UColor::parse(L"#F8BBD0");
D2D1::ColorF UColor::Pink200 = UColor::parse(L"#F48FB1"); 
D2D1::ColorF UColor::Pink300 = UColor::parse(L"#F06292");
D2D1::ColorF UColor::Pink400 = UColor::parse(L"#EC407A"); 
D2D1::ColorF UColor::Pink500 = UColor::parse(L"#E91E63");
D2D1::ColorF UColor::Pink600 = UColor::parse(L"#D81B60"); 
D2D1::ColorF UColor::Pink700 = UColor::parse(L"#C2185B");
D2D1::ColorF UColor::Pink800 = UColor::parse(L"#AD1457"); 
D2D1::ColorF UColor::Pink900 = UColor::parse(L"#880E4F");

//Material Color Green.
D2D1::ColorF UColor::Green50 = UColor::parse(L"#E8F5E9");
D2D1::ColorF UColor::Green100 = UColor::parse(L"#C8E6C9");
D2D1::ColorF UColor::Green200 = UColor::parse(L"#A5D6A7");
D2D1::ColorF UColor::Green300 = UColor::parse(L"#81C784");
D2D1::ColorF UColor::Green400 = UColor::parse(L"#66BB6A");
D2D1::ColorF UColor::Green500 = UColor::parse(L"#4CAF50");
D2D1::ColorF UColor::Green600 = UColor::parse(L"#43A047");
D2D1::ColorF UColor::Green700 = UColor::parse(L"#388E3C");
D2D1::ColorF UColor::Green800 = UColor::parse(L"#2E7D32");
D2D1::ColorF UColor::Green900 = UColor::parse(L"#1B5E20");

//Material Color Blue.
D2D1::ColorF UColor::Blue50 = UColor::parse(L"#E3F2FD");
D2D1::ColorF UColor::Blue100 = UColor::parse(L"#BBDEFB");
D2D1::ColorF UColor::Blue200 = UColor::parse(L"#90CAF9");
D2D1::ColorF UColor::Blue300 = UColor::parse(L"#64B5F6");
D2D1::ColorF UColor::Blue400 = UColor::parse(L"#42A5F5");
D2D1::ColorF UColor::Blue500 = UColor::parse(L"#2196F3");
D2D1::ColorF UColor::Blue600 = UColor::parse(L"#1E88E5");
D2D1::ColorF UColor::Blue700 = UColor::parse(L"#1976D2");
D2D1::ColorF UColor::Blue800 = UColor::parse(L"#1565C0");
D2D1::ColorF UColor::Blue900 = UColor::parse(L"#0D47A1");

//Material Color Grey.
D2D1::ColorF UColor::Grey50 = UColor::parse(L"#FAFAFA");
D2D1::ColorF UColor::Grey100 = UColor::parse(L"#F5F5F5");
D2D1::ColorF UColor::Grey200 = UColor::parse(L"#EEEEEE");
D2D1::ColorF UColor::Grey300 = UColor::parse(L"#E0E0E0");
D2D1::ColorF UColor::Grey400 = UColor::parse(L"#BDBDBD");
D2D1::ColorF UColor::Grey500 = UColor::parse(L"#9E9E9E");
D2D1::ColorF UColor::Grey600 = UColor::parse(L"#757575");
D2D1::ColorF UColor::Grey700 = UColor::parse(L"#616161");
D2D1::ColorF UColor::Grey800 = UColor::parse(L"#424242");
D2D1::ColorF UColor::Grey900 = UColor::parse(L"#212121");