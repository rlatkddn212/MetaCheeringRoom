#ifndef FBX_SETTING_H
#define FBX_SETTING_H

#include "dllheader.h"


typedef void(*progressfun)(float);

class FBXIMEXPORT FbxIOSetting
{
public:
	/*
	eMayaZUp,			0/*!< UpVector = ZAxis, FrontVector = -ParityOdd, CoordSystem = RightHanded
	eMayaYUp,			1/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded
	eMax,				2/*!< UpVector = ZAxis, FrontVector = -ParityOdd, CoordSystem = RightHanded
	eMotionBuilder,		3/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded
	eOpenGL,			4/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded
	eDirectX,			5/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = LeftHanded
	eLightwave			6/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = LeftHanded
	*/
	int ConvertToAxis;//Ҫת��Ϊ������ϵ		Ĭ��=eMax	2


	/*
	! Predefined system unit for millimeters.
	static const FbxSystemUnit mm;//			����		0
	//! Predefined system unit for decimeters.	����		1
	static const FbxSystemUnit dm;
	//! Predefined system unit for centimeters. ����		2
	static const FbxSystemUnit cm;
	//! Predefined system unit for meters.
	static const FbxSystemUnit m;				��			3
	//! Predefined system unit for kilometers.  ����		4
	static const FbxSystemUnit km;
	//! Predefined system unit for inches.		Ӣ��		5
	static const FbxSystemUnit Inch;
	//! Predefined system unit for feet.		Ӣ��		6
	static const FbxSystemUnit Foot;
	//! Predefined system unit for miles.		Ӣ��		7
	static const FbxSystemUnit Mile;
	//! Predefined system unit for yards.		��			8
	static const FbxSystemUnit Yard;
	*/
	int ConvertToUnit;//Ҫת��Ϊ�ĵ�λ,			Ĭ��=3 		��m

					  //�Ƿ������true  Ĭ��true
	bool ImportMaterial;

	//�Ƿ�����ͼ  Ĭ��true
	bool ImportTexture;

	//�Ƿ��붯��  Ĭ��true
	bool ImportAnimation;

	//���������ߵ�������Ƿ�������ߣ�Ĭ��false
	bool CalTangentIfNone;


	//password
	char PassWord[64];

	//���ش�����Ϣ
	char pErrorReturn[512];

	//������̽���
	progressfun progress;

	FbxIOSetting();
	~FbxIOSetting();
};
#endif
