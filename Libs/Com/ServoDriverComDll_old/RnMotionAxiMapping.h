#pragma once
#include "RnServoAxiMapping.h"
class CRnMotionAxiMapping :
	public CRnServoAxiMapping
{
public:
	CRnMotionAxiMapping();
	~CRnMotionAxiMapping();
public:
	short InitialMapping(CRingNetDriver* driver);
	Uint16 ConvertDspToStationId(int16 dsp_id);
};

