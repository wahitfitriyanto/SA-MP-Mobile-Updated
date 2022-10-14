#pragma once

typedef struct _ATTACHED_OBJECT
{
	int iModel;
	int iBoneID;
	VECTOR vecOffset;
	VECTOR vecRotation;
	VECTOR vecScale;
	uint32_t dwMaterialColor1;
	uint32_t dwMaterialColor2;
} ATTACHED_OBJECT;

class CObject : public CEntity
{
public:
	MATRIX4X4	m_matTarget;
	CQuaternion m_quatTarget;
	CQuaternion m_quatStart;
	MATRIX4X4	m_matCurrent;
	uint8_t		m_byteMoving;
	float		m_fMoveSpeed;
	bool		m_bIsPlayerSurfing;

	VECTOR 		m_vecRot;
	VECTOR		m_vecTargetRot;
	VECTOR		m_vecTargetRotTar;

	uint16_t 	m_iAttachVehicle;
	bool 		m_bAttached;
	VECTOR 		m_vecAttachOffset;
	VECTOR 		m_vecAttachRot;

	float m_fDistanceMoveObject;
	float m_fDistanceProgressMoveObject;

	CObject(int iModel, float fPosX, float fPosY, float fPosZ, VECTOR vecRot, float fDrawDistance);
	~CObject();

	void UpdateRwMatrixAndFrame();

	void Process(float fElapsedTime);
	float DistanceRemaining(MATRIX4X4 *matPos);
	float RotaionRemaining(VECTOR matPos);

	void SetPos(float x, float y, float z);
	void MoveTo(float x, float y, float z, float speed, float rX, float rY, float rZ);

	void InstantRotate(float x, float y, float z);

	void AttachProccess(uint16_t iAttachedVehicle, VECTOR AttachOffset, VECTOR AttachRot);
	void SetMaterial(int iModel, int iMaterialIndex, char* txdname, char* texturename, uint32_t dwColor);

public:
	uintptr_t	m_MaterialTexture[16];
	uint32_t	m_dwMaterialColor[16];
	bool		m_bHasMaterial;
};