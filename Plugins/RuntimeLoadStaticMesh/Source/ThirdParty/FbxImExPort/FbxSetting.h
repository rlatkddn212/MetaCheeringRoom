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
	int ConvertToAxis;//要转换为的坐标系		默认=eMax	2


	/*
	! Predefined system unit for millimeters.
	static const FbxSystemUnit mm;//			毫米		0
	//! Predefined system unit for decimeters.	分米		1
	static const FbxSystemUnit dm;
	//! Predefined system unit for centimeters. 厘米		2
	static const FbxSystemUnit cm;
	//! Predefined system unit for meters.
	static const FbxSystemUnit m;				米			3
	//! Predefined system unit for kilometers.  公里		4
	static const FbxSystemUnit km;
	//! Predefined system unit for inches.		英寸		5
	static const FbxSystemUnit Inch;
	//! Predefined system unit for feet.		英尺		6
	static const FbxSystemUnit Foot;
	//! Predefined system unit for miles.		英里		7
	static const FbxSystemUnit Mile;
	//! Predefined system unit for yards.		码			8
	static const FbxSystemUnit Yard;
	*/
	int ConvertToUnit;//要转换为的单位,			默认=3 		米m

					  //是否导入材质true  默认true
	bool ImportMaterial;

	//是否导入贴图  默认true
	bool ImportTexture;

	//是否导入动画  默认true
	bool ImportAnimation;

	//不存在切线的情况下是否计算切线，默认false
	bool CalTangentIfNone;


	//password
	char PassWord[64];

	//返回错误信息
	char pErrorReturn[512];

	//导入过程进度
	progressfun progress;

	FbxIOSetting();
	~FbxIOSetting();
};
#endif
