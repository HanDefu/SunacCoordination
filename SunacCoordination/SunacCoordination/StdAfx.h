#pragma once

#ifndef WINVER
#define WINVER 0x600
#endif

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC OLE automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>          // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>         // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef WAS_DEBUG
#define _DEBUG
#undef WAS_DEBUG
#endif

//////////////////////////////////////////////////////////////////////////

#include "adui.h"
#include "acui.h"
#include "acuiComboBox.h"
#include "acuiDialog.h"
#include "string"
#include "afx.h" 

#include "res\resource.h"


#include "acdb.h"               // acdb definitions
#include "rxregsvc.h"           // ARX linker
#include "dbapserv.h"           // Host application services
#include "aced.h"               // aced stuff
#include "adslib.h"             // RXADS definitions
#include "acdocman.h"           // MDI document manager
#include "rxmfcapi.h"           // ObjectARX MFC support
#include "AcExtensionModule.h"  // Utility class for extension dlls
#include "dbents.h"
#include "dbregion.h "
#include "dbsol3d.h"
#include "dbxrecrd.h"
#include "acedads.h"
#include "gelnsg3d.h"
#include "acestext.h"
#include "actrans.h"
#include "rxobject.h"
#include "brbrep.h"
#include "brbftrav.h"
#include "brface.h"
#include "brfltrav.h"
#include "dbelipse.h "

#include "brloop.h"
#include "bredge.h"
#include "brlvtrav.h"
#include "brvtx.h"
#include "brbetrav.h"
#include "breltrav.h"
#include "dbspline.h"
#include "dbgroup.h"
#include "gesurf.h"
#include "acarray.h"
#include "geponsrf.h"
#include "aduiTabMainDialog.h"
#include "dbpl.h"
#include <cmath>
#include <afxdlgs.h>
#include <vector>
#include <brcplx.h>

using std::vector;

#define  PI 3.14159265358979
#define TOL 0.00001
#define JHSOLID_MAX_STRING 256
#define JH_SOLID_MINIMIZE_SOLID_LEN 0.1

typedef vector<int> vint;
typedef vector<int>::iterator itvint;
typedef vector<CString> vCString;
typedef vector<CString>::iterator itvCString;
typedef vector<WCHAR *> vWCHAR;
typedef vector<WCHAR *>::iterator itvWCHAR;
typedef vector<AcDbObjectId> vAcDbObjectId;
typedef vector<AcDbObjectId>::iterator itvAcDbObjectId;
typedef vector<vAcDbObjectId> vvAcDbObjectId;

typedef vector<AcGePoint3d> vAcGePoint3d;
typedef vector<AcGePoint3d>::iterator itvAcGePoint3d;
typedef vector<AcDbCurve*> vpAcDbCurve;
typedef vector<AcDbCurve*>::iterator itpAcDbCurve;
typedef vector<double> vdouble;

typedef vector<AcDbGroup*> vpGroups;


typedef std::vector<AcDbObjectId> vAcDbObjectId;
typedef std::vector<double> vdouble;
typedef std::vector<int> vint;
typedef std::vector<vint> vvint;
typedef std::vector<CString> vCString;
typedef std::vector<AcGePoint3d> vAcGePoint3d;
typedef std::vector<vAcGePoint3d> vvAcGePoint3d;
typedef std::vector<AcBrFace> vAcBrFace;
typedef std::vector<vAcBrFace> vvAcBrFace;
typedef std::vector<AcBrEdge> vAcBrEdge;
typedef std::vector<vAcBrEdge> vvAcBrEdge;
typedef std::vector<AcDbVoidPtrArray> vAcDbVoidPtrArray;
typedef std::vector<AcDbEntity *> vAcDbEntityP;
typedef std::vector<AcGeCurve3d *> vAcGeCurve3dP;
typedef std::vector<AcBrLoop> vAcBrLoop;
typedef std::vector<AcBrEdge> vAcBrEdge;
typedef std::vector<AcBrVertex> vAcBrVertex;
typedef std::vector<AcBrComplex> vAcBrComplex;
typedef std::vector<AcBrShell> vAcBrShell;
typedef std::vector<bool> vbool;
typedef std::vector<CStringA> vCStringA;

typedef std::pair<CString, double> RCPairKeyDValue;
typedef std::pair<CString, Adesk::Int32> RCPairKeyIValue;
typedef std::pair<CString, CString> RCPairKeyStrValue;

typedef std::vector<RCPairKeyDValue> vRCPairKeyDValue;
typedef std::vector<RCPairKeyIValue> vRCPairKeyIValue;
typedef std::vector<RCPairKeyStrValue> vRCPairKeyStrValue;

#define  RAY_LEN 10000000

#include <adscodes.h>
#include <rxmfcapi.h>
#include <afxcontrolbars.h>
#include <afxdisp.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

