#pragma once
#include <vector>

using namespace std;

enum E_ParserStatus
{
	E_PS_Success,
	E_PS_UnknownVars,
	E_PS_InvalidFormula,
	E_PS_PaserOverflow
};

class CFormulaParser
{
public:
	CFormulaParser();
	CFormulaParser(CString p_sFormula);
	~CFormulaParser();

	E_ParserStatus TryParse(double& ret);

	void SetFormula(CString p_sFormula);
	CString GetFormula() { return m_formula; }
	void SetVar(CString p_sName, double p_dValue);
	bool GetVar(CString p_sName, double& p_dValue);
	void ResetVar() { m_dVars.clear(); }

	vCString GetUnknownVars() { return m_unknownVars; }

protected:
	E_ParserStatus ParsePartition(CString p_sFormula, double& ret);

	bool ToDouble(CString p_sValue, double& ret);
	bool IsValidVar(CString p_sVar);
	int SplitVar(CString p_sVar);
	void AddUnknownVar(CString p_sName);
	int GetPriorityLevel(wchar_t p_c);

	bool IsLetter(wchar_t p_c);
	bool IsNum(wchar_t p_c);
	bool IsOpt(wchar_t p_c);

	double Calc(wchar_t p_cOpt, double p_dLeft, double p_dRight);

	CString m_formula;
	vector<pair<CString, double>> m_dVars;
	vCString m_unknownVars;

	//避免公式循环推导导致栈溢出
	static int s_instanceCount;
};

