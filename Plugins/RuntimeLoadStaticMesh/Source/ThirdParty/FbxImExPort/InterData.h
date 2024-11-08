#ifndef _INTER_DATA_H
#define _INTER_DATA_H

namespace MeshIO
{

class Double2
{
public:
	double d[2];
    const double & operator[](int i)const {return d[i]; }
    template <class T>
    T to() const
    {
        return T(d[0],d[1]);
    }
};

class Double3
{
public:
	double d[3];
    const double& operator[](int i) const { return d[i]; }
    template <class T>
    T to(bool bSwapXY = false) const
    {
        return T(bSwapXY?d[1]:d[0], bSwapXY ? d[0] : d[1],d[2]);
    }
};

class Double4
{
public:
	double d[4];
    const double& operator[](int i)const { return d[i]; }
    template <class T>
    T to() const
    {
        return T(d[0], d[1], d[2],d[3]);
    }
};

class Triangle
{
public:
	int index[3];//三角形索引,数量是size个
    const int& operator[](int i)const { return index[i]; }
};

class MeshPt
{
public:
	Double3 vertex;
	Double3 normal;
	Double3 tangent;
	Double2 uv;
	Double4 clr;
};

class MeshPtVector
{
public:

	int vertexCount;//实际点个数
	MeshPt *vertexs;
    const MeshPt &operator[](int i)const{return vertexs[i];}
    void flipV()
    {
        if(vertexCount<1)return;
        for (int i = 0; i < vertexCount; ++i)
        {
            MeshPt &meshPt = vertexs[i];
            meshPt.uv.d[1]=1 - meshPt.uv.d[1];
        }
    }
};

class InterMesh;
class MeshVector
{
public:
	int count;
	InterMesh* *pMeshs;
    InterMesh* operator[](int i)const { return pMeshs[i]; }
};

class ColorChannel
{
public:
	char strPic[256];
	Double4 clr;
    const double & operator[](int i)const{return clr[i];}
};

class MeshMaterial
{
public:
	int id;//MaterialID
    char name[64];
	ColorChannel mEmissive;
	ColorChannel mAmbient;
	ColorChannel mDiffuse;
	ColorChannel mSpecular;
    ColorChannel mTransparency;
	float mShinness;
};

class InterSubMesh
{
public:
	int trianglesCount;
	Triangle *triangles;
	//MeshMaterial mat;//一个材质
	int MatID;//材质ID
    const Triangle &operator[](int i)const{return triangles[i]; }
};

class InterMesh
{
public:
	char strNode[64];
	double matrix[16];//节点矩阵
	MeshPtVector pts;//顶点
	int submeshCount;//子mesh数量
	InterSubMesh *psubmeshs;//所有的子mesh
	MeshVector vChildrend;	//子物体指针的数组
};

class FMaterials
{
public:
	int curUseNum;
	int matCount;
    unsigned int numMeshAll;
	MeshMaterial *mats;
    bool getMat(int nMatId, MeshMaterial*& pmat) const
    {
        for (int i = 0; i < matCount; ++i)
        {
            if (mats[i].id == nMatId)
            {
                pmat = &mats[i];
                return true;
            }
        }
        return false;
    }
};

class FbxFullData
{
public:
    char strRootName[64];
	FMaterials mats;
	MeshVector meshs;
    Double3 boxMinPt;
    Double3 boxMaxPt;
    Double3 boxCenterPt;
};
}

#endif
