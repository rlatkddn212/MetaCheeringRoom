// Copyright 2021 ls Sun, Inc. All Rights Reserved.


#include "CommonImport.h"
#include <FileActor.h>
#include <loadImage.h>
#include <StaticMeshAttributes.h>

#include "Runtime/Launch/Resources/Version.h"
#include "Materials/Material.h"

#if ENGINE_MAJOR_VERSION < 5//UE4
#define Vec2 FVector2D
#define Vec3 FVector
#define Vec4 FVector4
#else//UE5.+
#define Vec2 FVector2f
#define Vec3 FVector3f
#define Vec4 FVector4f

#if ENGINE_MINOR_VERSION >1//5.2+
#include "MaterialDomain.h"
#endif

#endif

namespace FileIO
{
void ForSeperateMesh::Init(const MeshIO::MeshPtVector& pts)
{
    Vertices.Reserve(pts.vertexCount);
    Normals.Reserve(pts.vertexCount);
    Tangents.Reserve(pts.vertexCount);
    UV0.Reserve(pts.vertexCount);
    VertexClrs.Reserve(pts.vertexCount);

    for (int i = 0; i < pts.vertexCount; ++i)
    {
        const MeshIO::MeshPt& meshPt = pts[i];
        FVector vertex = meshPt.vertex.to<FVector>();
        vertex.Y *= -1;
        Vertices.Add(vertex);
        FVector normal = meshPt.normal.to<FVector>();
        normal.Y *= -1;
        Normals.Add(normal);

        FVector tangent = meshPt.tangent.to<FVector>();
        tangent.Y *= -1;
        Tangents.Add(tangent);

        FVector2D uv = meshPt.uv.to<FVector2D>();
        uv.Y = 1 - uv.Y;
        UV0.Add(uv);
        FLinearColor clr = meshPt.clr.to<FLinearColor>();
        VertexClrs.Add(clr.ToRGBE());
    }
}

int32 ForSeperateMesh::BigToSub(int32 subMeshVertexIndexInBig, ForSeperateMesh& subMeshRender, TMap<int32, int32>& bigVIndex2SubVIndex) const
{
    int32* indexInSub = bigVIndex2SubVIndex.Find(subMeshVertexIndexInBig);
    if (indexInSub)
        return *indexInSub;

    int32 nVertexIndexInSub = subMeshRender.Vertices.Num();
    bigVIndex2SubVIndex.Add(subMeshVertexIndexInBig, nVertexIndexInSub);//整体的顶点索引到该mesh的顶点索引

    subMeshRender.Vertices.Add(Vertices[subMeshVertexIndexInBig]);
    subMeshRender.Normals.Add(Normals[subMeshVertexIndexInBig]);
    subMeshRender.Tangents.Add(Tangents[subMeshVertexIndexInBig]);
    subMeshRender.UV0.Add(UV0[subMeshVertexIndexInBig]);
    subMeshRender.VertexClrs.Add(VertexClrs[subMeshVertexIndexInBig]);
    return nVertexIndexInSub;
}

bool ForSeperateMesh::addMeshToSub(const MeshIO::InterSubMesh& subMesh, ForSeperateMesh& subMeshRender, TArray<int32>& Triangles) const
{
    int32 nTriIndexNum = subMesh.trianglesCount * 3;

    //可能用不完，因为可能有重复点
    subMeshRender.Vertices.Reserve(nTriIndexNum);
    subMeshRender.Normals.Reserve(nTriIndexNum);
    subMeshRender.Tangents.Reserve(nTriIndexNum);
    subMeshRender.UV0.Reserve(nTriIndexNum);
    subMeshRender.VertexClrs.Reserve(nTriIndexNum);

    TMap<int32, int32> bigVIndex2SubVIndex;
    for (int j = 0; j < subMesh.trianglesCount; ++j)
    {
        const MeshIO::Triangle& aTri = subMesh[j];
        /*Triangles.Add(aTri[0]);
        Triangles.Add(aTri[1]);
        Triangles.Add(aTri[2]);*/
        Triangles.Add(BigToSub(aTri[0], subMeshRender, bigVIndex2SubVIndex));
        Triangles.Add(BigToSub(aTri[1], subMeshRender, bigVIndex2SubVIndex));
        Triangles.Add(BigToSub(aTri[2], subMeshRender, bigVIndex2SubVIndex));
    }
    return subMeshRender.Vertices.Num() > 2;
}

//////////////////////////////////////////////////////////////////////////
UTexture* FileMats::GetOrCreateTexture(const FString& imagePath)
{
    UTexture** rlt = uTextureCreated.Find(imagePath);
    if (rlt)
        return *rlt;
    UTexture* tex2D = loadImage::Img2Texture(imagePath);
    uTextureCreated.Add(imagePath, tex2D);
    if (!imagePath.IsEmpty() && !tex2D)
    {
        //UE_LOG(LogTemp, Warning, TEXT("load Image failed: %s"), *imagePath);
    }
    return tex2D;
}

UMaterialInterface* FileMats::CreateMaterial(MeshIO::MeshMaterial* mat)
{
    FString matName = UTF8_TO_TCHAR(mat->name);

    //从用户设置的map中查找并试图使用用户设定的材质
    {
        UMaterialInterface** matMatch = _userMats.Find(matName);
        if (matMatch)
        {
            return *matMatch;
        }
    }

    const MeshIO::ColorChannel& diffuse = mat->mDiffuse;
    const MeshIO::ColorChannel& transparent = mat->mTransparency;

    /*
    0：没有贴图，默认白颜色
    1：漫反射
    2：透明
    */
    int nType = 0;


    FString TransparentPic = UTF8_TO_TCHAR(transparent.strPic);
    FString DiffusePic = UTF8_TO_TCHAR(diffuse.strPic);
    UTexture* DiffuseTex = GetOrCreateTexture(DiffusePic);
    if (!DiffusePic.IsEmpty())nType = 1;
    UTexture* TransparentTex = GetOrCreateTexture(TransparentPic);
    if (!TransparentPic.IsEmpty())nType = 2;

    UMaterialInterface* SrcMat = /*AFileActor::m_pVertexColorMaterial*/AFileActor::m_pDefaultColorMaterial;
    if (nType == 1)
    {
        SrcMat = AFileActor::m_pDefaultMaterial;
    }
    if (nType == 2)
    {
        SrcMat = AFileActor::m_pDefaultAlphaMaterial;
    }

    UMaterialInstanceDynamic* pMatInstance = UMaterialInstanceDynamic::Create(SrcMat, nullptr);

    if (nType == 0)
    {
        FLinearColor clr(diffuse.clr[0], diffuse.clr[1], diffuse.clr[2], diffuse.clr[3]);
        pMatInstance->SetVectorParameterValue(TEXT("MainClr"), clr);
    }
    else if (nType == 1)
    {
        pMatInstance->SetTextureParameterValue(TEXT("MainTex"), DiffuseTex);
    }
    else if (nType == 2)
    {
        pMatInstance->SetTextureParameterValue(TEXT("MainTex"), DiffuseTex);
        pMatInstance->SetTextureParameterValue(TEXT("TransparentTex"), TransparentTex);
    }

    return pMatInstance;
}

FileMats::FileMats(const MeshIO::FMaterials& mats_p)
    :mats(mats_p)
{

}

FileMats::FileMats()
{

}

void FileMats::setMat_p(const MeshIO::FMaterials& mats_p)
{
    mats = mats_p;
}

void FileMats::addUserMats(const TArray<UMaterialInterface*>& userMats)
{
    for (size_t i = 0; i < userMats.Num(); i++)
    {
        UMaterialInterface* pMat = userMats[i];
        if (!pMat)continue;
        FString name = pMat->GetName();
        UMaterialInterface** pmat = _userMats.Find(name);
        if (!pmat)
        {
            _userMats.Add(name, pMat);
        }
    }
}

UMaterialInterface* FileMats::getMat(int nMatId)
{
    //锁
    FScopeLock scopeLock(&_mutex);

    UMaterialInterface** rlt = umatsCreated.Find(nMatId);
    if (rlt)
        return *rlt;

    UMaterialInterface* pMatInstance = 0;
    {
        MeshIO::MeshMaterial* mat = 0;
        if (mats.getMat(nMatId, mat))
        {
            if (IsInGameThread())
            {
                pMatInstance = CreateMaterial(mat);
            }
            else
            {
                FileMats *pFMats = this;
                FGraphEventRef GameThreadCreateMatTask = FFunctionGraphTask::CreateAndDispatchWhenReady([&pMatInstance, pFMats, mat]()
                    {
                        pMatInstance = pFMats->CreateMaterial(mat);
                    }, TStatId(), NULL, ENamedThreads::GameThread);
                FTaskGraphInterface::Get().WaitUntilTaskCompletes(GameThreadCreateMatTask);
            }
        }
		if (!pMatInstance)pMatInstance = AFileActor::m_pDefaultColorMaterial;
		umatsCreated.Add(nMatId, pMatInstance);
    }
    return pMatInstance;
}


FMeshDescription BuildMeshDescriptionFromInterMesh(MeshIO::InterMesh* mesh, FileMats* fileMats, TArray<UMaterialInterface*>& sectionsMat)
{
    FMeshDescription MeshDescription;
    if(!mesh)return MeshDescription;

    FStaticMeshAttributes AttributeGetter(MeshDescription);
    AttributeGetter.Register();
    TPolygonGroupAttributesRef<FName> PolygonGroupNames = AttributeGetter.GetPolygonGroupMaterialSlotNames();
    TVertexAttributesRef<Vec3> VertexPositions = AttributeGetter.GetVertexPositions();
    TVertexInstanceAttributesRef<Vec3> Tangents = AttributeGetter.GetVertexInstanceTangents();
    TVertexInstanceAttributesRef<float> BinormalSigns = AttributeGetter.GetVertexInstanceBinormalSigns();
    TVertexInstanceAttributesRef<Vec3> Normals = AttributeGetter.GetVertexInstanceNormals();
    TVertexInstanceAttributesRef<Vec4> Colors = AttributeGetter.GetVertexInstanceColors();
    TVertexInstanceAttributesRef<Vec2> UVs = AttributeGetter.GetVertexInstanceUVs();

    /*
    MeshPtVector pts;//顶点
    int submeshCount;//子mesh数量
    InterSubMesh *psubmeshs;//所有的子mesh

    class InterSubMesh
    {
public:
    int trianglesCount;
    Triangle *triangles;
    //MeshMaterial mat;//一个材质
    int MatID;//材质ID
    };
    */
    const int32 NumSections = mesh->submeshCount;
    int32 VertexCount = mesh->pts.vertexCount;
    int32 VertexInstanceCount = 0;
    int32 PolygonCount = 0;
    


    TMap<UMaterialInterface*, FPolygonGroupID> UniqueMaterials;
    TMap<int, UMaterialInterface*> matId2Mat;
    for (int i = 0; i < NumSections; ++i)
    {
        MeshIO::InterSubMesh& section = mesh->psubmeshs[i];
        UMaterialInterface* pMat = nullptr;
        if (fileMats)
        {
            pMat = fileMats->getMat(section.MatID);
        }
        if (!pMat)
        {
            pMat = UMaterial::GetDefaultMaterial(MD_Surface);
        }

        if (!UniqueMaterials.Contains(pMat))
        {
            FPolygonGroupID NewPolygonGroup = MeshDescription.CreatePolygonGroup();

            PolygonGroupNames[NewPolygonGroup] = pMat->GetFName();
            UniqueMaterials.Add(pMat, NewPolygonGroup);
            matId2Mat.Add(section.MatID,pMat);
            sectionsMat.Add(pMat);
        }
    }

    TArray<FPolygonGroupID> PolygonGroupForSection;
    PolygonGroupForSection.Reserve(NumSections);

    // Calculate the totals for each ProcMesh element type
    for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
    {
        MeshIO::InterSubMesh& section = mesh->psubmeshs[SectionIdx];
        VertexInstanceCount += section.trianglesCount * 3;
        PolygonCount += section.trianglesCount;

        // Create the Polygon Groups
        UMaterialInterface** pMat = matId2Mat.Find(section.MatID);
        check(pMat != nullptr);
        FPolygonGroupID* PolygonGroupID = UniqueMaterials.Find(*pMat);
        check(PolygonGroupID != nullptr);
        PolygonGroupForSection.Add(*PolygonGroupID);
    }

    MeshDescription.ReserveNewVertices(VertexCount);
    MeshDescription.ReserveNewVertexInstances(VertexInstanceCount);
    MeshDescription.ReserveNewPolygons(PolygonCount);
    MeshDescription.ReserveNewEdges(PolygonCount * 2);

#if ENGINE_MAJOR_VERSION <5
	UVs.SetNumIndices(4);
#else
	UVs.SetNumChannels(4);
#endif


    // Create the vertex
    const MeshIO::MeshPtVector& allVertexs = mesh->pts;
    int32 NumVertex = allVertexs.vertexCount;
    TMap<int32, FVertexID> VertexIndexToVertexID;
    VertexIndexToVertexID.Reserve(NumVertex);

    for (int32 VertexIndex = 0; VertexIndex < NumVertex; ++VertexIndex)
    {
        const MeshIO::MeshPt& Vert = mesh->pts[VertexIndex];
        const FVertexID VertexID = MeshDescription.CreateVertex();
        Vec3 meshVertex = Vert.vertex.to<Vec3>();
        meshVertex.Y*=-1;
        VertexPositions[VertexID] = meshVertex;
        VertexIndexToVertexID.Add(VertexIndex, VertexID);
    }

    
    // Add Vertex and VertexInstance and polygon for each section
    for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
    {
        const MeshIO::InterSubMesh& ProcSection = mesh->psubmeshs[SectionIdx];
        
        FPolygonGroupID PolygonGroupID = PolygonGroupForSection[SectionIdx];
        
        // Create the VertexInstance
        int32 NumTri = ProcSection.trianglesCount;
        int32 NumIndices = NumTri * 3;
        
        for (int32 TriIdx = 0; TriIdx < NumTri; TriIdx++)
        {
            const MeshIO::Triangle& tri = ProcSection[TriIdx];
            TArray<FVertexInstanceID> VertexInstanceIDs;
            for (int32 CornerIndex = 0; CornerIndex < 3; ++CornerIndex)
            {
                const int VertexIndex = tri[CornerIndex];

                const FVertexID VertexID = VertexIndexToVertexID[VertexIndex];
                const FVertexInstanceID VertexInstanceID =
                    MeshDescription.CreateVertexInstance(VertexID);
                VertexInstanceIDs.Add(VertexInstanceID);

                const MeshIO::MeshPt & ProcVertex = allVertexs[VertexIndex];

                Vec3 tangent = ProcVertex.tangent.to<Vec3>();
                tangent.Y *= -1;
                Tangents[VertexInstanceID] = tangent;
                Vec3 normal = ProcVertex.normal.to<Vec3>();
                normal.Y *= -1;
                Normals[VertexInstanceID] = normal;
                BinormalSigns[VertexInstanceID] = 1.0f;
                Colors[VertexInstanceID] = ProcVertex.clr.to<FLinearColor>();
                Vec2 uv = ProcVertex.uv.to<Vec2>();
                uv.Y = 1-uv.Y;
                UVs.Set(VertexInstanceID, 0, uv);
                UVs.Set(VertexInstanceID, 1, uv);
                UVs.Set(VertexInstanceID, 2, uv);
                UVs.Set(VertexInstanceID, 3, uv);
            }
            MeshDescription.CreatePolygon(PolygonGroupID, VertexInstanceIDs);
        }
    }
    return MeshDescription;
    
}




FBox GetActorBoundWithChildren(AActor* pActor, bool bNonColliding /*= false*/
, bool bIncludeFromChildActors /*= true*/
    , TSubclassOf<AActor> ClassIgnore /*= nullptr*/)
{
    //const FBox Bounds = pactor->GetComponentsBoundingBox(true, true);
    FBox box(ForceInit);
    if (!pActor)
        return box;
    box = pActor->GetComponentsBoundingBox(bNonColliding, bIncludeFromChildActors);
    TArray<AActor*> ChildActors;

    //USceneComponent* pSceneComponent = pActor->GetDefaultAttachComponent();
    //TArray<USceneComponent*> GetAttachChildren
    pActor->GetAttachedActors(ChildActors, false);
    for (AActor* actor : ChildActors)
    {
        if (!actor || (ClassIgnore && actor->IsA(ClassIgnore)))
            continue;
        FBox boxCur = GetActorBoundWithChildren(actor, bNonColliding, bIncludeFromChildActors);
        box += boxCur;
    }
    return box;
}

FBox GetActorBoundWithChildren(const TSet<AActor*>& actors, bool bNonColliding /*= false*/
, bool bIncludeFromChildActors /*= true*/
    , TSubclassOf<AActor> ClassIgnore /*= nullptr*/)
{
    FBox box(ForceInit);
    for (AActor* actor : actors)
    {
        if (!actor)
            continue;
        FBox boxCur = GetActorBoundWithChildren(actor, bNonColliding, bIncludeFromChildActors, ClassIgnore);
        box += boxCur;
    }
    return box;
}

FBox GetActorBoundWithChildren(const TArray<AActor*>& actors, bool bNonColliding /*= false*/
, bool bIncludeFromChildActors /*= true */, TSubclassOf<AActor> ClassIgnore /*= nullptr*/)
{
    FBox box(ForceInit);
    for (AActor* actor : actors)
    {
        if (!actor)
            continue;
        FBox boxCur = GetActorBoundWithChildren(actor, bNonColliding, bIncludeFromChildActors, ClassIgnore);
        box += boxCur;
    }
    return box;
}

}