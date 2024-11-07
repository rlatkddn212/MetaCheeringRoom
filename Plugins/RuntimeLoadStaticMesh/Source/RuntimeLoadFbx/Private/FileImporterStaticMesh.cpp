// Copyright 2021 ls Sun, Inc. All Rights Reserved.


#include "FileImporterStaticMesh.h"
#include "Engine/World.h"
#include <Components/StaticMeshComponent.h>
#include <Async/TaskGraphInterfaces.h>
#include <Materials/MaterialInterface.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <HAL/FileManager.h>
#include "PhysicsEngine/BodySetup.h"
#include <FbxImExPort/FbxSetting.h>
#include <FbxImExPort/LoadFbxC.h>
#include <FbxImExPort/InterData.h>
#include <CommonImport.h>
#include "StaticMeshAttributes.h"

using namespace FileIO;

class AsyncDataStatic
{
    static void RenameActorRootComponent(AActor* actor, const TCHAR* NewName)
    {
		USceneComponent* rootComp = actor->GetRootComponent();
		if (rootComp)
		{
			rootComp->Rename(NewName);
		}
    }
    static void CreateActorWithMatrixAndComponent(TWeakObjectPtr<UWorld> world, AActor*& pActorCreated, MeshIO::InterMesh* mesh
    , TSubclassOf<AActor> actorClass)
    {
        FName fname(UTF8_TO_TCHAR(mesh->strNode));
        FString fstrName = fname.ToString();
        if(fstrName.StartsWith(TEXT("UCX_")))
            return;
        bool bHaveMeshComponent = mesh->psubmeshs ? true : false;
        pActorCreated = CreateActor(world, !bHaveMeshComponent, actorClass);
        if (!pActorCreated)return;
        
        //如果有mesh则创建组件
        if (bHaveMeshComponent)
        {
            //FBX右手矩阵转左手矩阵参照UE4源码：FbxUtilsImport.cpp/FFbxDataConverter::ConvertMatrix
            //{
            FPlane inx(mesh->matrix[0], -mesh->matrix[1], mesh->matrix[2], mesh->matrix[3]);
            FPlane iny(-mesh->matrix[4], mesh->matrix[5], -mesh->matrix[6], -mesh->matrix[7]);
            FPlane inz(mesh->matrix[8], -mesh->matrix[9], mesh->matrix[10], mesh->matrix[11]);
            FPlane inw(mesh->matrix[12], -mesh->matrix[13], mesh->matrix[14], mesh->matrix[15]);
            FMatrix fMat(inx, iny, inz, inw);
            FTransform transform(fMat);

            //}

            UStaticMeshComponent* MeshCmp = NewObject<UStaticMeshComponent>(pActorCreated, fname);
            USceneComponent* parentComponent = pActorCreated->GetRootComponent();
            if (parentComponent)
            {
                MeshCmp->AttachToComponent(parentComponent, FAttachmentTransformRules::KeepRelativeTransform);
            }
            else
            {
                pActorCreated->SetRootComponent(MeshCmp);
            }
            MeshCmp->RegisterComponent();
            pActorCreated->SetActorTransform(transform);
        }
        else
        {
            RenameActorRootComponent(pActorCreated ,*fstrName);
        }
    }

    AActor* GameThreadCreateActor(MeshIO::InterMesh* mesh)
    {
        AActor* pActorCreated = 0;
        TWeakObjectPtr<UWorld> world = _world;
        TSubclassOf<AActor> actorClassuse = _actorClass;

        if (!ReadFileTask.IsValid())
        {
            CreateActorWithMatrixAndComponent(world, pActorCreated, mesh, actorClassuse);
        }
        else
        {
            FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([world, &pActorCreated, mesh, actorClassuse]()
                {
                    CreateActorWithMatrixAndComponent(world, pActorCreated, mesh, actorClassuse);
                }, TStatId(), NULL, ENamedThreads::GameThread);

            FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
        }
        return pActorCreated;
    }

    static void GameThreadSetMeshMat(UMeshComponent* MeshCmp, int i, UMaterialInterface* pMat)
    {
        FFunctionGraphTask::CreateAndDispatchWhenReady([MeshCmp, i, pMat]()
            {
                MeshCmp->SetMaterial(i, pMat);
            }, TStatId(), NULL, ENamedThreads::GameThread);
    }

    void createFileActor(TArray<AActor*>& meshActors, const MeshIO::MeshVector& meshs,bool bRoot = false)
    {
        //float fPos = 0;
        int nNumMesh = meshs.count;
        for (int i = 0; i < nNumMesh; ++i)
        {
            MeshIO::InterMesh* mesh = meshs.pMeshs[i];
            bool hasMesh = mesh->submeshCount > 0;
            AActor* pchild = createAMesh(mesh);
            if(pchild)
                meshActors.Add(pchild);
            
            if (_fprogress&& hasMesh)
            {
                //fPos = float(i+1) / nNumMesh;

                if (!IsInGameThread())
                {
                    FGraphEventRef FinishTask = FFunctionGraphTask::CreateAndDispatchWhenReady([this]()
                        {
                            float fPos = (++_curMeshIndex * 1.0f) / _numMeshAll;
                            _fprogress->CalProgress(fPos);
                        }, TStatId(), NULL, ENamedThreads::GameThread);
                    FTaskGraphInterface::Get().WaitUntilTaskCompletes(FinishTask);
                }
                else
                {
					float fPos = (++_curMeshIndex * 1.0f) / _numMeshAll;
                    _fprogress->CalProgress(fPos);
                }
            }
        }
    }

    static void MakeRenderThreadBusy(FEvent* eventWait)
    {
        FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([eventWait]()
        {
            eventWait->Wait(MAX_uint32);
            FPlatformProcess::ReturnSynchEventToPool(eventWait);
        }, TStatId(), NULL, ENamedThreads::ActualRenderingThread);
    }

    static void CreteStaticMesh(UStaticMeshComponent* MeshCmp, const FMeshDescription& MeshDescriptionOrg
        , const TArray<UMaterialInterface*>& sectionsMat
    , TEnumAsByte<enum CalculateCollisionMethod> calCollision)
    {
        FMeshDescription MeshDescription = MeshDescriptionOrg;
        // If we got some valid data.
        if (MeshDescription.Polygons().Num() > 0)
        {
            FString sName = MeshCmp->GetName();
            // Then find/create it.
            UPackage* Package = GetTransientPackage();//CreatePackage(TEXT("Game/MeshIO/Meshs"));
            check(Package);

            // Create StaticMesh object
            //UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, NAME_None, RF_Public | RF_Standalone);
            UStaticMesh* StaticMesh = NewObject<UStaticMesh>(MeshCmp, *sName/*, RF_Public | RF_Standalone*/);
            /*StaticMesh->InitResources();
            StaticMesh->LightingGuid = FGuid::NewGuid();*/

            //添加材质并匹配描述材质名
            FStaticMeshAttributes AttributeGetter(MeshDescription);
            TPolygonGroupAttributesRef<FName> PolygonGroupNames = AttributeGetter.GetPolygonGroupMaterialSlotNames();
            for (int i = 0; i < sectionsMat.Num(); ++i)
            {
                UMaterialInterface* pMat = sectionsMat[i];
                //StaticMesh->StaticMaterials.Add(FStaticMaterial(pMat));
                PolygonGroupNames[FPolygonGroupID(i)] = StaticMesh->AddMaterial(pMat);
            }
            TArray<const FMeshDescription*> MeshDescriptions;
            MeshDescriptions.Add(&MeshDescription);
            UStaticMesh::FBuildMeshDescriptionsParams Params;
            Params.bBuildSimpleCollision = eSimpleCollision == calCollision;
            Params.bMarkPackageDirty = false;

            FEvent* sevent = nullptr;
            bool bCalComplexCollision = eComplexCollision == calCollision;
#if WITH_EDITOR

#else
#if ENGINE_MAJOR_VERSION > 4
            Params.bFastBuild = true;
#endif          
            if (bCalComplexCollision)
            {
                sevent = FPlatformProcess::GetSynchEventFromPool(false);
                MakeRenderThreadBusy(sevent);
            }
#endif

            StaticMesh->BuildFromMeshDescriptions(MeshDescriptions, Params);

            if (bCalComplexCollision)
            {
#if ENGINE_MAJOR_VERSION < 5
                StaticMesh->BodySetup->CollisionTraceFlag = CTF_UseComplexAsSimple;
#else
                StaticMesh->GetBodySetup()->CollisionTraceFlag = CTF_UseComplexAsSimple;
#endif
                StaticMesh->bAllowCPUAccess = true;
            }

            MeshCmp->SetStaticMesh(StaticMesh);


#if WITH_EDITOR

#else
            //完成Build+SetStaticMesh后，释放对Render线程的占用
            if (sevent&&bCalComplexCollision)
                sevent->Trigger();
#endif
            

            //MeshCmp->BodyInstance.UpdateTriMeshVertices(vertexsAll);
            //MeshCmp->UpdateCollisionFromStaticMesh();
        }
    }
    FGraphEventRef CreateStaticMeshFromDescription(UStaticMeshComponent* MeshCmp,const FMeshDescription& MeshDescriptionOrg
    , const TArray<UMaterialInterface*>& sectionsMat)
    {
        FGraphEventRef task;
        if (IsInGameThread())
        {
            CreteStaticMesh(MeshCmp,MeshDescriptionOrg,sectionsMat, _calCollision);
        }
        else
        {
            TEnumAsByte<enum CalculateCollisionMethod> calCollisionTmp = _calCollision;
            task = FFunctionGraphTask::CreateAndDispatchWhenReady([MeshCmp, MeshDescriptionOrg, sectionsMat, calCollisionTmp]()
                {
                    CreteStaticMesh(MeshCmp, MeshDescriptionOrg, sectionsMat, calCollisionTmp);
                }, TStatId(), NULL, ENamedThreads::GameThread);
        }
        return task;
    }

    AActor* createAMesh(MeshIO::InterMesh* mesh)
    {
        AActor* actor = GameThreadCreateActor(mesh);
        if (!actor)return nullptr;
        //actor->Rename(UTF8_TO_TCHAR(mesh->strNode));
        //创建自身
        {
            if (mesh->submeshCount > 0)
            {
                CreateUMesh(actor, mesh);
            }
        }
        //创建子
        TArray<AActor*> children;
        createFileActor(children, mesh->vChildrend);

        if (IsInGameThread())
        {
            for (AActor* child : children)
            {
                child->AttachToActor(actor, FAttachmentTransformRules::KeepWorldTransform);
            }
        }
        else
        {
            FGraphEventRef AttachTask = FFunctionGraphTask::CreateAndDispatchWhenReady([actor, children]()
                {
                    for (AActor* child : children)
                    {
                        child->AttachToActor(actor, FAttachmentTransformRules::KeepWorldTransform);
                    }
                }, TStatId(), NULL, ENamedThreads::GameThread);

            _AllParseTasks.Add(AttachTask);
        }
        return actor;
    }

    void CreateUMesh(TWeakObjectPtr<AActor> weakActor, MeshIO::InterMesh* mesh)
    {
        AActor* actor = weakActor.Get();
        if (!actor || !mesh || mesh->submeshCount < 1)return;

        UStaticMeshComponent* MeshCmp = Cast<UStaticMeshComponent>(actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
        if (!MeshCmp)return;
        //////////////////////////////////////////////////////////////////////////
        TArray<UMaterialInterface*> sectionsMat;
        FileMats* fileMats = &_fileMats;
        TArray<FVector> vertexsAllCollision;
        TArray<int32> indexsAllCollision;
        FMeshDescription MeshDescription = BuildMeshDescriptionFromInterMesh(mesh, fileMats, sectionsMat);
        FGraphEventRef createMeshTask = CreateStaticMeshFromDescription(MeshCmp,MeshDescription, sectionsMat);
        _AllParseTasks.Add(createMeshTask);

        /*for (int i = 0; i < matIDs.Num(); ++i)
        {
            int nMatId = matIDs[i];
            FGraphEventRef task2 = FFunctionGraphTask::CreateAndDispatchWhenReady([MeshCmp, i, nMatId, fileMats]()
                {
                    UMaterialInterface* pMat = fileMats->getMat(nMatId);
                    GameThreadSetMeshMat(MeshCmp, i, pMat);
                }, TStatId(), NULL, ENamedThreads::AnyBackgroundThreadNormalTask);
            _AllParseTasks.Add(task2);
        }*/

    }


public:
    FGraphEventRef ReadFileTask;//任务
    TWeakObjectPtr < AActor> _rootActor;
    TWeakObjectPtr<UWorld> _world;
    TEnumAsByte<enum CalculateCollisionMethod> _calCollision;
    TSubclassOf<AActor> _actorClass;
    URLFProgress* _fprogress;
    FString _filePath;
    bool _CalTangentIfNone;
    void* _scene;
    unsigned int _numMeshAll;
    unsigned int _curMeshIndex;
    FileMats _fileMats;
    FGraphEventArray _AllParseTasks;//所有的任务

    AsyncDataStatic() : _rootActor(0), _CalTangentIfNone(false), _scene(0), _numMeshAll(0), _curMeshIndex(0)
    {

    }
    ~AsyncDataStatic()
    {

    }

    bool valid()const
    {
        MeshIO::FbxFullData* fulldata = (MeshIO::FbxFullData*)(_scene);
        if (!fulldata || fulldata->meshs.count < 1)return false;
        return true;
    }



    void FileLoadedAsync()
    {
        FbxIOSetting ios;
        ios.ConvertToAxis = 2;//转为右手Z向上，下面Mesh顶点和uv反转，参照了UnFbx::FFbxImporter::BuildStaticMeshFromGeometry
        ios.ConvertToUnit = 2;
        ios.CalTangentIfNone = _CalTangentIfNone;
        //ios.progress = progressFunction;
        loadFbxMemory(TCHAR_TO_UTF8(*_filePath), _scene, &ios);

        MeshIO::FbxFullData* fulldata = (MeshIO::FbxFullData*)(_scene);
        if (!fulldata || fulldata->meshs.count < 1)return;

        MeshIO::MeshVector meshs = fulldata->meshs;
        _fileMats.setMat_p(fulldata->mats);
        _numMeshAll = fulldata->mats.numMeshAll;
        if (_numMeshAll == 0)
            _numMeshAll = 1;

        TArray<AActor*> meshActors;
        createFileActor(meshActors, meshs,true);
        if(ReadFileTask.IsValid())//Async
            FTaskGraphInterface::Get().WaitUntilTasksComplete(_AllParseTasks);
        FString strRootName = UTF8_TO_TCHAR(fulldata->strRootName);
        freeFbxMemory(_scene);

        TWeakObjectPtr < AActor> root = _rootActor;
        //根节点归底归中
        if(root.IsValid())
        {
            FBox box = FileIO::GetActorBoundWithChildren(meshActors);
            FVector vCenter = box.GetCenter();
            vCenter.Z = box.Min.Z;
            root->SetActorLocation(vCenter);

            if (ReadFileTask.IsValid())//Async
            {
                FGraphEventRef AttachTask = FFunctionGraphTask::CreateAndDispatchWhenReady([root, meshActors, strRootName]()
                    {
                        RenameActorRootComponent(root.Get(), *strRootName);

                        for (AActor* actor : meshActors)
                        {
                            if (actor&&root.IsValid())
                                actor->AttachToActor(root.Get(), FAttachmentTransformRules::KeepWorldTransform);
                        }
                    }, TStatId(), NULL, ENamedThreads::GameThread);
                FTaskGraphInterface::Get().WaitUntilTaskCompletes(AttachTask);
            }
            else
            {
                RenameActorRootComponent(root.Get(), *strRootName);
                for (AActor* actor : meshActors)
                {
                    if (root.IsValid())
                        actor->AttachToActor(root.Get(), FAttachmentTransformRules::KeepWorldTransform);
                }
            }
            
        }
        if (_fprogress)
        {
            if (!IsInGameThread())
            {
                FGraphEventRef FinishTask = FFunctionGraphTask::CreateAndDispatchWhenReady([this]()
                    {
                        _fprogress->CalFinish(_rootActor.Get());
                    }, TStatId(), NULL, ENamedThreads::GameThread);
                FTaskGraphInterface::Get().WaitUntilTaskCompletes(FinishTask);
            }
            else
            {
                _fprogress->CalFinish(_rootActor.Get());
            }
            _fprogress->RemoveFromRoot();
        }

        //_rootActor->GetRootComponent()->SetMobility(EComponentMobility::Type::Static);

    }
    static AActor* CreateActor(TWeakObjectPtr<UWorld> world,bool bCreateRoot = false, TSubclassOf<AActor> actorClass = nullptr)
    {
        if (!world.IsValid())
            return nullptr;
        //TSubclassOf<AActor> actorClassUse = actorClass ? actorClass : AActor::StaticClass();
		TSubclassOf<AActor> actorClassUse = actorClass;
		if (!actorClass)actorClassUse = AActor::StaticClass();

        AActor* aactor = world->SpawnActor<AActor>(actorClassUse);
        if (bCreateRoot)
        {
            USceneComponent* SceneComponent = NewObject<USceneComponent>(aactor, "SceneComponent");
            aactor->SetRootComponent(SceneComponent);
        }

        return aactor;
    }
};

TWeakObjectPtr<AActor> FileIO::LoadFileStaticMesh(const FString& filePath
    , bool ASync/* = true*/
    , URLFProgress* fprogress /*= nullptr*/
    , TEnumAsByte<enum CalculateCollisionMethod> calCollision
    , bool CalTangentIfNone
    , TSubclassOf<AActor> actorClass
    , UWorld* world)
{
    if (!IFileManager::Get().FileExists(*filePath))
    {
        if(fprogress)
            fprogress->RemoveFromRoot();
        return nullptr;
    }

    //FString fileName = FPaths::GetBaseFilename(filePath);

    TSharedPtr<AsyncDataStatic> asyncdata = MakeShared<AsyncDataStatic>();
    asyncdata->_filePath = filePath;
    //asyncdata->_actorClass = actorClass ? actorClass : AActor::StaticClass();
	asyncdata->_actorClass = actorClass;
	if (!asyncdata->_actorClass)asyncdata->_actorClass = AActor::StaticClass();

    asyncdata->_rootActor = AsyncDataStatic::CreateActor(world, true, asyncdata->_actorClass);
    asyncdata->_world = world;
    asyncdata->_calCollision = calCollision;
    asyncdata->_fprogress = fprogress;
    asyncdata->_CalTangentIfNone = CalTangentIfNone;
    if(ASync)
        asyncdata->ReadFileTask = FFunctionGraphTask::CreateAndDispatchWhenReady([asyncdata]()
        {
            asyncdata->FileLoadedAsync();
        }, TStatId(), NULL, ENamedThreads::AnyBackgroundThreadNormalTask);
    else
    {
        asyncdata->FileLoadedAsync();
    }


    return asyncdata->_rootActor;
}
