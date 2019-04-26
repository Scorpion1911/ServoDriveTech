#pragma once
#ifndef  __SPLINE__
#define	__SPLINE__
#endif

#ifdef SERVODRIVERCOMDLL_EXPORTS
#define SERVODRIVERCOMDLL_API __declspec(dllexport)
#else
#define SERVODRIVERCOMDLL_API __declspec(dllimport)
#endif

class SERVODRIVERCOMDLL_API CSPLine
{
public:
	CSPLine();
	~CSPLine();
protected:
	short Gauss(long n, double *pA, double *pResult);
	short DeleteRedundant(long n, double *pX, double *pY, double *pV, long *pCount);
	//short SplineComplete(long n, const double *pX, const double *pY, double *pG, double *pMu, double *pLambda, double dy0, double dyn);
	short PvSplitSame(double s, double v1, double v2, double v, double acc, double dec, long *pCount, double *pTime, double *pPos, double *pVel);
	short PvSplitDifferent(double s, double v1, double v2, double v, double acc, double dec, long *pCount, double *pTime, double *pPos, double *pVel);
public:
	// 3��������ֵ��ָ���߽��һ�׵���
	short SplineComplete(long n, const double *pX, const double *pY, double *pG, double *pMu, double *pLambda, double dy0, double dyn);

	// 3��������ֵ�����ں���
	short SplinePeriodic(long n, const double *pX, const double *pY, double *pG, double *pMu, double *pLambda);

	// 3��������ֵ��Hermite
	short Hermite(long n, const double *pX, const double *pY, const double *pV, double *pA, double *pB, double *pC);

	// PV�ٶ�ǰհ
	short PvLookahead(long n, double *pPos, double *pVel, double *pAcc, double *pDec);

	// PV�ֽ�
	short PvSplit(long n, double time, double *pPos, double *pVel, double *pVelMax, double *pAcc, double *pDec, long *pCount, double *pOutTime, double *pOutPos, double *pOutVel);

	// �ٷֱ�S���߷ֽ�
	short PercentSplit(long n, double *pTime, double *pPos, double *pVel, double *pPercent, long *pCount, double *pOutTime, double *pOutPos, double *pOutVel);

};

