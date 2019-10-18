#include "StdAfx.h"
#include "TYFormula.h"


CFormulaParser::CFormulaParser()
{
	s_instanceCount++;
}

CFormulaParser::CFormulaParser(CString p_sFormula)
{
	s_instanceCount++;
	SetFormula(p_sFormula);
}

CFormulaParser::~CFormulaParser()
{
	s_instanceCount--;
}

E_ParserStatus CFormulaParser::TryParse(double& ret)
{
	m_unknownVars.clear();
	if (s_instanceCount > 10000)
		return E_PS_PaserOverflow;
	return ParsePartition(m_formula, ret);
}

void CFormulaParser::SetFormula(CString p_sFormula)
{
	m_formula = p_sFormula;
	m_formula.Remove(L' ');
	m_formula.Remove(L'\r');
	m_formula.Remove(L'\n');
	m_formula.Remove(L'\t');
}

void CFormulaParser::SetVar(CString p_sName, double p_dValue)
{
	if (!IsValidVar(p_sName))
		return;
	for (UINT i = 0; i < m_dVars.size(); i++)
	{
		if (m_dVars[i].first == p_sName)
		{
			m_dVars[i].second = p_dValue;
			return;
		}
	}
	m_dVars.push_back(make_pair(p_sName, p_dValue));
}

bool CFormulaParser::GetVar(CString p_sName, double& p_dValue)
{
	for (UINT i = 0; i < m_dVars.size(); i++)
	{
		if (m_dVars[i].first == p_sName)
		{
			p_dValue = m_dVars[i].second;
			return true;
		}
	}
	return false;
}

E_ParserStatus CFormulaParser::ParsePartition(CString p_sFomula, double& ret)
{
	int depth = 0, pos = -1, length = p_sFomula.GetLength();
	int leftMark = -1, rightMark = -1;

	if (length == 0)
		return E_PS_InvalidFormula;

	//将表达式拆分成两个子表达式，递归解析
	for (int i = 0; i < length; i++)
	{
		wchar_t c = p_sFomula[i];
		if (c == L'(')
		{
			depth++;
			if (depth == 1)
				leftMark = i;
		}
		else if (c == L')')
		{
			depth--;
			if (depth < 0)
				return E_PS_InvalidFormula;
			if (depth == 0)
				rightMark = i;
		}
		else if (depth == 0 && IsOpt(c))
		{
			//忽略符号位
			if ((i == 0) || (p_sFomula[i-1] == L'(') || IsOpt(p_sFomula[i-1]))
			{
				if (c == L'*' || c == L'/')
					return E_PS_InvalidFormula;
				continue;
			}
			if (pos == -1 || (GetPriorityLevel(c) <= GetPriorityLevel(p_sFomula[pos])))
			{
				pos = i;
			}
		}
	}

	//括号不匹配
	if (depth != 0)
		return E_PS_InvalidFormula;

	//无法拆分为子表达式
	if (pos == -1)
	{
		if (p_sFomula[0] == L'+')
			return ParsePartition(p_sFomula.Mid(1), ret);
		else if (p_sFomula[0] == L'-')
			return ParsePartition(L"0" + p_sFomula, ret);
		//进入括号内
		else if ((leftMark == 0) && (rightMark == length - 1))
			return ParsePartition(p_sFomula.Mid(1, length - 2), ret);
		//解析为常量或变量
		else if ((leftMark == -1) && (rightMark == -1))
		{
			if ((pos = SplitVar(p_sFomula)) != -1)
			{
				p_sFomula.Insert(pos, L'*');
				return ParsePartition(p_sFomula, ret);
			}
			if (ToDouble(p_sFomula, ret))
				return E_PS_Success;
			if (!IsValidVar(p_sFomula))
				return E_PS_InvalidFormula;
			if (GetVar(p_sFomula, ret))
				return E_PS_Success;
			else
			{
				AddUnknownVar(p_sFomula);
				return E_PS_UnknownVars;
			}
		}
		else
			return E_PS_InvalidFormula;
	}
	//表达式拆分
	else
	{
		double dLeft, dRight;
		E_ParserStatus eLeft = ParsePartition(p_sFomula.Left(pos), dLeft);
		E_ParserStatus eRight = ParsePartition(p_sFomula.Mid(pos+1), dRight);
		E_ParserStatus es = max(eLeft, eRight);
		if (es == E_PS_Success)
			ret = Calc(p_sFomula[pos], dLeft, dRight);
		return es;
	}
}

bool CFormulaParser::ToDouble(CString p_sValue, double& ret)
{
	CString sValue = p_sValue;

	if (sValue.IsEmpty() || !IsNum(sValue[0]) || !IsNum(sValue[sValue.GetLength() - 1]))
		return false;
	sValue.Trim(L"0123456789");
	if (!sValue.IsEmpty() && (sValue != L"."))
		return false;

	ret = _ttof(p_sValue);
	return true;
}

bool CFormulaParser::IsValidVar(CString p_sVar)
{
	if (p_sVar.IsEmpty() || (!IsLetter(p_sVar[0]) && p_sVar[0] != L'_'))
		return false;
	for (int i = 0; i < p_sVar.GetLength(); i++)
		if (!IsLetter(p_sVar[i]) && !IsNum(p_sVar[i]) && (p_sVar[i] != L'_'))
			return false;
	return true;
}

int CFormulaParser::SplitVar(CString p_sVar)
{
	CString sNum = p_sVar.SpanIncluding(L"0123456789.");
	if (sNum.IsEmpty() || (sNum == p_sVar))
		return -1;
	return sNum.GetLength();
}

void CFormulaParser::AddUnknownVar(CString p_sName)
{
	for (UINT i = 0; i < m_unknownVars.size(); i++)
		if (m_unknownVars[i] == p_sName)
			return;
	m_unknownVars.push_back(p_sName);
}

int CFormulaParser::GetPriorityLevel(wchar_t p_c)
{
	switch (p_c)
	{
	case L'+':
	case L'-':
		return 1;
	case L'*':
	case L'/':
		return 2;
	default:
		return 0;
	}
}

bool CFormulaParser::IsLetter(wchar_t p_c)
{
	if (p_c >= L'A' && p_c <= L'Z')
		return true;
	if (p_c >= L'a' && p_c <= L'z')
		return true;
	return false;
}

bool CFormulaParser::IsNum(wchar_t p_c)
{
	return (p_c >= L'0' && p_c <= L'9');
}

bool CFormulaParser::IsOpt(wchar_t p_c)
{
	if (p_c == L'+' || p_c == L'-' || p_c == L'*' || p_c == L'/')
		return true;
	return false;
}

double CFormulaParser::Calc(wchar_t p_cOpt, double p_dLeft, double p_dRight)
{
	switch (p_cOpt)
	{
	case L'+':
		return p_dLeft + p_dRight;
	case L'-':
		return p_dLeft - p_dRight;
	case L'*':
		return p_dLeft * p_dRight;
	case L'/':
		return p_dLeft / p_dRight;
	default:
		assert(false);
		return 0;
	}
}

int CFormulaParser::s_instanceCount = 0;
