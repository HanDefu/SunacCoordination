// �����Ϳ������á������ࡱ�����ļ�������ɵ� IDispatch ��װ��

//#import "acax19enu.tlb" no_namespace
// CAcadMenuBar ��װ��

class CAcadMenuBar : public COleDispatchDriver
{
public:
	CAcadMenuBar(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CAcadMenuBar(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcadMenuBar(const CAcadMenuBar& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IAcadMenuBar ����
public:
	LPDISPATCH Item(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IAcadMenuBar ����
public:

};